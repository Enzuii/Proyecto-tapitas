#include "Adafruit_TCS34725.h"
#include <Servo.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

float average;
float rojo, verde, azul;
int pos = 90;
int colorpos = 90;
int base = 34;
int counter = 16; 
bool red, green, blue, white, black, grey, pink = false;
bool list[7] = {white, grey, blue, red, green, black, pink};

Servo servo_9;


//Sacar todos los comandos "Serial" al momento de usar el programa sin computadora. Esto porque sino puede entorpecerse el programa.
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
  
  average = (R+G+B)/3;
  rojo = R/average;
  verde = G/average;
  azul = B/average;

  
  if (R > 100) { //diferenciacion del rojo o colores claros
    list[0] = true;
  }
  else if (R < 50) { //diferenciacion del negro o colores oscuros
    list[6] = true;
  }

  
  if(list[0] == true) {
    colorpos = base;
    list[0] = false;
  }
  else if(list[1] == true) {
    colorpos = base + counter;
    list[1] = false;
  }
  else if(list[2] == true) {
    colorpos = base + (2*counter);
    list[2] = false;
  }
  else if(list[3] == true) {
    colorpos = base + (3*counter);
    list[3] = false;
  }
  else if(list[4] == true) {
    colorpos = base + (4*counter);
    list[4] = false;
  }
  else if(list[5] == true) {
    colorpos = base + (5*counter);
    list[5] = false;
  }
  else if(list[6] == true) {
    colorpos = base + (6*counter);
    list[6] = false;
  }
  /*else if (list[7] == true) {
    colorpos = base + (7*counter);
    list[7] = false;
  } */



  if (pos < colorpos) {
    for (pos; pos <= colorpos; pos += 1) {
      servo_9.write(pos);
      delay(10);
    }   
  }
  else if (pos > colorpos) {
    for (pos; pos >= colorpos; pos -= 1) {
      servo_9.write(pos);
      delay(10);
    }
  }
  
  pos = colorpos;
  
  
  Serial.println(" R = "+String(R)+ " G = "+String(G)+ " B = "+String(B)); //mensaje en consola
  Serial.println(" rojo = "+String(rojo)+ " verde = "+String(verde)+ " azul = "+String(azul)); //mensaje en consola
  delay(2000); // cada cuanto el sensor va a detectar un color

}
