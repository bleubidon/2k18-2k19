/*!
 * \file Deplacement.cpp
 * \brief génère les vitesses en fonction des valeurs des codeuses
 * \author Simon DUPONT-NIVET
 * \version 1.0 
 */
#include <Deplacement.h>
#include <GeometrieRobot.h>

Deplacement::Deplacement()
{
	currentConsigne = 0;
	consigne_vitesse_old = 0;
	erreur_position_old = 0;
	erreur_position = 0;
	integral_position = 0;
	consigne_vitesse = 50;
	consigne_vitesse2 = 50;
	consigne_vitesse_old2 = 0;
	erreur_position_old2 = 0;
	erreur_position2 = 0;
	integral_position2 = 0;
	Vmax = 50;
	Amax = 5;
	EncodeurPosition = 0;
	EncodeurPosition2 = 0;
	EncodeurPositionPrecedent = 0;
	EncodeurPositionPrecedent2 = 0;
	tmp = 0;
	tmp2 = 0;
	consigne_position2 = 0;
	consigne_position = 0;
	NB_PAS = 20000; //19400;
	firstTime = true;
	evitementFlag = true;
	EncodeurPositionEvitement = 0;
	EncodeurPosition2Evitement = 0;
	POWER_COEFF = 1;
	timeCount = 0;
	rampe = 0;
	for (int i = 0; i < 3; i++)
	{
		arrayConsigne[i] = 0;
	}
}

