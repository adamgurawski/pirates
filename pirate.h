#ifndef _PIRATE_H
#define _PIRATE_H

#include "ship.h"

/* I had to create this wrapper because TPirate::Target can be nullptr and TSimpleBrain
	 holds a reference to Target. */
class TTargetWrapper
{
public:
	// Do not delete Target, wrapper does not own this resource.
	~TTargetWrapper() = default;

	// Constructors.
	TTargetWrapper() { Target = nullptr; }
	TTargetWrapper(const IShip* target) { Target = target; }
	TTargetWrapper(const TTargetWrapper& rhs) : Target(rhs.Target) {}
	TTargetWrapper(TTargetWrapper&& rhs) : Target(std::move(rhs.Target)) {}

	// Operators.
	const IShip* operator*() { return Target;	}
	const IShip* const operator*() const { return Target; }
	const IShip* operator->() { return Target; }
	const IShip* const operator->() const { return Target; }

	TTargetWrapper& operator=(const IShip* ship);
	TTargetWrapper& operator=(const TTargetWrapper& rhs);
	TTargetWrapper& operator=(TTargetWrapper&& rhs);

	bool IsEmpty() const { return Target == nullptr; }

private:
	const IShip* Target;
};

class IPirateBrain
{
public:
	virtual ~IPirateBrain() = default;
	virtual TCoordinates GetDesiredDestination(bool& nedsCorrection) = 0;
	virtual void SetMapBorders(unsigned int maxX, unsigned int maxY) = 0;

protected:
	IPirateBrain() = default;
};

class TSimpleBrain : public IPirateBrain
{
public:
	TSimpleBrain(TCoordinates& position, TCoordinates& destination,  float& velocity, 
		TTargetWrapper& target);

	// Move assignment operator.
	TSimpleBrain& operator=(TSimpleBrain&& rhs);

	virtual TCoordinates GetDesiredDestination(bool& needsCorrection) override;
	
	virtual void SetMapBorders(unsigned int maxX, unsigned int maxY) override;

private:
	// TODO: comment.
	TCoordinates HandleDesiredDestination(int modifier = 0);
	// TODO: shouldn't it be called "CanReachTarget" in the current form?
	bool CanReachDesiredDestination(int modifier = 0) const;
	
private:
	unsigned int MaxX;
	unsigned int MaxY;
	
	// References to TPirate's members.
	float& Velocity;
	TCoordinates& Position;
	TCoordinates& Destination;
	TTargetWrapper& Target;
	
	// TODO: ! is it legacy?
	TCoordinates LongTermDestination;
};

class TPirate final : public AShip<IShip>
{
public:
	// Default constructor needed by TGame's constructor.
	TPirate() = default;
	// Do not call "delete" on Target, pirate does not own this pointer.
	~TPirate() = default;

	TPirate(TCoordinates position);
	// Move constructor.
	TPirate(TPirate&& rhs);
	// Move assignment operator.
	TPirate& operator=(TPirate&& rhs);

	// Methods below are meant to be called by TGame, because it is the only entity that holds
	// a TPirate object.
	

	TCoordinates GetDesiredDestination(bool& needsCorrection) const;
	// Set Velocity as fastestCivilian multiplied by 1.25.
	void ModifyVelocity(float fastestCivilianVelocity);

	void ChangeTarget(const IShip* target);

	const IShip* GetTarget() const;

	void ChangeDestination(const TCoordinates& destination);
	// Pass map borders to Brain.
	void SetMapBorders(unsigned int maxX, unsigned int maxY);

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