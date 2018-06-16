/*!
 * \file Deplacement.h
 * \brief génère les vitesses en fonction des valeurs des codeuses
 * \author Simon DUPONT-NIVET
 * \version 1.0 
 */
#ifndef _Deplacement_H_
#define _Deplacement_H_

#define LIGNE_DROITE 0
#define TOURNANT 1
#define EFFECTEUR 3
#define EVITEMENT 4

class Deplacement {

public :
Deplacement();
/*!
 * \fn deplacement(double consigne, int type, long cntr1, long cntr2)
 * \brief fonction coeur du déplacement du robot : génère les vitesses en fonction des valeurs des codeuses
 * \param la valeur de la consigne (getConsigne(currentConsigne)), le type de consigne (LIGNE_DROITE,EVITEMENT, ..), les valeurs des codeuses
 * \version 1.0 
 */
void deplacement(double consigne, int type, long cntr1, long cntr2);
int getVitesse();
int getVitesse2();
/// retourne l'index courrant du tableau arrayConsigne
int getCurrentDeplacement();
int getCurrentConsigne();
void setCurrentConsigne(int i);
double getConsigne(int i);
double getConsigneRampe(int i);
/// permet d'écrire la valeur d'une consigne (pour le calcul des consignes cf; computeConsigne1 et computeConsigne2)
void setArrayConsigne(double consigne, int i);
double getErreur_position();
double getErreur_position2();
double getEncodeurPosition();
double getEncodeurPosition2();
double getTmp();
double getTmp2();
/// set la limite de vitesse
void setVmax(int v);
/// met les vitesses à zéro 
void stopMove();

private :
int timeCount;
double NB_PAS;
/// tableau des consignes 
double arrayConsigne[3];
int currentConsigne;
/// coefficient proportionnel du filtre PID
double PKp; 
/// coefficient intégral du filtre PID
double PKi; 
/// coefficient dérivé du filtre PID
double PKd;
/// valeur de tolérence de l'erreur_position pour mettre fin à une consigne  
int precision;
/// consigne pour moteur 2 dépend du type de consigne (LIGNE_DROITE,EVITEMENT, ..)
double consigne_position2;
/// consigne pour moteur 1 dépend du type de consigne (LIGNE_DROITE,EVITEMENT, ..)
double consigne_position;
int consigne_vitesse_old;
double erreur_position_old;
double erreur_position;
double integral_position;
int consigne_vitesse;
int consigne_vitesse2;
int consigne_vitesse_old2;
double erreur_position_old2;
double erreur_position2;
double integral_position2;
int Vmax;
/// valeur d'écratage de l'accélération en phase d'accélération
int Amax;
/// valeur d'écratage de l'accélération en phase de décélération 
int AmaxDecel;
double EncodeurPosition;
double EncodeurPosition2;
double EncodeurPositionPrecedent;
double EncodeurPositionPrecedent2;
double tmp;
double tmp2; 
bool firstTime;
bool evitementFlag;
double EncodeurPositionEvitement;
double EncodeurPosition2Evitement;
double POWER_COEFF;
double rampe;
};

#endif