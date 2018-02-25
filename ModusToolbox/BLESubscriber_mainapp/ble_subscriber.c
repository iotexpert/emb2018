/*
 * ble_subscriber.c
 *
 * This the main file that contains the BLE Subscriber part of the EMB2018 Project.
 *
 * The project
 * 		1. Starts in application_start
 *     		- Calls aws_start - which turns on Wifi and connects to AWS and subcribes
 *     		- Starts the BLE Stack and registers the callbacks
 *
 *		2. It starts scanning ... looking for a BLE device that is advertising with the right service UUID
 *
 *		3. When it finds that device it stops scanning and makes a connection
 *
 *		4. Once the connection is made... it waits for a right
 *
 *		5. If it is disconnected it starts scanning again
 *
 *		6. It also provies the function ble_subscriber_write_motor which is how the AWS Subscribes writes to
 *		   the P6 Robot.
 *
 */

#include <string.h>
#include "wiced_bt_dev.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced.h"
#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "wwd_debug.h"
#include "wiced_bt_dev.h"
#include "wiced_low_power.h"
#include "wiced_bt_ble.h"

#include "aws_app.h"

// Activate this macro to follow the Function prints
//#define FUNCTION_PRINT(args) WPRINT_MACRO(args)
#define FUNCTION_PRINT(args)


/******************************************************************************
 *                                Constants
 ******************************************************************************/
/* UUID value of the Hello Sensor Service */
#define UUID_HELLO_SERVICE  0x23, 0x20, 0x56, 0x7c, 0x05, 0xcf, 0x6e, 0xb4, 0xc3, 0x41, 0x77, 0x28, 0x51, 0x82, 0x7e, 0x1b
#define UUID_MOTOR_SERVICE  0x27,0x74,0x70,0xF7,0x6D,0x4B,0x49,0x82,0x01,0x4A,0x9E,0xCD,0x56,0x04,0x55,0x50
#define HANDLE_MOTOR_M1_VAL 0x12
#define HANDLE_MOTOR_M2_VAL 0x16

/******************************************************************************
 *                          Type  Definitions
 ******************************************************************************/

/* Peer Info */
typedef struct
{
    uint16_t conn_id;                   // Connection Identifier
    uint8_t  role;                      // master or slave in the current connection
    uint8_t  addr_type;                 // peer address type
    uint8_t  transport;                 // peer connected transport
    uint8_t  peer_addr[BD_ADDR_LEN];    // Peer BD Address
} ble_subscriber_peer_info_t;


/******************************************************************************
 *                            Variables Definitions
 ******************************************************************************/

ble_subscriber_peer_info_t g_ble_subscriber;
const uint8_t hello_service[16] = {UUID_MOTOR_SERVICE};

extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];

/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/
static wiced_result_t           ble_subscriber_management_cback( wiced_bt_management_evt_t event,  wiced_bt_management_evt_data_t *p_event_data );
static wiced_bt_gatt_status_t   ble_subscriber_gatt_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data );
static void                     ble_subscriber_scan_result_cback( wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data );

/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
void application_start( )
{
	wiced_init();
    wiced_gpio_output_low(WICED_LED1);
    wiced_gpio_output_high(WICED_LED2);

	WPRINT_APP_INFO(("Starting AWS ---- Subscriber\n"));

    aws_start();
    wiced_bt_stack_init( ble_subscriber_management_cback, &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools );
}

/*
 *  ble_subscriber_management_cback
 *
 * This function is the main BLE callback handler.
 * Its only role is to start the scanning when the stack turns on.
 *
 */
wiced_result_t ble_subscriber_management_cback( wiced_bt_management_evt_t event,  wiced_bt_management_evt_data_t *p_event_data)
{
	wiced_result_t               result = WICED_BT_SUCCESS;

	FUNCTION_PRINT(("ble_subscriber_management_cback: %x\n", event ));

	switch( event )
	{
	/* Bluetooth  stack enabled */
	case BTM_ENABLED_EVT:
		memset( &g_ble_subscriber, 0, sizeof( g_ble_subscriber ) );
		wiced_bt_gatt_register( ble_subscriber_gatt_callback );
		result = wiced_bt_ble_scan( BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, ble_subscriber_scan_result_cback );
		FUNCTION_PRINT(( "ble_subscriber_start_scan: %d\n", result ));
		break;

	case BTM_BLE_SCAN_STATE_CHANGED_EVT:
		FUNCTION_PRINT(( "BTM_BLE_SCAN_STATE_CHANGED_EVT: %d\n", p_event_data->ble_scan_state_changed ));
		break;

	case BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT:
		FUNCTION_PRINT(("BTM_LOCAL_IDENTITY_KEYS_REQUEST_EVT\n"));
		break;

	case BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT:
		FUNCTION_PRINT(("BTM_PAIRED_DEVICE_LINK_KEYS_REQUEST_EVT\n"));
		return WICED_ERROR;
		break;

	case BTM_LPM_STATE_LOW_POWER:
		break;

	default:
		WPRINT_APP_INFO(("Unknown BTM event %d\n",event));
		break;
	}
	return result;
}

/*
 *
 *	ble_subscriber_gatt_callback
 *
 * 	Callback function is executed to process various GATT events
 *
 * 	The only event that matter is the GATT_CONNECTION_STATUS_EVT which means either a connection or a disconnection
 *
 */
