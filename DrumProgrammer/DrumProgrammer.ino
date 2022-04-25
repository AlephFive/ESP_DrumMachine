/*
  Ellipse drawing example
  
  This sketch does not use any fonts.
*/

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

#define BUTTON_RIGHT 35

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library



int score[ 3 ][ 8 ] = {{},{},{}};
int bpm = 80;
int cursorPos[2] = {1,0};
int beatPos = 0;
bool paused = true;

int lastDebounceTime = millis();


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

}

void IRAM_ATTR toggle_pause(){
  
  if(lastDebounceTime + 200 < millis()){
    lastDebounceTime = millis();
    paused = !paused;
  }
  
}

void buttonSetup(){
  pinMode(BUTTON_RIGHT, INPUT);

  attachInterrupt(digitalPinToInterrupt(BUTTON_RIGHT), toggle_pause, FALLING);
}


void loop() {



  int quater_note_length = round(500.0 * 60.0/float(bpm));
  tft.setCursor(150, 5, 2);
  tft.println("bpm: " + String(bpm)+ "    ");

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
          Serial.println("{" + String(score[0][j]) + String(score[1][j]) + String(score[2][j]) + "}");  
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

  //cursorPos[1] = (cursorPos[1] + 1) % 8;


  if(!paused){
    tft.setCursor(5, 5, 2);  
    tft.println("Playing ");
    beatPos = (beatPos + 1) % 8;
  }
  else{
    tft.setCursor(5, 5, 2);  
    tft.println("Paused " );
  }
  



  
  delay(quater_note_length);
  //tft.fillScreen(TFT_BLACK);
}
