#include "pirate.h"

namespace
{
#define PIRATES_VISIBILITY 3.0f
#define PIRATES_INITIAL_VELOCITY 1.0f	
}

TPirate::TPirate(TCoordinates position, unsigned int mapWidth, unsigned int mapHeight) :
	AShip("The Green Oyster", PIRATES_INITIAL_VELOCITY, PIRATES_VISIBILITY, position,
	{ 0,0 }),
	Target(nullptr)
{
	Brain = std::make_unique<TSimpleBrain>(mapWidth - 1, mapHeight - 1, Position, Destination,
		Velocity, Target);
}

TPirate::TPirate(TPirate&& rhs) :  
	AShip(rhs.Name, rhs.Velocity, rhs.Visibility, rhs.Position, rhs.Destination),
	Target(std::move(rhs.Target))
{
	Brain = std::move(rhs.Brain);
}

TPirate& TPirate::operator=(TPirate&& rhs)
{
	Brain = std::move(rhs.Brain);
	Destination = std::move(rhs.Destination);
	Name = std::move(rhs.Name);
	Position = std::move(rhs.Position);
	Velocity = std::move(rhs.Velocity);
	Visibility = std::move(rhs.Visibility);
	Target = std::move(rhs.Target);

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

TSimpleBrain::TSimpleBrain(int maxX, int maxY, TCoordinates& position,
	TCoordinates& destination, float& velocity, TTargetWrapper& target) : MaxX(maxX),
	MaxY(maxY),	LongTermDestination({ 0,0 }), Position(position), 
	Destination(destination), Velocity(velocity), Target(target)
{
}

/* TODO: !! Describe what should be done.
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
		HandleDesiredDestination(modifier);
	}


	return desiredDestination;
}

TCoordinates TSimpleBrain::HandleDesiredDestination(int modifier)
{
	if (Target.IsEmpty())
	{ // Chase the target.
		// legacy?
		// LongTermDestination = Target->GetPosition();
		if (CanReachDesiredDestination(modifier))
			return Destination;
	}
	else
	{ // Zig-zag.
		// TODO: !! Firstly align with target's X or Y, then follow along 
		// the second axis.
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