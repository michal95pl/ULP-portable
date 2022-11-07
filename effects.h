class Effects{
  
    
    int color_mode1[3] = {0,0,0}; 
    int ar2=255,br2=0,cr2=0, nr2 = 0; // rainbow
    bool states2[6] = {true, false, false, false, false, false}; // rainbow states
    unsigned long long int rTimer = 0; // rainbow timer
    int gradient[21][3], multi[3] = {0,0,0}; int r,g,b; int temp_gradient[21][3]; // gradient
    int color1_temp[3] = {0,0,0}, color2_temp[3] = {0,0,0};
    
    public:
    void run_1(CRGB leds[]){

        for(int i=0; i < 20; i++){
            if(i > 1){ 
                leds[i-2] = CRGB(0,0,0);
            }

            if(i > 0){
                leds[i-1] = CRGB(0,0,0);     
            }
        
            leds[i] = CRGB(255,255,255);
            leds[i+1] = CRGB(255,255,255);   
            FastLED.show();
            delay(30);
        }

        delay(50);

        for(int i=20; i >= 0; i--){
            if(i < 19){
                leds[i+2] = CRGB(0,0,0);      
            }
      
            if(i < 20){  
                leds[i+1] = CRGB(0,0,0);    
            }
      
            leds[i-1] = CRGB(255,255,255);
            leds[i] = CRGB(255,255,255);

            FastLED.show();
            delay(30);
        }
  
        leds[0] = CRGB(0,0,0);
        FastLED.show();    
    }

    void run_2(CRGB leds[]){
  
        for(int i=0; i <= 255; i+=15){
            for(int j=0; j<21; j++){
                leds[j] = CRGB(0,0,i);
            }
            FastLED.show();
            delay(20);
        }

        delay(50);
    
        for(int i=255; i >= 0; i-=15){
            for(int j=0; j<21; j++){
                leds[j] = CRGB(0,0,i);
            }
            FastLED.show();
            delay(20);
        }
    
    }


    void color(CRGB leds[], int r, int g, int b){

        for(int i=0; i<21; i++){
            leds[i] = CRGB(r,g,b); 
        }

        if(color_mode1[0] != r || color_mode1[1] != g || color_mode1[2] != b){
            FastLED.show();
            color_mode1[0] = r; color_mode1[1] = g; color_mode1[2] = b;
        }
    }


    void rainbow(CRGB leds[], int speed){

        if((millis() - rTimer) >= speed){
            if(states2[0] == true){
                br2 += 51;
                if(br2 == 255){states2[0] = false; states2[1] = true;}
            }

            if(states2[1] == true){
                ar2 -= 51;
                if(ar2 == 0){states2[1] = false; states2[2] = true;}
            }

            if(states2[2] == true){
                cr2 += 51;
                if(cr2 == 255){states2[2] = false; states2[3] = true;}
            }

            if(states2[3] == true){
                br2 -= 51;
                if(br2 == 0){states2[3] = false; states2[4] = true;}
            }

            if(states2[4] == true){
                ar2 += 51;
                if(ar2 == 255){states2[4] = false; states2[5] = true;}
            }

            if(states2[5] == true){
                cr2 -= 51;
                if(cr2 == 0){states2[5] = false; states2[0] = true;}
            }

            nr2 += 1;
            if(nr2 == 21){nr2 = 0;}
            leds[nr2] = CRGB(ar2,br2,cr2);
            FastLED.show();
            rTimer = millis();
        }
    }

    bool setSpeed1 = false, setSpeed2 = false, setSpeed3 = false;
    int indxSlider = 3;
    bool sideSpeed = false;
    int ar=255,br=0,cr=0, nr = 0;
    bool pierwszy = true, drugi = false, trzeci = false, czwarty = false, piaty = false, szosty = false;
    unsigned long long int timer = 0, saveTimer1 = 0, saveTimer2 = 0, saveTimer3 = 0, saveTimer = 0, saveTimerSlider = 0, saveTimerSlider2 = 0;

    void szybkiBong(CRGB leds[], int speed){

        speed -= 15;
        timer = millis();

        if( (timer - saveTimer1) >= 100 + speed ){
            leds[0] = CRGB(255,255,255);
            leds[20] = CRGB(255,255,255);
            leds[4] = CRGB(255,255,255);
            leds[16] = CRGB(255,255,255);
            saveTimer1 = millis();
            setSpeed1 = true;
        }

        if( (timer - saveTimer1) >= 30 + speed && setSpeed1 == true){
            leds[0] = CRGB(0,0,0);
            leds[20] = CRGB(0,0,0);
            leds[4] = CRGB(0,0,0);
            leds[16] = CRGB(0,0,0);
            setSpeed1 = false;
        }

        if( (timer - saveTimer2) >= 130 + speed ){
            leds[1] = CRGB(255,255,255);
            leds[19] = CRGB(255,255,255);
            saveTimer2 = millis();
            setSpeed2 = true;
        }

        if( (timer - saveTimer2) >= 50 + speed && setSpeed2 == true){
            leds[1] = CRGB(0,0,0);
            leds[19] = CRGB(0,0,0);
            setSpeed2 = false;
        }

        if( (timer - saveTimer3) >= 160 + speed){
            leds[2] = CRGB(255,255,255);
            leds[3] = CRGB(255,255,255);
            leds[18] = CRGB(255,255,255);
            leds[17] = CRGB(255,255,255);
            saveTimer3 = millis();
            setSpeed3 = true;
        }

        if( (timer - saveTimer3) >= 70 + speed && setSpeed3 == true){
            leds[2] = CRGB(0,0,0);
            leds[3] = CRGB(0,0,0);
            leds[18] = CRGB(0,0,0);
            leds[17] = CRGB(0,0,0);
            setSpeed3 = false;
        }
    
        if( (timer - saveTimerSlider) >= speed + 15){

            if(pierwszy == true){
                br += 15;
                if(br == 255){pierwszy = false; drugi = true;}
            }

            if(drugi == true){
                ar -= 15;
                if(ar == 0){drugi = false; trzeci = true;}
            }

            if(trzeci == true){
                cr += 15;
                if(cr == 255){trzeci = false; czwarty = true;}
            }

            if(czwarty == true){
                br -= 15;
                if(br == 0){czwarty = false; piaty = true;}
            }

            if(piaty == true){
                ar += 15;
                if(ar == 255){piaty = false; szosty = true;}
            }

            if(szosty == true){
                cr -= 15;
                if(cr == 0){szosty = false; pierwszy = true;}
            }
      
            if(sideSpeed){
                if(indxSlider > 5){ 
                    leds[indxSlider-1] = CRGB(0,0,0);       
                }
                leds[indxSlider] = CRGB(ar,br,cr);
                indxSlider += 1;
            }
            else{
                leds[indxSlider] = CRGB(ar,br,cr); 
                if(indxSlider < 15){
                    leds[indxSlider+1] = CRGB(0,0,0);          
                }
                indxSlider -= 1;        
            }

            if(indxSlider < 5){indxSlider = 5; sideSpeed = true;}    
            if(indxSlider > 15){indxSlider = 15; sideSpeed = false;}  
      
            saveTimerSlider = millis();
        }

        FastLED.show();
    }

   
    void gradientCreate(int color1[3], int color2[3]){

        r = color1[0]; g = color1[1]; b = color1[2];

        for(int i=0; i<3; i++){
            multi[i] = round(abs(color1[i]-color2[i])/18);
        }

        for(int i=0; i<21; i++){
            if(i > 0 && i < 20){

                if(color1[0] > color2[0] && r > color2[0]) {r -= multi[0];}
                else if(r < color2[0]) {r += multi[0];}

                if(color1[1] > color2[1] && r > color2[1]) {g -= multi[1];}
                else if(g < color2[1]) {g += multi[1];}

                if(color1[2] > color2[2] && b > color2[2]) {b -= multi[2];}
                else if(b < color2[2]) {b += multi[2];}
            }

            if(r > 255){r=255;}
            if(g > 255){g=255;}
            if(b > 255){b=255;}

            if(i == 0) { for(int n=0; n<3;n++){gradient[0][n] = color1[n];} }
            else if(i == 20) {for(int n=0; n<3;n++){gradient[20][n] = color2[n];}}
            else {
                gradient[i][0] = r;
                gradient[i][1] = g;
                gradient[i][2] = b;
            }
        }
    }

    void gradientStatic(CRGB leds[], int color1[3], int color2[3]){
        for(int i = 0; i<21; i++){ 
            leds[i] = CRGB(gradient[i][0],gradient[i][1],gradient[i][2]);
        }

        if(color1[0] != color1_temp[0] || color1[1] != color1_temp[1] || color1[2] != color1_temp[2] ||
          color2[0] != color2_temp[0] || color2[1] != color2_temp[1] || color2[2] != color2_temp[2]){
            FastLED.show();
        }
        
    }

  
};