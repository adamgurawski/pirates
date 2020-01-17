#include "game.h"

#include <cassert>
#include <cmath> // std::round
#include <time.h> // srand

TGame::TGame(options::TOptions& options)
	: Map({ options.GetMapWidth(), options.GetMapHeight() }),
	SimDuration(options.GetSimulationTime()), CurrentTime(0), Attempts(0),
	SuccessfulAttempts(0), CurrentMaxVelocity(0), Messenger(options.IsGraphical(),
		options.IsVerbose())
{
	// Srand must be called before Map.RollPiratesPosition.
	srand(static_cast<unsigned int>(time(NULL)));

	// Create pirate and place it on the map.
	Pirate = std::move(TPirate(Map.RollPiratesPosition()));
	Pirate.SetMapBorders(Map.GetWidth() - 1, Map.GetHeight() - 1);
	Map.PlaceOnMap(Pirate.GetPosition(), static_cast<const IShip*>(&Pirate));

	// Will not be using options anymore, can steal ShipInfo to prevent unnecessary copying.
	ShipsInfo = options.StealShipInfo();
	Messenger.OnGenerate(&Pirate, true);
}

bool TGame::Run()
{ 
	for (; CurrentTime < SimDuration; ++CurrentTime)
	{
		try
		{
			// Generate ships which TimeToGeneration equals CurrentTime.
			Messenger.OnTurn(CurrentTime + 1);
			GenerateShips();
			Messenger.PrintMap(Map);
			RunTurn();
		}
		catch (const std::out_of_range&)
		{ // Catch std::map::at exception and describe it the human way.
			std::cerr << "Error: Coordinates out of range!" << std::endl;
			return false;
		}
		catch (const std::logic_error& logicError)
		{ 
			std::cerr << logicError.what() << std::endl;
			return false;
		}
	}

	Messenger.OnEnd(Attempts, SuccessfulAttempts);
	return true;
}

void TGame::RunTurn()
{
	// Change ships' destination if they are in danger (get to the closest border).
	LookForPirates();
	// Move all civilians. If some can escape, remove it from the game.
	MoveCivilians();
	// Check if pirate can spot any civilians.
	LookForCivilians();
	// Move the pirate using its own brain (ask for desired destination).
	MovePirate();
	// If pirate's target is in range, try to attack.
	AttackTarget();
}

void TGame::CreateShip(const TShipInfo& shipInfo)
{ 
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
	if (ship->GetVelocity() > GetMaxVelocity())
		CorrectMaxVelocity(ship->GetVelocity());

	// Add ship to ship container.
	Ships.push_back(std::move(ship));

	// Place added ship on a map.
	const IShip* addedShip = Ships.back().get();
	Map.PlaceOnMap(addedShip->GetPosition(), addedShip);
	Messenger.OnGenerate(addedShip);
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
			Messenger.OnChangeTarget(Pirate);
		}

		Messenger.OnLeave(ship.get(), destination);
		// Remove ship from map and ship list.
		removed = Remove(it);
	}
	else
	{	// Can not leave.
		if (!Map.HasEmptyCoordinates())
		{ // Do not move, there is nowhere to go.
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
			{ // Do not move, can't get closer to destination.
				return;
			}

			target = SetTemporaryDestination(velocity, position, destination);
		}

		Map.Move(ship.get(), target);
		ship->Move(target);
		Messenger.OnMove(ship.get(), position, false);
	}
}

void TGame::MovePirate()
{
	bool				 needsCorrection = false;
	TCoordinates destination;
	unsigned int attempts = 0;

	do
	{
		++attempts;
		destination = Pirate.GetDesiredDestination(attempts);
		needsCorrection = !Map.IsEmpty(destination);
	} while (needsCorrection && attempts < 4);
	
	TCoordinates positionBeforeMove = Pirate.GetPosition();
	Map.Move(&Pirate, destination);
	Pirate.Move(destination);
	Messenger.OnMove(&Pirate, positionBeforeMove, true);
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
			TCoordinates oldDestination = ship->GetDestination();
			TCoordinates newDestination = Map.CalculateClosestExit(ship->GetPosition());
			ship->ChangeDestination(newDestination);
			ship->SetRunningAway();
			Messenger.OnPirateSpotted(ship.get(), oldDestination, newDestination);
		}
	}
}

