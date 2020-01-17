#include "map.h"
#include "messenger.h"
#include "pirate.h"
#include "ship.h"

#include <cassert>
#include <iostream>

void TMessenger::OnGenerate(const IShip* ship, bool isPirate) const
{
	assert(ship);
	std::cout << "\n##### New ship was generated! #####" << std::endl;
	if (isPirate)
		std::cout << "Arghh!" << std::endl;

	ship->IntroduceYourself();
}

void TMessenger::OnTurn(int turn) const
{
	std::cout << "\n##### Turn: " << turn << " #####" << std::endl;
}

void TMessenger::PrintMap(const TMap& map) const
{
	if (Graphical)
		map.Display();
}

void TMessenger::OnLeave(const IShip* ship, const TCoordinates& lastPosition) const
{
	assert(ship);
	std::cout << "Civilian ship " << ship->GetName() << " left the map at " << 
		lastPosition << "." << std::endl;
}

void TMessenger::OnAttack(bool successfulAttack, const IShip* target) const
{
	TCoordinates position = target->GetPosition();

	std::string middlePart = successfulAttack ?
		" was successfully attacked and got sunk at " :
		" was unsuccessfully attacked at ";

	std::cout << "Civilian ship " << target->GetName() << middlePart << position << "." << std::endl;
}

void TMessenger::OnEnd(unsigned int attempts, unsigned int successfulAttempts) const
{
	std::cout << "\nSummary: \nPirate's attack attempts: " << attempts <<	
		"\nNumber of sunken ships: " << successfulAttempts << std::endl;
}

void TMessenger::OnMove(const IShip* ship, const TCoordinates& lastPosition,
	bool isPirate) const
{
	if (!Verbose)
		return;

	assert(ship);
	TCoordinates currentPosition = ship->GetPosition();
	std::string shipType = isPirate ?
		"Pirate ship " : "Civilian ship ";

	if (currentPosition == lastPosition)
		std::cout << shipType << ship->GetName() << " stayed at position " <<
			currentPosition << "." << std::endl;
	else
		std::cout << shipType << ship->GetName() << " moved from " << lastPosition <<
			" to " << currentPosition << "." << std::endl;
}

void TMessenger::OnPirateSpotted(const IShip* ship, const TCoordinates& oldDestination,
	const TCoordinates& newDestination) const
{
	if (!Verbose)
		return;

	assert(ship);
	std::cout << "Civilian ship " << ship->GetName() << " spotted the pirate, " <<
		"so it changed its destination from " << oldDestination << " to " <<
		newDestination << "." << std::endl;
}

void TMessenger::OnChangeTarget(const TPirate& pirate) const
{
	if (!Verbose)
		return;

	if (pirate.GetTarget())
	{
		std::string targetName = pirate.GetTarget()->GetName();
		TCoordinates targetPosition = pirate.GetTarget()->GetPosition();

		std::cout << "Pirate ship " << pirate.GetName() << " changed its target to " <<
			targetName << " located at position " << targetPosition << "." << std::endl;
	}
	else
	{ // Pirate has no target.
		std::cout << "Pirate ship " << pirate.GetName() << " does not have a target anymore."
			<< std::endl;
	}
}
