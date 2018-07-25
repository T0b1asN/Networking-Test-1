#include "InputHandler.h"
std::map<std::string, std::function<void(int, int)>> lMouseCallbacks;
std::vector<std::string> lMCbDelete;

std::map<std::string, std::function<void()>> closeCallbacks;
std::vector<std::string> cCbDelete;

std::map<std::string, std::function<void(sf::Event::TextEvent)>> textEnteredCallbacks;
std::vector<std::string> tECbDelete;

sf::RenderWindow* focus;


void input::addLeftMouseCallback(mouseCallback cb, std::string name)
{
	lMouseCallbacks[name] = cb;
	own_log::pushMsgToCommandIfDebug("Added a callback; ID: " + name);
}

void input::deleteLMouseCallback(std::string name)
{
	lMCbDelete.push_back(name);
}

std::map<std::string, input::mouseCallback> input::getLMouseCallbacks()
{
	return lMouseCallbacks;
}

input::mouseCallback input::getLMouseCallback(std::string name)
{
	return lMouseCallbacks[name];
}

void input::addCloseCallback(closeCallback cb, std::string name)
{
	closeCallbacks[name] = cb;
	own_log::pushMsgToCommandIfDebug("Added a callback; ID: " + name);
}

void input::deleteCloseCallback(std::string name)
{
	cCbDelete.push_back(name);
}

std::map<std::string, input::closeCallback> input::getCloseCallbacks()
{
	return closeCallbacks;
}

input::closeCallback input::getCloseCallback(std::string name)
{
	return closeCallbacks[name];
}

void input::addTextEnteredCallback(textEnteredCallback cb, std::string name)
{
	textEnteredCallbacks[name] = cb;
	own_log::pushMsgToCommandIfDebug("Added a callback; ID: " + name);
}

void input::deleteTextEnteredCallback(std::string name)
{
	tECbDelete.push_back(name);
}

std::map<std::string, input::textEnteredCallback> input::getTextEnteredCallbacks()
{
	return textEnteredCallbacks;
}

input::textEnteredCallback input::getTextEnteredCallback(std::string name)
{
	return textEnteredCallbacks[name];
}

void input::setFocus(sf::RenderWindow * newFocus)
{
	focus = newFocus;
}

void input::cleanCallbacks()
{
	int count = 0;
	while (lMCbDelete.size() > 0)
	{
		std::string s = lMCbDelete.back();
		lMouseCallbacks.erase(s);
		lMCbDelete.pop_back();
		count++;
	}
	while (cCbDelete.size() > 0)
	{
		std::string s = cCbDelete.back();
		closeCallbacks.erase(s);
		cCbDelete.pop_back();
		count++;
	}
	while (tECbDelete.size() > 0)
	{
		std::string s = tECbDelete.back();
		textEnteredCallbacks.erase(s);
		tECbDelete.pop_back();
		count++;
	}

	lMCbDelete.shrink_to_fit();
	cCbDelete.shrink_to_fit();
	tECbDelete.shrink_to_fit();

	if(count > 0)
		own_log::pushMsgToCommandIfDebug("Deleted " + std::to_string(count) + " callbacks!");
	int ncount = lMouseCallbacks.size() + closeCallbacks.size() + textEnteredCallbacks.size();
	if(count > 0)
		own_log::pushMsgToCommandIfDebug("There are " + std::to_string(count) + " callbacks left");
}

void input::handleInput()
{
	cleanCallbacks();
	sf::Event evnt;
	while (focus->pollEvent(evnt))
	{
		switch (evnt.type)
		{
		case sf::Event::Closed:
			if (closeCallbacks.size() <= 0)
				return;
			for (auto func : closeCallbacks)
			{
				func.second();
			}
			break;
		case sf::Event::TextEntered:
			if (textEnteredCallbacks.size() <= 0)
				return;
			for (auto func : textEnteredCallbacks)
			{
				func.second(evnt.text);
			}
			break;
		case sf::Event::MouseButtonPressed:
			if (evnt.mouseButton.button == sf::Mouse::Left)
			{
				if (lMouseCallbacks.size() <= 0)
					return;
				sf::Vector2i pos = sf::Mouse::getPosition(*focus);
				std::cout << lMouseCallbacks.size();
				for(auto func : lMouseCallbacks)
				{
					func.second(pos.x, pos.y);
				}
			}
			break;
		}
	}
}
