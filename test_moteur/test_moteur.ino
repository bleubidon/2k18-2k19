
#include <PWM.h>

int inApin[2] = {7, 4};  // INA: Clockwise input
int inBpin[2] = {8, 9}; // INB: Counter-clockwise input
int pwmpin[2] = {5, 6}; // PWM input

void setup() {
    Serial.begin(9600);


    //hug les fonctions suivantes proviennent de PMW.h, permettent d'initializer la commande des moteurs par pmw
    InitTimersSafe();  //hug cette fonction permet de ne pas initialiser de timer pour les fonctions suivant l'avancée temporelle
    SetPinFrequencySafe(pwmpin[0], 20000);   //hug idem
    SetPinFrequencySafe(pwmpin[1], 20000);

    
    // Initialize digital pins as outputs
    for (int i = 0; i < 2; i++)
    {
        pinMode(inApin[i], OUTPUT);
        pinMode(inBpin[i], OUTPUT);
        pinMode(pwmpin[i], OUTPUT);
    }

    
    // Initialize braked
    for (int i = 0; i < 2; i++)
    {
        digitalWrite(inApin[i], LOW);
        digitalWrite(inBpin[i], LOW);
    }
}

void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)
{
    if (stopMotors)
        return;
    
  if (motor <= 1)
  {
    if (direct <=4)
    {
      // Set inA[motor]
      if (direct <=1)
        digitalWrite(inApin[motor], HIGH);
      else
        digitalWrite(inApin[motor], LOW);

      // Set inB[motor]
      if ((direct==0)||(direct==2))
        digitalWrite(inBpin[motor], HIGH);
      else
        digitalWrite(inBpin[motor], LOW);

      analogWrite(pwmpin[motor], pwm);
    }
  }
}

/***************** execute la consigne moteur ****************/
void consigneMoteur(int consigne_vitesse, int consigne_vitesse2)
{
  consigne_vitesse  = max(-255, min(consigne_vitesse , 255));
  consigne_vitesse2 = max(-255, min(consigne_vitesse2, 255));
  
  if(consigne_vitesse>0){
    motorGo(1, CCW , consigne_vitesse); //1023); //moteur 1
  }
  else{
    motorGo(1, CW , -consigne_vitesse);
  }
  if(consigne_vitesse2>0){
    motorGo(0, CW , consigne_vitesse2); //1023); //moteur 2
  }
  else{
    motorGo(0, CCW , -consigne_vitesse2);
  }
}


void loop() {
    consigneMoteur(50, 50);

}
