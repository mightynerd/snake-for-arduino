//snake-for-arduino
//Licenced under the GPL v2 licence
#include <SPI.h>
#include <TFT.h>  // Arduino LCD library

enum GameState{Logo, Game, GameOver} gameState;

// pin definition for the Uno
#define cs   10
#define dc   9
#define rst  8  

//Constants for pins
const byte gameReset = 6;

TFT tft = TFT(cs, dc, rst);

//Variables
byte mapGrid[800];

//Snake variables
byte x = 5;
byte y = 5;
byte startSnakeLenght = 4;
byte snakeLenght = startSnakeLenght;

//Apple variables
byte appleX = 40;
byte appleY = 40;

enum SnakeDir{Up, Down, Right, Left} snakeDir = Right;

void setup() {
  
  //Pin mode
  pinMode(gameReset, INPUT);
  digitalWrite(gameReset, HIGH);
  
  tft.begin();
  tft.background(10, 10, 250);
  Serial.begin(9600);

}

void loop() {
  
  if (gameState == Logo)
  {
      logo();
  }
  else if (gameState == Game)
  {
      game();
  }
  else if (gameState = GameOver)
  {
    gameOver();
  }
  
}

void gameOver()
{
   tft.stroke(0, 255, 0);
  tft.fill(0, 255, 0);
  
  tft.setTextSize(2);
  tft.text("GAME OVER", 28, 40);
  
  char pointChars[4];
  String pointVal = String(snakeLenght);
  pointVal.toCharArray(pointChars, 4);
  
  tft.setTextSize(2);
  tft.text("Score:", 37, 60);
  tft.text(pointChars, 107, 60);
  
  tft.setTextSize(1);
  tft.text("[D6] to restart", 15, 110);
  
  while(true)
  {
    if (digitalRead(gameReset) == 0)
    {
       resetGame();
       gameState = Game;
       break;
    } 
  }
  
}

void resetGame()
{
   snakeLenght = startSnakeLenght;
   x = 5;
   y = 5;
   appleX = 40;
   appleY = 40;
   snakeDir = Right;
   resetMap();
}

void resetMap()
{
  for (short i = 0; i < 800; i++)
  {
     mapGrid[i] = 0;
  } 
}

void logo()
{
  //Write the logo
  tft.background(129, 150, 192);
  
  tft.stroke(0, 255, 0);
  tft.fill(0, 255, 0);
  
  tft.setTextSize(4);
  tft.text("Snake", 20, 10);
  
  tft.setTextSize(2);
  tft.text("for Arduino", 12, 45);
  
  tft.setTextSize(1);
  tft.text("Stas Zwierzchowski", 25, 75);
  tft.text("Daniel Back", 45, 85);
  
  tft.text("Herrgardsgymnasiet 2013", 12, 105);
  
  delay(4000);
  tft.background(129, 150, 192);
  tft.setTextSize(2);
  tft.text("Press [D6]", 20, 45);
  tft.text("to start", 30, 65);
  
  
  //Wait until the user presses gameReset
  while(true)
  {
    if (digitalRead(gameReset) == 0)
    {
      gameState = Game;
      break;
    }
  }
  

}

  
void game()
{
  //Main game loop
  //tft.background(50, 50, 250);
  tft.background(129, 150, 192);
  
  //Spawn apple in the beginning
  if (appleX == 40 && appleY == 40)
  {
     spawnApple(); 
  }
  
  //Apple with snake collision
  if (x == appleX && y == appleY)
  {
    //Reset apple coordinates
    appleX = 40;
    appleY = 40;
    
    //MAke snake longer
    snakeLenght++;
  }
  Serial.println(snakeLenght);
  
  handleInput();
  movement();
  timerDecrease();
  updateMap();
  draw();
  
  //Adjust this to change the game speed
  delay(250);
}

void spawnApple()
{
  appleX = (byte)random(1, 31);
  appleY = (byte)random(1, 24);
  Serial.print("RANDOM: x=");
  Serial.print(appleX);
  Serial.print(", y=");
  Serial.print(appleY);
  
}

