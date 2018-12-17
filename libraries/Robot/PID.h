#pragma once

class PID
{
	public:
		PID(float _Kp = 0, float _Ki = 0, float _Kd = 0);

		void set_consigne(float _consigne);
		void set_coefs(float _Kp, float _Ki, float _Kd);

		float compute(float mesure, float dt);

		float Kp, Ki, Kd;
		float consigne, erreur, vitesse, integrale;
};
