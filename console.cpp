
#include "console.h"
#include "game.h"

#include <iostream>
#include <exception>
#include <string>
#include <vector>

/* Required arguments:
	 (default) program name
	 -ships (one minimum)
	 ship name
	 ship speed
	 ship time 
	 -map_size
	 length
	 width
	 -time
	 time in seconds
	*/
/*

const int requiredArgs = 10;

// Errors used solely by console.cpp functions.
namespace error
{
	bool InvalidFormatOfInput()
	{
		std::cerr << "Error: Invalid format of input." << std::endl;
		return false;
	}

	bool InvalidNumberOfArguments()
	{
		std::cerr << "Error: Invalid number of arguments." << std::endl;
		return false;
	}
}

namespace console
{
	typedef std::pair<std::string, int> TPair;

	bool HandleShipList(size_t& idx, std::vector<std::string>& input, std::vector<game::TShip>& ships)
	{
		size_t argNo = input.size();

		for (size_t& i = idx; i < argNo; ++i)
		{
			std::string name = input.at(i + 1);
			int speed = std::stoi(input.at(i + 2));
			int time = std::stoi(input.at(i + 3));
			i += 3;
			game::TShip ship({ name, speed, time });
			ships.push_back(ship);
		}
		return true;
	}

	bool ValidateInput(int argc, const char** argv, std::map<std::string, int>& params,
		std::vector<game::TShip>& ships)
	{ 
		if (argc < requiredArgs)
			return error::InvalidNumberOfArguments();
			
		std::vector<std::string> input;

		// Store argv as std::vector "input" to simplify operations.
		for (int i = 1; i < argc; i++)
		{
			input.push_back(argv[i]);
			if (!strcmp(argv[i], "-h"))
				return false;
		}

		try
		{
			for (size_t i = 0; i < input.size(); i++)
			{
				std::string& parameter = input.at(i);

				if (parameter == "-ships")
				{
					HandleShipList(i, input, ships);
				}
				else if (parameter == "-map_size")
				{
					int length = std::stoi(input.at(i + 1));
					int width = std::stoi(input.at(i + 2));
					i += 2;
					params.insert(TPair("length", length));
					params.insert(TPair("width", width));
				}
				else if (parameter == "-time")
				{
					int time = std::stoi(input.at(i + 1));
					i++;
					params.insert(TPair("time", time));
				}
			}
		} 
		catch (std::invalid_argument invalid)
		{ // std::stoi failed.
			return error::InvalidFormatOfInput();
		}
		catch (std::out_of_range oor)
		{ // std::vector::at failed.
			return error::InvalidFormatOfInput();
		}

		return true;
	}

	void DisplayHelp()
	{ // TODO: implement.
		std::cout << "Help called, to be implemented." << std::endl;
	}
}
*/