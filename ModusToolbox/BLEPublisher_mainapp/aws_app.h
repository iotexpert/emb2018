#ifndef AWS_APP_H
#define AWS_APP_H


void aws_start(void); // Turn on WiFi, Connect to AWS, Create the MQTT Connection

void robot_publish(char *message); // Send a message to the Subscriber

#endif
