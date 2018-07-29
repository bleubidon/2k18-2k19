#pragma once

#include "Codeuse.h"
#include "Gyroscope.h"

const uint8_t DOUBLE_CODEUSE = 0;
const uint8_t CODEUSE_GYROSCOPE = 1;

const uint8_t GAUCHE = 0;
const uint8_t DROITE = 1;

class Odometrie
{
	public:
		struct Config
		{
			uint8_t mode;
			
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
		uint8_t mode;
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

		void updateDoubleCodeuse();
		void updateCodeuseGyroscope();
};