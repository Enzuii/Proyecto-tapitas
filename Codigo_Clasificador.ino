#include <ColorConverterLib.h>
#include <Adafruit_TCS34725.h>
#include <Servo.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_1X);
#include <Stepper.h>


/*///////////////////////////////
Pines usados:
A4, A5 = sensor RGB
2, 3, 4, 5 = Stepper
6 = LED sensor
8 = bocina 
9 = servo final
10 = servo de la tolva
11 = Motor que destraba
12, 13 = LEDs de funcionando-encendido-emergencia ---- PIN 12: Verde --- PIN 13: ROJO
///////////////////////////////*/


/////////////////////////////////
///////  STEPPER  SETUP /////////
/////////////////////////////////

const int stepsPerRevolution = 2048;  // steps per revolution

// inicializa la libreria stepper desde el pin 2 al 5:
Stepper myStepper(stepsPerRevolution, 2, 4, 3, 5);

int i = 2048;  //  i usado para el for del stepper
int k = 2048;  //  auxiliar del for

/////////////////////////////////
///////  VARIABLES SETUP ////////
/////////////////////////////////
double h, s, v;   //hsv para definir rangos de colores

bool red, yellow, green, blue, white, grey, black = false;      //Variables para la lista con el check de colores
bool check[7] = {red, yellow, green, blue, white, grey, black};
String stringcheck[7] = {"Rojo", "Amarillo", "Verde", "Azul", "Blanco", "Gris", "Negro"};
int empty = 0;  // Contar colores negros para saber si esta vacio
int vuelta = 0; // Contar vueltas para impedir que siga corriendo el programa si no hay tapitas que clasificar

//Bocina
const int pinBuzzer = 8; //bocina

bool working = false;

/// Servos Setup
  // Variables necesarias para mover el servo - base y multiplier para los colores
int tolvapos = 120;
int servopos = 90;  //Posicion del servo
int colorpos = 90;  //Posicion a dondde se va a mover
int base = 30;
int multiplier = 20; 
int p = 0;  //Usado para el for

Servo servo_tolva;
Servo servo_final;


/*/////////////////////////////////////////////////////////////////
Sacar todos los comandos "Serial" al momento de usar el programa
sin computadora. Esto porque sino puede entorpecerse el programa.
/////////////////////////////////////////////////////////////////*/
void setup() {
  pinMode(11, OUTPUT); //Motor que destraba
  
  myStepper.setSpeed(15);     //Set speed al Stepper
  
  servo_final.attach(9, 500, 2500); //Attach de los Servos
  servo_tolva.attach(10, 500, 2500);
  servo_tolva.write(120);

  pinMode(12, OUTPUT); //LEDs de encendido, apagado
  pinMode(13, OUTPUT);
  pinMode(6, OUTPUT); //LED del sensor
  analogWrite(6, 100);
  
  Serial.begin(9600);
  if(tcs.begin()){

    Serial.println("Inicializando");
    delay(2000);
    digitalWrite(12, HIGH);
    digitalWrite(13, HIGH);
  }
  else{
    Serial.println("Error");
    Serial.println("Debe de revisar las conexiones e iniciar nuevamente");
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);    
    while(1){
      tone(pinBuzzer, 440); // bocina
      delay(1000); //detener tono durante 500ms  
      noTone(pinBuzzer);
      delay(500); //generar tono de 523Hz durante 500ms, y detenerlo durante 500ms.
      tone(pinBuzzer, 523, 300);
    }
  }
}

