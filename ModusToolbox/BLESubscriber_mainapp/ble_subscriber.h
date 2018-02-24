#ifndef BLE_SUBSCRIBER_H
#define BLE_SUBSCRIBER_H

// These two function wil cause a write of the "val" to the BLE Characteristic
void ble_subscriber_write_motor_m1 ( uint8_t val );
void ble_subscriber_write_motor_m2 ( uint8_t val );

#endif
