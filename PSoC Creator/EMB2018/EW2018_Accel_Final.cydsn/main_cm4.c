#include "project.h"

#include "stdio.h"

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"

#define OS_MAX_PRI                  ((configMAX_PRIORITIES)-1)
#define OS_MIN_PRI                  ((tskIDLE_PRIORITY)+1)
#define OS_MID_PRI                  ((OS_MAX_PRI-OS_MIN_PRI)/2)

#define SENSOR_QUEUE_SIZE           (5)

#define MAXACCEL                    (8000)

#define PI                          (3.141)
#define RADIANS_TO_DEGREES(x)       (x*180.0/PI)
#define DEGREES_TO_PERCENT(x)       (x*100.0/180.0)
#define RADIANS_TO_PERCENT(x)       DEGREES_TO_PERCENT(RADIANS_TO_DEGREES(x))

#define ROUND_TO_INT16(x)           ((int16_t)((double)x + 0.5))

#define MIN_MOVEMENT                (2)


QueueHandle_t sensorDataQueue;
SemaphoreHandle_t bleEventSemaphore;

#include "bmi160.h"

static struct bmi160_dev bmi160Dev;


int8_t BMI160BurstWrite( uint8_t dev_addr, uint8_t reg_addr,uint8_t *data, uint16_t len )
{
    Cy_SCB_I2C_MasterSendStart( I2C_HW, dev_addr, CY_SCB_I2C_WRITE_XFER, 0, &I2C_context );
    Cy_SCB_I2C_MasterWriteByte( I2C_HW, reg_addr, 0, &I2C_context );
    
    for(int i = 0; i<len; i++ )
    { 
        Cy_SCB_I2C_MasterWriteByte( I2C_HW, data[i], 0, &I2C_context );
    }
    
    Cy_SCB_I2C_MasterSendStop( I2C_HW, 0, &I2C_context );
    
    return 0;
}

int8_t BMI160BurstRead( uint8_t dev_addr, uint8_t reg_addr,uint8_t *data, uint16_t len )
{
    
    Cy_SCB_I2C_MasterSendStart( I2C_HW, dev_addr, CY_SCB_I2C_WRITE_XFER, 0, &I2C_context );
    Cy_SCB_I2C_MasterWriteByte( I2C_HW, reg_addr, 0, &I2C_context );
    Cy_SCB_I2C_MasterSendReStart( I2C_HW, dev_addr, CY_SCB_I2C_READ_XFER, 0, &I2C_context );
    
    for( int i = 0; i<len-1; i++ )
    {
        Cy_SCB_I2C_MasterReadByte( I2C_HW, CY_SCB_I2C_ACK, &data[i], 0, &I2C_context );
    }
    
    Cy_SCB_I2C_MasterReadByte( I2C_HW, CY_SCB_I2C_NAK, &data[len-1], 0, &I2C_context );
    Cy_SCB_I2C_MasterSendStop( I2C_HW, 0, &I2C_context );
    
    
    return 0;
}

void sensorsDeviceInit(void)
{

  int8_t rslt;
    
    printf("BMI160 I2C connection [INIT].\r\n");
  vTaskDelay(500); // guess

  /* BMI160 */
  // assign bus read function
  bmi160Dev.read = (bmi160_com_fptr_t)BMI160BurstRead;
  // assign bus write function
  bmi160Dev.write = (bmi160_com_fptr_t)BMI160BurstWrite;
  // assign delay function
  bmi160Dev.delay_ms = (bmi160_delay_fptr_t)vTaskDelay;
  bmi160Dev.id = BMI160_I2C_ADDR;  // I2C device address

  rslt = bmi160_init(&bmi160Dev); // initialize the device
  if (rslt == 0)
    {
      printf("BMI160 I2C connection [OK].\r\n");
      /* Select the Output data rate, range of Gyroscope sensor
       * ~92Hz BW by OSR4 @ODR=800Hz */
      bmi160Dev.gyro_cfg.odr = BMI160_GYRO_ODR_800HZ;
      bmi160Dev.gyro_cfg.range = BMI160_GYRO_RANGE_125_DPS;
      bmi160Dev.gyro_cfg.bw = BMI160_GYRO_BW_OSR4_MODE;
      bmi160Dev.gyro_cfg.power = BMI160_GYRO_NORMAL_MODE;

      bmi160Dev.accel_cfg.odr = BMI160_ACCEL_ODR_1600HZ;
      bmi160Dev.accel_cfg.range = BMI160_ACCEL_RANGE_4G;
      bmi160Dev.accel_cfg.bw = BMI160_ACCEL_BW_OSR4_AVG1;
      bmi160Dev.accel_cfg.power = BMI160_ACCEL_NORMAL_MODE;

      /* Set the sensor configuration */
      rslt |= bmi160_set_sens_conf(&bmi160Dev);
      bmi160Dev.delay_ms(50);
    }
  else
    {
      printf("BMI160 I2C connection [FAIL].\r\n");
    }
}

