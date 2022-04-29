/*
  Ellipse drawing example
  
  This sketch does not use any fonts.
*/

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define BUTTON_RIGHT 35

const int BTN_PIN = 36;
const int JOY_X = 27;
const int JOY_Y = 25;
const int POT = 38;

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library



int score[ 3 ][ 8 ] = {{},{},{}};
int bpm = 80;
int cursorPos[2] = {1,0};
int beatPos = 0;
bool paused = true;

int lastDebounceTime = millis();



int Xprev = 2800;
int Yprev = 2800;



TaskHandle_t TimerThreadHandle = NULL;


void setup(void) {
  Serial.begin(115200);
  buttonSetup();
  
  tft.init();

  tft.setRotation(1);

  tft.fillScreen(TFT_BLACK);

  //tft.setCursor(5, 5, 2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  //tft.println("Playing");
  
  tft.setCursor(150, 5, 2);
  tft.println("bpm: " + String(bpm));


  xTaskCreatePinnedToCore(
      TimerThread,                       /* Function to implement the task */
      "TimerThread",                    /* Name of the task */
      4000,                           /* Stack size in words */
      NULL,                           /* Task input parameter */
      5,                              /* Priority of the task */
      &TimerThreadHandle,                /* Task handle. */
      1);                             /* Core where the task should run */

}

void IRAM_ATTR toggle_pause(){
  
  if(lastDebounceTime + 300 < millis()){
    lastDebounceTime = millis();
    paused = !paused;
  }
  
}

void IRAM_ATTR toggle_input(){
  
  if(lastDebounceTime + 300 < millis()){
    lastDebounceTime = millis();
    score[ cursorPos[0] ][ cursorPos[1] ] = !score[ cursorPos[0] ][ cursorPos[1] ];
  }
  
}

void buttonSetup(){
  pinMode(BUTTON_RIGHT, INPUT);
  pinMode(BTN_PIN, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), toggle_pause, FALLING);
  attachInterrupt(digitalPinToInterrupt(BTN_PIN), toggle_input, FALLING);
}


void loop() {

  
  //measure XY axis
  int Xcurr = analogRead(JOY_X);
  if(Xprev < 4000 && Xcurr >= 4000){
    //move right
    cursorPos[1] = (cursorPos[1] + 1) % 8;
  }
  if(Xprev > 100 && Xcurr <= 100){
    //move left
    cursorPos[1] = (cursorPos[1] - 1 + 8) % 8;
  }

  int Ycurr = analogRead(JOY_Y);
  if(Yprev < 4000 && Ycurr >= 4000 && Xcurr <= 4000){
    //move up
    cursorPos[0] = (cursorPos[0] + 1) % 3;
  }
  if(Yprev > 100 && Ycurr <= 100){
    //move down
    cursorPos[0] = (cursorPos[0] - 1 + 3) % 3;
  }

  //Serial.println("Y: " + String(Ycurr) + " X: " + String(Xcurr));
  Yprev = Ycurr;
  Xprev = Xcurr;
  

  bpm = int(float(analogRead(POT))*(200.0/4095.0)) + 60;
  //Serial.println(String(bpm)); 


  

  //tft.fillScreen(TFT_BLACK);

  // Draw some random filled elipses
  for(int i = 0; i < 3; i++){
    for (int j = 0; j < 8; j++)
    {
      int rx = 5;
      int ry = 5;
      int x = j*26 + 26;
      int y = i*20 + 60;


      tft.fillEllipse(x, y, rx+5, ry+5, TFT_BLACK);
      if(cursorPos[1] == j && cursorPos[0] == i){
        tft.drawEllipse(x, y, rx+2, ry+2, 0x07E0);
      } 

      tft.drawLine(x - 10 ,60 - 5, x - 10,60 - 10 + 20*3,TFT_BLACK);
      if(beatPos == j && i == 2){
        tft.drawLine(x - 10 ,60 - 5, x - 10,60 - 10 + 20*3,0x07E0);

        if(!paused){
          //Serial.println("{" + String(score[0][j]) + String(score[1][j]) + String(score[2][j]) + "}");  
        }
      }
    
      if(score[i][j] == 0){
        tft.drawEllipse(x, y, rx, ry, 0xFFFF);
      }
      else{
        tft.fillEllipse(x, y, rx, ry, 0xFFFF);
      }
      
    }
  }



  if(!paused){
    tft.setCursor(5, 5, 2);  
    tft.println("Playing ");
    //beatPos = (beatPos + 1) % 8;
  }
  else{
    tft.setCursor(5, 5, 2);  
    tft.println("Paused " );
  }

  tft.setCursor(150, 5, 2);
  tft.println("bpm: " + String(bpm)+ "    ");
  



  
  delay(50);
  //tft.fillScreen(TFT_BLACK);
}

void TimerThread(void *pvParameters) {
  //Serial.println("time loop started");
  int quater_note_length = 0;
  while (1) // tempTask loop
  {
    quater_note_length = round(500.0 * 60.0/float(bpm));
    if(!paused){
      beatPos = (beatPos + 1) % 8;
      Serial.println("{" + String(score[0][beatPos]) + String(score[1][beatPos]) + String(score[2][beatPos]) + "}");  
    }
    
       
    delay(quater_note_length);
  }
}
