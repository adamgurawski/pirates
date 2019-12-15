#ifndef _SHIP_H
#define _SHIP_H

#include "map.h"

#include <string>

class IShip
{

private:
	// Speed.
	float Velocity;

	// Range of view.
	int Visibility;

	// Position on a map.
	TCoordinates Position;

	// Position the ship is travelling to.
	TCoordinates Destination;
};

// Common base class for all of civilian ships (potential pirate targets).
class ACivilian : public IShip
{
public:
	bool WasAttacked() const
	{
		return Attacked;
	}

private:
	// Chance to be attacked (percents?).
	const int Vulnerability;

	// Name. [?] Separate identificator to be added? Is it neccessary?
	std::string Name;

	/* Set to true when the ship was unsuccessfully attacked.
		 In case when it was successfully attacked - it sinks and gets deleted
		 from the map, so there is no need to set Attacked to true. */
	bool Attacked;
};

class TBulkCarrier : public ACivilian
{

};

class TTanker : public ACivilian
{

};

class TPassenger : public ACivilian
{

};

class TPirate : public IShip
{ // "The Green Oyster"

	
};

#endif // _SHIP_H