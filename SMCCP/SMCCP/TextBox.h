#pragma once

#include "SFML.h"
#include "curr.h"
#include <iostream>

class TextBox
{
private:
	sf::RenderWindow* win;

	sf::Vector2f pos;
	sf::Vector2f size;

	unsigned int fontSize;

	sf::String standardText;
	sf::String actualText;

	sf::Text text;
	sf::RectangleShape background;

	bool selected = false;

	bool deleteStdMsg = true;
	bool canReturnStdText = false;

	unsigned int maxChars = 100000000U;
public:
	TextBox(sf::Vector2f pPos, sf::Vector2f pSize,  
		std::string pStandardText = "Input Text...",
		sf::Color pBackColor = sf::Color::Black, 
		sf::Color pTextColor = sf::Color::White,
		sf::RenderWindow* winPtr = &cr::currWin());
	~TextBox();

	void display();

	void SelectOrUnselect();
	void SelectOrUnselect(int x, int y);
	void Update(sf::String add);

	sf::String Text();

	void SetNormal();

	sf::String getStdText() { return standardText; }

	void Select();
	void Unselect();

	bool changed() { return (text.getString() != standardText || text.getString() != ""); }
	void set_deleteStdMsg(bool newState) { deleteStdMsg = newState; }
	void set_canReturnStdText(bool newState) { canReturnStdText = newState; }

	void set_maxChars(int newVal) { maxChars = newVal; }

	void setWinPtr(sf::RenderWindow* ptr) { win = ptr; }

	void setText(std::string _text) { actualText = _text; text.setString(_text); }
};