void loop() {
  // Programa clasificador
  digitalWrite(12, HIGH);
  digitalWrite(13, LOW);

  int empty = 0; // Resetea por si salio por el empty
  
  if (vuelta >=3){  // Chequea la vuelta, si hizo muchas vueltas empty, da aviso y sale del programa
    digitalWrite(12, LOW);
    digitalWrite(13, HIGH);
    while(1) {
      tone(pinBuzzer, 440); // bocina
      delay(1000); //detener tono durante 500ms  
      noTone(pinBuzzer);
      delay(500); //generar tono de 523Hz durante 500ms, y detenerlo durante 500ms.
      tone(pinBuzzer, 523, 300);
    }
  }
  
  // Servo de la tolva abre y cierra
  for (tolvapos = 120; tolvapos >= 93; tolvapos -= 1) {
    servo_tolva.write(tolvapos);
    delay(10);
  }
  delay(2000);
  for (tolvapos = 93; tolvapos <=120 ; tolvapos += 1) {
    servo_tolva.write(tolvapos);
    delay(10);
  }
/*
  analogWrite(11, 51); //Encender cuando este puesto el motor para destrabar
  delay(500);
  analogWrite(11, 0);
  delay(200);
  analogWrite(11, 51);
  delay(500);
  analogWrite(11, 0);
  */
  delay(1000); //Espera a que se acomoden las tapitas
  
  // Stepper inicial step
  for (i; i<=(k+512); i+=1){  // Stepper va a la posicion i-512 (== to k-512)
    myStepper.step(-1);
    delay(3);
  }
  working = true;
  
  while (working) {      
    ///////////////////
    ///// SENSOR //////
    ///////////////////
    uint16_t R, G, B, C;
    tcs.getRawData(&R,&G,&B,&C); //tomar datos del sensor, (& antes, para que R,G,B,C haga una copia y no cambie de valor
    
    if(R>255){R=255;} //limitar sensor
    if(G>255){G=255;}
    if(B>255){B=255;}
    //  necesario para la libreria ColorConverter
  
    //RGB a HSV
    ColorConverter::RgbToHsv(R, G, B, h, s, v); // Devuelve h[0->1] ; s [0->1] ; v [0->1]
    h = h*360; s = s*100; v = v*100;
  
    ////////////////////
    ///// Colores //////
    ////////////////////
      //Blancos, negros, grises
    if ((v<=32 && h>=170 && h>=260) || v<=10) { //  Negro
      check[6] = true;
      empty = empty+1;
    }
    else if ((v<=65 && s<=30 && h<=240 && h>=175) || (v <= 60 && s<=5)){ // Gris
      check[5] = true;
      empty = 0;
      vuelta = 0;
    }
    else if ((v>=65 && s<=15 && h>=180 && h<=240) || (v>=80 && s<=5)){ // Blanco
      check[4] = true;
      empty = 0;
      vuelta = 0;
    }

      //Colores
    else if (h>262 || (h<15 && s>25) || (h<36 && s>=90) || h<=32) { //Rojos, violetas, rosados || Naranjas (2 limites) HSV
      check[0] = true;
      empty = 0;
      vuelta = 0;
    }
    else if (h>=32 && h<=54) { //Amarillos HSV
      check[1] = true;
      empty = 0;
      vuelta = 0;
    }
    else if ((h<=170 && v<=50) || h<=165) { //Verdes HSV
      check[2] = true;
      empty = 0;
      vuelta = 0;
    }
    else if (h>165 && h<=262) { // Azules HSV
      check[3] = true;
      empty = 0;
      vuelta = 0;
    }
  
    ////////////////////////
    ///// Servo Angle //////
    ////////////////////////
    for (p=0; p<=6; p+=1) {
        if (check[p] == true){
          if (check[p] == check[6]) { //Chequeo para que no siga corriendo el programa si se vacia
            empty = empty+1;
          }
          else {
            empty = 0;
            vuelta = 0;
          }
          colorpos = (base + (p*multiplier));
          check[p] = false;
          Serial.println("Color: " + stringcheck[p]);
        }
    }
    
    //////////////////////////
    ///  FINAL SERVO  MOVE ///
    //////////////////////////
    if (servopos < colorpos) {
      for (servopos; servopos <= colorpos; servopos += 1) {
        servo_final.write(servopos);
        delay(5);
      }   
    }
    else if (servopos > colorpos) {
      for (servopos; servopos >= colorpos; servopos -= 1) {
        servo_final.write(servopos);
        delay(5);
      }
    }
    servopos = colorpos; //Reassign del servopos para tener su posicion

    delay(500); //Delay provisional para separar los movimientos del stepper
    
    //////////////////////////
    /////  STEPPER  MOVE /////
    //////////////////////////
    if (i >= 2048) { //Checkea que el Stepper no haya completado la vuelta
      i = 0;
      k = 0;
    }
    for (i; i<=(k+512); i+=1){  //Mueve el Stepper de i hasta i+512 (+90º)
      myStepper.step(-1);
      delay(3);} 
    k = i; //Reassign del k para el Stepper

    //Comunicar qué hizo el programa
    Serial.println("   rojo = "+String(R)+ " verde = "+String(G)+ " azul = "+String(B)); //mensaje en consola
    Serial.println("   H = " + String(h) + " S = " + String(s) + " V = " +String(v)); 
    
    /////////////////////////
    /////  Check empty  /////
    /////////////////////////
    if (empty>=4){
      vuelta = vuelta+1;  // Registra la vuelta del empty
      working = false;    // Sale del ciclo clasificador
    }
        
    delay(1000); // cada cuanto el sensor va a detectar un color
  }   
}
