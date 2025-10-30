#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C // because the screen is 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define FRAMERATE 60.0 // the game's target framerate, in frames per second
#define FRAME_DELTA 1000/FRAMERATE

class Ball {
  int8_t posX, posY;
  int8_t velX, velY;
  public:

  void draw() {

  }

  void move() {
    posX += velX;
    posY += velY;
  }

  void bounce(bool offPaddle) {
    if (offPaddle) {
      velX = -velX;
      // todo: what to do with the vertical speed
    }
    else { // if it didn't bounce off a paddle, it must've bounced off the ceiling or floor
      velY = -velY; // either way, the vertical speed is just negated
    }
  }
};

class Paddle {
  bool player;
  int8_t posX, posY;
  public:
  int8_t length = 5;

  Paddle(bool p, int8_t y) {
    player = p;
    posX = (player ? 0 : display.width()-1);
    posY = y;
  }

  void draw() {
    display.drawLine(posX, posY, posX, posY+length-1, SSD1306_WHITE); // without the -1, the paddle would be 6 pixels long
    display.display();
  }

  void move(int8_t vel) {
    if (posY + vel <= 7) return;
    posY += vel;
  }
};

Paddle player(true, 0);
Paddle cpu(false, 20);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    pinMode(LED_BUILTIN, OUTPUT);
    for(;;) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1/FRAMERATE * 1000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(1/FRAMERATE * 1000);
    } // Don't proceed, loop forever
  }
  player.draw();
  cpu.draw();
}

void loop() {
  
  display.clearDisplay();
  player.draw();
  cpu.draw();
  delay(1000);
}
