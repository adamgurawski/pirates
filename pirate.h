#ifndef _PIRATE_H
#define _PIRATE_H

#include "ship.h"

class TTargetWrapper
{
public:
	// Do not delete Target, wrapper does not own this resource.
	~TTargetWrapper() = default;

	TTargetWrapper()
	{
		Target = nullptr;
	}

	TTargetWrapper(const IShip* target)
	{
		Target = target;
	}

	// Copy constructor.
	TTargetWrapper(const TTargetWrapper& rhs) : Target(rhs.Target)
	{}

	// Move constructor.
	TTargetWrapper(TTargetWrapper&& rhs) : Target(std::move(rhs.Target))
	{}

	bool IsEmpty() const
	{
		return Target == nullptr;
	}

	// Should these two be the same?
	const IShip* operator*()
	{
		return Target;
	}

	const IShip* const operator*() const
	{
		return Target;
	}

	TTargetWrapper& operator=(const IShip* ship)
	{
		Target = ship;
		return *this;
	}

	// Copy assignment operator.
	TTargetWrapper& operator=(const TTargetWrapper& rhs)
	{
		Target = rhs.Target;
		return *this;
	}

	// Move assignment operator.
	TTargetWrapper& operator=(TTargetWrapper&& rhs)
	{
		if (this != &rhs)
			Target = std::move(rhs.Target);
		
		return *this;
	}

	const IShip* operator->()
	{
		return Target;
	}

	const IShip* const operator->() const
	{
		return Target;
	}

private:
	const IShip* Target;
};

class IPirateBrain
{
public:
	virtual ~IPirateBrain() = default;

	virtual TCoordinates GetDesiredDestination(bool& nedsCorrection) = 0;

protected:
	IPirateBrain() = default;
};

class TSimpleBrain : public IPirateBrain
{
public:
	TSimpleBrain(int maxX, int maxY, TCoordinates& position, 
		TCoordinates& destination, float& velocity, TTargetWrapper& target);

	virtual TCoordinates GetDesiredDestination(bool& needsCorrection) override;

private:
	// TODO: comment.
	TCoordinates HandleDesiredDestination(int modifier = 0);
	// TODO: shouldn't it be called "CanReachTarget" in the current form?
	bool CanReachDesiredDestination(int modifier = 0) const;
	
private:
	int MaxX;
	int MaxY;
	
	// References to TPirate's members.
	float& Velocity;
	TCoordinates& Position;
	TCoordinates& Destination;
	TTargetWrapper& Target;
	
	// TODO: ! Is it legacy?
	TCoordinates LongTermDestination;
};

class TPirate final : public AShip<IShip>
{
public:
	TPirate(TCoordinates position, unsigned int mapWidth, unsigned int mapHeight);

	// Can't copy unique_ptr.
	TPirate(const TPirate& rhs) = delete;
	TPirate operator=(const TPirate& rhs) = delete;

	// Move constructor.
	TPirate(TPirate&& rhs);

	// Move assignment operator.
	TPirate& operator=(TPirate&& rhs);

	// Default constructor needed by TGame's constructor.
	TPirate() = default;

	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;


	// Methods below are meant to be called by TGame, because it is the only entity that holds
	// a TPirate object:
	
	TCoordinates GetDesiredDestination(bool& needsCorrection) const;

	// Set Velocity as fastestCivilian multiplied by 1.25.
	void ModifyVelocity(float fastestCivilianVelocity);

	void ChangeTarget(const IShip* target);
	const IShip* GetTarget() const;

	void ChangeDestination(const TCoordinates& destination);

	virtual void debug_IntroduceYourself() const override;
	virtual bool debug_IsPirate() const override
	{
		return true;
	}

private:
	// Note that the target can be nullptr (of course not the wrapper itself).
	TTargetWrapper Target;

	// Brain responsible for deciding where to go.
	std::unique_ptr<IPirateBrain> Brain;
};

#endif // _PIRATE_H