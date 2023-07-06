#include <Wire.h>
#include <Adafruit_VL53L0X.h>

Adafruit_VL53L0X sensor = Adafruit_VL53L0X();

int Count = 0;
int distance;

//Motor Pins 
int in1 = 6;
int in2 = 9;
int in3 = 11;
int in4 = 10;

//IR Pins
int IRSensorL = 2;
int IRSensorR = 3;
int IRL;
int IRR;

void Stop()
{
  analogWrite(in1, 0);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, 0);
  Serial.println("Stop");
}

void North()
{
  int speed = 70;
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, speed-7);
  analogWrite(in4, 0);
  Serial.println("North");
}

void East()
{
  int speed = 50;
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, speed-5);
  analogWrite(in4, 0);
  Serial.println("East");
}

void South()
{
  int speed = 70;
  analogWrite(in1, 0);
  analogWrite(in2, speed);
  analogWrite(in3, 0);
  analogWrite(in4, speed-7);
  Serial.println("South");
}

void West()
{
  int speed = 50;
  analogWrite(in1, speed);
  analogWrite(in2, 0);
  analogWrite(in3, 0);
  analogWrite(in4, speed-5);
  Serial.println("West");
}

void handleInterrupt() 
{
  IRL = digitalRead(IRSensorL);
  IRR = digitalRead(IRSensorR);
  while (IRL==0 || IRR==0)
  {
    IRL = digitalRead(IRSensorL);
    IRR = digitalRead(IRSensorR);
    if (IRL==1 && IRR==1)
    {
      break;
    }
    else if (IRL==0 && IRR==1)
    {
      Count = 1;
      South();
      West();
    }
    else if (IRL==1 && IRR==0)
    {
      Count = 0;
      South();
      East();
    }
    else if (IRL==0 && IRR==0)
    {
      South();
      West();
    }
  }
}

void setup() 
{
  Serial.begin(9600);
  Wire.begin();
  
  if (!sensor.begin()) 
  {
    Serial.println("Failed to initialize VL53L0X sensor!");
    while (1);
  }
  
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  pinMode(IRSensorL, INPUT);
  pinMode(IRSensorR, INPUT);

  attachInterrupt(digitalPinToInterrupt(IRSensorL), handleInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(IRSensorR), handleInterrupt, CHANGE);
}

void loop()
{
  VL53L0X_RangingMeasurementData_t measure
  sensor.rangingTest(&measure, false);
  if (measure.RangeStatus != 4) // check for a valid measurement
  { 
    distance = measure.RangeMilliMeter;
    if (distance<=1300)
    {
      North();
    }
    else if (Count == 1)
    {
      West();
    }
    else if (Count == 0)
    {
      East();
    }
  }
}
