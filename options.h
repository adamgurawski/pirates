// File options.h, contains functions used for console-input validation.

#ifndef _OPTIONS_H
#define _OPTIONS_H

#include <string>
#include <utility>
#include <vector>

namespace options
{
	struct TShipInfo
	{
		std::string Name;
		float Velocity;
		int TimeToGeneration;
	};

/*	Required arguments :
	(default) program name
		- s (ship list)
		ship name
		ship speed
		ship time
		-m (map size)
		length
		width
		-t (time)
		time in seconds */
	
class TOptions
{
public:
	TOptions(int argc, const char** argv);

	unsigned int GetSimulationTime() const
	{
		return SimulationTime;
	}

	unsigned int GetMapWidth() const
	{
		return MapWidth;
	}

	unsigned int GetMapHeight() const
	{
		return MapHeight;
	}

	std::vector<TShipInfo>&& GetShipInfoVector()
	{
		return std::move(Ships);
	}

	bool IsGood() const
	{
		return Good;
	}

private:
	// Input validation:

	// Checks if exactly one -t/-m is present and minimum of one -s.
	// Verifies that one (-t), two (-m), or three (-s) next arguments
	// are not switches.
	bool PreValidate() const;

	/* Validates parameters. When wrong parameters (wrong values, wrong order) were passed or
		"-h" present, returns false. Otherwise returns true. */
	bool ValidateInput();

	bool IsNotSwitch(int idx) const;

	// Takes first ship's name's idx as parameter, add its info to the vector.
	void HandleShip(int& idx);

	void HandleMapDimensions(int& widthIdx);

	void HandleSimulationTime(int& timeIdx);

	// Errors.
	bool InvalidFormatOfInput() const;
	bool InvalidNumberOfArguments() const;

	void DisplayHelp() const;

private:
	int ArgCount;
	const char** Args;
	bool Good;

	// Minimal number of args.
	const int RequiredArgs = 10;

	// Game options.
	unsigned int SimulationTime;
	unsigned int MapWidth;
	unsigned int MapHeight;

	// Based on data stored in this vector, the ships will be created by TGame.
	std::vector<TShipInfo> Ships;
};

} // namespace options
#endif // _OPTIONS_H
