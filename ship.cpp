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

bool AShip::IsRunningAway() const
{
	return RunningAway;
}

void AShip::SetRunningAway()
{
	RunningAway = true;
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

TCoordinates AShip::GetDestination() const
{
	return Destination;
}

void AShip::ChangeDestination(TCoordinates coordinates)
{
	Destination = coordinates;
}

float TPirate::GetVelocity() const
{
	return Velocity;
}

float TPirate::GetRangeOfView() const
{
	return Visibility;
}

TCoordinates TPirate::GetPosition() const
{
	return Position;
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
