#include "game.h"

#include <cassert>
#include <iostream> 
#include <time.h> // srand

TGame::TGame(options::TOptions& options)
	: Map({ options.GetMapWidth(), options.GetMapHeight() }),
	SimulationTime(options.GetSimulationTime())
{
	// Srand must be called before Map.RollPiratesPosition.
	srand(time(NULL));

	// Create pirate and place it on the map.
	Pirate = TPirate(0, 5, Map.RollPiratesPosition(), { 0,0 });
	Map.PlaceOnMap(Pirate.GetPosition(), &Pirate);

	std::vector<options::TShipInfo> shipInfoVector = options.GetShips();
	
	// Temp helper for inserting to list. TODO: delete.
	int i = 0;
	for (options::TShipInfo shipInfo: shipInfoVector)
	{
		TCoordinates startingPosition = Map.RollCivilianPosition();
		TCoordinates destination = SetCivilianStartingDestination(startingPosition);
		std::unique_ptr<IShip> ship;


		// TODO: how to set it?
		int visibility = 5;

		int generate = rand() % 3 + 1;
		switch (generate)
		{
		case 1:
			ship = std::make_unique<TPassenger>(TPassenger(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
			break;
		case 2:
			ship = std::make_unique<TBulkCarrier>(TBulkCarrier(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
			break;
		case 3:
			ship = std::make_unique<TTanker>(TTanker(shipInfo.Name, shipInfo.Velocity, visibility,
				startingPosition, destination));
			break;
		default:
			assert(false && "How did we get here?");
		}
	
		// If new ship is the fastest, update max velocity.
		CorrectMaxVelocity(shipInfo.Velocity);
		
		// Add ship to list.
		Ships.push_back(std::move(ship));
		IShip* shipInList = std::next(Ships.begin(), i)->get();
		Map.PlaceOnMap(shipInList->GetPosition(), shipInList);

		// Debug.
		shipInList->debug_IntroduceYourself();

		++i;
	} // for
			
	

	
	Pirate.debug_IntroduceYourself();
	Map.debug_PrintMap();
}

bool TGame::Run()
{ // TODO: implement Run().
	return true;
}

TCoordinates TGame::SetCivilianStartingDestination(TCoordinates position) const
{
	TCoordinates destination = position;
	unsigned int xMax = Map.GetWidth() - 1;
	unsigned int yMax = Map.GetHeight() - 1;

	/* Get opposite side of map.
			Not using else because of these cases:
			- for x = 0, y = 0 we want to set x = max, y = max.
			- for x = max, y = max we want to set x = 0, y = 0. */
	if (position.X == 0)
		destination.X = xMax;
	if (position.Y == 0)
		destination.Y = yMax;
	if (position.X == xMax)
		destination.X = 0;
	if (position.Y == yMax)
		destination.Y = 0;
	
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