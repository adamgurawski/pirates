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
	Brain = std::make_unique<TSimpleBrain>(mapWidth - 1, mapHeight - 1);
}

TPirate::TPirate(TPirate&& rhs) : Target(rhs.Target),
	AShip(rhs.Name, rhs.Velocity, rhs.Visibility, rhs.Position, rhs.Destination)
{
	Brain = std::move(rhs.Brain);
}

TPirate& TPirate::operator=(TPirate&& rhs)
{
	Brain = std::move(rhs.Brain);
	Destination = rhs.Destination;
	Name = rhs.Name;
	Position = rhs.Position;
	Velocity = rhs.Velocity;
	Visibility = rhs.Visibility;
	Target = rhs.Target;

	return *this;
}


void TPirate::Move()
{
	if (HeadToDestination())
	{ // Is at destination.
		UpdateDestination();
	}
	

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
	return Target;
}

void TPirate::UpdateDestination()
{
	Destination = Brain->SetNewDestination(Position, Target, Velocity);
}

bool TPirate::HeadToDestination()
{ // TODO: check if velocity allows Position = Destination.
	return true;
}

TSimpleBrain::TSimpleBrain(int maxX, int maxY) : MaxX(maxX), MaxY(maxY),
	LongTermDestination({ 0,0 })
{
}

TCoordinates TSimpleBrain::SetNewDestination(TCoordinates position, const IShip* target,
	float velocity)
{
	if (target)
	{ // Chase the target.
		LongTermDestination = target->GetPosition();
		return LongTermDestination;
	}
	else
	{ // Zig-zag.
		// TODO: !! Firstly align with target's X or Y, then follow along the second axis.
	}
}

void TPirate::debug_IntroduceYourself() const
{
	std::cout << "Argghh!" << std::endl;
	std::cout << "Name: " << Name << std::endl << "Velocity: " <<
		Velocity << std::endl << "Visibility: " << Visibility << std::endl <<
		"Position: [" << Position.X << " " << Position.Y << "]" << std::endl <<
		"Destination: " << Destination.X << " " << Destination.Y << std::endl;
}