#include "game.h"

#include <cassert>
#include <cmath> // round()
#include <iostream> 
#include <time.h> // srand

TGame::TGame(options::TOptions& options)
	: Map({ options.GetMapWidth(), options.GetMapHeight() }),
	SimDuration(options.GetSimulationTime()), CurrentTime(0)
{
	// Srand must be called before Map.RollPiratesPosition.
	srand(static_cast<unsigned int>(time(NULL)));

	// Create pirate and place it on the map.
	Pirate = std::move(TPirate(Map.RollPiratesPosition()));
	Pirate.SetMapBorders(Map.GetWidth(), Map.GetHeight());
	Map.PlaceOnMap(Pirate.GetPosition(), static_cast<const IShip*>(&Pirate));

	// Will not be using options anymore, can steal ShipInfo to prevent unnecessary copying.
	ShipsInfo = options.StealShipInfo();
			
	Pirate.debug_IntroduceYourself();
}

// TODO: implement Run().
bool TGame::Run()
{ 
	for (; CurrentTime < SimDuration; ++CurrentTime)
	{
		// Generate ships which TimeToGeneration equals CurrentTime.
		GenerateShips();
		RunTurn();
	}

	return true;
}

/* 1. Check whether civilians see the pirate.
	 2. Move the civilians.	
	 3. If one can get out of map, acknowledge it.
	 4. Move the pirate.
	 5. Check whether pirate has an opportunity to attack any ship.
	 6. Attack ships, acknowledge if destroyed.*/
// TODO: implement RunTurn.
bool TGame::RunTurn()
{
	Map.debug_PrintMap();
	// Change ships' destination if they are in danger (get to the closest border).
	LookForPirates();
	MoveCivilians();
	MovePirate();
	// CanAttack?
	// Attack

	return true;
}

void TGame::CreateShip(const TShipInfo& shipInfo)
{ // TODO: throw exception when there is no available space on map?
	TCoordinates startingPosition = Map.RollCivilianPosition();
	TCoordinates destination = SetCivilianStartingDestination(startingPosition);
	TShipPtr ship;

	int generate = rand() % 3 + 1;
	switch (generate)
	{
	case 1:
		ship = std::make_unique<TPassenger>(shipInfo.Name, shipInfo.Velocity, startingPosition, 
			destination);
		break;
	case 2:
		ship = std::make_unique<TBulkCarrier>(shipInfo.Name, shipInfo.Velocity, startingPosition,
			destination);
		break;
	case 3:
		ship = std::make_unique<TTanker>(shipInfo.Name, shipInfo.Velocity, startingPosition, 
			destination);
		break;
	default:
		assert(false && "How did we get here?");
	}

	// If new ship is the fastest, update max velocity.
	CorrectMaxVelocity(shipInfo.Velocity);

	// Add ship to list.
	Ships.push_back(std::move(ship));

	// Place added ship on a map.
	const IShip* addedShip = Ships.back().get();
	Map.PlaceOnMap(addedShip->GetPosition(), addedShip);

	// Debug.
	addedShip->debug_IntroduceYourself();
}

void TGame::GenerateShips()
{
	TShipInfoSet::iterator it = ShipsInfo.begin();

	while (it != ShipsInfo.end() && it->TimeToGeneration == CurrentTime)
	{
		CreateShip(*it);
		it = ShipsInfo.erase(it);
	}
}

TCoordinates TGame::SetTemporaryDestination(int velocity, 
	TCoordinates position, TCoordinates destination) const
{
	TCoordinates result;

	if (position.X == destination.X)
	{ // Moving on Y axis.
		if (position.Y > destination.Y)
			result = { position.X, position.Y - velocity };
		else
			result = { position.X, position.Y + velocity };
	}
	else
	{ // Moving on X axis.
		if (position.X > destination.X)
			result = { position.X - velocity, position.Y };
		else
			result = { position.X + velocity, position.Y };
	}

	return result;
}

void TGame::MoveCivilian(TShipIt& it, bool& removed)
{
	TShipPtr& ship = *it;
	TCoordinates destination = ship->GetDestination();
	TCoordinates position = ship->GetPosition();

	if (CanLeave(ship))
	{
		if (ship.get() == Pirate.GetTarget())
		{ // Ship was pirate's target.
			Pirate.ChangeTarget(nullptr);
		}

		// TODO: print message?
		// Remove ship from map and ship list.
		Remove(it, removed);
	}
	else
	{	// Can not leave.
		if (!Map.HasEmptyCoordinates())
		{ // Do not move, there is nowhere to go. TODO: throw exception?
			return;
		}
		// Civilians move only in a straight line so treat velocity as an integer
		// in this case.
		int velocity = static_cast<int>(std::trunc(ship->GetVelocity()));
		TCoordinates target = SetTemporaryDestination(velocity, position, 
			destination);

		while (!Map.IsEmpty(target))
		{ // Try moving one unit less.
			velocity -= 1;

			if (velocity < 1)
			{ // Do not move. Should throw exception?
				return;
			}

			target = SetTemporaryDestination(velocity, position, destination);
		}

		Map.Move(ship.get(), target);
		ship->Move(target);
	}
}

