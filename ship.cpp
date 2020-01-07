#include "ship.h"

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

void TPirate::ModifyVelocity(float baseVelocity)
{
	Velocity = baseVelocity * 1.25f;
}

void TPirate::ChangeTarget(const IShip* target)
{
	Target = target;
}

const IShip* TPirate::GetTarget() const
{
	return Target;
}
