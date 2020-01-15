#include "pirate.h"

#include <cassert>
#include <vector>

namespace
{
// If one wishes to change the name, shall do it in "map.cpp" as well.
#define PIRATE_NAME "The Green Oyster"
#define PIRATE_VISIBILITY 7.0f
#define PIRATE_INITIAL_VELOCITY 1.0f	
}

TTargetWrapper& TTargetWrapper::operator=(const IShip* ship)
{
	Target = ship;
	return *this;
}

TTargetWrapper& TTargetWrapper::operator=(const TTargetWrapper& rhs)
{
	Target = rhs.Target;
	return *this;
}

TTargetWrapper& TTargetWrapper::operator=(TTargetWrapper&& rhs)
{
	Target = std::move(rhs.Target);
	return *this;
}

TPirate::TPirate(TCoordinates position) :
	AShip(PIRATE_NAME, PIRATE_INITIAL_VELOCITY, PIRATE_VISIBILITY, position,
	{ 0,0 }),
	Target(nullptr)
{
	Brain = std::make_unique<TSimpleBrain>(Position, Destination, Velocity, Target);
}

TPirate::TPirate(TPirate&& rhs) : AShip(std::move(rhs)),
Target(std::move(rhs.Target))
{
	Brain = std::move(rhs.Brain);
}

TPirate& TPirate::operator=(TPirate&& rhs)
{
	AShip::operator=(std::move(rhs));
	// Delete old Brain.
	rhs.Brain.reset();;
	Target = std::move(rhs.Target);
	// Create new brain referencing proper TPirate's members. 
	Brain = std::make_unique<TSimpleBrain>(Position, Destination, Velocity, Target);
	return *this;
}

TCoordinates TPirate::GetDesiredDestination(bool needsCorrection, 
	unsigned int attempts)
{
	return Brain->GetDesiredDestination(needsCorrection, attempts);
}

void TPirate::ModifyVelocity(float fastestCivilianVelocity)
{
	Velocity = fastestCivilianVelocity * 1.25f;
}

void TPirate::ChangeTarget(const IShip* target)
{
	Target = target;
	if (!Target.IsEmpty())
		Destination = Target->GetPosition();
}

const IShip* TPirate::GetTarget() const
{
	return *Target;
}

void TPirate::ChangeDestination(const TCoordinates& destination)
{
	Destination = destination;
}

void TPirate::SetMapBorders(unsigned int maxX, unsigned int maxY)
{
	Brain->SetMapBorders(maxX, maxY);
}

TSimpleBrain::TSimpleBrain(TCoordinates& position,
	TCoordinates& destination, float& velocity, TTargetWrapper& target) : 
	MaxX(0), MaxY(0),	LongTermDestination({ 0,0 }), Position(position), 
	Destination(destination), Velocity(velocity), Target(target)
{
}

/*
	 1. If doesn't need correction, return genuine desired destination.
	 2. If needs correction, return genuine desired destination - 1 tile.
	 3. Repeat 2, when Destination = Position return Position?
*/
TCoordinates TSimpleBrain::GetDesiredDestination(bool needsCorrection,
	unsigned int attempts)
{
	TCoordinates destination = Position;

	if (!Target.IsEmpty())
	{ // Chase the target.
		TCoordinates targetPosition = Target->GetPosition();

		if (CanReach(targetPosition))
		{ // Go as close to target as possible.
			bool isAnyEmpty = true;
			destination = GetPositionNearTarget(needsCorrection, isAnyEmpty, 
				attempts);
			if (!isAnyEmpty)
			{ // No space near target to get to. Return position.
			}
		}
		else
		{ // Can't reach target in this turn, follow the target.
			// TODO: !! Firstly align with target's X or Y, then follow along 
			// the second axis.
		}
	}
	else
	{ // TODO: Zig-zag.

	}

	// Temporary until implemented.
	Destination = destination;
	return Destination;
}

void TSimpleBrain::SetMapBorders(unsigned int maxX, unsigned int maxY)
{
	MaxX = maxX;
	MaxY = maxY;
}

TSimpleBrain& TSimpleBrain::operator=(TSimpleBrain&& rhs)
{
	MaxX = std::move(rhs.MaxX);
	MaxY = std::move(rhs.MaxY);
	Velocity = std::move(rhs.Velocity);
	Position = std::move(rhs.Position);
	Destination = std::move(rhs.Destination);
	Target = std::move(rhs.Target);
	LongTermDestination = std::move(LongTermDestination);

	return *this;
}

bool TSimpleBrain::CanReach(TCoordinates point) const
{
	int x = std::abs(static_cast<int>(point.X - Position.X));
	int y = std::abs(static_cast<int>(point.Y - Position.Y));

	float powX = std::pow(x, 2);
	float powY = std::pow(y, 2);
	float powDistance = powX + powY;
	float distance = std::sqrt(powDistance);

	return !(distance > Velocity);
}
TCoordinates TSimpleBrain::GetPositionNearTarget(bool needsCorrection,
	bool& isAnyEmpty, unsigned int attempts) const
{ // TODO: ! implement.
	assert(!Target.IsEmpty() && "Target must be set.");
	assert(CanReach(Target->GetPosition()) &&
		"Can be called only when target is reachable.");

	TCoordinates target = Target->GetPosition();

	bool upInaccessible = target.Y == MaxY;
	bool downInaccessible = target.Y == 0;
	bool leftInaccessible = target.X == 0;
	bool rightInaccessible = target.X == MaxX;
	
	TCoordinates up = target + TCoordinates({ static_cast<unsigned>(0),
		static_cast<unsigned>(1) });
	TCoordinates down = target - TCoordinates({ static_cast<unsigned>(0),
		static_cast<unsigned>(1) });
	TCoordinates left = target - TCoordinates({ static_cast<unsigned>(1),
		static_cast<unsigned>(0) });
	TCoordinates right = target + TCoordinates({ static_cast<unsigned>(1),
		static_cast<unsigned>(0) });
	
	std::vector<TCoordinates> coordinates;

	if (!upInaccessible && CanReach(up))
		coordinates.push_back(up);
	else if (!leftInaccessible && CanReach(left))
		coordinates.push_back(left);
	else if (!rightInaccessible && CanReach(right))
		coordinates.push_back(right);
	else if (!downInaccessible && CanReach(down))
		coordinates.push_back(down);
	else
		isAnyEmpty = false;

	if (attempts - 1 < coordinates.size())
	{
		Destination = coordinates.at(attempts - 1);
	}

	return Destination;
}


void TPirate::debug_IntroduceYourself() const
{
	std::cout << "Argghh!" << std::endl;
	std::cout << "Name: " << Name << std::endl << "Velocity: " <<
		Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
		"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
		"Destination: " << Destination.X << " " << Destination.Y << std::endl;
}