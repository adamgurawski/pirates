// File messenger.h, contains TMessenger responsible for printing events to console.

#ifndef _MESSENGER_H
#define _MESSENGER_H

#include "coordinates.h"

class IShip;
class TMap;
class TPirate;

class TMessenger
{
public:
	TMessenger() : Graphical(false), Verbose(false)
	{}

	TMessenger(bool graphical, bool verbose) : Graphical(graphical), 
		Verbose(verbose)
	{}

	// Print ship's details after generating.
	void OnGenerate(const IShip* ship, bool isPirate = false) const;
	// Print turn number.
	void OnTurn(int turn) const;
	// Print graphical representation of the map if -g was passed.
	void PrintMap(const TMap& map) const;
	// Print position where the ship has left the map.
	void OnLeave(const IShip* ship, const TCoordinates& lastPosition) const;
	// Print info about success or failure of pirate's attack.
	void OnAttack(bool successfulAttack, const IShip* target) const;
	// Print summary (attack statistics).
	void OnEnd(unsigned int attempts, unsigned int successfulAttempts) const;

	// Verbose:
	// Print info about movement of a ship (pirate/civilian).
	void OnMove(const IShip* ship, const TCoordinates& lastPosition, 
		bool isPirate = false) const;
	// Print info about ship changing destination due to proximity to the pirate.
	void OnPirateSpotted(const IShip* ship, const TCoordinates& oldDestination,
		const TCoordinates& newDestination) const;
	// Print new target's name when pirate changes the target.
	void OnChangeTarget(const TPirate& pirate) const;

private:
	bool Graphical;
	bool Verbose;
};

#endif // _MESSENGER_H
