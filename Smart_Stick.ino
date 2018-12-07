#include <TinyGPS.h>
#include <SoftwareSerial.h>
SoftwareSerial Gsm(10, 11);
char phone_no[] = "9790222634"; //replace with phone no. to get sms
 
TinyGPS gps;  //Creates a new instance of the TinyGPS object
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const int motorPin = 3;
const int ledPin = 8;
const int ldrPin = A0;
 
void setup()
{
  Serial.begin(9600);
  Gsm.begin(9600);  
  pinMode(motorPin, OUTPUT);
  pinMode(ledPin,OUTPUT);
  pinMode(ldrPin,INPUT);
}
 
void loop()
{
  int ldrStatus = analogRead(ldrPin);
  if(ldrStatus<=400){
    digitalWrite(ledPin,HIGH);
    Serial.print("Its DARK, Turn on the LED: ");
    Serial.println(ldrStatus);
    }
  else{
    digitalWrite(ledPin,LOW);
    Serial.print("Its BRIGHT, Turn off the LED: ");
    Serial.println(ldrStatus);
    }
   long duration, inches, cm;
   pinMode(pingPin, OUTPUT);
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   pinMode(echoPin, INPUT);
   duration = pulseIn(echoPin, HIGH);
   //inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
   if (cm < 50)
   {
    digitalWrite(motorPin, HIGH);
    delay(1000);
    digitalWrite(motorPin, LOW);
    delay(1000);
   }
   
   Serial.print(cm);
   Serial.print("cm");
   Serial.println();

  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
 
  // For one second we parse GPS data and report some key values
  
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      Serial.print(c);
      if (gps.encode(c)) 
        newData = true;  
    }
  }
 
  if (newData)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);   
    Gsm.print("AT+CMGF=1\r"); 
    Serial.print("sending");
    delay(400);
    Gsm.print("AT+CMGS=\"");
    Serial.print("phone");
    Gsm.print(phone_no);
    Gsm.println("\"");
    
    delay(300);
    Gsm.print("http://maps.google.com/maps?q=");
    Serial.print("sent");
    
   // Gsm.print("Latitude = ");
    Gsm.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    //Gsm.print(" Longitude = ");
    Gsm.print(",");
    Gsm.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(200);
    Gsm.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(200);
    Gsm.println();
    delay(10000);
  }
 
  Serial.println(failed);
 // if (chars == 0)
   // Serial.println("** No characters received from GPS: check wiring **");
} 
long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
