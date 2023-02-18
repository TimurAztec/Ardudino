#include <LiquidCrystal.h>
#define BUTTON_PIN 7
#define BUZZER_PIN 8
#define DINO_MAX_FLY_TICKS 10

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

boolean dinoOnGround = true;
int dinoTicksToFly = DINO_MAX_FLY_TICKS;
int buttonState = LOW;
int buzzerState = LOW;
int highScore = 0;
int score = 0;
boolean gameStarted = false;
boolean scoreScreen = false;

byte dino[8] = {
  0x0F,
  0x0B,
  0x0F,
  0x0E,
  0x0F,
  0x1E,
  0x0A,
  0x0A
};

byte cactus[8] = {
  0x04,
  0x04,
  0x05,
  0x15,
  0x17,
  0x1C,
  0x04,
  0x04
};

int cactuses[16];

void setup() {  
  lcd.begin(16,2);
  lcd.createChar(0, dino);
  lcd.createChar(1,cactus);
  memset(cactuses, 0, sizeof(cactuses));
  randomSeed(analogRead(A0));
  pinMode(BUTTON_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  lcd.setCursor(0,0);                
  lcd.print("REBOOT");
}

void loop() {
  lcd.clear();
  buttonState = digitalRead(BUTTON_PIN);
  digitalWrite(BUZZER_PIN, buzzerState);
  if (buzzerState == HIGH) {
    buzzerState = LOW;
  }

  if (scoreScreen) {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Game over: ");
    lcd.print(score);
    if (score > highScore) {
      highScore = score;
    }
    lcd.setCursor(0,1);
    lcd.print("HighScore: ");
    lcd.print(highScore);

    if(buttonState == HIGH){
      scoreScreen = false;
    }
  }
  
  if(!gameStarted && !scoreScreen){  
    lcd.setCursor(0,0);                
    lcd.print("Press to start");
  
    if(buttonState == HIGH){
      gameStarted = true;
    }
  }
    
  if(gameStarted){
    gameLoop();
    delay(500);
  } else {
    delay(100);
  }
}

void gameLoop(){
  if (!dinoOnGround && dinoTicksToFly <= 0 || buttonState == LOW) {
    dinoOnGround = true;
  }
  if (dinoOnGround && dinoTicksToFly > 0 &&  buttonState == HIGH) {
    dinoOnGround = false;
  }
  if (dinoOnGround && cactuses[1] >= 1) {
    scoreScreen = true;
    gameStarted = false;
    memset(cactuses, 0, sizeof(cactuses));
    buzzerState = HIGH;
  }

  lcd.setCursor(1, dinoOnGround ? 1 : 0);
  lcd.write(byte(0));
  lcd.setCursor(10, 0);
  lcd.print(score);

  unsigned int cactuses_size = sizeof(cactuses)/sizeof(cactuses[0]);
  unsigned int cactussum = 0;
  for (int i=0 ; i < cactuses_size ; i++) {
    if(i+1 < cactuses_size) {
      cactuses[i] = cactuses[i+1];
    }
    if (i>0 && cactuses[i]>0) {
      cactussum++;
    }
    lcd.setCursor(i, 1);
    if (cactuses[i] > 0) {
      lcd.write(byte(1));
    }
  }
  cactuses[cactuses_size-1] = (random(0, 2) == 1 && cactussum <= 4) ? 1 : 0;

  if (!dinoOnGround) {
    dinoTicksToFly--;
  }
  if (dinoOnGround) {
    dinoTicksToFly = DINO_MAX_FLY_TICKS;
  }
  score++;
}
