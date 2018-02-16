//declaring constants
int FWD_LEFT = 9;
int BCK_LEFT = 10;
int FWD_RIGHT = 6;
int BCK_RIGHT = 5;
int DUTY_R = 100;
int DUTY_L = 150;

//init ports as I/O
void setup()
{
  //config serial
  //Serial.begin(9600);
  //Serial.print("serial init\n");
  
  //set motor GPIO as output
  pinMode(FWD_LEFT , OUTPUT);
  pinMode(BCK_LEFT , OUTPUT);
  pinMode(FWD_RIGHT , OUTPUT);
  pinMode(BCK_RIGHT , OUTPUT);
}


//print values of IR sensor
void printSensor(boolean r, boolean l, boolean c)
{
  Serial.print("\n\nR: ");
  Serial.print(r, DEC);
  Serial.print("\nL: ");
  Serial.print(l, DEC);
  Serial.print("\nC: ");
  Serial.print(c, DEC);
}



//operate wheels
void drive(char dir)
{
  analogWrite(BCK_RIGHT, 0);
  analogWrite(BCK_LEFT,  0);
  analogWrite(FWD_LEFT,  0);
  analogWrite(FWD_RIGHT, 0);
  switch(dir)
  {
    case('f'):
      analogWrite(FWD_LEFT,  DUTY_L);
      analogWrite(FWD_RIGHT, DUTY_R);
      break;
    case('l'):
      analogWrite(FWD_RIGHT, DUTY_R);
      analogWrite(BCK_LEFT,  DUTY_L);
      break;
    case('r'):
      analogWrite(BCK_RIGHT, DUTY_R);
      analogWrite(FWD_LEFT,  DUTY_L);
      break;
    case('b'):
      analogWrite(BCK_RIGHT, DUTY_R);
      analogWrite(BCK_LEFT,  DUTY_L);
      break;
  }
}


//test drive
void testDrive()
{
  drive('f');
  delay(5000);
  drive('r');
  delay(5000);
  drive('l');
  delay(5000);
  drive('b');
  delay(5000);
  drive('h');
  delay(5000);
}


//main
void loop()
{
  //poll sensors (true for tape, false for no tape)
  bool tapeRight = (digitalRead(7) == 0) ? false : true;
  bool tapeLeft = (digitalRead(8) == 0) ? false : true;
  bool tapeCenter = (digitalRead(11) == 0) ? false : true;

  //end reached
  if (tapeLeft && tapeCenter && tapeRight)
  {
    drive('h');
  }
  //turn right
  else if (!tapeLeft && tapeRight)
  {
    drive('r');
  }
  //turn left
  else if (tapeLeft && !tapeRight)
  {
    drive('l');
  }

  //straight T
  else if (tapeLeft && !tapeCenter && tapeRight)
  {
    drive('r');
  }
  //right T |--
  else if (!tapeLeft && tapeCenter && tapeRight)
  {
    drive('r');
  }
  //left T --|
  else if (tapeLeft && tapeCenter && !tapeRight)
  {
    drive('f');
  }

  //forward
  else
  {
    drive('f');
  }
}
