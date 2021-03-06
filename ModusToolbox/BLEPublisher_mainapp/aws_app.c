
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
wiced_result_t aws_init();


/******************************************************
 *               Variable Definitions
 ******************************************************/
static wiced_aws_data_callback_t callbacks = aws_data_callback;
static wiced_aws_security_t security;
static wiced_aws_app_info_t app_info;
static wiced_aws_connect_t connection_parameters;
static wiced_bool_t connected = WICED_FALSE;

/******************************************************
 *               Function Definitions
 ******************************************************/


wiced_result_t aws_start( void )
{
	wiced_result_t ret = WICED_SUCCESS;
	uint32_t size_out;

	WPRINT_APP_INFO(("Starting AWS - Publisher\n"));

	/* Disable roaming to other access points */
	wiced_wifi_set_roam_trigger( -99 ); /* -99dBm ie. extremely low signal level */

	/* Bring up the network interface */
	do {
		ret = wiced_network_up( WICED_STA_INTERFACE, WICED_USE_EXTERNAL_DHCP_SERVER, NULL );
		if(ret != WICED_SUCCESS)
			WPRINT_APP_INFO(("Failed to network up\n"));
	} while(ret != WICED_SUCCESS);


	do {
	WPRINT_APP_INFO( ( "Resolving IP address of AWS cloud...%s\n", WICED_AWS_IOT_HOST_NAME ) );
	ret = wiced_hostname_lookup( WICED_AWS_IOT_HOST_NAME, &app_info.broker_address, 10000, WICED_STA_INTERFACE );
	WPRINT_APP_INFO( ("Resolved Broker IP: %u.%u.%u.%u\n\n", (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 24), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 16), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 8), (uint8_t)(GET_IPV4_ADDRESS(app_info.broker_address) >> 0)) );
	if ( ret == WICED_ERROR || app_info.broker_address.ip.v4 == 0 )
	{
		WPRINT_APP_INFO( ("Error in resolving DNS\n") );
	}
	}while (ret != WICED_SUCCESS);

	/* Get AWS root certificate, client certificate and private key respectively */
	resource_get_readonly_buffer( &examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_rootca_cer, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.ca_cert );
	security.ca_cert_len = size_out;

	resource_get_readonly_buffer( &examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_client_cer, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.cert );
	if ( size_out < 64 )
	{

		WPRINT_APP_INFO( ( "\nNot a valid Certificate! Please replace the dummy certificate file 'resources/app/aws_iot/client.cer' with the one got from AWS\n\n" ) );
		return WICED_ERROR;
	}
	security.cert_len = size_out;

	resource_get_readonly_buffer( &examples_DIR_apps_DIR_snips_DIR_aws_iot_app_DIR_resources_privkey_cer, 0, MAX_RESOURCE_SIZE, &size_out, (const void **) &security.key );
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


	do {
		WPRINT_APP_INFO( ("[AWS] Opening connection... ") );
		ret = wiced_aws_connection_open( &app_info, callbacks, &connection_parameters );
		if(ret == WICED_SUCCESS)
			WPRINT_APP_INFO(("Success\n"));
		else
			WPRINT_APP_INFO(("Failed\n"));
	} while (ret != WICED_SUCCESS);

	connected = WICED_TRUE;
	return WICED_SUCCESS;
}


void aws_reconnect()
{

	wiced_aws_connection_close(&app_info);

	wiced_result_t ret;
	do {
			WPRINT_APP_INFO( ("[AWS] Opening connection... ") );
			ret = wiced_aws_connection_open( &app_info, callbacks, &connection_parameters );
			if(ret == WICED_SUCCESS)
			{
				WPRINT_APP_INFO(("Success\n"));
				connected = 1;
			}
				else
			{
				wiced_rtos_delay(100);
				WPRINT_APP_INFO(("Failed\n"));
			}
		} while (ret != WICED_SUCCESS);
}

void robot_publish(char *message)
{
	static int pub_in_progress = 0;
	wiced_result_t result;
	int retry=0;

	if(connected == WICED_FALSE || pub_in_progress)
		return;

	WPRINT_APP_INFO(("Message = %s\n",message));
	do {
		pub_in_progress = 1;
		result = wiced_aws_publish( &app_info, WICED_MQTT_QOS_DELIVER_AT_LEAST_ONCE, (uint8_t*)WICED_TOPIC,(uint8_t *)message,(uint32_t)strlen(message));
		retry += 1;
	} while(retry <5 && result != WICED_SUCCESS);


	if(result != WICED_SUCCESS)
	{
		connected = 0;
		pub_in_progress = 0;
		WPRINT_APP_INFO(("Publish failed\n"));
		// Reconnect
		aws_reconnect();
	}
	else
	{
		pub_in_progress = 0;
	}

}


static wiced_result_t aws_data_callback( void *app_info, wiced_aws_event_type_t event_type ,void *data )
{

	switch ( event_type )
	{
	case WICED_AWS_EVENT_TYPE_DISCONNECTED:
			WPRINT_APP_INFO(("WICED_AWS_EVENT_TYPE_DISCONNECTED\n"));
		break;
	case WICED_AWS_EVENT_TYPE_DATA_RECEIVED: // Only publishing
	break;
	case WICED_AWS_EVENT_TYPE_CONNECT_REQ_STATUS:
	case WICED_AWS_EVENT_TYPE_UNKNOWN:
	default:
		break;

	}
	return WICED_SUCCESS;
}
