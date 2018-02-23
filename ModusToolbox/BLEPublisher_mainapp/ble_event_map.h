/*
 * $ Copyright Cypress Semiconductor $
 */

/** @file
 *
 * This file provides helper functions that turn event codes into human readable strings.
 * There is a map for events of types:
 *
 */
#ifndef ble_event_map_h
#define ble_event_map_h
#include <stdint.h>

/*
 * This function returns a string for Bluetooth GATT Events : tGATT_EVENT e.g  GATT_
 */
char*getEventEnumGATT(uint32_t event);

/*
 * This function returns a string for Bluetooth Management Events : tBTM_EVENT e.g  BTM_
 */
char*getEventEnumBTM(uint32_t event);

/*
 * This function returns a string for Bluetooth GATT Server Request Events :  tGATTS_REQ_TYPE e.g. GATTS_REQ_
 */

char*getEventEnumGATTS_REQ(uint32_t event);

/*
 * This function returns a string for Bluetooth Management Advertising Events :  tBTM_BLE_AVERT_MODE e.g. BTM_BLE_ADVERT_
 */
char*getEventEnumBTM_BLE_ADVERT(uint32_t event);


#endif
