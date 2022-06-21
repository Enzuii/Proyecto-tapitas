#include <Servo.h>

int pos = 0;
int entero;
int colorpos = 0;
int xran;
bool red, green, blue, white, black, grey, pink = false;
bool list[7] = {red, green, blue, white, black, grey, pink};

Servo servo_9;

void setup()
{
  servo_9.attach(9, 500, 2500);
  Serial.begin(9600);
}

void loop()
{
  xran = random(0,7);
  list[xran] = true;
  Serial.println(xran);
  
/*  for (entero = 0; entero <= 8; +=1) {
    if (list[entero] == true) {
      entero = 9
    }
  }
*/
  
  if(list[0] == true) {
    colorpos = 22;
  }
  else if(list[1] == true) {
    colorpos = 44;
  }
  else if(list[2] == true) {
    colorpos = 66;
  }
  else if(list[3] == true) {
    colorpos = 88;
  }
  else if(list[4] == true) {
    colorpos = 110;
  }
  else if(list[5] == true) {
    colorpos = 132;
  }
  else if(list[6] == true) {
    colorpos = 154;
  }
  else{
    colorpos = 176;
  }
  
  if (pos < colorpos) {
    for (pos; pos <= colorpos; pos += 1) {
    servo_9.write(pos);
    delay(5);
    }   
  }
  else if (pos > colorpos) {
    for (pos; pos >= colorpos; pos -= 1) {
    servo_9.write(pos);
    delay(5);
    }
  }
  delay(1000);
  list[xran] = false;
}
