# Proyecto-tapitas

## Contaminación lumínica

Al usar el sensor tenemos que tener en cuenta la contaminación lumínica ,para poder tener el minimo margen de error posible.
Para poder comprobar si tenemos contaminación ,podremos verlo en los valores que devuelve el sensor en el monitor de serie.
Si los valores de RGB indican todos 0,quiere decir que no hay contaminación.


## Código del sensor


#include "Adafruit_TCS34725.h"
#include <Servo.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

int pos = 0;
int entero;
int colorpos = 0;
int base = 25 ;
int counter = 12; 
bool red, green, blue, white, black, grey, pink = false;
bool list[7] = {red, green, blue, white, black, grey, pink};

Servo servo_9;

void setup() {
 servo_9.attach(9, 500, 2500);
 Serial.begin(9600);
 if(tcs.begin()){

   Serial.println("Inicializando");
   delay(2000);
 }
 else{
  Serial.println("Error");
  Serial.println("Debe de revisar las conexiones e iniciar nuevamente");
  while(1)delay(500); //el while es para que se forme un bucle en el else y el usuario tenga que revisar las conexiones para que salga

 }
}





void loop() {
  uint16_t R,G,B,C; //variable de 16 bits
  tcs.getRawData(&R,&G,&B,&C); //tomar datos del sensor, (& antes, para que R,G,B,C haga una copia y no cambie de valor
  
  if(R>255){R=255;} //limitar sensor
  if(G>255){G=255;}
  if(B>255){B=255;}
  
  if(R == 0 && G == 0 && B == 0) {
    colorpos = base;
  }
  else if(list[1] == true) {
    colorpos = base + counter;
  }
  else if(list[2] == true) {
    colorpos = base + (2*counter);
  }
  else if(R == 13 && G == 9 && B == 9) {
    colorpos = base + (3*counter);
  }
  else if(list[4] == true) {
    colorpos = base + (4*counter);
  }
  else if(list[5] == true) {
    colorpos = base + (5*counter);
  }
  else if(list[6] == true) {
    colorpos = base + (6*counter);
  }
  /* else{
    colorpos = base + (7*counter);
  } */
  
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
  
 Serial.println(" R = "+String(R)+ " G = "+String(G)+ " B = "+String(B)); //mensaje en consola
 delay(1000); // cada cuanto el sensor va a detectar un color



}
