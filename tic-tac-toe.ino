struct ledCoord_t {int cathode; int anode;};
typedef struct ledCoord_t led;

int movePin = 6;
int selectPin = 7;
int cathodePin1 = 8;
int cathodePin2 = 12;
int cathodePin3 = 13;
int anodePin1 = 11;
int anodePin2 = 10;
int anodePin3 = 9;

led startup = {0,0};
int leds[3][3];

void setup() 
{
 pinMode(movePin, INPUT_PULLUP);
 pinMode(selectPin, INPUT_PULLUP);
 pinMode(anodePin1, OUTPUT);
 pinMode(anodePin2, OUTPUT);
 pinMode(anodePin3, OUTPUT);
 pinMode(cathodePin1, OUTPUT);
 pinMode(cathodePin2, OUTPUT);
 pinMode(cathodePin3, OUTPUT);
 Serial.begin(9600);
 // lights up the first led at start
 leds[startup.cathode][startup.anode] = 1;
 leds[1][1] = 1;
 leds[2][2] = 1;
}

void loop() 
{
  LightSelected();
  CheckMovePin(); // if moved -> 0 the current pos and puts 1 to the next posssible pos
  CheckSelectPin();

}

int CheckSelectPin()
{
  int period = 10;
  int timeNow = millis();
  int selectPrevious = digitalRead(selectPin);
  while(millis() < timeNow + period)
  {
    LightSelected();
    int selectNow = digitalRead(selectPin);
    if(selectNow == LOW && selectNow != selectPrevious)  //if pressed select
    {
      led lastLed = {2,2};
      startup = next(lastLed);
      break;
    }
    selectPrevious = selectNow;
  }
}

int CheckMovePin()
{
  int period = 10;
  int timeNow = millis();
  int movePrevious = digitalRead(movePin);
  while(millis() < timeNow + period) // delay that ensures taht the leds will glow with full capacity
  {
    LightSelected();
  }
  int moveNow = digitalRead(movePin);
  if(moveNow == LOW && moveNow != movePrevious)  //if pressed move
  {
    leds[startup.cathode][startup.anode] = 0;
    startup = next(startup);
    leds[startup.cathode][startup.anode] = 1; 
  }
}

led next(led current) //check next acessible led array
{
  led next;
  for(int c = current.cathode; c < 3; c++)
  {
    for(int a = current.anode + 1; a < 3; a++) //alwas work for the next
    {
      if(leds[c][a] == 0)
      {
        next.anode = a;
        next.cathode = c;
        return next;
      }
    }
    current.anode = -1;
    if(c == 2)
    {
      c = -1;
    }
  }
}

void LightUp(led current)
{
  switch(current.cathode)
  {
   case 0: current.cathode = cathodePin1; break;
   case 1: current.cathode = cathodePin2; break;
   case 2: current.cathode = cathodePin3; break;
   default: Serial.println("ERR: incorrect led Coord (cathode)");//print
  }
  switch(current.anode)
  {
   case 0: current.anode = anodePin1; break;
   case 1: current.anode = anodePin2; break;
   case 2: current.anode = anodePin3; break;
   default: Serial.println("ERR: incorrect led Coord (anode)");//print
  }
  ClearAll();
  digitalWrite(current.anode, HIGH);
  digitalWrite(current.cathode, LOW);
}

int LightSelected()  //lights up the pins with 1 at start of the loop
{
  int selectedNum = 0;
  for(int c = 0; c < 3; c++)
  {
    for(int a = 0; a < 3; a++)
    {
      if(leds[c][a] == 1)
      {
        selectedNum++;
        led led = {c, a};
        LightUp(led);
      }
    }
  }
  return selectedNum;
}

void ClearAll()
{
 digitalWrite(anodePin1, LOW);
 digitalWrite(anodePin2, LOW);
 digitalWrite(anodePin3, LOW);
 digitalWrite(cathodePin1, HIGH);
 digitalWrite(cathodePin2, HIGH);
 digitalWrite(cathodePin3, HIGH);
}

void printLeds()
{
  for(int c = 0; c < 3; c++)
  {
    for(int a = 0; a < 3; a++)
    {
      Serial.print(leds[c][a]);
    }
    Serial.println();
  }
  Serial.println(); 
}
