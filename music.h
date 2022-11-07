class MusicTools{
  
  int data_music[2][10002], j, indx = 4, number_pipeline[2] = {-1,-1}, array_indx=0, max_containers = 0;
  bool data_flag = false;
  unsigned long long int timer = 0, indx_led;
  String s = "", nr_pipeline = "";  
  
  public:
  bool rqst_data = false;
  
  void defaultValues(){
    for(int i=0; i < 2; i++){
      for(int j=0; j < 10000; j++){data_music[i][j] = 0;}
    }
    
  }

// 000 000 000 -> container index, number of container, max container
  void rcvData(String data){
    j = 0;
    indx = 13;

    s = "";
    s += data[10]; s += data[11]; s += data[12];
    max_containers = s.toInt();
    s = "";
    
    while(true){
      if(data[indx] == 'k'){break;}
      s += data[indx];
      indx += 1;
          
      if (s.length() == 2) { data_music[String(data[6]).toInt()][j] = s.toInt(); j += 1; s = ""; number_pipeline[String(data[6]).toInt()] = nr_pipeline.toInt(); }
    }
    
    //data_flag = true;
  }

  void runLed(){
    data_flag = true;    
  }

  void requestData(WiFiClient client) {
    if (rqst_data) {
      if (array_indx > 0){client.print("RDS_" + String(array_indx + 2) + 'k'); rqst_data = false; }
      Serial.println("d");
    }
  }
  bool tests = false;
  
  void run(CRGB leds[]){
        
    if(data_flag == true){  
      
      
      indx_led = millis() - timer;
      if(!tests) {Serial.println(indx_led); tests = true;}      
      if(indx_led < 10000){
        for(int i=0; i < 21; i++){
          leds[i] = CRGB(data_music[(array_indx+2) % 2][indx_led], 0, 0);
        }
        FastLED.show();

      } else {
        array_indx += 1;
        rqst_data = true;
        if(array_indx == max_containers) {data_flag = false; array_indx = 0;}
        else {timer = millis();}
      }
      
    } else {timer = millis();}

  }
    
};