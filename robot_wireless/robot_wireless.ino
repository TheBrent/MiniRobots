#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

RF24 radio(9,10);

const uint64_t pipe = 0xE8E8F0F0E1LL;

int DriveRight1 = 7;
int DriveRight2 = 8;
int DriveLeft1 = 4;
int DriveLeft2 = 5;
int DriveEnable = 6;

int forward[] = {0,1,0,1};
int reverse[] = {1,0,1,0};
int right[] = {1,0,0,1};
int left[] = {0,1,1,0};


int EncoderLeftCount = 0;
int EncoderRightCount = 0;
int EncoderLeftStop = 0;
int EncoderRightStop = 0;

void setup() {
  radio.begin();
  radio.openReadingPipe(1,pipe);
  radio.startListening();
  
  pinMode(DriveRight1, OUTPUT);
  pinMode(DriveRight2, OUTPUT);
  pinMode(DriveLeft1, OUTPUT);
  pinMode(DriveLeft2, OUTPUT);
  pinMode(DriveEnable, OUTPUT);
  
  attachInterrupt(0,EncoderLeft,CHANGE);
  attachInterrupt(1,EncoderRight,CHANGE);
}

void EncoderLeft() {
  ++EncoderLeftCount;
}

void EncoderRight() {
  ++EncoderRightCount;
}

void loop() {
  if (radio.available()){
    
}

void drive(int s, char d) {
  if (d == 'F') {
    digitalWrite(DriveLeft1, forward[0]);
    digitalWrite(DriveLeft2, forward[1]);
    digitalWrite(DriveRight1, forward[2]);
    digitalWrite(DriveRight2, forward[3]);
    analogWrite(DriveEnable, s);
  } else if (d == 'B') {
    digitalWrite(DriveLeft1, reverse[0]);
    digitalWrite(DriveLeft2, reverse[1]);
    digitalWrite(DriveRight1, reverse[2]);
    digitalWrite(DriveRight2, reverse[3]);
    analogWrite(DriveEnable, s);
  } else if (d == 'R') {
    digitalWrite(DriveLeft1, right[0]);
    digitalWrite(DriveLeft2, right[1]);
    digitalWrite(DriveRight1, right[2]);
    digitalWrite(DriveRight2, right[3]);
    analogWrite(DriveEnable, s);
  } else if (d == 'L') {
    digitalWrite(DriveLeft1, left[0]);
    digitalWrite(DriveLeft2, left[1]);
    digitalWrite(DriveRight1, left[2]);
    digitalWrite(DriveRight2, left[3]);
    analogWrite(DriveEnable, s);
  }
  delay(10);
  analogWrite(DriveEnalbe, 0);
}

