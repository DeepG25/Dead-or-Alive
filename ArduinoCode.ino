#include<SoftwareSerial.h>
#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;
SoftwareSerial xbee(10,11); // RX , TX

int sound_pin = 7;
int s=0;
int e=0;
int count=0;
int startMin = -90;
int startMax = -65;
int endMin = -50;
int endMax = 0;
int l=250;
int a[250],b[250];
int c=0;

void setup() 
{
  Serial.begin(9600);
  xbee.begin(9600);
  pinMode(sound_pin,INPUT);
  Serial.println("Initialize MPU6050");
  Serial.println("XBEE Initialized");
  xbee.println("XBEE Initialized");
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
  pinMode(13,OUTPUT);
}

void loop()
{
  // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();
  
  int sound_value = digitalRead(sound_pin);
  // Calculate Pitch & Roll
  int pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
  
  if(c==l)
  {
    int d=0;
    for(int i=0;i<l;i++)
    {
      if(a[i] == 1 || b[0]!=b[l-1])
      {
        digitalWrite(13,HIGH);
        xbee.println("\n\nThe person is ALIVE\n\n");
        d++;
        break;
      }
    }
    if(d==0)
    {
      digitalWrite(13,HIGH);
    xbee.println("\n\nThe preson is DEAD\n\n");
    while(1){}
    }
    c=0;
  }
  else
  {
    a[c]=sound_value;
    b[c]=count;
    c++;
  }

  // Output
  //xbee.println(pitch);
  if(startMin < pitch && startMax > pitch && s==0)
  {
    s=1;
    e=0;
  }
  if(endMin < pitch && endMax > pitch && e==0)
  {
    e=1;
    s=0;
    count=count+1;    // Step Counter
  Serial.print("No of Steps: ");
  Serial.println(count);
  xbee.print("No of Steps: ");
  xbee.println(count);
  }
  Serial.print("Sound :");
  Serial.println(sound_value);
  //xbee.print("Sound :");
  //xbee.println(sound_value);
  digitalWrite(13,LOW);
}
