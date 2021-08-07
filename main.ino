#include <FastLED.h>
// #include <avr/pgmspace.h>

#include <math.h>

#include "LED_MAP/LedMap.h"

// How many leds in your strip?


// For led chips like WS2812, which have a data line, ground, and power, you just
// need to define DATA_PIN. For led chipsets that are SPI based (four wires - data, clock,
// ground, and power), like the LPD8806 define both DATA_PIN and CLOCK_PIN
// Clock pin only needed for SPI based chipsets when not using hardware SPI

#define DATA_PIN 5
// #define CLOCK_PIN 13

extern const uint8_t xPos[];
extern const uint8_t yPos[];

uint16_t nodeCount = NUM_LEDS;

// Define the array of leds
CRGB leds[NUM_LEDS];


void setup() {
  // Serial.begin(9600);
  // while(!Serial){;}

  nodeCount = 49;// pgm_read_word_near(PROG_nodeCount);



  pinMode(LED_BUILTIN, OUTPUT);

  pinMode(2, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, INPUT_PULLUP);
  pinMode(12, INPUT_PULLUP);

  pinMode(3, INPUT_PULLUP);
    // Uncomment/edit one of the following lines for your leds arrangement.
    // ## Clockless types ##
    // FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS); // GRB ordering is assumed
    // FastLED.addLeds<SM16703, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1829, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1812, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1809, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1804, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<TM1803, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1903B, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS1904, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<UCS2903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2852, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2812B, DATA_PIN, RGB>(leds, NUM_LEDS); // GRB ordering is typical
    // FastLED.addLeds<GS1903, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6812, DATA_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA106, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<PL9823, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SK6822, DATA_PIN, RGB>(leds, NUM_LEDS);
    FastLED.addLeds<WS2811, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2813, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<APA104, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2811_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GE8822, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<GW6205_400, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886, DATA_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<LPD1886_8BIT, DATA_PIN, RGB>(leds, NUM_LEDS);
    // ## Clocked (SPI) types ##
    // FastLED.addLeds<LPD6803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<LPD8806, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // GRB ordering is typical
    // FastLED.addLeds<WS2801, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<WS2803, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<SM16716, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);
    // FastLED.addLeds<P9813, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<DOTSTAR, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS);  // BGR ordering is typical
    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS); // BGR ordering is typical
    // FastLED.addLeds<SK9822, DATA_PIN, CLOCK_PIN, RGB>(leds, NUM_LEDS); // BGR ordering is typical


}

int loopCout = 0;
int colLoop = 0;

uint8_t r = 0;
uint8_t g = 0;
uint8_t b = 0;

uint8_t targ_r = 250;
uint8_t targ_g = 250;
uint8_t targ_b = 250;

uint64_t startTime = 0;
uint64_t lastEnd = 0;
uint64_t endTime = 0;

#define MAX_TEMPO_CLICKS 20
uint64_t tempoClicks[MAX_TEMPO_CLICKS];
uint32_t tempoGaps[MAX_TEMPO_CLICKS];
uint8_t tempoClicksLen = 0;
uint32_t tempo = 469;
uint32_t tempoOffSet = 0;

uint8_t lastLoopFrac = 0;
uint8_t loopFrac = 0;


uint8_t clickSetting = 1;

bool buttonVals[7];
bool clicked[7];
uint8_t clickCoolDown[7];
uint8_t clickCoolDownLoop = 50;

bool ledVal = 0;

