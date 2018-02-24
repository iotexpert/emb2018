#ifndef BLE_SUBSCRIBER_H
#define BLE_SUBSCRIBER_H

// These two function wil cause a write of the "val" to the BLE Characteristic
void ble_subscriber_write_motor ( uint8_t motor,uint8_t val );
#endif
