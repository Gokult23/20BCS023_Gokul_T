#define USE_ARDUINO_INTERRUPTS true
#include <PulseSensorPlayground.h>
#include <WiFi.h>
#include <ThingSpeak.h>
#define ADC_VREF_mV    3300.0 // in millivolt
#define ADC_RESOLUTION 4096.0
#define PIN_LM35       32

const int PulseWire=33;
const int LED13=2;
int Threshold=3000;
const char* ssid = "Phantom Surge"; 
const char* password = "nexusflux";  

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "18S8UGKMAB8NEV6V";
unsigned long lastTime = 0;
unsigned long timerDelay = 3000;

float temperatureC;

PulseSensorPlayground pulseSensor;

void setup() 
 {
         Serial.begin(115200); 
         WiFi.mode(WIFI_STA);     
         pinMode(32,INPUT);
         pinMode(35,INPUT);
         pinMode(34,INPUT);
         pinMode(33,INPUT);
         pulseSensor.analogInput(PulseWire);   
         pulseSensor.blinkOnPulse(LED13);       
         pulseSensor.setThreshold(Threshold); 
  
      if (pulseSensor.begin()) 
      {
         Serial.println("We created a pulseSensor Object !");
      } 
      
      ThingSpeak.begin(client); 
 }

void loop() {
   if ((millis() - lastTime) > timerDelay) 
   {
    
       if(WiFi.status() != WL_CONNECTED)
         {
           Serial.print("Attempting to connect");
           while(WiFi.status() != WL_CONNECTED){
           WiFi.begin(ssid, password); 
           delay(5000);     
         } 
           Serial.println("\nConnected.");
    }
    
       int adcVal = analogRead(PIN_LM35);
       float milliVolt = adcVal * (ADC_VREF_mV / ADC_RESOLUTION);
       float temperatureC = milliVolt / 10;
       float temperatureF = 1.8 * temperatureC + 32;
 
        float ppm=analogRead(35);
        float ppm1= ppm * (ADC_VREF_mV / ADC_RESOLUTION);
        float ppm2=ppm1/10;
       
        float s1=analogRead(34);
        int  s2= s1/20;
      
       
        int pulsesensor=analogRead(33);
        Serial.println("heart beat ");
        Serial.println(pulsesensor);
        
         Serial.print("Temperature (ºC): ");
         Serial.println(temperatureC);
        
        Serial.print("Air Quality : ");
        Serial.println(ppm2);
        
        Serial.print("Hydrogen Sulphite : ");
        Serial.println(s2);

        if (pulsesensor>Threshold)
        {
          Serial.println("♥  A HeartBeat detechted \n Workers Status : ALIVE ");
        }
        else{
          Serial.println(" No HeartBeat detechted \n Workers Status : !! CRITICAL !!!!!!!!!!");
        }
       float x = ThingSpeak.writeField(myChannelNumber, 1, temperatureC, myWriteAPIKey);
       Serial.println("Field 'TEMPERATURE' update update successful.");
       float y= ThingSpeak.writeField(myChannelNumber, 2, ppm2, myWriteAPIKey);
        Serial.println("Field 'AIR QUATILY' update update successful.");
       float z= ThingSpeak.writeField(myChannelNumber, 3, s2 , myWriteAPIKey);
        Serial.println("Field 3 'HYDRATE' update successful.");
       int p= ThingSpeak.writeField(myChannelNumber, 4, pulsesensor, myWriteAPIKey);
        Serial.println("Field 4 'PULSE' update successful.");
    if(x == 200){
      Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
}
