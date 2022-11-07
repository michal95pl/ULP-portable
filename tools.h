class Tools {

  float v, x;
  
  public:  
    float getVoltage(int pin=0, int zworka=13){
      v = 0;
      for(int i=0; i<100; i++){
        v += analogRead(pin);        
      }
      v = (int)(v / 100);
      v = v*0.0038709677419354843 + 2.7483870967741932;
      if(v <= 14){v = v*1.19047 -2.3095;}
      else if(v <= 15){v = v*1.111 - 1.2222;}
      else{v = v*0.8510 + 2.5744;}
      /*if(digitalRead(zworka) == 0){v += 0.2;}
      else if(digitalRead(zworka) == 1){v -= 0.4;}*/  
      v += 0.4;
      return v;      
    }

    float getCurrent(){
      x = 0;
      for(int i=0; i<200;i++){x += analogRead(35);}
      x = x/200;
      x -= 2000;

      if(x < 560){x = 0;}
      else{
        if(x <= 743){x = -0.002558*x + 2.420697;}
        else if(x <= 800){x = -0.0028070*x + 2.60561403;}
        else if(x > 800){x = -0.0025*x + 2.36;}   
        x -= 0.06;            
      }      
      
      return x;
    }
};

class Communications {
  
  // veriables data rcv pipeline
  String temp;
  int indx, indxArray, indxArray_color;
  
  public:

  // main veriables
  int gain = 0, speed = 50;
  int mainColor[3] = {0,0,0}; 
  int first_gradientColor[3] = {0,0,0};
  int second_gradientColor[3] = {0,0,0};  
  int mode = 0;
  int battery_mode = 2;
  bool precision_battery_meter = true;

    

  void logist(String data){ // recive data decode
      
    indx = 4;
    temp = "";  
    indxArray_color = 0;
    
    if(data[0] == 'D' && data[1] == 'S' && data[2] == 'G' && data[3] == '_'){
      
      while(true){
        temp += data[indx];
        indx += 1;
        if(data[indx] == 'k'){
          break;
        }
      }

      gain = temp.toInt();
      Serial.println("x");
      FastLED.setBrightness(gain);
      FastLED.show();
    }

        else if(data[0] == 'S' && data[1] == 'E' && data[2] == 'F' && data[3] == '_'){
      
            while(true){
                temp += data[indx];
                indx += 1;
                if(data[indx] == 'k'){
                break;
                }
            }
            speed = temp.toInt();
        }

        else if(data[0] == 'C' && data[1] == 'P' && data[2] == 'G' && data[3] == '1'){

            while(true){
                if(data[indx] == ','){
                    first_gradientColor[indxArray_color] = temp.toInt();
                    temp = "";
                    indxArray_color += 1;                
                }
                else {
                    temp += data[indx];
                }
        
                indx += 1;

                if(data[indx] == 'k'){
                    break;
                }
            }    
            first_gradientColor[indxArray_color] = temp.toInt();
        }

        else if(data[0] == 'C' && data[1] == 'P' && data[2] == 'G' && data[3] == '2'){

            while(true){
                if(data[indx] == ','){
                    second_gradientColor[indxArray_color] = temp.toInt();
                    temp = "";
                    indxArray_color += 1;                
                }
                else{
                    temp += data[indx];
                }

                indx += 1;

                if(data[indx] == 'k'){
                    break;
                }
            }    
            second_gradientColor[indxArray_color] = temp.toInt();
        }

        else if(data[0] == 'C' && data[1] == 'P' && data[2] == 'C' && data[3] == '_'){

            while(true){

                if(data[indx] == ','){
                    mainColor[indxArray_color] = temp.toInt();
                    temp = "";
                    indxArray_color += 1;                
                }
                else{
                    temp += data[indx];
                }

                indx += 1;

                if(data[indx] == 'k'){
                    break;
                }
            }  

            mainColor[indxArray_color] = temp.toInt();
        }

        else if(data[0] == 'D' && data[1] == 'P' && data[2] == 'M' && data[3] == '_'){
            temp = data[4];
            mode = temp.toInt();
        }

        else if (data[0] == 'P' && data[1] == 'V' && data[2] == 'M' && data[3] == '_'){
          temp = data[4];
          precision_battery_meter = temp.toInt();
        }

  }

  void sendPipeline(int color_static[], int first_gradientColor[], int second_gradientColor[], int state, int voltage, int gain, int speed, bool prec_battery, int battery_mode, int current, WiFiClient client){
    
    temp = "";
    
    temp += "D";
    for(int i=0; i<3; i++){
      temp += String(color_static[i]);
      temp += ",";
    }
    for(int i=0; i<3; i++){
      temp += String(first_gradientColor[i]);
      temp += ",";
    }
    for(int i=0; i<3; i++){
      temp += String(second_gradientColor[i]);
      temp += ",";
    }
    temp += voltage;
    
    temp += ",";
    temp += String(state);

    temp += ",";
    temp += String(gain);

    temp += ",";
    temp += String(speed);

    temp += ",";
    temp += String(prec_battery);

    temp += ",";
    temp += String(battery_mode);

    temp += ",";
    temp += String(current);
    
    temp += "k";
    
    client.print(temp);
    Serial.println(temp);
  
  }

};


class BatteryTools{

  int chart_data[200], indx_array = 0; String s = "";
  unsigned long long int save_timer = 0;
  int voltage_send;

  
  public:
  BatteryTools(){
    for(int i=0; i<200; i++){chart_data[i] = 0;}     
  }
  
  void collectBatteryData(int voltage){

    if((millis() - save_timer) >= 60000){
      chart_data[indx_array] = voltage;
      indx_array += 1;
      save_timer = millis();
    }
    
  }

  void sendChartData(WiFiClient client){
    
    s = "";
    s += "B";
    
    for(int i=0; i<indx_array;i++){
      voltage_send = chart_data[i];
      if(voltage_send < 10){voltage_send *= 100;}
      else if(voltage_send < 100){voltage_send *= 10;}
      s += String(voltage_send);
    }
    s += "k";
    
    client.print(s);
    
  }

};
