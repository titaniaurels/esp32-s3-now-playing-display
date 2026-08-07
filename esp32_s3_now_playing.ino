#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// ESP32-S3 OLED pins
#define OLED_SDA 8
#define OLED_SCL 9

#define OLED_RESET -1
#define OLED_ADDRESS 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  OLED_RESET
);

// Duration for each text segment
const unsigned long TEXT_DURATION = 800;

// For animation speed
const unsigned long FRAME_DURATION = 100;

//Love me not-lyrics
const char* textList[] = {
  "And, oh,", "it's hard", "to see you,", "but I wish", "you were", "right here",
  "Oh, it's hard", "to leave you", "when I get", "you everywhere",
  "All this time", "I'm thinking", "we could never", "be a pair",
  "Oh, no,", "I don't", "need you,", "but I miss you,", "come here",
  "And, oh,", "it's hard", "to see you,", "but I wish", "you were", "right here",
  "Oh, it's", "hard to", "leave you", "when I get", "you everywhere",
  "All this time,", "I'm thinking", "I'm strong", "enough to", "sink it",
  "Oh, no,", "I don't", "need you,", "but I miss you,", "come here"
};

const int textCount = sizeof(textList) / sizeof(textList[0]);

unsigned long textStartTime = 0;
unsigned long lastFrameTime = 0;

int currentText = 0;
int animationFrame = 0;


// Draws a heart icon
void drawHeart(int x, int y) {
  display.fillCircle(x + 3, y + 3, 3, SSD1306_WHITE);
  display.fillCircle(x + 9, y + 3, 3, SSD1306_WHITE);

  display.fillTriangle(
    x,
    y + 4,
    x + 12,
    y + 4,
    x + 6,
    y + 12,
    SSD1306_WHITE
  );
}

// Draws a music note icon
void drawMusicNote(int x, int y) {
  display.drawLine(
    x + 4,
    y,
    x + 4,
    y + 9,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 4,
    y,
    x + 10,
    y + 2,
    SSD1306_WHITE
  );

  display.drawLine(
    x + 10,
    y + 2,
    x + 10,
    y + 10,
    SSD1306_WHITE
  );

  display.fillCircle(
    x + 1,
    y + 10,
    3,
    SSD1306_WHITE
  );

  display.fillCircle(
    x + 7,
    y + 11,
    3,
    SSD1306_WHITE
  );
}


// Draws the sparkle icon
void drawSparkle(int x, int y) {
  display.drawPixel(x, y, SSD1306_WHITE);
  display.drawPixel(x - 1, y, SSD1306_WHITE);
  display.drawPixel(x + 1, y, SSD1306_WHITE);
  display.drawPixel(x, y - 1, SSD1306_WHITE);
  display.drawPixel(x, y + 1, SSD1306_WHITE);
}


// Displays the text in the center
void drawCenteredText(const char* text) {
  int16_t x1;
  int16_t y1;
  uint16_t width;
  uint16_t height;

  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  display.getTextBounds(
    text,
    0,
    0,
    &x1,
    &y1,
    &width,
    &height
  );

  int x = (SCREEN_WIDTH - width) / 2;
  int y = 29;

  display.setCursor(x, y);
  display.print(text);
}


// Draws the progress bar
void drawProgressBar(unsigned long elapsedTime) {
  display.drawRect(
    9,
    55,
    110,
    6,
    SSD1306_WHITE
  );

  int progressWidth = map(
    elapsedTime,
    0,
    TEXT_DURATION,
    0,
    106
  );

  progressWidth = constrain(
    progressWidth,
    0,
    106
  );

  display.fillRect(
    11,
    57,
    progressWidth,
    2,
    SSD1306_WHITE
  );
}


// Draws a single animation frame
void drawScreen(unsigned long elapsedTime) {
  display.clearDisplay();

// Title
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(29, 2);
  display.print("NOW PLAYING");

  display.drawLine(
    19,
    13,
    108,
    13,
    SSD1306_WHITE
  );

  // Blinking heart icon
  if (animationFrame % 2 == 0) {
    drawHeart(4, 1);
  } else {
    display.drawRect(
      7,
      4,
      6,
      6,
      SSD1306_WHITE
    );
  }

  // Music note icon
  drawMusicNote(112, 1);

  // Moving sparkle
  int sparkleX = 20 + ((animationFrame * 8) % 88);

  drawSparkle(sparkleX, 20);

  if (animationFrame % 3 == 0) {
    drawSparkle(15, 42);
    drawSparkle(112, 40);
  }

  // Main text
  drawCenteredText(textList[currentText]);

  // Page number
  display.setTextSize(1);
  display.setCursor(54, 44);
  
  

  drawProgressBar(elapsedTime);

  display.display();
}


void setup() {
  Serial.begin(115200);

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDRESS
      )) {

    Serial.println("OLED not found!");

    while (true) {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();

  textStartTime = millis();
  lastFrameTime = millis();
}


void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime =
    currentTime - textStartTime;

  // Move to the next text segment
  if (elapsedTime >= TEXT_DURATION) {
    currentText++;

    if (currentText >= textCount) {
      currentText = 0;
    }

    textStartTime = currentTime;
    elapsedTime = 0;
  }

  // Update the animation
  if (currentTime - lastFrameTime >= FRAME_DURATION) {
    lastFrameTime = currentTime;
    animationFrame++;

    drawScreen(elapsedTime);
  }
}
