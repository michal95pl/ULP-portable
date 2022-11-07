#include <WiFi.h>
#include <FastLED.h>
#include <EEPROM.h>

#include "tools.h"
#include "effects.h"
#include "music.h"

#define LED_PIN     5
#define NUM_LEDS    21
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB

WiFiServer server(81);
WiFiClient client;

CRGB leds[NUM_LEDS];

Communications com;
Effects effts;
Tools tool;
BatteryTools bTools;
MusicTools mTools;

unsigned long long int voltageTimer = 0;

int batteryLow = 0;
float voltagePrep = 0;
float current = 0;

TaskHandle_t Task1;

void setup(){
  
  Serial.begin(115200); // debug
  mTools.defaultValues();
  delay(50);
 
  //zworki
  pinMode(32, INPUT_PULLUP);
  pinMode(21, INPUT_PULLUP);
  pinMode(13, INPUT_PULLUP);
  pinMode(35, INPUT_PULLUP);
  
  WiFi.mode(WIFI_AP); // access point 
  WiFi.softAP("ULP-LED", "2462123456"); // set wifi pass
  
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  
  server.begin();
  
  xTaskCreatePinnedToCore(coreLed, "Task1", 10000, NULL, 1, &Task1, 0); // run core
             
}

String dataRcv = "";

void loop(){
  WiFiClient client = server.available();

  while (client.connected()) {  
    
    while (client.available()>0) {
      char c = client.read();
      dataRcv += c;
      if(c == 'k')
        break;  
    }

    if(dataRcv != ""){

      if(dataRcv[0] == 'S' && dataRcv[1] == 'S' && dataRcv[2] == 'D' && dataRcv[3] == '_'){      
        com.sendPipeline(com.mainColor, com.first_gradientColor, com.second_gradientColor, com.mode, (int)(voltagePrep*10), com.gain, com.speed, com.precision_battery_meter, com.battery_mode, (int)(current*10), client);
      }
      
      else if(dataRcv[0] == 'B' && dataRcv[1] == 'S' && dataRcv[2] == 'D' && dataRcv[3] == '_'){
        bTools.sendChartData(client);   
      }

      else if(dataRcv[0] == 'M' && dataRcv[1] == 'D' && dataRcv[2] == 'S' && dataRcv[3] == '_'){
        mTools.rcvData(dataRcv);
        
      } 
      
      else if(dataRcv[0] == 'M' && dataRcv[1] == 'R' && dataRcv[2] == 'L' && dataRcv[3] == '_'){mTools.runLed(); Serial.println("x");}
      else {com.logist(dataRcv);}
            

        
      //Serial.println(dataRcv); 
      dataRcv = "";
    }
    
    mTools.requestData(client);

    voltagePrep = tool.getVoltage(32);
    current = tool.getCurrent();
    /*Serial.print(voltagePrep);
    Serial.print(", ");
    Serial.print(tool.getCurrent());
    Serial.print(", ");
    Serial.println(voltagePrep + (0.444444 * tool.getCurrent()) - 0.0444444);*/
    bTools.collectBatteryData((int)(voltagePrep*10));
    
    //if(digitalRead(21) == 0 && ( (digitalRead(13) == 0 && voltagePrep <= 12.0) || (digitalRead(13) == 1 && voltagePrep <= 9.0) ) ){batteryLow += 1;}
    
  }
  
  voltagePrep = tool.getVoltage(32);
  current = tool.getCurrent();
  /*Serial.print(voltagePrep);
  Serial.print(", ");
  Serial.print(tool.getCurrent());
  Serial.print(", ");
  Serial.println(voltagePrep + (0.444444 * tool.getCurrent()) - 0.04444444);*/
  bTools.collectBatteryData((int)voltagePrep*10);
  
  //if(digitalRead(21) == 0 && ( (digitalRead(13) == 0 && voltagePrep <= 12.0) || (digitalRead(13) == 1 && voltagePrep <= 9.0) ) ){batteryLow += 1;}

  if( (millis() - voltageTimer) >= 500){
    if(batteryLow <= 100){batteryLow = 0;}
    voltageTimer = millis();
  } 
  
}

void coreLed(void * parameter){
  
  effts.run_1(leds);  
  effts.run_2(leds);
  
  while(true){

    mTools.run(leds);

    if( batteryLow < 200 || digitalRead(21) == 1 ){
      if(com.mode == 0){effts.color(leds, com.mainColor[0], com.mainColor[1], com.mainColor[2]);}
      else if(com.mode == 1){effts.gradientCreate(com.first_gradientColor,com.second_gradientColor); effts.gradientStatic(leds, com.first_gradientColor,com.second_gradientColor);}
      else if(com.mode == 2){effts.rainbow(leds, com.speed);}
      else if(com.mode == 3){effts.szybkiBong(leds, com.speed);}      
    } 
    else{
      for(int i=0; i<21; i++){leds[i] = CRGB(0,0,0);}
      FastLED.show();
    }   
    
    delay(1);
  }

}
