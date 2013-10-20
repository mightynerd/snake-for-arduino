//snake-for-arduino
//Licenced under the GPL v2 licence
#include <TFT.h>  // Arduino LCD library
#include <SPI.h>
#include <SD.h>

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
byte snakeLenght = 15;

//Apple variables
byte appleX = 0;
byte appleY = 0;

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
  tft.setTextSize(2);
  tft.text("GAME OVER", 10, 10);
  
  char pointChars[4];
  String pointVal = String(snakeLenght);
  pointVal.toCharArray(pointChars, 4);
  
  tft.text(pointChars, 10, 30);
  
  while(true)
  {
     if (digitalRead(gameReset) == 0)
    {
       gameState = Game;
       resetGame();
    } 
  }
  
}

void resetGame()
{
   //Reset coordinates etc. 
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
  tft.background(10, 10, 250);
  tft.setTextSize(4);
  tft.text("Snake", 14, 10);
  
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

int a = 0;
  
void game()
{
  //Main game loop
  tft.background(50, 50, 250);
  
  //Spawn apple in the beginning
  if (appleX == 0 && appleY == 0)
  {
     spawnApple(); 
  }
  
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
  tft.fill(200, 20, 20);
  
  for (byte xi = 0; xi < 5; xi++)
  {
    for (byte yi = 2; yi < 4; yi++)
    {    
      tft.point((x * 5) + xi, (y * 5) + yi);
    }
  }
  
  //TODO: Gör klart äpplet
  
  tft.fill(20, 200, 20);
  tft.point((x * 5) + 1, y * 5);
}

void drawSnakePoint(byte x, byte y)
{
  //Screen resolution: 160 x 128
  //Game grid size: 32 x 25 
  const byte tileWidth = 5;

  tft.fill(20, 200, 20);
  //Draw a 5x5 square (should be replaced with rect)
  for (byte xi = 0; xi < tileWidth; xi++)
  {
    for (byte yi = 0; yi < tileWidth; yi++)
    {    
      tft.point((x * tileWidth) + xi, (y * tileWidth) + yi);
    }
  }
}
