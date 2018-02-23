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

#define RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR( command, ok_message, error_message )   \
		{                                                                                   \
	ret = (command);                                                                \
	print_status( ret, (const char *)ok_message, (const char *)error_message );\
	if ( ret != WICED_SUCCESS ) return;                                              \
		}

/* Set one of these MACROS based on the desired role for this AWS application. */
#define AWS_IOT_MODE_PUBLISHER
//#define AWS_IOT_MODE_SUBSCRIBER

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

static void print_status( wiced_result_t restult, const char * ok_message, const char * error_message );

static wiced_result_t aws_data_callback( void *app_info, wiced_aws_event_type_t event_type, void *data1 );

/******************************************************
 *               Variable Definitions
 ******************************************************/
static wiced_aws_data_callback_t callbacks = aws_data_callback;
static     wiced_aws_security_t security;

/******************************************************
 *               Function Definitions
 ******************************************************/

wiced_result_t aws_init();


void aws_start( void )
{
	wiced_result_t ret = WICED_SUCCESS;

	//wiced_init( );

	/* Disable roaming to other access points */
	wiced_wifi_set_roam_trigger( -99 ); /* -99dBm ie. extremely low signal level */

	/* Bring up the network interface */
	ret = wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );
	if ( ret != WICED_SUCCESS )
	{
		WPRINT_APP_INFO( ( "\nNot able to join the requested AP\n\n" ) );
		return;
	}

	aws_init();
}



wiced_aws_app_info_t app_info;
wiced_aws_connect_t connection_parameters;

wiced_result_t aws_init()
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
	RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR( wiced_aws_connection_open( &app_info, callbacks, &connection_parameters ), NULL, "Did you configure you broker IP address?\n" );


	WPRINT_APP_INFO( ("[AWS] Subscribing...") );
	RUN_COMMAND_PRINT_STATUS_AND_BREAK_ON_ERROR( wiced_aws_subscribe( &app_info, WICED_TOPIC , WICED_MQTT_QOS_DELIVER_AT_MOST_ONCE ), NULL, NULL );

	return WICED_SUCCESS;
}


/******************************************************
 *               Static Function Definitions
 ******************************************************/

/*
 * A simple result log function
 */
static void print_status( wiced_result_t result, const char * ok_message, const char * error_message )
{
	if ( result == WICED_SUCCESS )
	{
		if ( ok_message != NULL )
		{
			WPRINT_APP_INFO( ( "OK (%s)\n\n", (ok_message)) );
		}
		else
		{
			WPRINT_APP_INFO( ( "OK.\n\n" ) );
		}
	}
	else
	{
		if ( error_message != NULL )
		{
			WPRINT_APP_INFO( ( "ERROR (%s)\n\n", (error_message)) );
		}
		else
		{
			WPRINT_APP_INFO( ( "ERROR.\n\n" ) );
		}
	}
}

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

		WPRINT_APP_INFO(("0 char = *%c*\n",msg->data[0]));
		WPRINT_APP_INFO(("1 char = *%c*\n",msg->data[1]));

		if(msg->data_len >= 6 && msg->data[0] == 'M' && (msg->data[1]  == '1' || msg->data[1] == '2')) // || msg->data[1] == '2'))
		{
			WPRINT_APP_INFO(("Found a motor message\n"));
			sscanf(&msg->data[5],"%2X",&val);
			WPRINT_APP_INFO(("Val = %2X\n",val));

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
