#include "Actions.h"

void define_actions(Robot& bot)
{
	Action butiner_abeille[] = {
		GOTO(10, 20),
		EFFECTEUR(lever_bras),
		ROTATE(30),

		WAIT(10),
		
		GOTO(20, 90),
		ROTATE(70),
		EFFECTEUR(baisser_bras)
	};
	
	//bot.register_action(butiner_abeille);
}

void baisser_bras()
{
	Serial.println("Et je baisse le bras");
}

void lever_bras()
{
	Serial.println("Et je leve le bras");
}
