#include <LedControl.h>

const int DIN_PIN = 12;
const int CS_PIN = 11;
const int CLK_PIN = 10;
const int BUTTON_PIN = 2;

LedControl lc = LedControl(DIN_PIN, CLK_PIN, CS_PIN, 1);

int dinoY = 6; // Initial position of the dinosaur
bool isJumping = false;
int jumpHeight = 2;
int jumpCounter = 0;

int obstacleX = 7; // Initial position of the obstacle
int obstacleY = 6; // Position of the obstacle

bool gameOver = false;
unsigned long gameOverTime = 0;

void setup() {
  lc.shutdown(0, false);
  lc.setIntensity(0, 8);
  lc.clearDisplay(0);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  if (gameOver) {
    lc.clearDisplay(0);
    lc.setLed(0, 3, 3, true); // Display a simple game over indicator
    lc.setLed(0, 3, 4, true);
    lc.setLed(0, 4, 3, true);
    lc.setLed(0, 4, 4, true);

    if (millis() - gameOverTime > 3000) { // Check if 3 seconds have passed
      restartGame();
    }
    return;
  }

  if (digitalRead(BUTTON_PIN) == HIGH && !isJumping) {
    isJumping = true;
    jumpCounter = 0;
  }

  if (isJumping) {
    if (jumpCounter < jumpHeight) {
      dinoY--;
    } else if (jumpCounter < 2 * jumpHeight) {
      dinoY++;
    } else {
      isJumping = false;
    }
    jumpCounter++;
  }

  // Move the obstacle
  obstacleX--;
  if (obstacleX < 0) {
    obstacleX = 7; // Reset obstacle to the right side of the screen
  }

  // Check for collision
  if (obstacleX == 1 && dinoY == obstacleY) {
    gameOver = true;
    gameOverTime = millis(); // Record the time when game over occurs
  }

  lc.clearDisplay(0);
  lc.setLed(0, dinoY, 1, true); // Display the dinosaur
  lc.setLed(0, obstacleY, obstacleX, true); // Display the obstacle

  delay(100); // Adjust the speed of the game
}

void restartGame() {
  dinoY = 6;
  isJumping = false;
  jumpCounter = 0;
  obstacleX = 7;
  gameOver = false;
  lc.clearDisplay(0);
}