void sensorConvertPercent( struct bmi160_sensor_data * acc_data )
{
    static double lastX = 50.0;
    static double lastY = 50.0;
    
    double newX, newY;
    
    /* Set and correct the maxima and minima to filter out rough handling */
    acc_data->x = (  acc_data->x > MAXACCEL ) ?  MAXACCEL : acc_data->x;
    acc_data->x = ( -acc_data->x > MAXACCEL ) ? -MAXACCEL : acc_data->x;
    acc_data->y = (  acc_data->y > MAXACCEL ) ?  MAXACCEL : acc_data->y;
    acc_data->y = ( -acc_data->y > MAXACCEL ) ? -MAXACCEL : acc_data->y;    
    
    /* Normalize the readings */
    newX = (double)( (double)acc_data->x  / (double)MAXACCEL );
    newY = (double)( (double)acc_data->y  / (double)MAXACCEL );
        
    /* Get arc cosine for linear "feel" */
    newX = acos( newX );     
    newY = acos( newY );
    
    /* Convert to percentage */
    newX = RADIANS_TO_PERCENT( newX );
    newY = RADIANS_TO_PERCENT( newY );
  
    /*
    Use an IIR Filter to get rid of noise
    Add one eigth of the latest reading to seven eigths of the previous value
    */
    newX =  ( newX / 8.0 ) + ( lastX * 7.0 / 8.0 );
    newY =  ( newY / 8.0 ) + ( lastY * 7.0 / 8.0 );
    
    /* Round the data and convert to integer */
    acc_data->x = (int16_t)( newX + 0.5 );
    acc_data->y = (int16_t)( newY + 0.5 );
    
    lastX = newX;
    lastY = newY;
}

void accelTask( void * arg )
{
    struct bmi160_sensor_data acc_data;
    int16_t prev_x = 50, prev_y = 50;       // Assume the board is horizontal
    
    (void)arg;
    
    printf( "accelTask running\r\n" );
        
    /* Connect to the BMI sensor */
    I2C_Start();
    
    sensorsDeviceInit();
    vTaskDelay( 500 );
    
    for(;;)
    {
        /* Get data from the accelerometer */
        bmi160_get_sensor_data( BMI160_ACCEL_ONLY, &acc_data, NULL, &bmi160Dev );
        
        /* Convert the X- and Y-axis values into percentages */
        sensorConvertPercent( &acc_data );
        
        /* Send data if changed */
        if( abs( prev_x - acc_data.x ) > MIN_MOVEMENT || abs( prev_y - acc_data.y ) > MIN_MOVEMENT )
        {
        
            printf( "accelTask sending %d %d\r\n", acc_data.x, acc_data.y );
            prev_x = acc_data.x;
            prev_y = acc_data.y;
            
            xQueueSend( sensorDataQueue, &acc_data, 0 );
        }
        
        vTaskDelay(50);
    }
}

typedef struct  {
    char *name;
    int name_len;
    uint8_t *serviceUUID;
    uint8 servUUID_len;
} advInfo_t;

advInfo_t currentAdvInfo;

int findAdvInfo(uint8_t *adv,uint8_t len)
{
    memset(&currentAdvInfo,0,sizeof(currentAdvInfo));
    
    for(uint8_t i=0;i<len;)
    {
        switch(adv[i+1])
        {
            case 7: // Service UUID
                currentAdvInfo.serviceUUID = &adv[i+2];
                currentAdvInfo.servUUID_len = adv[i]-1;
            break;
            
            case 9: // Name
                currentAdvInfo.name = (char *)&adv[i+2];
                currentAdvInfo.name_len = adv[i]-1;
            break;
        }
        i = i + adv[i]+1;
    }
    return (currentAdvInfo.name_len > 0 && currentAdvInfo.serviceUUID > 0);
}


