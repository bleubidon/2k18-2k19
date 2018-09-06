#pragma once

#include "Codeuse.h"
#include "Gyroscope.h"

extern const uint8_t GAUCHE;
extern const uint8_t DROITE;

const uint8_t DOUBLE_CODEUSE = 0;
const uint8_t CODEUSE_GYROSCOPE = 1;

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
	
		float dirX, dirY;

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