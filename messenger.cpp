#include "messenger.h"
#include "ship.h"

#include <cassert>
#include <iostream>

void TMessenger::OnLeave(const IShip* ship, const TCoordinates& lastPosition) const
{
	assert(ship);
	std::cout << "Ship " << ship->GetName() << " left the map at " << 
		lastPosition << "." << std::endl;
}

void TMessenger::OnAttack(bool successfulAttack, const IShip* target) const
{
	TCoordinates position = target->GetPosition();

	std::string middlePart = successfulAttack ?
		" was successfully attacked and got sunk at " :
		" was unsuccessfully attacked at ";

	std::cout << "Ship " << target->GetName() << middlePart << position << "." << std::endl;
}

void TMessenger::OnEnd(unsigned int attempts, unsigned int successfulAttempts) const
{
	std::cout << "\nSummary: \nPirate's attack attempts: " << attempts <<	
		"\nNumber of sunken ships: " << successfulAttempts << std::endl;
}