#pragma once
#include "SFML.h"
#include "LogUtil.h"

namespace cr
{
	sf::RenderWindow& currWin();
	unsigned int winWidth();
	unsigned int winHeight();
	sf::Font& currFont();

}
