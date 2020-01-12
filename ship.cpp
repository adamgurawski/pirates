#include "ship.h"

// TODO: set some rules and hardcode it.
// TODO: (debug) temporary doubled vision
namespace
{
#define TANKERS_VISIBILITY 8.0f
#define PASSENGERS_VISIBILITY 10.0f
#define BULKCARRIERS_VISIBILITY 6.0f

#define TANKERS_VULNERABILITY 80
#define PASSENGERS_VULNERABILITY 30	
#define BULKCARRIERS_VULNERABILITY 60
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

void ACivilian::Move(TCoordinates coordinates)
{
	Position = coordinates;
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