void Deplacement::deplacement(double consigne, int type, long cntr1, long cntr2)
{
	/// deplacement en ligne droite
	if (type == LIGNE_DROITE)
	{
		Amax = 5;
		AmaxDecel = 20;
		timeCount++;
		PKp = 0.4; //0.28;
		PKi = 0;   //0.0002;//0.0002;
		PKd = 0;   //0.08;//0.08;
		precision = 10;
		consigne_position2 = consigne;
		consigne_position = consigne;
	}
	/// deplacement en angle
	else if (type == TOURNANT)
	{
		Amax = 20;
		AmaxDecel = 10;
		timeCount++;
		PKp = 1; //0.32;
		PKi = 0; //0.0006;
		PKd = 0; //0.04;
		precision = 10;
		double tmpAngle = consigne / 360;
		consigne_position = tmpAngle * _DISTANCE_ENTRE_ROUES * 3.14;
		consigne_position2 = -consigne_position;
	}
	/// cas evitement ou effecteur
	else if (type == EFFECTEUR || type == EVITEMENT)
	{
		Amax = 20;
		AmaxDecel = 20;
		PKp = 0.4;
		PKi = 0.0002;
		PKd = 0.08;
		consigne_position2 = consigne;
		consigne_position = consigne;
	}
	if (firstTime)
	{
		firstTime = false;
		consigne_vitesse_old = 0;
		erreur_position_old = 0;
		erreur_position = 0;
		integral_position = 0;
		consigne_vitesse = 0;  //50
		consigne_vitesse2 = 0; //50
		consigne_vitesse_old2 = 0;
		erreur_position_old2 = 0;
		erreur_position2 = 0;
		integral_position2 = 0;
		timeCount = 0;
		rampe = 0;
	}

	// Calcule de l'erreur
	tmp = cntr1 / NB_PAS;
	EncodeurPosition = -tmp * _DIAMETRE_ROUES * 3.14; //cntr1 : nbr de pas effectué, nbPas : nbr de pas pour un tour
	EncodeurPosition = EncodeurPosition - EncodeurPositionPrecedent;

	tmp2 = cntr2 / NB_PAS;
	EncodeurPosition2 = tmp2 * _DIAMETRE_ROUES * 3.14 - EncodeurPositionPrecedent2;

	if (type == EVITEMENT)
	{
		if (evitementFlag)
		{
			EncodeurPositionEvitement = EncodeurPosition;
			EncodeurPosition2Evitement = EncodeurPosition2;
			erreur_position = 0;
			erreur_position2 = 0;
			evitementFlag = false;
		}
		consigne_position = EncodeurPositionEvitement;
		consigne_position2 = EncodeurPosition2Evitement;
	}
	else
	{
		evitementFlag = true;
	}

	///filtre PID
	consigne_vitesse_old = consigne_vitesse;				// Sauvegarde de l’ancienne consigne
	erreur_position_old = erreur_position;					// Sauvegarde de l’ancienne erreur
	erreur_position = consigne_position - EncodeurPosition; // Calcul de la nouvelle erreur de position
	integral_position += erreur_position;					// Recalcul de l’intégrale
	consigne_vitesse = PKp * erreur_position + PKi * integral_position + PKd * (erreur_position - erreur_position_old);

	consigne_vitesse_old2 = consigne_vitesse2;				   // Sauvegarde de l’ancienne consigne
	erreur_position_old2 = erreur_position2;				   // Sauvegarde de l’ancienne erreur
	erreur_position2 = consigne_position2 - EncodeurPosition2; // Calcul de la nouvelle erreur de position
	integral_position2 += erreur_position2;					   // Recalcul de l’intégrale
	consigne_vitesse2 = PKp * erreur_position2 + PKi * integral_position2 + PKd * (erreur_position2 - erreur_position_old2);

	/// Écrêtage de la vitesse
	if (consigne_vitesse > Vmax)
	{
		consigne_vitesse = Vmax;
	}
	else if (consigne_vitesse < -Vmax)
	{
		consigne_vitesse = -Vmax;
	}
	if (consigne_vitesse2 > Vmax)
	{
		consigne_vitesse2 = Vmax;
	}
	else if (consigne_vitesse2 < -Vmax)
	{
		consigne_vitesse2 = -Vmax;
	}

	if (type == 0)
	{
		consigne_vitesse += (erreur_position - erreur_position2);
		consigne_vitesse2 += (erreur_position2 - erreur_position);
	}

	/// Écrêtage de l’accélération
	if ((consigne_vitesse - consigne_vitesse_old) > Amax)
	{
		consigne_vitesse = consigne_vitesse_old + Amax;
	}
	else if ((consigne_vitesse - consigne_vitesse_old) < -AmaxDecel)
	{
		consigne_vitesse = consigne_vitesse_old - AmaxDecel;
	}
	if ((consigne_vitesse2 - consigne_vitesse_old2) > Amax)
	{
		consigne_vitesse2 = consigne_vitesse_old2 + Amax;
	}
	else if ((consigne_vitesse2 - consigne_vitesse_old2) < -AmaxDecel)
	{
		consigne_vitesse2 = consigne_vitesse_old2 - AmaxDecel;
	}

	/****** Test Asserv ******/
	///condition de fin de consigne en précision

	if ((type != EVITEMENT && type != EFFECTEUR) && ((erreur_position > -precision && erreur_position < precision) && (erreur_position2 > -precision && erreur_position2 < precision)) //){
		&& consigne == arrayConsigne[currentConsigne])
	{ //A REMETTRE ABSOLUMENT AVEC TABLEAU ARRAY_CONSIGNE !!!!!!!!!!!!
		currentConsigne++;
		EncodeurPositionPrecedent2 = tmp2 * _DIAMETRE_ROUES * 3.14; //+=EncodeurPosition2;
		EncodeurPositionPrecedent = -tmp * _DIAMETRE_ROUES * 3.14;  //+=EncodeurPosition;
		firstTime = true;
	}

	/*
  ///condition de fin de consigne en temps
  if(timeCount>200 && type==TOURNANT){
    currentConsigne++;
    EncodeurPositionPrecedent2=tmp2*_DIAMETRE_ROUES*3.14;//+=EncodeurPosition2;
    EncodeurPositionPrecedent=-tmp*_DIAMETRE_ROUES*3.14;//+=EncodeurPosition;
    firstTime=true; 
  }
    if(timeCount>700 && type==LIGNE_DROITE){
    currentConsigne++;
    EncodeurPositionPrecedent2=tmp2*_DIAMETRE_ROUES*3.14;//+=EncodeurPosition2;
    EncodeurPositionPrecedent=-tmp*_DIAMETRE_ROUES*3.14;//+=EncodeurPosition;
    firstTime=true; 
  }
  //*/
}
double Deplacement::getConsigneRampe(int i)
{
	rampe = rampe + 5;
	if (rampe > arrayConsigne[i])
	{
		rampe = arrayConsigne[i];
	}

	return rampe;
}

int Deplacement::getVitesse()
{
	return consigne_vitesse;
}
int Deplacement::getVitesse2()
{
	return consigne_vitesse2;
}
int Deplacement::getCurrentConsigne()
{
	return currentConsigne;
}
void Deplacement::setArrayConsigne(double consigne, int i)
{
	arrayConsigne[i] = consigne;
}
double Deplacement::getConsigne(int i)
{
	return arrayConsigne[i];
}
void Deplacement::setCurrentConsigne(int i)
{
	currentConsigne = i;
}
double Deplacement::getErreur_position()
{
	return erreur_position;
}
double Deplacement::getErreur_position2()
{
	return erreur_position2;
}
double Deplacement::getEncodeurPosition()
{
	return EncodeurPosition;
}
double Deplacement::getEncodeurPosition2()
{
	return EncodeurPosition2;
}
double Deplacement::getTmp()
{
	return tmp;
}
double Deplacement::getTmp2()
{
	return tmp2;
}
void Deplacement::setVmax(int v)
{
	Vmax = v;
}
void Deplacement::stopMove()
{
	consigne_vitesse = 0;
	consigne_vitesse2 = 0;
}