void AppCallback( uint32_t event, void *eventParam )
{    
    cy_stc_ble_gapc_adv_report_param_t  *scanProgressParam;
    static cy_stc_ble_bd_addr_t connectAddr;
    
    switch( event )
    {
        case CY_BLE_EVT_STACK_ON:
            Cy_BLE_GAPC_StartScan( CY_BLE_SCANNING_FAST, CY_BLE_CENTRAL_CONFIGURATION_0_INDEX );
        break;

        case CY_BLE_EVT_GAP_DEVICE_DISCONNECTED:
            Cy_BLE_GAPC_StartScan( CY_BLE_SCANNING_FAST, CY_BLE_CENTRAL_CONFIGURATION_0_INDEX );
        break;
            
        case CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT:   
            /* Found a peripheral - is it the right one? */
            scanProgressParam = (cy_stc_ble_gapc_adv_report_param_t  *)eventParam;
            
            findAdvInfo(scanProgressParam->data,scanProgressParam->dataLen);
            
            
            if(currentAdvInfo.servUUID_len == 16 && memcmp(currentAdvInfo.serviceUUID,cy_ble_customCServ [CY_BLE_CUSTOMC_MOTOR_SERVICE_INDEX].uuid,16) ==0 )
            {
                    printf( "CY_BLE_EVT_GAPC_SCAN_PROGRESS_RESULT\tFound RobotArm\r\n" );
            
                    memcpy( &connectAddr.bdAddr[0], &scanProgressParam->peerBdAddr[0], CY_BLE_BD_ADDR_SIZE );
                    connectAddr.type = scanProgressParam->peerAddrType;
                    Cy_BLE_GAPC_StopScan();
            }
            
        break;
                
        case CY_BLE_EVT_GAPC_SCAN_START_STOP:
            printf("CY_BLE_EVT_GAPC_SCAN_START_STOP\t\t");
            if(Cy_BLE_GetScanState() == CY_BLE_SCAN_STATE_STOPPED)
            {
                printf("Complete\r\n");
                Cy_BLE_GAPC_ConnectDevice( &connectAddr, 0 );
            }
            else
            {
                printf("Starting\r\n");
            }                
        break;
            
        case CY_BLE_EVT_GATT_CONNECT_IND:
            printf("CY_BLE_EVT_GATT_CONNECT_IND\t\tStarting discovery\r\n");
            
            Cy_BLE_GATTC_StartDiscovery(cy_ble_connHandle[0]);
        break;
            
        case CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE:
            printf("CY_BLE_EVT_GATTC_DISCOVERY_COMPLETE\r\n");
            printf("Attr M1 = %X\r\n",cy_ble_customCServ [CY_BLE_CUSTOMC_MOTOR_SERVICE_INDEX].customServChar[CY_BLE_CUSTOMC_MOTOR_M0_CHAR_INDEX].customServCharHandle[0]);
            printf("Attr M2 = %X\r\n",cy_ble_customCServ [CY_BLE_CUSTOMC_MOTOR_SERVICE_INDEX].customServChar[CY_BLE_CUSTOMC_MOTOR_M1_CHAR_INDEX].customServCharHandle[0]);
            
        break;  

        default:
        break;
    }
}

void bleEventOccurred( void )
{
    /* Wake up bleTask so it can run ProcessEvents */
    BaseType_t taskWoken = pdFALSE;
    xSemaphoreGiveFromISR( bleEventSemaphore, &taskWoken ); 
    portYIELD_FROM_ISR( taskWoken );
}

