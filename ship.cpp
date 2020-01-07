#include "ship.h"

// TODO: set some rules and hardcode it.
namespace
{
	#define PIRATES_VISIBILITY 3.0f

	#define TANKERS_VISIBILITY 4.0f
	#define PASSENGERS_VISIBILITY 5.0f
	#define BULKCARRIERS_VISIBILITY 3.0f

	#define TANKERS_VULNERABILITY 80
	#define PASSENGERS_VULNERABILITY 30
	#define BULKCARRIERS_VULNERABILITY 60
}

void AShip::debug_IntroduceYourself() const
{
	std::cout << "Name: " << Name << std::endl << "Velocity: " <<
		Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
		"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
		"Destination: " << Destination.X << " " << Destination.Y << std::endl;
}

void TPirate::debug_IntroduceYourself() const
{
	std::cout << "Argghh!" << std::endl;
	std::cout << "Name: " << Name << std::endl << "Velocity: " <<
		Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
		"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
		"Destination: " << Destination.X << " " << Destination.Y << std::endl;
}

bool ACivilian::WasAttacked() const
{
	return Attacked;
}

void ACivilian::SetAttacked()
{
	Attacked = true;
}

bool ACivilian::IsRunningAway() const
{
	return RunningAway;
}

void ACivilian::SetRunningAway()
{
	RunningAway = true;
}

TCoordinates ACivilian::GetDestination() const
{
	return Destination;
}

void ACivilian::ChangeDestination(TCoordinates coordinates)
{
	Destination = coordinates;
}

float AShip::GetVelocity() const
{
	return Velocity;
}

float AShip::GetRangeOfView() const
{
	return Visibility;
}

TCoordinates AShip::GetPosition() const
{
	return Position;
}

void AShip::Move(TCoordinates coordinates)
{
	Position = coordinates;
}

void TPirate::ModifyVelocity(float fastestCivilianVelocity)
{
	Velocity = fastestCivilianVelocity * 1.25f;
}

void TPirate::ChangeTarget(const IShip* target)
{
	Target = target;
}

const IShip* TPirate::GetTarget() const
{
	return Target;
}

TBulkCarrier::TBulkCarrier(const std::string& name, float velocity, TCoordinates position,
	TCoordinates destination) : ACivilian(name, velocity, BULKCARRIERS_VISIBILITY, position, 
		destination, BULKCARRIERS_VULNERABILITY)
{}

TTanker::TTanker(const std::string& name, float velocity, TCoordinates position,
	TCoordinates destination) : ACivilian(name, velocity, TANKERS_VISIBILITY, position, 
		destination, TANKERS_VULNERABILITY)
{}

TPassenger::TPassenger(const std::string& name, float velocity, TCoordinates position, 
	TCoordinates destination) : ACivilian(name, velocity, PASSENGERS_VISIBILITY, position,
		destination, PASSENGERS_VULNERABILITY)
{}

TPirate::TPirate(float velocity, TCoordinates position, TCoordinates destination) :
	AShip("The Green Oyster", velocity, PIRATES_VISIBILITY, position, destination), Target(nullptr)
{}