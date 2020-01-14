#include "ship.h"

// TODO: (debug) temporary doubled vision
namespace
{
#define TANKERS_VISIBILITY 8.0f
#define PASSENGERS_VISIBILITY 10.0f
#define BULKCARRIERS_VISIBILITY 6.0f

#define TANKERS_VULNERABILITY 0.8f
#define PASSENGERS_VULNERABILITY 0.3f
#define BULKCARRIERS_VULNERABILITY 0.6f
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

float ACivilian::GetVulnerability() const
{
	return Vulnerability;
}

void ACivilian::ChangeDestination(TCoordinates coordinates)
{
	Destination = coordinates;
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