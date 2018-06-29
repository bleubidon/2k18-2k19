#pragma once

#include "Codeuse.h"
#include "Gyroscope.h"

const int DOUBLE_CODEUSE = 0;
const int CODEUSE_GYROSCOPE = 1;

const int GAUCHE = 0;
const int DROITE = 1;

class Odometrie
{
	public:
		struct Config
		{
			int mode;
			
			union
			{
				// DOUBLE_CODEUSE
				struct {
					Codeuse::Config gauche, droite;
					float ecart_entre_roues;
				};

				// CODEUSE_GYROSCOPE
				Codeuse::Config codeuse;
			};
		};


		void setup(Odometrie::Config config);

		void update();
		
		float getX();
		float getY();
		float getAlpha();

		float getPositionCodeuse(int num);
	
	private:
		int mode;
		union
		{
			// DOUBLE_CODEUSE
			struct {
				Codeuse codeuses[2];
				float ecart_entre_roues;
			};

			// CODEUSE_GYROSCOPE
			struct {
				Codeuse codeuse;
				Gyroscope gyro;
			};
		};

		float x, y;
		float alpha;

		float Lprecedent;

		// Could propably make an array of functions
		void updateDoubleCodeuse();
		void updateCodeuseGyroscope();
};