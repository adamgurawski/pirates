#include "ship.h"

namespace
{
#define BULKCARRIERS_VISIBILITY 5.0f
#define TANKERS_VISIBILITY 6.0f
#define PASSENGERS_VISIBILITY 8.0f

#define BULKCARRIERS_VULNERABILITY 0.6f
#define TANKERS_VULNERABILITY 0.8f
#define PASSENGERS_VULNERABILITY 0.3f
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

void ACivilian::IntroduceYourself() const
{
	AShip::IntroduceYourself();
	std::cout << "Vulnerability: " << Vulnerability << std::endl <<
		"Was attacked: " << std::boolalpha << Attacked << std::endl <<
		"Is running away: " << RunningAway << std::endl << std::endl;
}

TBulkCarrier::TBulkCarrier(const std::string& name, float velocity, TCoordinates position,
	TCoordinates destination) : ACivilian(name, velocity, BULKCARRIERS_VISIBILITY, position, 
		destination, BULKCARRIERS_VULNERABILITY)
{}

void TBulkCarrier::IntroduceYourself() const
{
	std::cout << "Type of vessel: bulk carrier" << std::endl;
	ACivilian::IntroduceYourself();
}

TTanker::TTanker(const std::string& name, float velocity, TCoordinates position,
	TCoordinates destination) : ACivilian(name, velocity, TANKERS_VISIBILITY, position, 
		destination, TANKERS_VULNERABILITY)
{}

void TTanker::IntroduceYourself() const
{
	std::cout << "Type of vessel: tanker" << std::endl;
	ACivilian::IntroduceYourself();
}

TPassenger::TPassenger(const std::string& name, float velocity, TCoordinates position, 
	TCoordinates destination) : ACivilian(name, velocity, PASSENGERS_VISIBILITY, position,
		destination, PASSENGERS_VULNERABILITY)
{}

void TPassenger::IntroduceYourself() const
{
	std::cout << "Type of vessel: passenger" << std::endl;
	ACivilian::IntroduceYourself();
}