#include "InputHandler.h"
std::map<std::string, input::mouseCallback> lMouseCallbacks;
std::vector<std::string> lMCbDelete;

std::map<std::string, input::closeCallback> closeCallbacks;
std::vector<std::string> cCbDelete;

std::map<std::string, input::textEnteredCallback> textEnteredCallbacks;
std::vector<std::string> tECbDelete;

std::map<std::string, input::lostFocusCallback> lostFocusCallbacks;
std::vector<std::string> lFCbDelete;

sf::RenderWindow* focus;


void input::addLeftMouseCallback(mouseCallback cb, id name)
{
	lMouseCallbacks[name] = cb;
	debug::log("Added a left mouse callback; ID: " + name);
}

void input::deleteLMouseCallback(id name)
{
	lMCbDelete.push_back(name);
}

std::map<std::string, input::mouseCallback> input::getLMouseCallbacks()
{
	return lMouseCallbacks;
}

input::mouseCallback input::getLMouseCallback(id name)
{
	return lMouseCallbacks[name];
}



void input::addCloseCallback(closeCallback cb, id name)
{
	closeCallbacks[name] = cb;
	debug::log("Added a close callback; ID: " + name);
}

void input::deleteCloseCallback(id name)
{
	cCbDelete.push_back(name);
}

std::map<std::string, input::closeCallback> input::getCloseCallbacks()
{
	return closeCallbacks;
}

input::closeCallback input::getCloseCallback(id name)
{
	return closeCallbacks[name];
}



void input::addTextEnteredCallback(textEnteredCallback cb, id name)
{
	textEnteredCallbacks[name] = cb;
	debug::log("Added a text entered callback; ID: " + name);
}

void input::deleteTextEnteredCallback(id name)
{
	tECbDelete.push_back(name);
}

std::map<std::string, input::textEnteredCallback> input::getTextEnteredCallbacks()
{
	return textEnteredCallbacks;
}

input::textEnteredCallback input::getTextEnteredCallback(id name)
{
	return textEnteredCallbacks[name];
}



void input::addLostFocusCallback(lostFocusCallback cb, id name)
{
	lostFocusCallbacks[name] = cb;
	debug::log("Added a lost focus callback; ID: " + name);
}

void input::deleteLostFocusCallback(id name)
{
	lFCbDelete.push_back(name);
}

std::map<std::string, input::lostFocusCallback> input::getLostFocusCallbacks()
{
	return lostFocusCallbacks;
}

input::lostFocusCallback input::getLostFocusCallback(id name)
{
	return lostFocusCallbacks[name];
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
	while (lFCbDelete.size() > 0)
	{
		std::string s = lFCbDelete.back();
		lostFocusCallbacks.erase(s);
		lFCbDelete.pop_back();
		count++;
	}

	lMCbDelete.shrink_to_fit();
	cCbDelete.shrink_to_fit();
	tECbDelete.shrink_to_fit();
	lMCbDelete.shrink_to_fit();

	if(count > 0)
		debug::log("Deleted " + std::to_string(count) + " callbacks!");
	int ncount =
		lMouseCallbacks.size() + closeCallbacks.size() + textEnteredCallbacks.size() +
		lostFocusCallbacks.size();
	if(count > 0)
		debug::log("There are " + std::to_string(ncount) + " callbacks left");
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
					debug::log(func.first + " --------------- Test");
					func.second(pos.x, pos.y);
				}
			}
			break;
		case sf::Event::LostFocus:
			for (auto func : lostFocusCallbacks)
			{
				func.second();
			}
			break;
		}
	}
}
