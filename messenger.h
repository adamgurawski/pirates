#ifndef _MESSENGER_H
#define _MESSENGER_H

#include "coordinates.h"
#include "map.h"

class IShip;

class TMessenger
{
public:
	TMessenger() : Graphical(false), Verbose(false)
	{}

	TMessenger(bool graphical, bool verbose) : Graphical(graphical), 
		Verbose(verbose)
	{}

	// Print turn number and print graphical representation of the map if -g was passed.
	void OnTurn(int turn, const TMap& map) const;
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

private:
	bool Graphical;
	bool Verbose;
};

#endif // _MESSENGER_H
