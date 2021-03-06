/*
 * $ Copyright Cypress Semiconductor $
 */

#include <string.h>

#include "wiced_bt_dev.h"
#include "wiced_bt_ble.h"
#include "wiced_bt_gatt.h"
#include "wiced_bt_cfg.h"
#include "wiced.h"

#include "bt_target.h"
#include "wiced_bt_stack.h"
#include "gattdefs.h"
#include "sdpdefs.h"
#include "simple_ble_peri.h"
#include "wwd_debug.h"

#include "ble_event_map.h"
#include "aws_app.h"

// Activate this macro to follow the Function prints
//#define FUNCTION_PRINT(args) WPRINT_MACRO(args)
#define FUNCTION_PRINT(args)

/******************************************************************************
 *                                Constants
******************************************************************************/

/******************************************************************************
 *                           Function Prototypes
 ******************************************************************************/
static void                     simple_ble_peri_application_init                   ( void );
static wiced_result_t           simple_ble_peri_management_callback                ( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data );
static wiced_bt_gatt_status_t   simple_ble_peri_gatts_callback                     ( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data);
static wiced_bt_gatt_status_t   simple_ble_peri_gatt_server_read_request_handler   ( uint16_t conn_id, wiced_bt_gatt_read_t * p_read_data );
static wiced_bt_gatt_status_t   simple_ble_peri_gatt_server_write_request_handler  ( uint16_t conn_id, wiced_bt_gatt_write_t * p_data );
static void                     simple_ble_peri_set_advertisement_data             ( void );


/******************************************************************************
 *                                Structures
 ******************************************************************************/

typedef struct
{
    BD_ADDR         remote_addr;                            /* remote peer device address */
    uint16_t        conn_id;                                /* connection ID referenced by the stack */
} simple_ble_peri_state_t;

typedef PACKED struct
{
	uint8_t m1;
	uint8_t m2;

} host_info_t;

typedef struct
{
    uint16_t        handle;
    uint16_t        attr_len;
    void*           p_attr;
} attribute_t;

/******************************************************************************
 *                                Variables Definitions
 ******************************************************************************/

static uint8_t simple_ble_peri_device_name[]          = "Hello";
static uint8_t simple_ble_peri_appearance_name[2]     = { BIT16_TO_8(APPEARANCE_GENERIC_TAG) };
static host_info_t              simple_ble_peri_hostinfo;
static simple_ble_peri_state_t     simple_ble_peri_state;

attribute_t gatt_user_attributes[] =
{
    { HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_NAME_VAL,       sizeof( simple_ble_peri_device_name ),         simple_ble_peri_device_name },
    { HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL, sizeof(simple_ble_peri_appearance_name),       simple_ble_peri_appearance_name },
	{ HANDLE_MOTOR_M1_VAL,             1,     (void *)&simple_ble_peri_hostinfo.m1 },
	{ HANDLE_MOTOR_M2_VAL,             1,     (void *)&simple_ble_peri_hostinfo.m2 }

};

/******************************************************************************
 *                                GATT DATABASE
 ******************************************************************************/
/*
 * This is the GATT database for the Hello Sensor application.  It defines
 * services, characteristics and descriptors supported by the sensor.  Each
 * attribute in the database has a handle, (characteristic has two, one for
 * characteristic itself, another for the value).  The handles are used by
 * the peer to access attributes, and can be used locally by application for
 * example to retrieve data written by the peer.  Definition of characteristics
 * and descriptors has GATT Properties (read, write, notify...) but also has
 * permissions which identify if and how peer is allowed to read or write
 * into it.  All handles do not need to be sequential, but need to be in
 * ascending order.
 */
