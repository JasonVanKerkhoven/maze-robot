//declaring constants
#define LEFT_PWN 10
#define LEFT_DIR 9
#define RIGHT_PWN 5
#define RIGHT_DIR 6

#define USS_FRONT_ECHO 3
#define USS_FRONT_TRIG 2

#define USS_LEFT_TRIG 7
#define USS_LEFT_ECHO 8

#define USS_RIGHT_TRIG 11
#define USS_RIGHT_ECHO 12

#define DUTY_R 140
#define DUTY_L 85


//declaring global variables
char mov = 'h';
bool front = true;
bool left = false;
bool right = false;


//init ports as I/O
void setup()
{
  //config serial for debug
  Serial.begin(9600);
  Serial.print("serial init\n");
  
  //set motor GPIO as output
  pinMode(LEFT_PWN , OUTPUT);
  pinMode(LEFT_DIR , OUTPUT);
  pinMode(RIGHT_PWN , OUTPUT);
  pinMode(RIGHT_DIR , OUTPUT);

  //set ultrasonic GPIO
  pinMode(USS_FRONT_TRIG, OUTPUT);
  pinMode(USS_FRONT_ECHO, INPUT);
  pinMode(USS_LEFT_TRIG, OUTPUT);
  pinMode(USS_LEFT_ECHO, INPUT);
  pinMode(USS_RIGHT_TRIG, OUTPUT);
  pinMode(USS_RIGHT_ECHO, INPUT);
  
  //set motors to known state
  drive('h');
}


/*
 * drive function
 * 
 * INPUT    MOTOR_R   MOTOR_L   EFFECT
 * f        fwd       rev       drive forward   
 * b        rev       rev       drive backwards
 * r        rev       fwd       rotate right, on-the-spot
 * l        fwd       rev       rotate left, on-the-spot
 * h        off       off       full stop
 * R        off       fwd       hang right while moving forward
 * L        fwd       off       hang left while moving forward
 * 
 */
void drive(char dir)
{
  //only change bridge outputs if direction differs from current movement
  if (mov != dir)
  {
    //overwrite current movement and change bridge outputs
    mov = dir;
    switch(dir)
    {
      //drive FORWARD
      case('f'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 40);
        analogWrite(RIGHT_PWN, 0);
        break;
        
      //drive BACKWARDS
      case('b'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 255);
        analogWrite(RIGHT_PWN, 255);
        break;
        
      //rotate on-spot RIGHT
      case('r'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 255);
        break;
        
      //rotate on-spot LEFT
      case('l'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 255);
        analogWrite(RIGHT_PWN, 0);
        break;

      //gradual turn LEFT
      case('L'):
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, HIGH);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 0);
        break;

      //gradual turn RIGHT
      case('R'):
        digitalWrite(LEFT_DIR, HIGH);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 0);
        break;
  
      //stop
      default:
        digitalWrite(LEFT_DIR, LOW);
        digitalWrite(RIGHT_DIR, LOW);
        analogWrite(LEFT_PWN, 0);
        analogWrite(RIGHT_PWN, 0);
        break;
    }
  }
}


//test motors
void testDrive()
{
  drive('h');
  delay(1000);
  drive('r');
  delay(1000);
  drive('l');
  delay(1000);
  drive('f');
  delay(1000);
  drive('L');
  delay(1000);
  drive('R');
  delay(1000);
  drive('b');
  delay(1000);
}


/*
 * poll a ultrasonic sensor
 * DO NOT CALL
 */
unsigned long trigUltrasonic(char trigPin, char echoPin)
{
  unsigned long long deltaSum = 0;
  for (int i=0; i<30; i++)
  {
    //trigger pulse
    digitalWrite(trigPin,LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin,LOW);

    deltaSum += pulseIn(echoPin, HIGH);
    delay(1);
  }
  return (deltaSum/30);
}


//poll FRONT sensor
unsigned long trigFront()
{
	unsigned long d = trigUltrasonic(USS_FRONT_TRIG, USS_FRONT_ECHO);
  //Serial.print("FRONT:  ");
  Serial.println(d, DEC);
  return d;
}


//poll LEFT sensor
unsigned long trigLeft()
{
	unsigned long d = trigUltrasonic(USS_LEFT_TRIG, USS_LEFT_ECHO);
  //Serial.print("LEFT:   ");
  //Serial.println(d, DEC);
  return d;
}


//poll RIGHT sensor
unsigned long trigRight()
{
	unsigned long d = trigUltrasonic(USS_RIGHT_TRIG, USS_RIGHT_ECHO);
  //Serial.print("RIGHT:  ");
  //Serial.println(d, DEC);
  return d;
}


//main runtime
void loop()
{
  /*
  if (trigFront() > 450)
  {
    drive('f');
  }
  else if (trigRight() > 450)
  {
    drive('r');
    while (trigFront() < 450) {}
  }
  else if (trigLeft() > 450)
  {
    drive('r');
    while (trigFront() < 450) {}
  }
  */

  drive('f');
  trigFront();
  //trigRight();
  //trigLeft();
}

