#include "messenger.h"
#include "ship.h"

#include <cassert>
#include <iostream>

void TMessenger::OnTurn(int turn, const TMap& map) const
{
	std::cout << "\n##### Turn: " << turn << " #####" << std::endl;
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