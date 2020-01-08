// File game.h, contains TGame responsible for managing the game.

#ifndef _GAME_H
#define _GAME_H

#include "map.h"
#include "options.h"
#include "pirate.h"
#include "ship.h"

#include <memory>
#include <string>
#include <list>

/* Responsible for running the game.
*/
class TGame
{
public:
	using TShipPtr = std::unique_ptr<ICivilian>;
	using TShipInfoSet = std::multiset<TShipInfo, TTimeComparer>;
	using TShipIt = std::list<TShipPtr>::iterator;

	TGame(options::TOptions& options);
	~TGame() = default;

	bool Run();

	bool RunTurn();

private:
	// Create a ship and place it on the map.
	void CreateShip(const TShipInfo& shipInfo);

	// Create ships which time of generation equals CurrentTime and erase their info
	// from ship info set.
	void GenerateShips();

	// Change ships position and change its coordinates on map.
	// Returns iterator to next valid element if ship was deleted (otherwise iterator points to
	// the current element).
	void Move(TShipIt& it, bool& removed);

	// Move all civilians.
	void MoveCivilians();

	// Check for every ships if it can see the pirates. If so, change its destination (flee).
	void LookForPirates();

	// Return true if the pirate can be spotted. 
	bool SeesPirate(const TShipPtr& ship) const;

	// Return true if ship can leave the map in this turn.
	bool CanLeave(const TShipPtr& ship) const;

	// Remove ship from ship list and from the map.
	// Returns iterator to next valid element if ship was deleted (otherwise iterator points to
	// the current element).
	void Remove(TShipIt& it, bool& removed);

	TCoordinates SetCivilianStartingDestination(TCoordinates position) const;

	// Save max velocity of the fastest ship and modify pirate's velocity.
	void CorrectMaxVelocity(float newVelocity);

private:
	TMap Map;

	unsigned int CurrentTime;

	unsigned int SimDuration;

	// Stores information about ships to be generated sorted ascending by time to generation.
	// (the closer to generation the higher in set)
	// When a ship gets created, it is added to Ship list and also deleted from shipInfo set.
	TShipInfoSet ShipsInfo;

	// List of generated ships.
	std::list<TShipPtr> Ships;

	// Number of pirate's attack attempts.
	unsigned int Attempts = 0;

	// Number of successful pirate's attack attempts.
	unsigned int SuccessfulAttempts = 0;

	// The speed of the fastest ship on map.
	float CurrentMaxVelocity = 0;

	// The pirate.
	TPirate Pirate;
};

#endif // _GAME_H