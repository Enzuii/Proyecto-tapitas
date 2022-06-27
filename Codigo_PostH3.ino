#include <Adafruit_TCS34725.h>
#include <Servo.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);

float average;
float rojo, verde, azul;
int pos = 90;
int colorpos = 90;
int base = 34;
int counter = 16; 
bool red, yellow, green, blue, white, grey, black = false;
bool list[7] = {red, yellow, green, blue, white, grey, black};

float promer, promeg, promeb = 0;

float maxir = 0;
float maxig = 0;
float maxib = 0;

float minir = 100;
float minig = 100;
float minib = 100;
int entero = 0;


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

  if (((rojo>1.85) && (verde<0.7) && (azul<0.6)) || ((rojo>1.3) && (verde<0.8) && (azul<0.8) && (azul>0.65)) ) { //Rojo, Naranja || Violeta
    list[0] = true;
  }
  else if ((rojo>1.6) && (verde<1) && (azul<0.6)) { //Amarillo patito
    list[1] = true;
  }
  else if ((rojo>1.25) && (verde>0.9) && (azul<0.7) || (rojo>0.8) && (verde>1.2) && (azul<0.9)) { //VerdeTrans, Verde PasoDLT || Verde Salus
    list[2] = true;
  }
  else if ((rojo<0.85) && (verde<1.1) && (azul>1.1) || (rojo<1.1) && (verde<1.3) && (azul>1)) { //Azul sucio || Celeste
    list[3] = true;
  }
  else if ((R>150) && (G>150) && (B>150)) { //Blanco
    list[4] = true;
  }
  else if ((R>50) && (G>50) && (B>50)) { //Gris
    list[5] = true;
  }
  else if ((R<51) && (G<51) && (B>51)) { //Negro
    list[6] = true;
  }
  
  if(list[0] == true) {
    colorpos = base;
    list[0] = false;
    Serial.println("Color: Rojo");
  }
  else if(list[1] == true) {
    colorpos = base + counter;
    list[1] = false;
    Serial.println("Color: Amarillo");
  }
  else if(list[2] == true) {
    colorpos = base + (2*counter);
    list[2] = false;
    Serial.println("Color: Verde");
  }
  else if(list[3] == true) {
    colorpos = base + (3*counter);
    list[3] = false;
    Serial.println("Color: Azul");
  }
  else if(list[4] == true) {
    colorpos = base + (4*counter);
    list[4] = false;
    Serial.println("Color: Blanco");
  }
  else if(list[5] == true) {
    colorpos = base + (5*counter);
    list[5] = false;
    Serial.println("Color: Gris");
  }
  else if(list[6] == true) {
    colorpos = base + (6*counter);
    Serial.println("Color: Negro");
    list[6] = false;
  }
  else {
    colorpos = base + (7*counter);
    Serial.println("Color: EMERGENCIA");
  }
  /*else if (list[7] == true) {
    colorpos = base + (7*counter);
    list[7] = false;
  } //
*/


  if (pos < colorpos) {
    for (pos; pos <= colorpos; pos += 1) {
      servo_9.write(pos);
      delay(6);
    }   
  }
  else if (pos > colorpos) {
    for (pos; pos >= colorpos; pos -= 1) {
      servo_9.write(pos);
      delay(6);
    }
  }
  
  pos = colorpos;
  //Serial.println(" R = "+String(R)+ " G = "+String(G)+ " B = "+String(B)); //mensaje en consola
  Serial.println(" rojo = "+String(rojo)+"/"+String(R)+ " verde = "+String(verde)+"/"+String(G)+ " azul = "+String(azul)+"/" +String(B)); //mensaje en consola
  
  /*entero = entero +1;
  promer = (promer+rojo)/2;
  promeg = (promeg+verde)/2;
  promeb = (promeb+azul)/2;
  
  if (rojo>maxir) {
    maxir = rojo;
  }
  if (verde>maxig) {
    maxig = verde;
  }
  if (azul>maxib) {
    maxib = azul;
  }


  if (rojo<minir) {
    minir = rojo;
  }
  if (verde<minig) {
    minig = verde;
  }
  if (azul<minib) {
    minib = azul;
  }

  while (entero >10){
    Serial.println("Datos Rojo: " + String(maxir) + "-" +String(promer) + "-" + String(minir));
    Serial.println("Datos Verde: " + String(maxig) + "-" + String(promeg) + "-" + String(minig));
    Serial.println("Datos Azul: " + String(maxib) + "-" +String(promeb) + "-" + String(minib));
    delay(1000);
  }
  */
  
  delay(1000); // cada cuanto el sensor va a detectar un color
  
  
}
