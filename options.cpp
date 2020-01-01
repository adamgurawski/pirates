
#include "options.h"
#include "game.h"

#include <iostream>
#include <exception>
#include <string>
#include <vector>

options::TOptions::TOptions(int argc, const char** argv) : ArgCount(argc), Args(argv),
	ValidParams(true)
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
		"-t (int) time_of_simulation\n" <<
		"-m (int int) map_width map_height\n" <<
		"-s (string float int) ship_name ship_velocity time_to_generation.\n" <<
		"Note that all of these switches are obligatory. There can't be more than one -t and -m " <<
		"and there can be multiple -s (each representing one ship)." << std::endl;
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
		if (ArgCount == 2 && !strcmp(Args[1], "-h"))
			return false;
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
				HandleMapDimensions(++i);
			}
			else if (parameter == "-t")
			{ // Simulation time.
				HandleSimulationTime(++i);
			}
			else
			{ 
				throw std::invalid_argument("Dummy");
			}
		}
	}
	catch (std::invalid_argument)
	{ // std::stoi failed or wrong order of parameters (f.e. 4 params after -s switch)
		return InvalidFormatOfInput();
	}
	catch (std::out_of_range)
	{ // (legacy?) std::vector::at failed.
		return InvalidFormatOfInput();
	}
	catch (std::logic_error logic)
	{ // time/speed negative
		std::cerr << logic.what() << std::endl;
		return false;
	}

	return true;
}

bool options::TOptions::IsNotSwitch(int idx) const
{
	std::string arg = Args[idx];

	if (arg == "-s")
		return false;
	else if (arg == "-m")
		return false;
	else if (arg == "-t")
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
			throw std::logic_error("Velocity value shall be positive.");
		else if (time < 0)
			throw std::logic_error("Time value cannot be negative.");

		TShipInfo ship({ name, velocity, time });
		Ships.insert(ship);
	}
	else
	{
		throw std::invalid_argument("Dummy");
	}
}

void options::TOptions::HandleMapDimensions(int& widthIdx)
{
	int width = std::stoi(Args[widthIdx++]);
	int height = std::stoi(Args[widthIdx++]);

	if (width < 0 || height < 0)
		throw std::logic_error("Map width/height value shall be positive.");

	MapWidth = width;
	MapHeight = height;
}

void options::TOptions::HandleSimulationTime(int& timeIdx)
{
	int time = std::stoi(Args[timeIdx++]);

	if (time < 0)
		throw std::logic_error("Time value cannot be negative.");

	SimulationTime = time;
}