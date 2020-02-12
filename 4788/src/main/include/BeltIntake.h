#pragma once

#include "controllers/Controllers.h"
#include "RobotMap.h"

class BeltIntake {
  public:
   // BeltIntake(wml::Gearbox &BeltIntakeMotors, wml::actuators::DoubleSolenoid &IntakeDown, wml::controllers::SmartControllerGroup &contGroup, std::shared_ptr<nt::NetworkTable> &BeltIntakeTable);
    BeltIntake(wml::Gearbox &BeltIntakeMotors, 
               wml::actuators::DoubleSolenoid &IntakeDown, 
               wml::controllers::SmartControllerGroup &contGroup, 
               std::shared_ptr<nt::NetworkTable> &BeltIntakeTable);
    
    void TeleopOnUpdate(double dt);
    void AutoOnUpdate(double dt);
    void TestOnUpdate(double dt); 

  private:
    wml::Gearbox &_BeltIntakeMotors;
    wml::actuators::DoubleSolenoid &_IntakeDown;
    wml::controllers::SmartControllerGroup &_contGroup;
    std::shared_ptr<nt::NetworkTable> &_BeltIntakeTable;


    frc::Timer timer;
    double IntakePower = 0;

    bool ToggleEnabled;
};