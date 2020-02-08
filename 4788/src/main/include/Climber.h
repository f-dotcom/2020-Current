#pragma once

#include "controllers/Controllers.h"
#include "RobotMap.h"

class Climber {
  public:
    Climber(wml::actuators::DoubleSolenoid &ClimberActuator, wml::actuators::DoubleSolenoid &ShiftPTO, wml::Gearbox &ClimberElevator, wml::controllers::SmartControllerGroup &contGroup);		
    void TeleopOnUpdate(double dt);
 		void AutoOnUpdate(double dt);
 		void TestOnUpdate(double dt);

	private:
 		wml::actuators::DoubleSolenoid &_ClimberActuator;
 		wml::actuators::DoubleSolenoid &_ShiftPTO;
 		wml::Gearbox &_ClimberElevator;
		wml::controllers::SmartControllerGroup &_contGroup;
 	
	 	frc::Timer ClimberTimer;

 	bool ToggleEnabled;
 };