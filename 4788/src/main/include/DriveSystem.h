#pragma once 

#include "controllers/Controllers.h"
#include "strategy/Strategy.h"
#include "RobotMap.h"
#include "Drivetrain.h"


// Class that Runs In Manual Drive e.g Human Drivers
class DrivetrainManual : public wml::Strategy {
  public:
    DrivetrainManual(std::string name, 
                     //wml::actuators::DoubleSolenoid &IntakeDown,
                     wml::Drivetrain &drivetrain, 
                     wml::actuators::DoubleSolenoid &ChangeGears, 
                     wml::actuators::DoubleSolenoid &Shift2PTO, 
                     wml::controllers::SmartControllerGroup &contGroup);

    void OnUpdate(double dt) override;

  private:
    wml::Drivetrain &_drivetrain;
    //wml::actuators::DoubleSolenoid &_IntakeDown;
    wml::actuators::BinaryActuator &_ChangeGears;
    wml::actuators::BinaryActuator &_Shift2PTO;
    wml::controllers::SmartControllerGroup &_contGroup;
    double leftSpeed = 0, rightSpeed = 0;
    double currentSpeed;
    bool PTOactive = false;
};

// Class that Runs in Autonomous
class DrivetrainAuto : public wml::Strategy {
  public:
    DrivetrainAuto(wml::Drivetrain &drivetrain, 
                   wml::control::PIDGains gains,
                   int &autoSelector,
                   bool &StartDoComplete,
                   bool &strt,
                   bool &p1,
                   bool &p2,
                   bool &p3,
                   bool &end);

    void OnUpdate(double dt) override;
    double LeftDriveToTarget(double dt);
    double RightDriveToTarget(double dt);

  private:
    wml::Drivetrain &_drivetrain;
    wml::control::PIDController _pid;
    double LeftPower = 0, RightPower = 0;
    double currentSpeed;

    int &_autoSelector;
    int AutoWaypointSwitcher = 1;
    bool &_StartDoComplete;
    bool &_strt;
    bool &_p1;
    bool &_p2;
    bool &_p3;
    bool &_end;

    double DistanceInRotations;
    double CurrentHeading;
};

// Class that Runs in Test Mode
class DrivetrainTest : public wml::Strategy {
  public:
    DrivetrainTest(wml::Drivetrain &drivetrain, 
                   wml::control::PIDGains gains);

    void OnUpdate(double dt) override;

  private:
    wml::Drivetrain &_drivetrain;
    wml::control::PIDController _pid;
    double leftSpeed = 0, rightSpeed = 0;

    bool drivetest = true;

    bool leftFwdTest = true;
    bool rightFwdTest = true;
    bool leftRevTest = true;
    bool rightRevTest = true;
};