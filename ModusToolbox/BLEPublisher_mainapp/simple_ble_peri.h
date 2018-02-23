/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * This file provides definitions and function prototypes for Hello Sensor
 * device
 *
 */
#include "wiced_bt_dev.h"

#ifdef __cplusplus
extern "C" {
#endif


/******************************************************************************
 *                                Constants
 ******************************************************************************/

/******************************************************************************
 *                          Constants
 ******************************************************************************/

#define UUID_MOTOR_SERVICE  0x27,0x74,0x70,0xF7,0x6D,0x4B,0x49,0x82,0x01,0x4A,0x9E,0xCD,0x56,0x04,0x55,0x51
#define UUID_MOTOR_M1 0x1B,0x0D,0x34,0x93,0x0F,0xFE,0xF5,0x97,0x28,0x47,0x44,0x6B,0xA7,0xC4,0x51,0x16
#define UUID_MOTOR_M2 0x6C,0xF6,0x11,0xD1,0xF7,0xF2,0xFB,0xBF,0x4B,0x41,0xFB,0x99,0xAF,0x03,0xDA,0x91

/******************************************************************************
 *                         Type Definitions
 ******************************************************************************/
typedef enum
{
    HANDLE_BASICPERI_GATT_SERVICE = 0x1,

    HANDLE_BASICPERI_GAP_SERVICE = 0x14,
        HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_NAME,
        HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_NAME_VAL,

        HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_APPEARANCE,
        HANDLE_BASICPERI_GAP_SERVICE_CHAR_DEV_APPEARANCE_VAL,

        HANDLE_MOTOR_SERVICE = 0x28,
                         HANDLE_MOTOR_M1,
                         HANDLE_MOTOR_M1_VAL,

                         HANDLE_MOTOR_M2,
                         HANDLE_MOTOR_M2_VAL


}simple_ble_peri_db_tags;

extern const wiced_bt_cfg_settings_t wiced_bt_cfg_settings;
extern const wiced_bt_cfg_buf_pool_t wiced_bt_cfg_buf_pools[];

