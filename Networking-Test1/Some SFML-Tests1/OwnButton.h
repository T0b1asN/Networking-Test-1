#pragma once
#include "SFML.h"
#include "curr.h"
#include <iostream>
#include <string>
#include "Defines.h"

class OwnButton
{
private:
	sf::Vector2f pos;
	sf::Vector2f size;

	unsigned int charSize;

	sf::FloatRect rect;
	sf::Text textField;
	sf::RectangleShape field;

	std::string text;

	bool checkClick();

	sf::Vector2f offset;

	void OnClick();

public:
	OwnButton(std::string pText, sf::Vector2f pSize, sf::Vector2f pPos, 
		sf::Color backColor = sf::Color::Black, sf::Color textColor = sf::Color::White, unsigned int pCharSize = 0);
	~OwnButton();

	void Update(bool click);
	void display();

	void SetPos(sf::Vector2f newPos);
	sf::Vector2f GetPos() { return pos; }

	void SetSize(sf::Vector2f newSize);
	sf::Vector2f GetSize() { return size; }

	sf::FloatRect GetRect() { return rect; }
	sf::FloatRect& Rect() { return rect; }

	void SetText(std::string newText);
	std::string GetText() { return text; }

	void SetOrigin(sf::Vector2f newOffset);

	//TODO: update on valuechange
};

