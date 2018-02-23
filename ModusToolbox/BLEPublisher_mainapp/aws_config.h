/*
 * $ Copyright Cypress Semiconductor $
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************
 *                      Macros
 ******************************************************/

/******************************************************
 *                    Constants
 ******************************************************/


#define WICED_AWS_IOT_HOST_NAME                     "amk6m51qrxr2u.iot.us-east-1.amazonaws.com"
#define WICED_AWS_IOT_PEER_COMMON_NAME              "*.iot.us-east-1.amazonaws.com"

//#define WICED_AWS_IOT_HOST_NAME                     "data.iot.us-east-1.amazonaws.com"
//#define WICED_AWS_IOT_PEER_COMMON_NAME              "*.iot.us-east-1.amazonaws.com"

/* Name of topic name */
#define WICED_TOPIC                                 "MOTOR_POSITION"
/* message content */
#define WICED_MESSAGE_STR                           "HELLO WICED"
/* client ID */
#define CLIENT_ID                                   "P6RobotPublisher"

#define WICED_APP_TIMEOUT                           (5000)

#define WICED_APP_DELAY_IN_MILLISECONDS             (1000)

#define MAX_RESOURCE_SIZE                           (0x7fffffff)

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
 *                 Global Variables
 ******************************************************/

/******************************************************
 *               Function Declarations
 ******************************************************/

#ifdef __cplusplus
} /*extern "C" */
#endif