wiced_bt_gatt_status_t ble_subscriber_gatt_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
	wiced_bt_gatt_status_t result = WICED_BT_SUCCESS;

	FUNCTION_PRINT(( "ble_subscriber_gatt_callback event %d \n", event ));

	switch( event )
	{
	case GATT_CONNECTION_STATUS_EVT:
		if ( p_data->connection_status.connected )
		{
			uint8_t dev_role;
			wiced_bt_gatt_connection_status_t *p_conn_status = &p_data->connection_status;

			wiced_bt_dev_get_role( p_conn_status->bd_addr, &dev_role, BT_TRANSPORT_LE );
			g_ble_subscriber.conn_id         =  p_conn_status->conn_id;
			g_ble_subscriber.role            = dev_role;
			g_ble_subscriber.transport       = p_conn_status->transport;
			g_ble_subscriber.addr_type       = p_conn_status->addr_type;
			memcpy( g_ble_subscriber.peer_addr, p_conn_status->bd_addr, BD_ADDR_LEN );

			WPRINT_BT_APP_INFO(( "P6 Robot Connection Complete Conn Id:%d Addr:<%X:%X:%X:%X:%X:%X> role:%d\n", p_conn_status->conn_id,
					p_conn_status->bd_addr[0], p_conn_status->bd_addr[1], p_conn_status->bd_addr[2],
					p_conn_status->bd_addr[3], p_conn_status->bd_addr[4], p_conn_status->bd_addr[5],
					dev_role ));

			g_ble_subscriber.conn_id = p_conn_status->conn_id;
			wiced_gpio_output_high(WICED_LED1); wiced_gpio_output_low(WICED_LED2); // Green On & Red Off
		}
		else
		{
			WPRINT_APP_INFO(("P6 Robot Disconnected\n"));
			g_ble_subscriber.conn_id = 0;
			wiced_bt_ble_scan( BTM_BLE_SCAN_TYPE_HIGH_DUTY, WICED_TRUE, ble_subscriber_scan_result_cback );
			wiced_gpio_output_low(WICED_LED1); wiced_gpio_output_high(WICED_LED2); // Red On & Green Off
		}
		break;

	default:
		break;
	}

	return result;
}

/*
 * ble_subscriber_scan_result_cback
 *
 * This function process the advertising packets aka Scan Results... and looks for a BLE device that has the correct service
 * ... meaning a UUID that matches the MOTOR Service.
 *
 * If finds a device that matches... it
 *    - turns off scanning
 *    - and initiates a connection
 *
 */

void ble_subscriber_scan_result_cback( wiced_bt_ble_scan_results_t *p_scan_result, uint8_t *p_adv_data )
{
	uint8_t                length;
	uint8_t *              p_data;

	WPRINT_BT_APP_INFO(("Found Device : %X:%X:%X:%X:%X:%X \n", p_scan_result->remote_bd_addr[0], p_scan_result->remote_bd_addr[1], p_scan_result->remote_bd_addr[2],
			p_scan_result->remote_bd_addr[3], p_scan_result->remote_bd_addr[4], p_scan_result->remote_bd_addr[5] ));

	if ( p_scan_result )
	{
		// Advertisement data from hello_server should have Advertisement type SERVICE_UUID_128
		p_data = wiced_bt_ble_check_advertising_data( p_adv_data, BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE, &length );

		// Check if  the hello service uuid is there in the advertisement
		if ( ( p_data == NULL ) || ( length != LEN_UUID_128 ) || ( memcmp( p_data, hello_service, LEN_UUID_128 ) != 0 ) )
			return;

		WPRINT_BT_APP_INFO(("Found P6 Robot & Motor Service\n"));

		/* Stop the scan since the desired device is found */
		wiced_bt_ble_scan( BTM_BLE_SCAN_TYPE_NONE, WICED_TRUE, ble_subscriber_scan_result_cback );
		wiced_bt_gatt_le_connect( p_scan_result->remote_bd_addr, p_scan_result->ble_addr_type, BLE_CONN_MODE_HIGH_DUTY, TRUE );
	}
}

/*
 *
 * ble_subscriber_write_motor
 *
 *   If you are connected then it write the value into the correct characteristic HANDLE_MOTOR_M1_VAL or HANDLE_MOTOR_M2_VAL
 *
 *
 */
void ble_subscriber_write_motor (uint8_t motor, uint8_t val )
{
	if(g_ble_subscriber.conn_id == 0)
		return;

	WPRINT_BT_APP_INFO(("Motor M%d=0x%02X\n", motor,val));

	wiced_bt_gatt_value_t write_buffer;
	wiced_bt_gatt_value_t *p_write = &write_buffer;

	if(motor == 1)
		p_write->handle   = HANDLE_MOTOR_M1_VAL;
	else
		p_write->handle   = HANDLE_MOTOR_M2_VAL;

	p_write->offset   = 0;
	p_write->len      = 1;
	p_write->auth_req = GATT_AUTH_REQ_NONE;
	p_write->value[0] = val;

	wiced_bt_gatt_send_write ( g_ble_subscriber.conn_id, GATT_WRITE_NO_RSP, p_write );
}
