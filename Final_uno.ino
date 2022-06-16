#include <LiquidCrystal.h>
#include <Wire.h>

/*****ultasonic definition*****/
const int echoPin=9;
const int trigPin=8;
unsigned long dist;
unsigned long distance;

/**temperature definition**/
const int tempPin=A0;

/*LCD definition*/
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);


void setup() {
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  Serial.begin(9600);

  lcd.begin(16, 2);

  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() {
  int temp=0;
  float temperature=0;
  temp = analogRead(tempPin);
  /*AD convertion for NTC*/
  temperature=(1/(log(1/(1023./temp-1))/3950+1.0/298.15)-273.15);
  Serial.print("Temperature:");
  Serial.print(temperature);
  Serial.print("\n");

  /***Real time speed adjustment***/
  float soundspeed;
  soundspeed=331.45+0.61*temperature;

  dist = distanceMeasurements()*soundspeed/340;
  Serial.print("Distance is = ");
  Serial.print(dist);
  Serial.print("\n");

  /***LCD display***/
  lcd.setCursor(0,0);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Vs:");
  lcd.print((int)soundspeed);        
  lcd.print("m/s");
  
  lcd.setCursor(0,1);
  lcd.print("S:");
  lcd.print(dist);        
  lcd.print("cm");

  if(temperature>-10)
  {
  lcd.setCursor(11,1);
  lcd.print("T:");
  lcd.print((int)temperature);
  lcd.print("C");
  }
  else
  {
  lcd.setCursor(10,1);
  lcd.print("T:");
  lcd.print((int)temperature);
  lcd.print("C");
  }

    if(dist<100)
  {
    lcd.setCursor(12,0);
    lcd.print("OPEN");
  }
  else
  {
    lcd.setCursor(11,0);
    lcd.print("CLOSE");
  }
  /*Important! Make sure the two boards are locked in a 
  same frequency in loop or the port communication may 
  send or recieve a same signal mutiple times*/
  delay(1000);
}

/***Port communication function***/
void requestEvent(){
  Wire.write(dist);
  delay(1000);
}

/***Distance detection function***/
unsigned long distanceMeasurements()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long dist = pulseIn(echoPin, HIGH);
  distance = dist*0.034/2;

  return distance;
}
