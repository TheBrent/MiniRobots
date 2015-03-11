#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <NewPing.h>

//radio setup
RF24 radio(9,10);
const uint64_t pipes[2] = { 0x05F0F0F0E1LL, 0x05F0F0F0D2LL };
//receive_payload is expected to be right direction expressed as 'F' or 'R' followed by the speed expressed as 00-99 followed by the left direction and speed using the same format
char receive_payload[7];
char payload_right_speed[3];
char payload_left_speed[3];

//drive pins
int drive_right1 = 4;
int drive_right2 = 3;
int drive_right_enable = 5;
int drive_left1 = 8;
int drive_left2 = 7;
int drive_left_enable = 6;

//configure sonar
#define echoPin 2
#define trigPin 2
NewPing sonar(trigPin, echoPin);
long duration, distance;

//drive calibration, allow for adjustments for differences between motors
//calibration function to be written later, results to be stored in / retrieved from EEPROM
int drive_right_max = 255;
int drive_left_max = 255;

//sensors (if any), sensor_string length should be sensor count (times 5) plus 2 (allowing 5 bytes for each sensor, plus a leading character for results of last command and the terminating 0), assuming that the trailing 0 from each of the sensor strings would be dropped
char sensor_string[7] = {0,0,0,0,0,0,0};
char payload_result;

void setup() {
  pinMode(drive_right1, OUTPUT);
  pinMode(drive_right2, OUTPUT);
  pinMode(drive_right_enable, OUTPUT);
  pinMode(drive_left1, OUTPUT);
  pinMode(drive_left2, OUTPUT);
  pinMode(drive_left_enable, OUTPUT);
  
  radio.begin();
  radio.enableDynamicPayloads();
  radio.setRetries(1,1);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);
  radio.startListening();
  Serial.begin(57600);
}

void loop() {
    //sonar read
    duration = sonar.ping();
    distance = duration/US_ROUNDTRIP_CM;
    
    if(radio.available()) {
    uint8_t len;
    bool done = false;
    while(!done) {
      len = radio.getDynamicPayloadSize();
      done = radio.read(receive_payload,len);
    }
    //drive
    //read direction from the payload and set the right and left motor directions
    payload_result = '0';
    if(receive_payload[0]=='F') {
      digitalWrite(drive_right1, 0);
      digitalWrite(drive_right2, 1);
    } else if (receive_payload[0]=='R') {
      digitalWrite(drive_right1, 1);
      digitalWrite(drive_right2, 0);
    } else {
      payload_result = '1';
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
      payload_result = '1';
      digitalWrite(drive_left1, 0);
      digitalWrite(drive_left2, 0);
    }
    //read the right and left motor speeds from the payload as 00-99 and set speed as a percentage of defined max speed for each motor
    char right_speed_string[3] = {receive_payload[1], receive_payload[2]};
    int right_speed;
    if (atoi(right_speed_string)>2){
      right_speed = (atoi(right_speed_string)/99.0)*(drive_right_max-128)+128;
    } else {
      right_speed = 0;
    }
    char left_speed_string[3] = {receive_payload[4], receive_payload[5]};
    int left_speed;
    if (atoi(left_speed_string)>2){
      left_speed = (atoi(left_speed_string)/99.0)*(drive_left_max-128)+128;
    } else {
      left_speed = 0;
    }
    if(right_speed >= 0 && right_speed <= 255 && left_speed >= 0 && left_speed <= 255) {
      // everything good
    } else {
      if(payload_result=='1'){
        payload_result = '3';
      } else {
        payload_result = '2';
      }
    }
    //drive - delay defines the step duration
    analogWrite(drive_right_enable, right_speed);
    analogWrite(drive_left_enable, left_speed);
    
    //more sensors

    radio.stopListening();
    //format message to be sent into sensor_string
    sensor_string[0] = payload_result;
    sensor_string[1] = distance;
    Serial.print("Right:");
    Serial.print(right_speed);
    Serial.print("Left:");
    Serial.print(left_speed);
    Serial.print("sending:");
    Serial.println(sensor_string);
    radio.write(sensor_string,7);
    radio.startListening();
    delay(50);
    analogWrite(drive_right_enable, 0);
    analogWrite(drive_left_enable, 0);
  }
}

