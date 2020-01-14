#include "pirate.h"

namespace
{
#define PIRATES_VISIBILITY 3.0f
#define PIRATES_INITIAL_VELOCITY 1.0f	
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
	AShip("The Green Oyster", PIRATES_INITIAL_VELOCITY, PIRATES_VISIBILITY, position,
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
	Brain = std::make_unique<TSimpleBrain>(Position, Destination, Velocity, Target);
	return *this;
}

TCoordinates TPirate::GetDesiredDestination(bool& needsCorrection) const
{
	return Brain->GetDesiredDestination(needsCorrection);
}

void TPirate::ModifyVelocity(float fastestCivilianVelocity)
{
	Velocity = fastestCivilianVelocity * 1.25f;
}

void TPirate::ChangeTarget(const IShip* target)
{
	Target = target;
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
TCoordinates TSimpleBrain::GetDesiredDestination(bool& needsCorrection)
{
	TCoordinates desiredDestination;

	if (!needsCorrection)
		desiredDestination = HandleDesiredDestination();
	else
	{
		int modifier = 1;
		desiredDestination = HandleDesiredDestination(modifier);
	}

	return desiredDestination;
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

// TODO: !! make it work.
TCoordinates TSimpleBrain::HandleDesiredDestination(int modifier)
{
	if (!Target.IsEmpty())
	{ // Chase the target.
		// TODO: !! Firstly align with target's X or Y, then follow along 
		// the second axis.
		// legacy?
		// LongTermDestination = Target->GetPosition();
		if (CanReachDesiredDestination(modifier))
			return Destination;
	}
	else
	{ // Zig-zag.

	}

	// Temporary until implemented.
	return Position;
}

bool TSimpleBrain::CanReachDesiredDestination(int modifier) const
{
	float velocity = Velocity - modifier;
	TCoordinates target = Target->GetPosition();

	int x = std::abs(static_cast<int>(target.X - Position.X));
	int y = std::abs(static_cast<int>(target.Y - Position.Y));

	float powX = std::pow(x, 2);
	float powY = std::pow(y, 2);
	float powDistance = powX + powY;
	float distance = std::sqrt(powDistance);

	return !(distance > velocity);
}

void TPirate::debug_IntroduceYourself() const
{
	std::cout << "Argghh!" << std::endl;
	std::cout << "Name: " << Name << std::endl << "Velocity: " <<
		Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
		"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
		"Destination: " << Destination.X << " " << Destination.Y << std::endl;
}