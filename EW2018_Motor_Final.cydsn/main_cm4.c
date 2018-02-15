#include "project.h"

#include "stdio.h"

/****************************************************************************/
/* FreeRTOS                                                                 */
/****************************************************************************/

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "event_groups.h"
#include "semphr.h"

#define OS_MAX_PRI                  ((configMAX_PRIORITIES)-1)
#define OS_MIN_PRI                  ((tskIDLE_PRIORITY)+1)

EventGroupHandle_t                  pwmUpdateEvent;
SemaphoreHandle_t                   bleEventSemaphore;


/****************************************************************************/
/* MOTOR PWMs                                                               */
/****************************************************************************/

/*
 * The robot arm is controlled by regular pulses from the PSoC to the shield.
 * The period of pulses is required to be approximately 50Hz (20ms (20000us)
 * cycle time) amd the pulses should be between 800us and 1800us duration.
 * 
 * PWM output for minimum duty cycle:
 *  ______
 * |      |__________________________________________________
 * 0  800us                                           20000us
 * 
 * PWM output for maximum duty cycle:
 *  ___________
 * |           |_____________________________________________
 * 0      1800us                                      20000us
 * 
 * If the PWM clock is 1MHz (default) each count represents 1us.
 * These macros convert 0% into an 800us pulse and 100% into 1800us.
 */
#define PWM_CMP_MIN                 (800)
#define PWM_CMP_MAX                 (1800)
#define PWM_CMP_RANGE               (PWM_CMP_MAX-PWM_CMP_MIN)
#define PWM_PCT_TO_CMP(percent)     (PWM_CMP_MIN+((PWM_CMP_RANGE*percent)/100))
#define PWM_CMP_TO_PCT(compare)     ((compare-PWM_CMP_MIN)*100/PWM_CMP_RANGE)

typedef enum { M0, M1 } motor_t;

#define M0_UPDATE_EVENT             (1<<M0)
#define M1_UPDATE_EVENT             (1<<M1)

int getCheckedPercent( uint8_t percent )
{
    return ( percent > 100 ) ? 100 : percent;       // Ensure 0 <= value <= 100
}

void setMotorPercent( motor_t motor, int percent )
{
    int compare;

    percent = getCheckedPercent( percent );         // Ensure 0 <= value <= 100

    compare = PWM_PCT_TO_CMP( percent );            // Translate % to duty cycle

    switch( motor )
    {
        case M0:
            PWM_M0_SetCompare0( compare );          // Move left-right
        break;

        case M1:
            PWM_M1_SetCompare0( compare );          // Move up-down
        break;
    }
}

uint8_t getMotorPercent( motor_t motor )
{
    int compare = 0;

    switch( motor )
    {
        case M0:
            compare = PWM_M0_GetCompare0();         // Get current M0 duty cycle
        break;

        case M1:
            compare = PWM_M1_GetCompare0();         // Get current M1 duty cycle
        break;
    }

    return PWM_CMP_TO_PCT( compare );               // Translate duty cycle to %
}

/****************************************************************************/
/* CAPSENSE                                                                 */
/****************************************************************************/

#define CAPSENSE_UPDATE_DELAY       (50)

void capsenseTask( void * arg )
{
    motor_t motor = M0;
    uint8_t percent = 0;
    
    (void)arg;
    
    /* Start the motor PWMs and set to 0, 0 (once SW2 pressed) */
    PWM_M0_Start();
    PWM_M1_Start();
    setMotorPercent( M0, 0 );
    setMotorPercent( M1, 0 );
    
    CapSense_Start();
    
    printf( "capsenseTask running\r\n" );
    
    for(;;)
    {
        CapSense_ScanAllWidgets();                  // Start a hardware scan
        
        while( CapSense_IsBusy() )
            vTaskDelay( 1 );                        // Task sleeps while scanning
            
        CapSense_ProcessAllWidgets();               // Get the widget state from hardware
        
        if( CapSense_IsWidgetActive( CapSense_M0_WDGT_ID ) )
            motor = M0;                             // Change to motor M0
        
        if( CapSense_IsWidgetActive( CapSense_M1_WDGT_ID ) )
            motor = M1;                             // change to motor M1
        
        if( CapSense_IsWidgetActive( CapSense_PERCENT_WDGT_ID ) )
        {
            /* Set the arm position based on the finger position */
            percent = CapSense_GetCentroidPos( CapSense_PERCENT_WDGT_ID );
            
            printf( "CapSense M%d %d\r\n", (int)motor, (int)percent );
            
            if( motor == M0 )
            {   // Update M0 to the desired position and notify bleTask
                setMotorPercent( M0, percent );
                xEventGroupSetBits( pwmUpdateEvent, M0_UPDATE_EVENT );
            }
            
            if( motor == M1 )
            {   // Update M1 to the desired position and notify bleTask
                setMotorPercent( M1, percent );
                xEventGroupSetBits( pwmUpdateEvent, M1_UPDATE_EVENT );
            }
        }
        vTaskDelay( CAPSENSE_UPDATE_DELAY );        // Delay to avoid constant scanning
    }
}


/****************************************************************************/
/* BLUETOOTH LOW ENERGY                                                     */
/****************************************************************************/