const uint8_t simple_ble_peri_gatt_database[]=
{
		/* Declare mandatory GATT service */
		PRIMARY_SERVICE_UUID16( HANDLE_BASICPERI_GATT_SERVICE, UUID_SERVCLASS_GATT_SERVER ),

		/* Declare mandatory GAP service. Device Name and Appearance are mandatory
		 * characteristics of GAP service                                        */
		PRIMARY_SERVICE_UUID16( HANDLE_BASICPERI_GAP_SERVICE, UUID_SERVCLASS_GAP_SERVER ),

		/* Declare mandatory GAP service characteristic: Dev Name */
		CHARACTERISTIC_UUID16( HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_NAME, HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_NAME_VAL,
				GATT_UUID_GAP_DEVICE_NAME, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE ),

				/* Declare mandatory GAP service characteristic: Appearance */
		CHARACTERISTIC_UUID16( HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_APPEARANCE, HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,
				GATT_UUID_GAP_ICON, LEGATTDB_CHAR_PROP_READ, LEGATTDB_PERM_READABLE ),

                /* Declare proprietary Hello Service with 128 byte UUID */
PRIMARY_SERVICE_UUID128( HANDLE_MOTOR_SERVICE, UUID_MOTOR_SERVICE ),

           /* Declare characteristic Hello Configuration */
   CHARACTERISTIC_UUID128_WRITABLE( HANDLE_MOTOR_M1,
                   HANDLE_MOTOR_M1_VAL,
                   UUID_MOTOR_M1,
                   (LEGATTDB_CHAR_PROP_WRITE ),
                   (LEGATTDB_PERM_WRITE_REQ  ) ),

                                           /* Declare characteristic Hello Configuration */
   CHARACTERISTIC_UUID128_WRITABLE( HANDLE_MOTOR_M2,
                   HANDLE_MOTOR_M2_VAL,
                   UUID_MOTOR_M2,
                   (LEGATTDB_CHAR_PROP_WRITE ),
                   (LEGATTDB_PERM_WRITE_REQ )),


};
/******************************************************************************
 *                          Function Definitions
 ******************************************************************************/

/*
 *  Entry point to the application. Set device configuration and start BT
 *  stack initialization.  The actual application initialization will happen
 *  when stack reports that BT device is ready.
 */
void application_start( void )
{
    wiced_init();
    WPRINT_APP_INFO( ( "--------- Started BLE Peripheral & BLE Publisher ----------\n" ) );

    /* Register call back and configuration with stack */
    wiced_bt_stack_init( simple_ble_peri_management_callback ,  &wiced_bt_cfg_settings, wiced_bt_cfg_buf_pools );

    // Turn on the red and turn the green off
    wiced_gpio_output_low(WICED_LED1);
    wiced_gpio_output_high(WICED_LED2);

    aws_start();
}


/*
 * This function is executed in the BTM_ENABLED_EVT management callback.
 */
static void simple_ble_peri_application_init( void )
{
    FUNCTION_PRINT( ( "---------FCN: simple_ble_peri_application_init\n" ) );

    /* Register with stack to receive GATT callback */
    wiced_bt_gatt_register( simple_ble_peri_gatts_callback );

    /*  Tell stack to use our GATT database */
    wiced_bt_gatt_db_init( simple_ble_peri_gatt_database, sizeof( simple_ble_peri_gatt_database ) );

    /* Set the advertising parameters and make the device discoverable */
    simple_ble_peri_set_advertisement_data();

    wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );

}

/*
 * Setup advertisement data with 16 byte UUID and device name
 */
static void simple_ble_peri_set_advertisement_data(void)
{
	FUNCTION_PRINT(("---------FCN: simple_ble_peri_set_advertisement_data\n"));
    wiced_bt_ble_advert_elem_t  adv_elem[3]; // Be Careful here... if you overwrite this array bad things can happen
    uint8_t ble_advertisement_flag_value        = BTM_BLE_GENERAL_DISCOVERABLE_FLAG | BTM_BLE_BREDR_NOT_SUPPORTED;
    uint8_t num_elem                            = 0;
    uint8_t motor_service_uuid[LEN_UUID_128]    = { UUID_MOTOR_SERVICE };

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_FLAG;
    adv_elem[num_elem].len          = 1;
    adv_elem[num_elem].p_data       = &ble_advertisement_flag_value;
    num_elem ++;

    adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_NAME_COMPLETE;
    adv_elem[num_elem].len          = strlen((const char *)wiced_bt_cfg_settings.device_name);
    adv_elem[num_elem].p_data       = (uint8_t *)wiced_bt_cfg_settings.device_name;
    num_elem++;

    	adv_elem[num_elem].advert_type  = BTM_BLE_ADVERT_TYPE_128SRV_COMPLETE;
    adv_elem[num_elem].len          = LEN_UUID_128;
    adv_elem[num_elem].p_data       = motor_service_uuid;
    num_elem ++;
    wiced_bt_ble_set_raw_advertisement_data( num_elem, adv_elem );
}