/* 1. Check if it's possible to go to desired destination (empty coordinates).
*/
void TGame::MovePirate()
{
	bool needsCorrection = false;
	TCoordinates destination;

	do
	{
		destination = Pirate.GetDesiredDestination(needsCorrection);
		needsCorrection = Map.IsEmpty(destination) ? false : true;
	} while (needsCorrection);
	
	Map.Move(&Pirate, destination);
	Pirate.Move(destination);
}

void TGame::MoveCivilians()
{
	TShipIt it = Ships.begin();

	while (it != Ships.end())
	{
		bool removed = false;
		MoveCivilian(it, removed);
		
		if (!removed)
			++it;
		// else iterator was moved by MoveCivilian()
	}
}

void TGame::LookForPirates()
{ 
	for (TShipPtr& ship : Ships)
	{
		if (!ship->IsRunningAway() && SeesPirate(ship))
		{ // Force ship to run away -> change its destination and set "RunningAway" to true.
			TCoordinates newDestination = Map.CalculateClosestExit(ship->GetPosition());
			ship->ChangeDestination(newDestination);
			ship->SetRunningAway();
		}
	}
}

bool TGame::SeesPirate(const TShipPtr& ship) const
{
	TCoordinates piratePosition = Pirate.GetPosition();
	TCoordinates shipPosition = ship->GetPosition();
	float visibility = ship->GetRangeOfView();

	return (Map.IsInRange(shipPosition, visibility, piratePosition));
}

bool TGame::CanLeave(const TShipPtr& ship) const
{
	TCoordinates destination = ship->GetDestination();
	TCoordinates position = ship->GetPosition();
	float velocity = ship->GetVelocity();

	if (destination.X == position.X)
	{ // Ship moving on Y axis.
		if (destination.Y > position.Y)
			if (destination.Y - position.Y < velocity)
				return true;
			else
				return false;
		else
			if (position.Y - velocity < 0)
				return true;
			else
				return false;
		
	}
	else if (destination.Y == position.Y)
	{ // Ship moving on X axis.
		if (destination.X > position.X)
			if (destination.X - position.X < velocity)
				return true;
			else
				return false;
		else
			if (position.X - velocity < 0)
				return true;
			else
				return false;
	}
	else
	{
		throw (std::logic_error("Which axis does the ship move on?"));
	}
}

void TGame::Remove(TShipIt& it, bool& removed)
{
	TShipPtr& ship = *it;
	Map.Remove(ship->GetPosition());
	it = Ships.erase(it);
	removed = true;
}

// TODO: make it less ugly.
TCoordinates TGame::SetCivilianStartingDestination(TCoordinates position) const
{
	TCoordinates destination = position;
	unsigned int xMax = Map.GetWidth() - 1;
	unsigned int yMax = Map.GetHeight() - 1;

	/* Get opposite side of map.
			- for x = 0,   y = 0,			set x = max, y = 0.
			- for x = max, y = max,		set x = max, y = 0. 
			- for x = 0,   y = max,		set x = 0,   y = 0.
			- for x = max, y = 0,			set x = 0,   y = 0 */
	if (position.Y == 0)
		destination.Y = yMax;
	if (position.Y == yMax)
		destination.Y = 0;
	if (position.X == 0)
		destination.X = xMax;
	if (position.X == xMax)
		destination.X = 0;

	// Temporary fix.
	if (position.X == 0 && position.Y == 0 || position.X == xMax && position.Y == xMax)
	{
		destination.X = xMax;
		destination.Y = 0;
	}
	else if (position.X == 0 && position.Y == yMax || position.X == xMax && position.Y == 0)
	{
		destination = { 0,0 };
	}

	return destination;
}

void TGame::CorrectMaxVelocity(float newVelocity)
{
	if (newVelocity > CurrentMaxVelocity)
	{
		CurrentMaxVelocity = newVelocity;
		Pirate.ModifyVelocity(CurrentMaxVelocity);
	}
}