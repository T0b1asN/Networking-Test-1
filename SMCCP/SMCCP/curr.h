#pragma once
#include "SFML.h"
#include "util.h"
//#include "discord_rpc.h"
//#include "UIElement.h"
class UIElement;

namespace cr
{
	sf::RenderWindow& currWin();
	unsigned int winWidth();
	unsigned int winHeight();
	sf::Font& currFont();
	
	void addUIElement(UIElement* element);
	void deleteUIElement(UIElement* element);
	void updateUIElements();
	std::vector<UIElement*> elements();

#ifdef DISCORD_RICH_PRESENCE
	void updateDiscordRPC(std::string details);
#endif
}
