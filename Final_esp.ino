#define BLINKER_WIFI 
#include <Blinker.h>
#include <Wire.h>

/***IO distribution**/
#define I2C_SDA 32
#define I2C_SCL 33
 
/***Connection to app***/ 
char auth[] = "53d3c097ed0b";//Secret key to the board
char ssid[] = "TP-LINK_B0A6";//WIFI name
char pswd[] = "20000429";//WIFI code
 
int freq = 50;     
int channel = 8;   
int resolution = 8; 
const int led = 16;
  
BlinkerButton Button1("btn-max"); 
BlinkerButton Button2("btn-min"); 
int servo_max=180,servo_min=90;

/**PWM function(ESP32 cannot directly use head file<servo.h> 
for some unknown reasons)**/
int calculatePWM(int degree)
{ 
  const float deadZone = 6.4;
  const float max = 32;
  if (degree < 0)
    degree = 0;
  if (degree > 180)
    degree = 180;
  return (int)(((max - deadZone) / 180) * degree + deadZone);
}

/**Open gate button**/
void button1_callback(const String & state)    
{    
  BLINKER_LOG("get button state: ", servo_max); 
  ledcWrite(channel, calculatePWM(90));  
  Blinker.vibrate();  
}

/**Close gate button**/
void button2_callback(const String & state)   
{         
  BLINKER_LOG("get button state: ", servo_min);  
  ledcWrite(channel, calculatePWM(180));
  Blinker.vibrate();    
}
 
void setup()    
{   
  Serial.begin(9600);
  /**Set channel of PWM for ESP**/   
  ledcSetup(channel, freq, resolution); 
  ledcAttachPin(led, channel);          
  BLINKER_DEBUG.stream(Serial);    
  Blinker.begin(auth, ssid, pswd); 
  Button1.attach(button1_callback);   
  Button2.attach(button2_callback);
  Wire.begin(I2C_SDA,I2C_SCL);     
}

/**Recieve distance data from uno and manipulate the gate**/
void loop() 
{   
  Wire.requestFrom(8,1);
  while(Wire.available())
{
  int c= Wire.read();
  Serial.print(c);
  if(c<10)
  {
    ledcWrite(channel, calculatePWM(180));
    delay(500);
    ledcWrite(channel, calculatePWM(90));    
  }

}
  delay(500);
  Blinker.run();
    
}
