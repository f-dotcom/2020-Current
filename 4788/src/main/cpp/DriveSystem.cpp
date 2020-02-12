#include "DriveSystem.h"

using namespace wml;
using namespace wml::controllers;


// Initializes & Defines groups for Manual Control
DrivetrainManual::DrivetrainManual(std::string name, 
                                   //actuators::DoubleSolenoid &IntakeDown,
                                   Drivetrain &drivetrain, 
                                   wml::actuators::DoubleSolenoid &ChangeGears, 
                                   actuators::DoubleSolenoid &Shift2PTO, 
                                   SmartControllerGroup &contGroup) : 
                                   
                                   Strategy(name), 
                                   //_IntakeDown(IntakeDown),
                                   _drivetrain(drivetrain), 
                                   _ChangeGears(ChangeGears), 
                                   _Shift2PTO(Shift2PTO), 
                                   _contGroup(contGroup) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(true);
}

// On Loop Update, this code runs (Just a drivebase)
void DrivetrainManual::OnUpdate(double dt) {

  #if __CONTROLMAP_USING_JOYSTICK__
    double joyForward = -_contGroup.Get(ControlMap::DrivetrainForward) * 0.9;
    double joyTurn = _contGroup.Get(ControlMap::DrivetrainTurn) * 0.6;
    joyForward = std::abs(joyForward);
    
    leftSpeed = joyForward + joyTurn;
    rightSpeed = joyForward - joyTurn;
  #else

    // Left Drive/ Acceleration
    if (fabs(_contGroup.Get(ControlMap::DrivetrainLeft)) > ControlMap::xboxDeadzone) { // I'm So fab
      // Forwards
      if (_contGroup.Get(ControlMap::DrivetrainLeft) < -(leftSpeed + ControlMap::MaxDrivetrainAcceleration)) {
        leftSpeed = leftSpeed + ControlMap::MaxDrivetrainAcceleration;
      } else if (_contGroup.Get(ControlMap::DrivetrainLeft) < leftSpeed) {
        leftSpeed = fabs(_contGroup.Get(ControlMap::DrivetrainLeft));
      }
      // Reverse 
      if (-_contGroup.Get(ControlMap::DrivetrainLeft) < (leftSpeed - ControlMap::MaxDrivetrainAcceleration)) {
        leftSpeed = leftSpeed - ControlMap::MaxDrivetrainAcceleration;
      } else if (_contGroup.Get(ControlMap::DrivetrainLeft) > leftSpeed) {
        leftSpeed = _contGroup.Get(ControlMap::DrivetrainLeft);
        leftSpeed = -leftSpeed;
      } 
    } else {
      leftSpeed = 0;
    }

    // Right Drive/ Acceleration
    if (fabs(_contGroup.Get(ControlMap::DrivetrainRight)) > ControlMap::xboxDeadzone) {
      // Forwards
      if (_contGroup.Get(ControlMap::DrivetrainRight) < -(rightSpeed + ControlMap::MaxDrivetrainAcceleration)) {
        rightSpeed = rightSpeed + ControlMap::MaxDrivetrainAcceleration;
      } else if (_contGroup.Get(ControlMap::DrivetrainRight) < rightSpeed) {
        rightSpeed = fabs(_contGroup.Get(ControlMap::DrivetrainRight));
      }
      // Reverse
      if (-_contGroup.Get(ControlMap::DrivetrainRight) < (rightSpeed - ControlMap::MaxDrivetrainAcceleration)) {
        rightSpeed = rightSpeed - ControlMap::MaxDrivetrainAcceleration;
      } else if (_contGroup.Get(ControlMap::DrivetrainRight) > rightSpeed) {
        rightSpeed = _contGroup.Get(ControlMap::DrivetrainRight);
        rightSpeed = -rightSpeed;
      }
    } else {
      rightSpeed = 0;
    }


  #endif

  if (_contGroup.Get(ControlMap::ReverseDrivetrain, Controller::ONRISE)) {
    _drivetrain.SetInverted(!_drivetrain.GetInverted());
  }

  if (_contGroup.Get(ControlMap::ShiftGears)) {
    _ChangeGears.SetTarget(actuators::BinaryActuatorState::kForward);
  } else {
    _ChangeGears.SetTarget(actuators::BinaryActuatorState::kReverse);
  }

  if (_contGroup.Get(ControlMap::Shift2PTO, Controller::ONRISE)) {
    if (!PTOactive) {
      _Shift2PTO.SetTarget(actuators::BinaryActuatorState::kForward);
      PTOactive = true;
    } else if (PTOactive) {
      _Shift2PTO.SetTarget(actuators::BinaryActuatorState::kReverse);
      PTOactive = false;
    }
  }

  _ChangeGears.Update(dt);
  _Shift2PTO.Update(dt);

  leftSpeed *= ControlMap::MaxDrivetrainSpeed;
  rightSpeed *= ControlMap::MaxDrivetrainSpeed;

  
  std::cout << "LeftDrive Encoder " << _drivetrain.GetConfig().leftDrive.encoder->GetEncoderRotations() << std::endl;
  std::cout << "RightDrive Encoder " << _drivetrain.GetConfig().rightDrive.encoder->GetEncoderRotations() << std::endl;

  _drivetrain.Set(leftSpeed, rightSpeed);
}


// Initializes & Defines groups for Test Mode
DrivetrainTest::DrivetrainTest(Drivetrain &drivetrain, control::PIDGains gains) : _drivetrain(drivetrain), _pid(gains) {
  Requires(&drivetrain);
  SetCanBeInterrupted(true);
  SetCanBeReused(false);
}

void DrivetrainTest::OnUpdate(double dt) {

  if (!leftRevTest && !rightRevTest) {
    if (drivetest) {
      std::cout << "Drivetrain Test Successful" << std::endl;
      drivetest = false;
    }
  } else {
    // Left Test
    if (leftFwdTest) {
      if (_drivetrain.GetConfig().leftDrive.encoder->GetEncoderRotations() < ControlMap::DriveTestCaseRotations) {
        leftSpeed = 0.25;
      } else {
        leftSpeed = 0;
        leftFwdTest = false;
      }
    }
    if (!leftFwdTest) {
      if (_drivetrain.GetConfig().leftDrive.encoder->GetEncoderRotations() > 0) {
        leftSpeed = -0.25;
      } else {
        leftSpeed = 0;
        std::cout << "LeftDrive Return Successful" << std::endl;
        leftRevTest = false;
      }
    } 

    // Right Test
    if (rightFwdTest) {
      if (_drivetrain.GetConfig().rightDrive.encoder->GetEncoderRotations() > -ControlMap::DriveTestCaseRotations) {
        rightSpeed = 0.25;
      } else {
        rightSpeed = 0;
        rightFwdTest = false;
      }
    }
    if (!rightFwdTest) {
      if (_drivetrain.GetConfig().rightDrive.encoder->GetEncoderRotations() < 0) {
        rightSpeed = -0.25;
      } else {
        rightSpeed = 0;
        std::cout << "RightDrive Return Successful" << std::endl;
        rightRevTest = false;
      }
    } 

    std::cout << "LeftDrive Encoder " << _drivetrain.GetConfig().leftDrive.encoder->GetEncoderRotations() << std::endl;
    std::cout << "RightDrive Encoder " << _drivetrain.GetConfig().rightDrive.encoder->GetEncoderRotations() << std::endl;

  }
  _drivetrain.Set(leftSpeed, rightSpeed);
}