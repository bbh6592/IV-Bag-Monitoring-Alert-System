#include <Servo.h>
#include <SoftwareSerial.h>
#include <HX711.h>
#define DOUT_PIN 2 // HX711 data pin
#define SCK_PIN 3  // HX711 clock pin


float weight;
float threshold;

Servo myservo; 
HX711 scale;

void setup() 
{ 
  Serial.begin(9600); // Change to 9600 for NodeMCU compatibility
  scale.begin(DOUT_PIN, SCK_PIN);
  scale.set_scale(464.6); 
  scale.tare(); 
  Serial.println("System is ON");
  
  myservo.attach(9);
  
  myservo.write(0);
  delay(250); 

  myservo.write(160);
  delay(250); 

  threshold = 120;
}

void loop() 
{
  if (scale.is_ready()) 
  {
    weight = scale.get_units(10);
    Serial.print("Weight: ");
    Serial.print(weight);
    Serial.println(" grams");
  } 
  else 
  {
    Serial.println("Error: Unable to detect HX711. Please check your connections.");
  }
  delay(250); 

  if(weight < threshold)
  {
    myservo.write(160); // Stop the flow
    delay(250);
  }
  else
  {
    myservo.write(0); // Let the flow go
    delay(250);
  }

  // Transmit data to NodeMCU
  Serial.print("W=");
  Serial.print(weight);
  Serial.print(",S=");
  Serial.println(myservo.read());
}
