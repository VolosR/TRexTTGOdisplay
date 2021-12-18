#include <TFT_eSPI.h>
#include "images.h"
#include "gameover.h"
#include "noInternet.h"

TFT_eSPI    tft   = TFT_eSPI();
TFT_eSprite img   = TFT_eSprite(&tft);
TFT_eSprite img2  = TFT_eSprite(&tft);
TFT_eSprite e     = TFT_eSprite(&tft);
TFT_eSprite e2    = TFT_eSprite(&tft);

#define btnJump           0
#define btnBrightness     35
#define pwmFreq           5000
#define pwmResolution     8
#define pwmLedChannelTFT  0

int brightnes[6]            = {40, 80, 120, 160, 200, 240};
byte b                      = 0;
float last_btnBright        = 0;

float game_start            = 0;

int   dinoW = 33;
int   dinoH = 35;
float linesX[6];
float linesX2[6];
int   linesW[6];
int   linesW2[6];
float clouds[2] = {random(0, 80), random(100, 180)};
float bumps[2];
int   bumpsF[2];
int   eW = 18;
int   eH = 38;

float eX[2] = {random(240, 310), random(380, 460)};
int   ef[2] = {0, 1};

float sped = 1;
int   x = 30;
int   y = 58;
float dir = -1.4;
bool  pressed = 0;
int   frames = 0;
int   f = 0;
float cloudSpeed = 0.4;
bool  gameRun = 1;
int   score = 0;
int   t = 0;
int   press2 = 0;

void IRAM_ATTR brightness_ISR() {
  float now = millis();
  if (now > (last_btnBright + 250) ) {
    last_btnBright = now;
    b++;
    b = (b > 5) ? 0 : b ;
    ledcWrite(pwmLedChannelTFT, brightnes[b]);
  }
}

void setup() {
  pinMode(btnJump, INPUT_PULLUP);
  pinMode(btnBrightness, INPUT_PULLUP);
  attachInterrupt(btnBrightness, brightness_ISR, FALLING);
  tft.init();
  #ifdef TFT_ROTATE // I Set this variable in the TFT_eSPI User Setup   (Setup136_LilyGo_TTV requires a screen rotation)
    tft.setRotation(TFT_ROTATE);
  #endif
  tft.setSwapBytes(true);
  tft.fillScreen(TFT_WHITE);
  ledcSetup(pwmLedChannelTFT, pwmFreq, pwmResolution);
  ledcAttachPin(TFT_BL, pwmLedChannelTFT);
  ledcWrite(pwmLedChannelTFT, brightnes[b]);

  img.setTextColor(TFT_BLACK, TFT_WHITE);
  img.setColorDepth(1);
  img2.setColorDepth(1);
  e.setColorDepth(1);
  e2.setColorDepth(1);

  img.createSprite(240, 100);
  img2.createSprite(33, 35);
  e.createSprite(eW, eH);
  e2.createSprite(eW, eH);
  tft.fillScreen(TFT_WHITE);

  for (int i = 0; i < 6; i++) {
    linesX[i] = random(i * 40, (i + 1) * 40);
    linesW[i] = random(1, 14);
    linesX2[i] = random(i * 40, (i + 1) * 40);
    linesW2[i] = random(1, 14);
  }
  for (int n = 0; n < 2; n++) {
    bumps[n] = random(n * 90, (n + 1) * 120);
    bumpsF[n] = random(0, 2);
  }
  tft.pushImage(0, 0, 217, 135, noInternet);
  while (digitalRead(btnJump) != 0) {
  }
  tft.fillScreen(TFT_WHITE);
  game_start = millis();
}


void loop() {
  if (gameRun == 1) {
    if (digitalRead(0) == 0 && pressed == 0) {
      pressed = 1;
      f = 0;
    }
    if (pressed == 1) {
      y = y + dir;
      if (y == 2) {
        dir = dir * -1.00;
      }
      if (y == 58) {
        pressed = 0;
        dir = dir * -1.00;
      }
    }
    if (frames < 8 && pressed == 0) {
      f = 1;
    }
    if (frames > 8 && pressed == 0) {
      f = 2;
    }
    drawS(x, y, f);
    frames++;
    if (frames == 16) {
      frames = 0;
    }
    checkColision();
  }
}

void drawS(int x, int y, int frame) {
  img.fillSprite(TFT_WHITE);
  img.drawLine(0, 84, 240, 84, TFT_BLACK);

  for (int i = 0; i < 6; i++) {
    img.drawLine(linesX[i], 87 , linesX[i] + linesW[i], 87, TFT_BLACK);
    linesX[i] = linesX[i] - sped;
    if (linesX[i] < -14) {
      linesX[i] = random(245, 280);
      linesW[i] = random(1, 14);
    }
    img.drawLine(linesX2[i], 98 , linesX2[i] + linesW2[i], 98, TFT_BLACK);
    linesX2[i] = linesX2[i] - sped;
    if (linesX2[i] < -14) {
      linesX2[i] = random(245, 280);
      linesW2[i] = random(1, 14);
    }
  }
  for (int j = 0; j < 2; j++) {
    img.drawXBitmap(clouds[j], 20, cloud, 38, 11, TFT_BLACK, TFT_WHITE);
    clouds[j] = clouds[j] - cloudSpeed;
    if (clouds[j] < -40)
      clouds[j] = random(244, 364);
  }

  for (int n = 0; n < 2; n++) {
    img.drawXBitmap(bumps[n], 80, bump[bumpsF[n]], 34, 5, TFT_BLACK, TFT_WHITE);
    bumps[n] = bumps[n] - sped;
    if (bumps[n] < -40) {
      bumps[n] = random(244, 364);
      bumpsF[n] = random(0, 2);
    }
  }

  for (int m = 0; m < 2; m++) {
    eX[m] = eX[m] - sped;
    if (eX[m] < -20)
      eX[m] = random(240, 300);
    ef[m] = random(0, 2);
  }

  e.drawXBitmap(0, 0, enemy[0], eW, eH, TFT_BLACK, TFT_WHITE);
  e2.drawXBitmap(0, 0, enemy[1], eW, eH, TFT_BLACK, TFT_WHITE);
  img2.drawXBitmap(0, 0, dino[frame], 33, 35, TFT_BLACK, TFT_WHITE);

  e.pushToSprite(&img, eX[0], 56, TFT_WHITE);
  e2.pushToSprite(&img, eX[1], 56, TFT_WHITE);
  img2.pushToSprite(&img, x, y, TFT_WHITE);

  score = (millis() - game_start) / 120;
  img.drawString(String(score), 204, 0, 2);
  //img.drawString(String(sped), 160, 0, 2);
  img.pushSprite(0, 17);

  if (score > t + 100) {
    t = score;
    sped = sped + 0.1;
  }
}

void checkColision() {
  for (int i = 0; i < 2; i++) {
    if (eX[i] < x + dinoW / 2 && eX[i] > x && y > 25) {
      gameRun = 0;
      tft.fillRect(0, 30, 240, 110, TFT_WHITE);
      tft.drawXBitmap(10, 30, gameover, 223, 100, TFT_BLACK, TFT_WHITE);
      delay(2500);
      ESP.restart();
    }
  }
}
