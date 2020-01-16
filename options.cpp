
#include "options.h"
#include "game.h"

#include <iostream>
#include <exception>
#include <string>
#include <vector>

options::TOptions::TOptions(int argc, const char** argv) : ArgCount(argc), Args(argv),
	ValidParams(true), Graphical(false), Verbose(false)
{
	if (!ValidateInput())
	{ 
		ValidParams = false;
		DisplayHelp();
	}
}

bool options::TOptions::InvalidFormatOfInput() const
{
	std::cerr << "Error: Invalid format of input." << std::endl;
	return false;
}

bool options::TOptions::InvalidNumberOfArguments() const
{
	std::cerr << "Error: Invalid number of arguments." << std::endl;
	return false;
}

void options::TOptions::DisplayHelp() const
{ 
	std::cout << "Help for Pirates:\n" <<
		"In order to run simulation, pass these arguments in random order:\n" <<
		"-t followed by (int) time_of_simulation\n" <<
		"-m followed by (int) map_width (int) map_height\n" <<
		"-s followed by (string) ship_name (float) ship_velocity (int) time_to_generation.\n" <<
		"Note that all of mentioned arguments are obligatory. There can't be more than one -t and -m " <<
		"and there can be multiple -s (each representing one ship).\n" << 
		"Optional switches:\n" <<
		"-g <graphical> display map on every turn\n" <<
		"-v <verbose> print every event happening in the game (f.e. ship changed position)" << 
		", by default only ship's generation, leaving the map or being attacked is printed." << std::endl;
}

bool options::TOptions::PreValidate() const
{
	bool timePresent = false, mapPresent = false, shipPresent = false;

	for (int i = 1; i < ArgCount; ++i)
	{
		if (IsNotSwitch(i))
			continue;
		else
		{
			const std::string switchArg = Args[i];

			if (switchArg == "-m")
			{
				if (!mapPresent)
					mapPresent = true;
				else
				{ // More than one -m, error.
					return false;
				}
			}
			else if (switchArg == "-t")
			{
				if (!timePresent)
					timePresent = true;
				else
				{ // More than one -t, error.
					return false;
				}
			}
			else if (switchArg == "-s")
				shipPresent = true;

		} // else
	} // for

	return shipPresent && timePresent && mapPresent;
}

bool options::TOptions::ValidateInput()
{
	if (ArgCount < RequiredArgs)
		if (ArgCount == 1 || ArgCount == 2 && !strcmp(Args[1], "-h"))
		{ // No args or only -h passed. Display help without errors.
			return false;
		}
		else
			return InvalidNumberOfArguments();

	if (!PreValidate())
		return InvalidFormatOfInput();

	try
	{
		for (int i = 1; i < ArgCount;)
		{
			const std::string parameter = Args[i];

			if (parameter == "-h")
			{
				return false;
			}
			else if (parameter == "-s")
			{ // Ship info.
				HandleShip(++i);
			}
			else if (parameter == "-m")
			{ // Map dimenions.
				SetMapDimensions(++i);
			}
			else if (parameter == "-t")
			{ // Simulation time.
				SetSimulationTime(++i);
			}
			else if (parameter == "-v")
			{
				++i;
				Verbose = true;
			}
			else if (parameter == "-g")
			{
				++i;
				Graphical = true;
			}
			else
			{ 
				throw std::invalid_argument("Dummy");
			}
		}
	}
	catch (const std::invalid_argument&)
	{ // std::stoi failed or wrong order of parameters (f.e. 4 non-switch params
		// after -s switch)
		return InvalidFormatOfInput();
	}
	catch (const std::out_of_range&)
	{ // (legacy?) std::vector::at failed.
		return InvalidFormatOfInput();
	}
	catch (const std::logic_error& logic)
	{ // Time/speed negative.
		std::cerr << logic.what() << std::endl;
		return false;
	}

	return true;
}

bool options::TOptions::IsNotSwitch(int idx) const
{
	std::string arg = Args[idx];

	if (arg == "-s" || arg == "-m" || arg == "-t" || arg == "-v" || arg == "-g" ||
		arg == "-h")
		return false;
	else
		return true;
}

void options::TOptions::HandleShip(int& idx)
{	
	int timeIdx = idx + 2;

	if (!(timeIdx >= ArgCount))
	{
		std::string name = Args[idx++];
		float velocity = std::stof(Args[idx++]);
		int time = std::stoi(Args[idx++]);

		if (velocity <= 0)
			throw std::logic_error("Error: Velocity value must be positive.");
		else if (time < 0)
			throw std::logic_error("Error: Time value cannot be negative.");

		TShipInfo ship({ name, velocity, time });
		Ships.insert(ship);
	}
	else
	{
		throw std::invalid_argument("Dummy");
	}
}

void options::TOptions::SetMapDimensions(int& widthIdx)
{
	int width = std::stoi(Args[widthIdx++]);
	int height = std::stoi(Args[widthIdx++]);

	if (width <= 0 || height <= 0)
		throw std::logic_error("Error: Map width/height value must be positive.");

	MapWidth = width;
	MapHeight = height;
}

void options::TOptions::SetSimulationTime(int& timeIdx)
{
	int time = std::stoi(Args[timeIdx++]);

	if (time <= 0)
		throw std::logic_error("Error: Simulation time value must be positive.");

	SimulationTime = time;
}