void handleInput()
{
  
  if (analogRead(A0) > 600)
  {
     snakeDir = Up; 
  }
  else if(analogRead(A0) < 400)
  {
     snakeDir = Down; 
  }
  else if(analogRead(A1) > 600)
  {
     snakeDir = Left; 
  }
  else if(analogRead(A1) < 400)
  {
     snakeDir = Right; 
  }
  
  Serial.print("A0:");
  Serial.print(analogRead(A0));
  Serial.print("\n");
  Serial.print("A1:");
  Serial.print(analogRead(A1));
  Serial.print("\n");
}

void updateMap()
{
   byte xi = 0;
   byte yi = 0;
  
   //Go through each byte in mapGrid
   for (short i = 0; i < 800; i++)
  {
     
      //If the snakes collides with itself
      if (x == xi && mapGrid[i] > 0 && y == yi)
      {
         gameState = GameOver; 
      }
      
      //The point the snake goes on
      if (xi == x && yi == y)
      {
         mapGrid[i] = snakeLenght; 
      }
      
      
      //Keep track on the x and y in mapGrid
      xi++;
      if (xi == 32)
      {
        xi = 0;
        yi++; 
      }
  } 
}

void timerDecrease()
{
  //Decrease each byte in mapGrid with one if it's > 0
  for (short i = 0; i < 800; i++)
  {
    if (mapGrid[i] > 0)
    {
         mapGrid[i] -= 1; 
    }
  } 
}

void movement()
{
  
  if (snakeDir == Up)
  {
    if (y != 0)
    {
      y-=1;
    }
    else 
    {
      y = 24; 
    }
  } 
  
  if (snakeDir == Down)
  {
    if (y != 24)
    {
      y++;
    }
    else
   {
      y = 0;
   } 
  }
  
  if (snakeDir == Right)
  {
    if (x != 31)
    {
      x++;
    }
    else
   {
      x = 0;
   } 
  }
  
  
  if (snakeDir == Left)
  {
    if (x != 0)
    {
      x-=1;
    }
    else
    {
      x = 31;
    }
    
  } 
  
  Serial.print("Movement x:");
  Serial.print(x);
  Serial.print(", y:");
  Serial.print(y);
  Serial.print("\n");
}

void wallCollision()
{
  //Doesn't work, should be removed
  if (x == 32)
  {
     x = 0; 
     Serial.println("A");
     return;
  }
  else if (x == 0)
  {
     x = 32; 
     Serial.println("B");
     return;
  }
  else if (y == 25)
  {
     y = 0; 
     Serial.println("C");
     return;
  }
  else if (y == 0);
  {
     y = 25; 
     Serial.println("D");
     return;
  }
}


void draw()
{
  drawApple(appleX, appleY);
  
  byte x = 0;
  byte y = 0;
  
  for (short i = 0; i < 800; i++)
  {
    if (mapGrid[i] > 0)
    {
      drawSnakePoint(x, y);
    }
     
    x++;
    if (x == 32)
    {
      x = 0;
      y++; 
    }
  } 
}

void drawApple(byte x, byte y)
{
  //Red
  tft.stroke(200, 20, 20);
  tft.fill  (200, 20, 20);
  
  //2x5 middle rectangle
  tft.rect((x * 5) + 0, (y * 5) + 2, 5, 2);
  
  //Bottom 3 pixels
  tft.rect((x * 5) + 1, (y * 5) + 4, 3, 1);
  
  //Top 3 pixels
  tft.rect((x * 5) + 1, (y * 5) + 1, 3, 1);
  
  //Top 2 green pixels
  tft.stroke(20, 200, 20);
  tft.rect((x * 5) + 1, (y * 5) + 0, 2, 1);
}

void drawSnakePoint(byte x, byte y)
{
  //Screen resolution: 160 x 128
  //Game grid size: 32 x 25 
  const byte tileWidth = 5;

  tft.stroke(20, 100, 20);
  tft.fill  (20, 100, 20);
  //Draw a 5x5 square
  tft.rect(x * 5, y * 5, 5, 5);
}
