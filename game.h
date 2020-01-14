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

// Responsible for running the game.
// It moves the ships (decides for civilians where to go,
// but asks the pirate where it wants to go).
class TGame
{
public:
	using TShipPtr = std::unique_ptr<ICivilian>;
	using TShipInfoSet = std::multiset<TShipInfo, TTimeComparer>;
	using TShipIt = std::list<TShipPtr>::iterator;

	TGame(options::TOptions& options);
	~TGame() = default;
	
	// Run the game, call RunTurn every tick and update time.
	bool Run();

	bool RunTurn();

private:
	// Create a ship and place it on the map.
	void CreateShip(const TShipInfo& shipInfo);
	// Create ships which time of generation equals CurrentTime and erase their info
	// from ship info set. Uses CreateShip for every ship.
	void GenerateShips();
	// Add velocity to destination.
	TCoordinates SetTemporaryDestination(int velocity, 
		TCoordinates position, TCoordinates destination) const;
	// Change ships position and change its coordinates on map.
	// Returns iterator (as parameter) to next valid element if ship was deleted 
	// (otherwise iterator points to the current element).
	// Note that it is intended only for civilians, the pirate has its own method.
	void MoveCivilian(TShipIt& it, bool& removed);
	// Move the pirate using its brain's methods to specify destination.
	void MovePirate();
	// Move all civilians.
	void MoveCivilians();
	// Check for every ship if it can see the pirates. If so, change its destination (flee).
	void LookForPirates();
	// Check if pirate can spot any civilian to set it as a target. Do nothing if pirate's target is
	// other than nullptr.
	void LookForCivilians();
	// Return true if the pirate is in ship's range of view. 
	bool SeesPirate(const TShipPtr& ship) const;
	// Return true if ship's velocity is sufficient to leave the map in this turn.
	bool CanLeave(const TShipPtr& ship) const;
	// Remove ship from ship list and from the map.
	// Returns iterator to next valid element if ship was deleted (otherwise iterator points to
	// the current element).
	bool Remove(TShipIt& it);
	// Try to attack the target (if any exists).
	void AttackTarget();

	TCoordinates SetCivilianStartingDestination(TCoordinates position) const;
	// Save max velocity of the fastest ship and modify pirate's velocity.
	void CorrectMaxVelocity(float newVelocity);

private:
	TMap Map;
	// Current simulation time.
	unsigned int CurrentTime;
	// User-defined simulation time.
	unsigned int SimDuration;
	// Stores information about ships to be generated sorted ascending by time to generation.
	// (the closer to generation the higher in set)
	// When a ship gets created, it is added to Ship list and also deleted from shipInfo set.
	TShipInfoSet ShipsInfo;
	// List of generated ships.
	std::list<TShipPtr> Ships;
	// Number of pirate's attack attempts.
	unsigned int Attempts;
	// Number of successful pirate's attack attempts.
	unsigned int SuccessfulAttempts;
	// The speed of the fastest ship on map.
	float CurrentMaxVelocity;
	// The pirate.
	TPirate Pirate;
};

#endif // _GAME_H