/*
 * simple_ble_peri bt/ble link management callback
 */
static wiced_result_t simple_ble_peri_management_callback( wiced_bt_management_evt_t event, wiced_bt_management_evt_data_t *p_event_data )
{
	wiced_result_t                   result = WICED_BT_SUCCESS;
	wiced_bt_ble_advert_mode_t*      p_mode;

	FUNCTION_PRINT(("---------FCN: simple_ble_peri_management_callback: 0x%x=%s\n", event,getEventEnumBTM(event) ));

	switch( event )
	{
	/* Bluetooth  stack enabled */
	case BTM_ENABLED_EVT:
		simple_ble_peri_application_init( );
		break;

	case BTM_BLE_ADVERT_STATE_CHANGED_EVT:
		p_mode = &p_event_data->ble_advert_state_changed;
		WPRINT_APP_INFO(( "Advertisement State Change: %s = %d \n", getEventEnumBTM_BLE_ADVERT(*p_mode),*p_mode));
		break;

	default:
		break;
	}

	return result;
}

/*
 * Find attribute description by handle
 */
static attribute_t * simple_ble_peri_get_attribute( uint16_t handle )
{
	FUNCTION_PRINT(("---------FCN: simple_ble_peri_get_attribute Handle=%X\n",handle ));

    int i;
    for ( i = 0; i <  sizeof( gatt_user_attributes ) / sizeof( gatt_user_attributes[0] ); i++ )
    {
        if ( gatt_user_attributes[i].handle == handle )
        {
            return ( &gatt_user_attributes[i] );
        }
    }
    WPRINT_BT_APP_INFO(( "attribute not found:%x\n", handle ));
    return NULL;
}

/*
 * Process Read request or command from peer device
 */
static wiced_bt_gatt_status_t simple_ble_peri_gatt_server_read_request_handler( uint16_t conn_id, wiced_bt_gatt_read_t * p_read_data )
{
	FUNCTION_PRINT(("---------FCN: simple_ble_peri_gatt_server_read_request_handler Read Handle=0x%X\n", p_read_data->handle ));
    attribute_t *puAttribute;
    int          attr_len_to_copy;

    if ( ( puAttribute = simple_ble_peri_get_attribute(p_read_data->handle) ) == NULL)
    {
        WPRINT_BT_APP_INFO(("read_hndlr attr not found hdl:%x\n", p_read_data->handle ));
        return WICED_BT_GATT_INVALID_HANDLE;
    }


    attr_len_to_copy = puAttribute->attr_len;


    if ( p_read_data->offset >= puAttribute->attr_len )
    {
        attr_len_to_copy = 0;
    }

    if ( attr_len_to_copy != 0 )
    {
        uint8_t *from;
        int      to_copy = attr_len_to_copy - p_read_data->offset;


        if ( to_copy > *p_read_data->p_val_len )
        {
            to_copy = *p_read_data->p_val_len;
        }

        from = ((uint8_t *)puAttribute->p_attr) + p_read_data->offset;
        *p_read_data->p_val_len = to_copy;

        memcpy( p_read_data->p_val, from, to_copy);
    }

    return WICED_BT_GATT_SUCCESS;
}

/*
 * Process write request or write command from peer device
 */
static wiced_bt_gatt_status_t simple_ble_peri_gatt_server_write_request_handler( uint16_t conn_id, wiced_bt_gatt_write_t * p_data )
{
	char buff[128];
    wiced_bt_gatt_status_t result    = WICED_BT_GATT_SUCCESS;
    uint8_t                *p_attr   = p_data->p_val;

    FUNCTION_PRINT(("---------FCN: write_handler: conn_id:%d hdl:0x%x prep:%d offset:%d len:%d\n ", conn_id, p_data->handle, p_data->is_prep, p_data->offset, p_data->val_len
    ));

    switch ( p_data->handle )
    {

    case HANDLE_MOTOR_M1_VAL:
    		FUNCTION_PRINT(("BLE GATT Wrote M1=%02X\r\n",p_attr[0]));
    		simple_ble_peri_hostinfo.m1 = p_attr[0];
    		sprintf(buff,"M1=0x%02X",p_attr[0]);
    		robot_publish(buff);
    		break;

    case HANDLE_MOTOR_M2_VAL:
    		FUNCTION_PRINT(("BLE GATT Wrote M2=%02X\r\n",p_attr[0]));
    		simple_ble_peri_hostinfo.m1 = p_attr[0];
    		sprintf(buff,"M2=0x%02X",p_attr[0]);
    		robot_publish(buff);
    		break;

    default:
        result = WICED_BT_GATT_INVALID_HANDLE;
        break;
    }
    return result;
}

