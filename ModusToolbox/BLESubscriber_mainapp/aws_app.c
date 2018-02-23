/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 */

#include "wiced.h"
#include "aws_config.h"
#include "aws_common.h"

/******************************************************
 *                      Macros
 ******************************************************/


/******************************************************
 *                    Constants
 ******************************************************/

/******************************************************
 *                   Enumerations
 ******************************************************/

/******************************************************
 *                 Type Definitions
 ******************************************************/

/******************************************************
 *                    Structures
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

static wiced_result_t aws_data_callback( void *app_info, wiced_aws_event_type_t event_type, void *data1 );
static wiced_result_t aws_init();

/******************************************************
 *               Variable Definitions
 ******************************************************/
static wiced_aws_data_callback_t callbacks = aws_data_callback;
static wiced_aws_security_t security;
static wiced_aws_app_info_t app_info;
static wiced_aws_connect_t connection_parameters;



void aws_start( void )
{
	wiced_result_t ret = WICED_SUCCESS;

	WPRINT_APP_INFO(("Starting AWS\n"));
	/* Disable roaming to other access points */
	wiced_wifi_set_roam_trigger( -99 ); /* -99dBm ie. extremely low signal level */

	/* Bring up the network interface */
	do {
		ret = wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );
		WPRINT_APP_INFO( ( "\nNot able to join the requested AP\n\n" ) );
	} while (ret != WICED_SUCCESS);

	aws_init();
}


static wiced_result_t aws_init()
{
	wiced_result_t ret;
	uint32_t size_out;

	WPRINT_APP_INFO( ( "Resolving IP address of AWS cloud...\n" ) );
	ret = wiced_hostname_lookup( WICED_AWS_IOT_HOST_NAME, &app_info.broker_address, 10000, WICED_STA_INTERFACE );
	WPRINT_APP_INFO( ("Resolved Broker IP: %u.%u.%u.%u\n\n", (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 24), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 16), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 8), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 0)) );
	if ( ret == WICED_ERROR || app_info.broker_address.ip.v4 == 0 )
	{
		WPRINT_APP_INFO( ("Error in resolving DNS\n") );
		return ret;
	}

	/* Get AWS root certificate, client certificate and private key respectively */
	resource_get_readonly_buffer( &myaws_iot_app_DIR_resources_rootca_cer	, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.ca_cert );
	security.ca_cert_len = size_out;

	resource_get_readonly_buffer( &myaws_iot_app_DIR_resources_client_cer, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.cert );
	if ( size_out < 64 )
	{

		WPRINT_APP_INFO( ( "\nNot a valid Certificate! Please replace the dummy certificate file 'resources/app/aws_iot/client.cer' with the one got from AWS\n\n" ) );
		return WICED_ERROR;
	}
	security.cert_len = size_out;

	resource_get_readonly_buffer( &myaws_iot_app_DIR_resources_privkey_cer, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.key );
	if ( size_out < 64 )
	{
		WPRINT_APP_INFO( ( "\nNot a valid Private Key! Please replace the dummy private key file 'resources/app/aws_iot/privkey.cer' with the one got from AWS\n\n" ) );
		return WICED_ERROR;
	}

	security.key_len = size_out;

	wiced_aws_app_init( &app_info, WICED_MQTT_PROTOCOL );

	connection_parameters.client_id = (uint8_t*) CLIENT_ID;
	connection_parameters.connection_timeout = WICED_APP_TIMEOUT;
	connection_parameters.password = NULL;
	connection_parameters.username = NULL;
	connection_parameters.port_number = 0;
	connection_parameters.security = &security;
	connection_parameters.peer_cn = (uint8_t*) WICED_AWS_IOT_PEER_COMMON_NAME;


	WPRINT_APP_INFO( ("[AWS] Opening connection...  ") );
	wiced_aws_connection_open( &app_info, callbacks, &connection_parameters );

	WPRINT_APP_INFO( ("[AWS] Subscribing...") );
	wiced_aws_subscribe( &app_info, WICED_TOPIC , WICED_MQTT_QOS_DELIVER_AT_MOST_ONCE );

	return WICED_SUCCESS;
}


/******************************************************
 *               Static Function Definitions
 ******************************************************/
static wiced_result_t aws_data_callback( void *app_info, wiced_aws_event_type_t event_type ,void *data )
{
	wiced_aws_data_info_t *msg = (wiced_aws_data_info_t *) data;

	uint8_t val;

	switch ( event_type )
	{
	case WICED_AWS_EVENT_TYPE_DISCONNECTED:
		break;
	case WICED_AWS_EVENT_TYPE_DATA_RECEIVED:
	{
		WPRINT_APP_INFO( ( "[AWS] Received %.*s  for TOPIC : %.*s  len=%d\n\n", (int) msg->data_len, msg->data, (int) msg->topic_len, msg->topic, msg->data_len ) );

		// This is beyond hard coded... if I catch you doing this... bad bad bad
		if(msg->data_len >= 6 && msg->data[0] == 'M' && (msg->data[1]  == '1' || msg->data[1] == '2')) // || msg->data[1] == '2'))
		{
			sscanf(&msg->data[5],"%2X",&val); // Pure unadulterated evil....
			switch(msg->data[1])
			{
			case '1':
			    hello_client_write_motor_m1(val);
			    break;

			case'2':
			    hello_client_write_motor_m2(val);
				break;

			default:
				break;
			}
		}
	}
	break;
	case WICED_AWS_EVENT_TYPE_CONNECT_REQ_STATUS:
	case WICED_AWS_EVENT_TYPE_UNKNOWN:
	default:
		break;

	}
	return WICED_SUCCESS;
}
