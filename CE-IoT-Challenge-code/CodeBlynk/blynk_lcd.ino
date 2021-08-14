/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "yHztWgoq4PFUfZfx95kCPabnriF_JdEH";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Liem";
char pass[] = "1234567890";
const int ledpin = 12;
WidgetTerminal terminal(V3);
WidgetLCD lcd(V2);
BLYNK_WRITE(V1)
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  if(pinValue){
    digitalWrite(ledpin, HIGH);
    Serial.println("high");
    }
  else{
    digitalWrite(ledpin, LOW);
    Serial.println("low");
    }
}

void setup()
{
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(ledpin, OUTPUT);
  digitalWrite(ledpin, LOW);
  // You can also specify server:
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  //Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,100), 8080);

  //Serial.readBytes(mystr,5); //Read the serial data and store in var
  //String mystr = Serial.readString();
  lcd.clear();
  lcd.print(0,0,Serial.readString());
}

void loop()
{
  Blynk.run();
  if(Serial.available()>0){
  //terminal.clear();
  //lcd.print(0,0,Serial.readString());
  terminal.println(Serial.readString()) ;
  terminal.flush();
  }
  //delay(10000);
}