/* This function is invoked when connection is established */
static wiced_bt_gatt_status_t simple_ble_peri_gatts_connection_up( wiced_bt_gatt_connection_status_t *p_status )
{
	FUNCTION_PRINT( ( "---------FCN: simple_ble_peri_gatts_connection_up  id:%d\n", p_status->conn_id) );
    /* Update the connection handler.  Save address of the connected device. */
    simple_ble_peri_state.conn_id = p_status->conn_id;
    memcpy(simple_ble_peri_state.remote_addr, p_status->bd_addr, sizeof(BD_ADDR));

    /* Stop advertising */
    wiced_bt_start_advertisements( BTM_BLE_ADVERT_OFF, 0, NULL );

    // Turn on the red and turn the green off
    wiced_gpio_output_high(WICED_LED1);
    wiced_gpio_output_low(WICED_LED2);

    return WICED_BT_GATT_SUCCESS;
}

/*
 * This function is invoked when connection is lost
 */
static wiced_bt_gatt_status_t simple_ble_peri_gatts_connection_down( wiced_bt_gatt_connection_status_t *p_status )
{
    wiced_result_t result;
    FUNCTION_PRINT( ( "---------FCN: simple_ble_peri_gatts_connection_down  conn_id:%d reason:%d\n", p_status->conn_id, p_status->reason ) );

    /* Resetting the device info */
    memset( simple_ble_peri_state.remote_addr, 0, 6 );
    simple_ble_peri_state.conn_id = 0;

    result =  wiced_bt_start_advertisements( BTM_BLE_ADVERT_UNDIRECTED_HIGH, 0, NULL );
    WPRINT_BT_APP_INFO( ( "Restarting Advertisements %d\n", result ) );

    // Turn on the red and turn the green off
    wiced_gpio_output_low(WICED_LED1);
    wiced_gpio_output_high(WICED_LED2);

    return WICED_BT_SUCCESS;
}

/*
 * Process GATT request from the peer
 */
static wiced_bt_gatt_status_t simple_ble_peri_gatt_server_request_handler( wiced_bt_gatt_attribute_request_t *p_data )
{
	wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

	FUNCTION_PRINT(( "---------FCN: simple_ble_peri_gatt_server_request_handler. conn %d, type %d %s\n", p_data->conn_id, p_data->request_type,getEventEnumGATTS_REQ(p_data->request_type) ));

	switch ( p_data->request_type )
	{
	case GATTS_REQ_TYPE_READ:
		result = simple_ble_peri_gatt_server_read_request_handler( p_data->conn_id, &(p_data->data.read_req) );
		break;

	case GATTS_REQ_TYPE_WRITE:
		result = simple_ble_peri_gatt_server_write_request_handler( p_data->conn_id, &(p_data->data.write_req) );
		break;

	case GATTS_REQ_TYPE_MTU:
		FUNCTION_PRINT(("Requested MTU: %d\n", p_data->data.mtu));
		break;

	default:
		break;
	}
	return result;
}

/*
 * Callback for various GATT events.  As this application performs only as a GATT server, some of
 * the events are ommitted.
 */
static wiced_bt_gatt_status_t simple_ble_peri_gatts_callback( wiced_bt_gatt_evt_t event, wiced_bt_gatt_event_data_t *p_data)
{
	FUNCTION_PRINT(("---------FCN: simple_ble_peri_gatts_callback EVT=%s\n",getEventEnumGATT(event)));

	wiced_bt_gatt_status_t result = WICED_BT_GATT_INVALID_PDU;

	switch(event)
	{
	case GATT_CONNECTION_STATUS_EVT:
		if (p_data->connection_status.connected)
			result = simple_ble_peri_gatts_connection_up( &p_data->connection_status );
		else
		    result = simple_ble_peri_gatts_connection_down( &p_data->connection_status );
		break;

	case GATT_ATTRIBUTE_REQUEST_EVT:
		result = simple_ble_peri_gatt_server_request_handler( &p_data->attribute_request );
		break;
	default:
		break;
	}
	return result;
}