void bleTask( void * arg )
{    
    struct bmi160_sensor_data accel_data;
    cy_stc_ble_gattc_write_req_t write_req_1;
    cy_stc_ble_gattc_write_req_t write_req_2;
    
    cy_en_ble_api_result_t result;
    
    /* Get a pointer into the database to make the update code simpler */
    cy_stc_ble_customc_char_t * char_array = cy_ble_customCServ [CY_BLE_CUSTOMC_MOTOR_SERVICE_INDEX].customServChar;
    
    (void)arg;
    
    printf( "bleTask running\r\n" );
        
    /* Counting semaphore to indicate BLE stack events */
    bleEventSemaphore = xSemaphoreCreateCounting( 0xFFFFFFFF, 0 );
        
    Cy_BLE_Start( AppCallback );                    // Start BLE
    
    while( Cy_BLE_GetState() != CY_BLE_STATE_ON )   // Hog CPU until stack is up
        Cy_BLE_ProcessEvents();
    
    /* Run this function on every stack event (allows bleTask to sleep) */
    Cy_BLE_RegisterAppHostCallback( bleEventOccurred );
    
    for(;;)
    {
        /* If bleEventOccurred process it - otherwise let accelTask run */
        xSemaphoreTake( bleEventSemaphore, portMAX_DELAY );
        Cy_BLE_ProcessEvents();
        
            /* If not discovered, go back and wait for the event */
        if( Cy_BLE_GetConnectionState( cy_ble_connHandle[0]) != CY_BLE_CONN_STATE_CLIENT_DISCOVERED)
            continue;

        /* If busy, go back and complete that action */
        if( Cy_BLE_GATT_GetBusyStatus(cy_ble_connHandle[0].attId) == CY_BLE_STACK_STATE_BUSY)
            continue;
        
        /* bleTask is now ready to accept requests to send data */
        if( pdTRUE == xQueueReceive( sensorDataQueue, &accel_data, 0 ) )
        {
            write_req_1.handleValPair.attrHandle = char_array[CY_BLE_CUSTOMC_MOTOR_M0_CHAR_INDEX].customServCharHandle[0];
            write_req_1.handleValPair.value.val = (uint8_t *)&accel_data.x;
            write_req_1.handleValPair.value.len = 1;
            write_req_1.connHandle = cy_ble_connHandle[0]; 
            
            result = Cy_BLE_GATTC_WriteCharacteristicValue( &write_req_1 );
            if( result != CY_BLE_SUCCESS)
            {
                switch(result)
                {
                    case CY_BLE_ERROR_INVALID_PARAMETER:
                        printf("Bad CY_BLE_ERROR_INVALID_PARAMETER\r\n");
                        break;
                    case CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED:
                        printf("Bad CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED\r\n");
                        break;
                    case CY_BLE_ERROR_INVALID_OPERATION:
                        printf("Bad CY_BLE_ERROR_INVALID_OPERATION\r\n");
                        break;
                    default:
                        printf("Bad Documentation\r\n");
                        break;
                }
            }
            
            
            write_req_2.handleValPair.value.len = 1;
            write_req_2.connHandle = cy_ble_connHandle[0]; 
            write_req_2.handleValPair.attrHandle = char_array[CY_BLE_CUSTOMC_MOTOR_M1_CHAR_INDEX].customServCharHandle[0];
            write_req_2.handleValPair.value.val = (uint8_t *)&accel_data.y; 
            
            result = Cy_BLE_GATTC_WriteCharacteristicValue( &write_req_2 );
            if( result != CY_BLE_SUCCESS)
            {
                printf("M1 Failed\r\n");
                switch(result)
                {
                    case CY_BLE_ERROR_INVALID_PARAMETER:
                        printf("Bad CY_BLE_ERROR_INVALID_PARAMETER\r\n");
                        break;
                    case CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED:
                        printf("Bad CY_BLE_ERROR_MEMORY_ALLOCATION_FAILED\r\n");
                        break;
                    case CY_BLE_ERROR_INVALID_OPERATION:
                        printf("Bad CY_BLE_ERROR_INVALID_OPERATION\r\n");
                        break;
                    default:
                        printf("Bad Documentation\r\n");
                        break;
                }
            }
                
            printf( "bleTask sending M0 %d M1 %d\r\n", accel_data.x, accel_data.y );
        }
    }
}

int main(void)
{
    /* Create OS resources */   
    sensorDataQueue = xQueueCreate( SENSOR_QUEUE_SIZE, sizeof( struct bmi160_sensor_data ) );
    
    /* Create OS tasks */
    xTaskCreate(
        accelTask,                  /* Task function */
        "Accel",                    /* Task name (string) */
        500,                        /* Task stack, allocated from heap */
        (void *)NULL,               /* No argument to task function */
        OS_MID_PRI,                 /* Middle priority */
        NULL );                     /* Not using task handle */
    
    int g = xTaskCreate(
        bleTask,
        "BLE",
        1000,                       /* BLE needs more stack space */
        (void *)NULL,
        OS_MAX_PRI,                 /* High priority */
        NULL );
    
    UART_Start();
    printf( "Starting BLE Central Application %d\r\n",g );
    
    /* Start the RTOS */
    vTaskStartScheduler();

    for(;;)
    {
        /* vTaskStartScheduler never returns */
    }
}
