#pragma once

#include "vec.h"
#include "Codeuse.h"
#include "Gyroscope.h"

extern const uint8_t DOUBLE_CODEUSE;
extern const uint8_t CODEUSE_GYROSCOPE;

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
					float ecart_entre_roues; // centimetres
				};

				// CODEUSE_GYROSCOPE
				Codeuse::Config codeuse;
			};
		};


		void setup(Odometrie::Config config);

		void update();
		
		const vec& pos(); // centimetres
		const vec& dir();
		const float& rot(); // degrés

		float getPositionCodeuse(int num);

	private:
		uint8_t mode;
		union
		{
			// DOUBLE_CODEUSE
			struct {
				Codeuse codeuses[2];
				float ecart_entre_roues; // centimetres
			};

			// CODEUSE_GYROSCOPE
			struct {
				Codeuse codeuse;
				Gyroscope gyro;
			};
		};

		vec position, direction;
		float angle;

		float Lprecedent;

		void updateDoubleCodeuse();
		void updateCodeuseGyroscope();
};