void updateGattLocal( cy_ble_gatt_db_attr_handle_t handle )
{
    cy_stc_ble_gatt_handle_value_pair_t vpair;
    uint8_t percent = 0;
    
    /* Set up vpair with the percentage for the required motor */
    vpair.attrHandle = handle;
    
    if( CY_BLE_MOTOR_M0_CHAR_HANDLE == handle )
    {
        percent = getMotorPercent( M0 );
    }
    else if( CY_BLE_MOTOR_M1_CHAR_HANDLE == handle )
    {
        percent = getMotorPercent( M1 );
    }
                
    vpair.value.val = &percent;
    vpair.value.len = 1;

    /* Update the local database */
    Cy_BLE_GATTS_WriteAttributeValueLocal( &vpair );
                
    /* Notify connected device */
    Cy_BLE_GATTS_SendNotification( &cy_ble_connHandle[0], &vpair );
}

void bleEventOccurred( void )
{
    BaseType_t taskWoken = pdFALSE;
    xSemaphoreGiveFromISR( bleEventSemaphore, &taskWoken ); 
    portYIELD_FROM_ISR( taskWoken );
}

void AppCallback( uint32_t event, void *eventParam )
{
    cy_stc_ble_conn_handle_t connection;
    cy_stc_ble_gatt_handle_value_pair_t vPair;
    
    switch( event )
    {
        case CY_BLE_EVT_STACK_ON:
            Cy_BLE_GAPP_StartAdvertisement( CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX );
        break;

        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            Cy_BLE_GAPP_StartAdvertisement( CY_BLE_ADVERTISING_FAST, CY_BLE_PERIPHERAL_CONFIGURATION_0_INDEX );
        break;

        case CY_BLE_EVT_GATTS_WRITE_REQ:
            /* Extract required information from the event parameter */
            vPair = ( (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParam )->handleValPair;
            connection = ( (cy_stc_ble_gatts_write_cmd_req_param_t *) eventParam )->connHandle;
            
            /* vPair.value.val points to the percentage so check it */
            *(vPair.value.val) = getCheckedPercent( *(vPair.value.val) );
            
            /* vPair.attrHandle is the characteristic in the motor service */
            switch( vPair.attrHandle )
            {
                case CY_BLE_MOTOR_M0_CHAR_HANDLE:   // Change M0 position
                    setMotorPercent( M0, *(vPair.value.val) );
                    printf( "BLE M0 %d\r\n", (int)*(vPair.value.val) );
                    break;
                    
                case CY_BLE_MOTOR_M1_CHAR_HANDLE:   // change M1 position
                    setMotorPercent( M1, *(vPair.value.val) );
                    printf( "BLE M0 %d\r\n", (int)*(vPair.value.val) );
                break;
                
                default:                            // Bad data so exit handler
                    return;
            }
            
            /* Update the GATT database and respond to the central device */
            Cy_BLE_GATTS_WriteAttributeValuePeer( &cy_ble_connHandle[0], &vPair );
            Cy_BLE_GATTS_WriteRsp( connection );
        break;
    }
}

void bleTask( void * arg )
{
    EventBits_t bits;
    
    (void)arg;
    
    Cy_BLE_Start( AppCallback );                    // Start BLE
    
    while( Cy_BLE_GetState() != CY_BLE_STATE_ON )   // Hog CPU until stack is up
        Cy_BLE_ProcessEvents();

    /* Run this function on every stack event (allows bleTask to sleep) */
    Cy_BLE_RegisterAppHostCallback( bleEventOccurred );
    
    /* Copy the actual motor settings into the BLE database */
    updateGattLocal( CY_BLE_MOTOR_M0_CHAR_HANDLE );
    updateGattLocal( CY_BLE_MOTOR_M1_CHAR_HANDLE );

    printf( "bleTask running\r\n" );
    
    for(;;)
    {
        /* If bleEventOccurred process it - allow time for Capsense to run */
        if( pdTRUE == xSemaphoreTake( bleEventSemaphore, CAPSENSE_UPDATE_DELAY ) )
            Cy_BLE_ProcessEvents();
        
        /* If CapSense indicates new data, update the BLE GATT database */
        bits = xEventGroupWaitBits( pwmUpdateEvent, M0_UPDATE_EVENT | M1_UPDATE_EVENT, pdTRUE, pdFALSE, 0 );
        
        if( bits & M0_UPDATE_EVENT )                // M0 position changed
            updateGattLocal( CY_BLE_MOTOR_M0_CHAR_HANDLE );
            
        if( bits & M1_UPDATE_EVENT )                // M1 position changed
            updateGattLocal( CY_BLE_MOTOR_M1_CHAR_HANDLE ); 
    }
}

/****************************************************************************/
/* APPLICATION STARTUP                                                      */
/****************************************************************************/

int main(void)
{
    /* Create OS resources */
        
    /* Counting semaphore to indicate BLE stack events */
    bleEventSemaphore = xSemaphoreCreateCounting( 0xFFFFFFFF, 0 );
    
    /* Event flag to signal a locally-changed motor values */
    pwmUpdateEvent = xEventGroupCreate();
    
    /* Create OS tasks */
   
    xTaskCreate(
        capsenseTask,               /* Task function */
        "CapSense",                 /* Task name */
        250,                        /* stack, allocated from FreeRTOS heap */
        (void *)NULL,               /* Argument to task function */
        OS_MIN_PRI,                 /* Low priority */
        NULL );                     /* Task handle (unused) */

    xTaskCreate(
        bleTask,
        "BLE",
        1000,                       /* Task stack, increased for BLE stack */
        (void *)NULL,
        OS_MAX_PRI,                 /* Must be maximum to respond to events */
        NULL );    
    
    /* Start UART */
    UART_Start();
    printf( "\r\nStarting FreeRTOS\r\n" );
    
    /* Start the OS */
    vTaskStartScheduler();
    
    for(;;)
    {
        /* vTaskStartScheduler should never return */
    }
}
