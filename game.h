// File game.h, contains TGame responsible for managing the game.

#ifndef _GAME_H
#define _GAME_H

#include <string>

namespace game
{
	struct TShip
	{
		std::string Name;
		int Speed;
		int TimeToGeneration;
	};

	class TGame
	{
	public:
		bool Run();
	};
}
#endif // _GAME_H