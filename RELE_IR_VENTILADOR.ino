#include <IRremote.h>
#include <IRremoteInt.h>

#define FNV_PRIME_32 16777619
#define FNV_BASIS_32 2166136261
#define codigoLD 9755 //code button control

const int irReceiverPin = 5; //pin the receiver is connected to
IRrecv irrecv(irReceiverPin); //create an IRrecv object
decode_results results; //stores results from IR detector
unsigned long last = millis();
int pRele=2;

void setup()
{
  irrecv.enableIRIn();
  Serial.begin(9600);
  pinMode(pRele,OUTPUT);
  digitalWrite(pRele,LOW);

}

void loop()
{
  if (irrecv.decode(&results)) {
    if (millis() - last > 200) {

      unsigned long hash = decodeHash(&results); 
      int valor = (unsigned long) hash;
      ligarDesligar(valor);
    }
    irrecv.resume(); // Obrigatório a reinicialização da recepção!
    last = millis();
  }
}

void ligarDesligar(int valor){

  if(valor==codigoLD && digitalRead(pRele)==0){

    digitalWrite(pRele,HIGH);

  }
  else if(valor==codigoLD && digitalRead(pRele==1)){

    digitalWrite(pRele,LOW);
  }

}

int compare(unsigned int oldval, unsigned int newval) {  
  if (newval < oldval * .8) {
    return 0;
  } 
  else if (oldval < newval * .8) {
    return 2;
  } 
  else {
    return 1;
  }
}

unsigned long decodeHash(decode_results *results) {
  unsigned long hash = FNV_BASIS_32;
  for (int i = 1; i+2 < results->rawlen; i++) {
    int value =  compare(results->rawbuf[i], results->rawbuf[i+2]);
    hash = (hash * FNV_PRIME_32) ^ value;
  }
  return hash;
}  