void loop() {
  clicked[0] = !(digitalRead( 2));
  // clicked[1] = !(digitalRead( 7));
  clicked[2] = !(digitalRead( 8));
  clicked[3] = !(digitalRead( 9));
  clicked[4] = !(digitalRead(10));
  clicked[5] = !(digitalRead(11));
  clicked[6] = !(digitalRead(12));

  clicked[1] = !(digitalRead( 3));

  for(size_t ii = 0; ii < 7; ii++){
    if(buttonVals[ii] and clicked[ii]){
      clicked[ii] = 0;
    }else{
      buttonVals[ii] = clicked[ii];
    }

    if(clickCoolDown[ii] < clickCoolDownLoop){
      clickCoolDown[ii] += 1;
      clicked[ii] = 0;
    }else if(clicked[ii]){
      clickCoolDown[ii] = 0;
    }
  }



  //Initialized whether each click is active

  if(clickSetting == 0){ //Set new setting
    if(clicked[1]){
      clickSetting = 1;
      tempoClicksLen = 0;
    }
  }
  else if(clickSetting == 1){ //Set tempo

    if(clicked[0]){ //Additional tap input
        tempoClicks[tempoClicksLen] = millis();
        tempoClicksLen += 1;

        if(tempoClicksLen >= MAX_TEMPO_CLICKS){
          clicked[1] = 1;
        }
    }

    if(clicked[1]){ 
      //End setting tempo
      //Calculate tempo and initial offset

      if(tempoClicksLen > 1){
        for(size_t ii = 0; ii < tempoClicksLen -1; ii++){
          tempo += tempoClicks[ii +1] - tempoClicks[ii];
          tempoGaps[ii] = tempoClicks[ii +1] - tempoClicks[ii];
        }

        uint16_t numGaps = tempoClicksLen -1;
        uint16_t numDrop = 3;
        uint32_t tmpTempo = 0;

        for(size_t ii = 0; ii<numDrop; ii++){  //Eliminate specified number of gaps
          for(size_t jj = 0; jj<numGaps; jj++){  //Get curr tempo
            tmpTempo += tempoGaps[jj];  //Sum current gaps
          }

          tmpTempo /= numGaps; //Get average

          uint32_t maxGap = 0;
          uint16_t maxGapInd = 0;
          for(size_t jj = 0; jj<numDrop; jj++){
            uint32_t fooDiff = abs(tmpTempo - tempoGaps[jj]); //Get difference of average from specific gap

            if(fooDiff > maxGap){
              maxGap = fooDiff;
              maxGapInd = jj;
            }
          }

          //Found max gap
          numGaps -= 1;
          for(size_t jj = maxGap; jj < numGaps; jj++){
            tempoGaps[jj] = tempoGaps[jj +1];
          }
        }

        ///////////////////////////////////////////////////////////////

        tempo /= tempoClicksLen -1;

        tempoOffSet = tempoClicks[0]%tempo;

        // Serial.print(tempo);
        // Serial.print(' ');
        // Serial.println(tempoOffSet);
      }

      clickSetting = 0;
    }
  }




  uint64_t currTime = millis();
  lastLoopFrac = loopFrac;
  loopFrac = (255 * ((currTime + tempoOffSet) % tempo) / tempo);

  // Serial.print(loopFrac);
  // Serial.println();

  if(loopFrac < 127 || clickSetting > 0){
    digitalWrite(LED_BUILTIN, 1);
  }
  else{
    digitalWrite(LED_BUILTIN, 0);
  }



  // if(clicked[0]){

  //   for(size_t ii = 0; ii < 7; ii++){
      // Serial.print(buttonVals[ii]);
      // Serial.print(' ');
  //   }

  //   ledVal = !ledVal;
  //   digitalWrite(LED_BUILTIN, ledVal);
    // Serial.print('\n');
  // }
  

  uint16_t minPos = abs(loopFrac - 127);
  for(int i = 0; i < nodeCount; i++){
    if((yPos[i]) + 10 > minPos){
      leds[i] = CRGB(r, g, b);
    }
    else{
      leds[i] = CRGB(0,0,0);
    }
  }

  FastLED.show();

  if(lastLoopFrac > loopFrac){
    r = random8(0,100);
    g = random8(0, 100-r);
    b = random8(0, 100-r-g);
    colLoop = 0;
  }

}