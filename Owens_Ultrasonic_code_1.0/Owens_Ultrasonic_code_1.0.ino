#include <NewPing.h>

#define echoPin 2 // Echo Pin
#define trigPin 2 // Trigger Pin
#define LEDPin 13 // Onboard LED
NewPing sonar(trigPin, echoPin);

int ARRRRRRRGH = 3;
int maximumRange = 20; // Maximum range needed
int minimumRange = 10; // Minimum range needed
long duration, distance; // Duration used to calculate distance

void setup() {
 Serial.begin (9600);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
}

void loop() {
 duration = sonar.ping();
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/US_ROUNDTRIP_CM;
 
 if (distance >= maximumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.print(distance);
 Serial.println("cm too far away!");
 digitalWrite(LEDPin, HIGH); 
 }
 else if (distance <= ARRRRRRRGH){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.print(distance);
 Serial.println("cm Get out of my face!");
 digitalWrite(LEDPin, HIGH);
delay(25);
digitalWrite(LEDPin, LOW);
delay(25);
digitalWrite(LEDPin, HIGH);
delay(25);
digitalWrite(LEDPin, LOW);
delay(25); 
digitalWrite(LEDPin, HIGH);

 }
 else if (distance <= minimumRange){
 /* Send a negative number to computer and Turn LED ON 
 to indicate "out of range" */
 Serial.print(distance);
 Serial.println("cm too close!");
 digitalWrite(LEDPin, HIGH);
 }
 else{
 /* Send the distance to the computer using Serial protocol, and
 turn LED OFF to indicate successful reading. */
 Serial.print(distance);
 Serial.println("cm");
 digitalWrite(LEDPin, LOW); 
 }
 
 //Delay 50ms before next reading.
 delay(250);
}