void TGame::LookForCivilians()
{
	if (!Pirate.GetTarget())
	{
		for (const TShipPtr& civilian : Ships)
		{
			TCoordinates targetPosition = civilian->GetPosition();
			TCoordinates piratePosition = Pirate.GetPosition();
			float visibility = Pirate.GetRangeOfView();
			bool wasNotAttacked = !civilian->WasAttacked();

			if (IsInRange(piratePosition, visibility, targetPosition) && wasNotAttacked)
			{
				Pirate.ChangeTarget(civilian.get());
				Messenger.OnChangeTarget(Pirate);
				break;
			}
		}
	}
	// else do nothing, pirate already has a target.
}

bool TGame::SeesPirate(const TShipPtr& ship) const
{
	TCoordinates piratePosition = Pirate.GetPosition();
	TCoordinates shipPosition = ship->GetPosition();
	float visibility = ship->GetRangeOfView();

	return (IsInRange(shipPosition, visibility, piratePosition));
}

bool TGame::CanLeave(const TShipPtr& ship) const
{
	TCoordinates destination = ship->GetDestination();
	TCoordinates position = ship->GetPosition();

	if (position == destination)
		return true;

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
		throw (std::logic_error("Error: Cannot determine whether ship can leave the map."));
	}
}

bool TGame::IsInRange(const TCoordinates& center, float visibility,
	const TCoordinates& target) const
{
	int x = std::abs(static_cast<int>(target.X - center.X));
	int y = std::abs(static_cast<int>(target.Y - center.Y));

	float powX = std::pow(x, 2);
	float powY = std::pow(y, 2);
	float powDistance = powX + powY;
	float distance = std::sqrt(powDistance);

	return !(distance > visibility);
}

bool TGame::Remove(TShipIt& it)
{
	// Remove ship from map.
	TShipPtr& ship = *it;
	Map.Remove(ship->GetPosition());
	// Remove ship from ship list.
	it = Ships.erase(it);
	// Update max velocity.
	float currentMaxVelocity = GetMaxVelocity();
	CorrectMaxVelocity(currentMaxVelocity);
	return true;
}

void TGame::AttackTarget()
{
	if (!Pirate.GetTarget())
		return;

	TCoordinates piratePosition = Pirate.GetPosition();
	TCoordinates targetPosition = Pirate.GetTarget()->GetPosition();

	if (IsInRange(piratePosition, 1, targetPosition))
	{ // If the pirate is next to the target, carry out an attack.
		int roll = std::rand() % 100;
		const IShip* target = Pirate.GetTarget();

		for (auto it = Ships.begin(); it != Ships.end(); ++it)
			if (target == it->get())
			{
				int vulnerability = it->get()->GetVulnerability() * 100;
				
				if (!(roll > vulnerability))
				{ // Success.
					SuccessfulAttempts++;
					Messenger.OnAttack(true, target);
					Remove(it);
				}
				else
				{ // Attack failed.
					it->get()->SetAttacked();
					Messenger.OnAttack(false, target);
				}

				Pirate.ChangeTarget(nullptr);
				Messenger.OnChangeTarget(Pirate);
				Attempts++;
				break;
			}
	}
}

TCoordinates TGame::SetCivilianStartingDestination(TCoordinates position) const
{ // TODO: refactor this.
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

	// Ugly fix, but it works.
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
	CurrentMaxVelocity = newVelocity;
	Pirate.ModifyVelocity(CurrentMaxVelocity);
}

float TGame::GetMaxVelocity() const
{
	float velocity = 0;
	for (const TShipPtr& ship : Ships)
		if (ship->GetVelocity() > velocity)
			velocity = ship->GetVelocity();

	if (velocity == 0)
		return 0;
	else
		return velocity;
		
}