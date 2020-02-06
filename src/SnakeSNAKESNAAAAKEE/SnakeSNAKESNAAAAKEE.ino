#include <Arduboy2.h>
#define TILE_SIZE 4
#define FIELD_WIDTH WIDTH / TILE_SIZE
#define FIELD_HEIGHT HEIGHT / TILE_SIZE
#define MAX_SPACE FIELD_WIDTH * FIELD_HEIGHT
#define TITLE_SCREEN 0
#define SNAKING 1
#define GAMEOVER 2
#define WIN_LENGTH MAX_SPACE
#define BORDER 600


Arduboy2 arduboy;
BeepPin1 beep;
uint8_t MaxLength = 3;
uint8_t Direction = RIGHT_BUTTON;
uint8_t Gamestate=TITLE_SCREEN;
uint16_t Tail[WIN_LENGTH];
uint16_t Head = 102;
uint16_t Apple = 300;


void setup() {
  // put your setup code here, to run once:
  arduboy.begin();
  arduboy.setFrameRate(5);
  beep.begin();
  SetField();
}

void loop() {
  if(Gamestate==SNAKING)
  {
      if(arduboy.pressed(RIGHT_BUTTON) && Direction != LEFT_BUTTON)
      {
        Direction = RIGHT_BUTTON;
      }
      if(arduboy.pressed(LEFT_BUTTON) && Direction != RIGHT_BUTTON)
      {
        Direction = LEFT_BUTTON;
      }
      if(arduboy.pressed(UP_BUTTON) && Direction != DOWN_BUTTON)
      {
        Direction = UP_BUTTON;
      }
      if(arduboy.pressed(DOWN_BUTTON) && Direction != UP_BUTTON)
      {
        Direction = DOWN_BUTTON;
      }
  }
  if(!arduboy.nextFrame()){
    return;
  }
  beep.timer();
  arduboy.clear();
  
  arduboy.pollButtons();
  switch(Gamestate)
  {
    case TITLE_SCREEN:
      TitleScreen();
      break;
    case SNAKING:
      Snake();
      break;
    case GAMEOVER:
      GameOver();
      break;  
  }
  
  arduboy.display();
}

void TitleScreen()
{
  arduboy.print("Snake!\nSNAKE!!\nSNAAAAKEE!!");     
  if(arduboy.justPressed(A_BUTTON))
  {
    arduboy.initRandomSeed();
    beep.tone(beep.freq(659.255), 1);
    Apple = GetApple();
    Gamestate = SNAKING;
  }
}

void Snake()
{
      
      switch(Direction){
        case RIGHT_BUTTON:
          Head++;
          break;
        case LEFT_BUTTON:
          Head--;
          break;
        case UP_BUTTON:
          Head -= FIELD_WIDTH;
          break;
        case DOWN_BUTTON:
          Head += FIELD_WIDTH;
          break;  
      }
      
      if(Tail[Head]>0)
      {
        Gamestate=GAMEOVER;
        beep.tone(beep.freq(200), 3);
        return;
      }
      Tail[Head] = 1;
      
      if(Head == Apple && MaxLength < WIN_LENGTH)
      {
          MaxLength++;
          beep.tone(beep.freq(659.255), 1);
          Apple = GetApple();
      }
      
      uint16_t counter = 0;
      
      for(int y = 0; y < FIELD_HEIGHT; y++)
      {
        for(int x = 0; x < FIELD_WIDTH; x++)
        {
          if(Tail[counter]!=BORDER && Tail[counter] > 0)
          {
            if(counter != Head)
            {
              if(Tail[counter] == MaxLength)
              {
                Tail[counter] = 0;
              }
              else
              {
                Tail[counter]++;
              }
            }
          }
          
          if(Tail[counter] > 0 || counter == Apple)
          {
            arduboy.fillRect(x*TILE_SIZE,y*TILE_SIZE,TILE_SIZE,TILE_SIZE);
          }
          counter++;
        }
      }
}

void GameOver()
{
  arduboy.print("You lost\nYour Score:");
  arduboy.print(MaxLength);
  if(arduboy.justPressed(A_BUTTON))
  {
    SetField();
    MaxLength = 3;
    Direction = RIGHT_BUTTON;
    Head = 102;
    Apple = random(-1,MAX_SPACE);
    Gamestate = TITLE_SCREEN;
  }
}

uint16_t GetApple()
{
  uint16_t newApple = random(-1,MAX_SPACE);
  while(Tail[newApple]>0)
  {
    newApple = random(-1,MAX_SPACE);
  }
  return newApple;
}

void SetField()
{
  for(uint16_t i=0;i<MAX_SPACE;i++)
  {
    if(i<33)
    {
      Tail[i]=BORDER;
    }
    else if(i%32==0)
    {
      Tail[i]=BORDER;
    }
    else if(i%32==31)
    {
      Tail[i]=BORDER;
    }
    else if(i>480)
    {
      Tail[i]=BORDER;
    }
    else{
      Tail[i]=0;  
    }
  }
  Tail[100] = 3;
  Tail[101] = 2;
  Tail[102] = 1;
  
}
