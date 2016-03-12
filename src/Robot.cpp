#include <Commands/DriveForTime.h>
#include "Robot.h"

#include "RobotMap.h"
#include "Commands/DriveForTime.h"

std::shared_ptr<DriveTrain> Robot::drive_train;
std::shared_ptr<Shooter> Robot::shooter;
std::shared_ptr<BallRetriever> Robot::ball_retriever;

std::unique_ptr<OI> Robot::oi;

void Robot::RobotInit() {
	drive_train.reset(new DriveTrain());
	shooter.reset(new Shooter());
	ball_retriever.reset(new BallRetriever());

	oi.reset(new OI());

	last_inverted_time = GetTime();

	gyro = std::unique_ptr<Gyro>(new AnalogGyro(1));
}

void Robot::DisabledInit() {

}

void Robot::DisabledPeriodic() {
	Scheduler::GetInstance()->Run();
}

void Robot::AutonomousInit() {
	autonomousCommand.reset(new DriveForTime(1, 0.4));

	if (autonomousCommand != NULL) autonomousCommand->Start();
}

void Robot::AutonomousPeriodic() {
	Scheduler::GetInstance()->Run();
}

void Robot::TeleopInit() {
	if (autonomousCommand != NULL) autonomousCommand->Cancel();

	drive_train->Reset();
	oi->xbox_controller->Calibrate();
}

void Robot::TeleopPeriodic() {
	Scheduler::GetInstance()->Run();

	SmartDashboard::PutNumber("Gyro", gyro->GetAngle());

	if(oi->xbox_controller->GetButton(XBoxController::BUTTON_START)) {
		oi->xbox_controller->Calibrate();
	}

	SmartDashboard::PutNumber("LAST", GetTime() - last_inverted_time);
	drive_train->PrintInvertedStatus();
	if(GetTime() - last_inverted_time >= 0.5 && oi->xbox_controller->GetButton(XBoxController::BUTTON_A)) {
		drive_train->InvertDirection();
	}
}

void Robot::TestPeriodic() {
	LiveWindow::GetInstance()->Run();
}

START_ROBOT_CLASS(Robot)

