

int DriveRight1 = 6;
int DriveRight2 = 7;
int DriveLeft1 = 4;
int DriveLeft2 = 5;
int DriveEnable = 9;
int EncoderLeftCount = 0;
int EncoderRightCount = 0;
int EncoderLeftStop = 0;
int EncoderRightStop = 0;

void setup() {
  Serial.begin(9600);
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
  int speed = 255;
  //Drive(speed, 'f', 100);
 // Drive(speed, 'l', 100);
 // Drive(speed, 'r', 100);
 // delay(1000);
  //Drive(speed, 'b', 100);
  //delay(1000);
}

void Drive(int speed, char d, int l) {
  int distance = 500;
  if (d == 'f') {
    EncoderLeftStop = EncoderLeftCount + l;
    EncoderRightStop = EncoderRightCount + l;
    while (EncoderLeftCount < EncoderLeftStop && EncoderRightCount < EncoderRightStop) {
      if (EncoderLeftStop - EncoderLeftCount > EncoderRightStop - EncoderRightCount) {
        int EncoderLeftStep = 5 + EncoderLeftCount;
        while (EncoderLeftCount < EncoderLeftStep) {
          digitalWrite(DriveLeft1, 0);
          digitalWrite(DriveLeft2, 1);
          digitalWrite(DriveRight1, 0);
          digitalWrite(DriveRight2, 0);
          analogWrite(DriveEnable, speed);
        }
        analogWrite(DriveEnable, 0);
      } else {
        int EncoderRightStep = 5 + EncoderRightCount;
        while (EncoderRightCount < EncoderRightStep) {
          digitalWrite(DriveLeft1, 0);
          digitalWrite(DriveLeft2, 0);
          digitalWrite(DriveRight1, 0);
          digitalWrite(DriveRight2, 1);
          analogWrite(DriveEnable, speed);
        }
        analogWrite(DriveEnable, 0);
      }
      Serial.print(d);
      Serial.print(" LC:");
      Serial.print(EncoderLeftCount);
      Serial.print(" LS:");
      Serial.print(EncoderLeftStop);
      Serial.print(" DL:");
      Serial.print(EncoderLeftStop - EncoderLeftCount);
      Serial.print(" RC:");
      Serial.print(EncoderRightCount);
      Serial.print(" RS:");
      Serial.print(EncoderRightStop);
      Serial.print(" DR:");
      Serial.print(EncoderRightStop - EncoderRightCount);
      Serial.print(" ERR:");
      Serial.println(EncoderLeftStop - EncoderRightStop);
    }
  } else if (d == 'b') {
    EncoderLeftStop = EncoderLeftCount + l;
    EncoderRightStop = EncoderRightCount + l;
    while (EncoderLeftCount < EncoderLeftStop && EncoderRightCount < EncoderRightStop) {
      if (EncoderLeftStop - EncoderLeftCount > EncoderRightStop - EncoderRightCount) {
        int EncoderLeftStep = 5 + EncoderLeftCount;
        while (EncoderLeftCount < EncoderLeftStep) {
          digitalWrite(DriveLeft1, 1);
          digitalWrite(DriveLeft2, 0);
          digitalWrite(DriveRight1, 0);
          digitalWrite(DriveRight2, 0);
          analogWrite(DriveEnable, speed);
        }
        analogWrite(DriveEnable, 0);
      } else {
        int EncoderRightStep = 5 + EncoderRightCount;
        while (EncoderRightCount < EncoderRightStep) {
          digitalWrite(DriveLeft1, 0);
          digitalWrite(DriveLeft2, 0);
          digitalWrite(DriveRight1, 1);
          digitalWrite(DriveRight2, 0);
          analogWrite(DriveEnable, speed);
        }
        analogWrite(DriveEnable, 0);
      }
      Serial.print(d);
      Serial.print(" LC:");
      Serial.print(EncoderLeftCount);
      Serial.print(" LS:");
      Serial.print(EncoderLeftStop);
      Serial.print(" DL:");
      Serial.print(EncoderLeftStop - EncoderLeftCount);
      Serial.print(" RC:");
      Serial.print(EncoderRightCount);
      Serial.print(" RS:");
      Serial.print(EncoderRightStop);
      Serial.print(" DR:");
      Serial.print(EncoderRightStop - EncoderRightCount);
      Serial.print(" ERR:");
      Serial.println(EncoderLeftStop - EncoderRightStop);
    }
  } else if (d == 'l') {
    digitalWrite(DriveRight1, 1);
    digitalWrite(DriveRight2, 0);
    digitalWrite(DriveLeft1, 0);
    digitalWrite(DriveLeft2, 0);
    analogWrite(DriveEnable, speed);
    delay(distance);
    analogWrite(DriveEnable, 0);
  } else if (d == 'r') {
    digitalWrite(DriveRight1, 0);
    digitalWrite(DriveRight2, 0);
    digitalWrite(DriveLeft1, 1);
    digitalWrite(DriveLeft2, 0);
    analogWrite(DriveEnable, speed);
    delay(distance);
    analogWrite(DriveEnable, 0);
  }
}
