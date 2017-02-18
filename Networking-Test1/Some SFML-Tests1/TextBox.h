#pragma once

#include "SFML.h"
#include "curr.h"
#include <iostream>

#include "stringutil.h"

class TextBox
{
private:
	sf::Vector2f pos;
	sf::Vector2f size;

	unsigned int fontSize;

	sf::String standardText;
	sf::String actualText;

	sf::Text text;
	sf::RectangleShape background;

	bool selected = false;
	bool changed = false;
public:
	TextBox(sf::Vector2f pPos, sf::Vector2f pSize, std::string pStandardText = "Input Text...",
		sf::Color pBackColor = sf::Color::Black, 
		sf::Color pTextColor = sf::Color::White);
	~TextBox();

	void display();

	void SelectOrUnselect();
	void Update(sf::String add);

	sf::String Text() { if (changed) return text.getString(); else { return ""; } }

	void SetNormal();

	sf::String getStdText() { return standardText; }
};
