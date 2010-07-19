#include <WProgram.h>

const int motorId = 5;
const int turnId = 5;

#define forever for(;;)

int main ()
{
  init ();
  
  pinMode (motorId, OUTPUT);
  pinMode (turnId, OUTPUT);
  
  // initialisiere Motor
  analogWrite (motorId, 128);
  delay (1000);
  analogWrite (motorId, 0);
  delay (1000);
  analogWrite (motorId, 255);
  delay (1000);
  
  //bleibe zunächst stehen
  analogWrite (motorId, 128);
  
  Serial.begin (9600);
  
  while (Serial.available() == 0)
  {
    Serial.write ('B');
  }
  
  if (Serial.read() != 'I')
    error (1);
  
  //start running
  forever
  {
    int turn = readTurnData ();
    analogWrite(turnId, turn);
    analogWrite (motorId, 270);
  }
  
}