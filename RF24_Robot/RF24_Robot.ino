//it compliles, but there is a missing chunk of code for formatting the sensor readings...
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

//radio setup
RF24 radio(9,10);
const uint64_t pipes[2] = { 0xB5F0F0F0E1LL, 0xB5F0F0F0D2LL };
//receive_payload is expected to be right direction expressed as 'F' or 'R' followed by the speed expressed as 00-99 followed by the left direction and speed using the same format
char receive_payload[7];
char payload_right_speed[3];
char payload_left_speed[3];

//drive pins
int drive_right1 = 0;
int drive_right2 = 1;
int drive_right_enable = 5;
int drive_left1 = 7;
int drive_left2 = 8;
int drive_left_enable = 6;

//drive calibration, allow for adjustments for differences between motors
//calibration function to be written later, results to be stored in / retrieved from EEPROM
int drive_right_max = 255;
int drive_left_max = 255;

//sensors (if any), sensor_string length should be sensor count times 5 plus 1 (allowing 5 bytes for each sensor, plus the terminating 0), assuming that the trailing 0 from each of the sensor strings would be dropped
char sensor_string[11];
char sensor1[6];
char sensor2[6];
int encoder_right_count = 0;
int encoder_right_last = 0;
int encoder_right_step = 0;
int encoder_left_count = 0;
int encoder_left_last = 0;
int encoder_left_step = 0;

void setup() {
  pinMode(drive_right1, OUTPUT);
  pinMode(drive_right2, OUTPUT);
  pinMode(drive_right_enable, OUTPUT);
  pinMode(drive_left1, OUTPUT);
  pinMode(drive_left2, OUTPUT);
  pinMode(drive_left_enable, OUTPUT);
  attachInterrupt(0, encoder_right, CHANGE);
  attachInterrupt(1, encoder_left, CHANGE);
  
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(15,15);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
}

void encoder_right() {
  ++encoder_right_count;
}

void encoder_left() {
  ++encoder_left_count;
}

void loop() {
  if(radio.available()) {
    uint8_t len;
    bool done = false;
    while(!done) {
      len = radio.getDynamicPayloadSize();
      done = radio.read(receive_payload,len);
    }
    //encoder read / set last
    encoder_right_last = encoder_right_count;
    encoder_left_last = encoder_left_count;
    //drive
    //read direction from the payload and set the right and left motor directions
    if(receive_payload[0]=='F') {
      digitalWrite(drive_right1, 0);
      digitalWrite(drive_right2, 1);
    } else if (receive_payload[0]=='R') {
      digitalWrite(drive_right1, 1);
      digitalWrite(drive_right2, 0);
    } else {
      digitalWrite(drive_right1, 0);
      digitalWrite(drive_right2, 0);
    }
    if(receive_payload[3]=='F') {
      digitalWrite(drive_left1, 0);
      digitalWrite(drive_left2, 1);
    } else if (receive_payload[3]=='R') {
      digitalWrite(drive_left1, 1);
      digitalWrite(drive_left2, 0);
    } else {
      digitalWrite(drive_left1, 0);
      digitalWrite(drive_left2, 0);
    }
    //read the right and left motor speeds from the payload as 00-99 and set speed as a percentage of defined max speed for each motor
    char right_speed_string[3] = {receive_payload[1], receive_payload[2]};
    int right_speed = (atoi(right_speed_string)/99)*drive_right_max;
    char left_speed_string[3] = {receive_payload[4], receive_payload[5]};
    int left_speed = (atoi(left_speed_string)/99)*drive_left_max;
    //drive - delay defines the step duration
    analogWrite(drive_right_enable, right_speed);
    analogWrite(drive_left_enable, left_speed);
    delay(10);
    analogWrite(drive_right_enable, 0);
    analogWrite(drive_left_enable, 0);
    //encoder read / calc distance
    encoder_right_step = encoder_right_count - encoder_right_last;
    encoder_left_step = encoder_left_count - encoder_left_last;
    //MISSING CODE!!
    //need to take the encoder_right_step and encoder_left_step int values, convert to strings and store in the char arrays sensor1 and sensor2 respectivly...
    //thinking 5 char for each sensor should be enough, as we're reading between each step...  formatting idea is first char '+' if direction is 'F' and '-' if direction is 'R'
    //then filling the remaining 4 char with the count from the encoder step calc (left filling with zeros).  example forward with a count of 150 on one sensor would be '+0150'
    radio.stopListening();
    //format message to be sent into sensor_string
    char sensor_string[11] = {sensor1[0], sensor1[1], sensor1[2], sensor1[3], sensor1[4], sensor2[0], sensor2[1], sensor2[2], sensor2[3], sensor2[4]};
    radio.write(sensor_string,11);
    radio.startListening();
  }
}

