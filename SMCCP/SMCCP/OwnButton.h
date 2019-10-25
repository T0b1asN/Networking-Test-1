#pragma once
#include "SFML.h"
#include "curr.h"
#include <iostream>
#include <string>
#include "Defines.h"
#include "UIElement.h"
#include "InputHandler.h"

//TODO support list of callbacks

class OwnButton: public UIElement
{
private:
	sf::RenderWindow* win;

	sf::Vector2f pos;
	sf::Vector2f size;

	unsigned int charSize;

	sf::FloatRect rect;
	sf::Text textField;
	sf::RectangleShape field;

	std::string text;

	bool checkClick();

	sf::Vector2f offset;

	std::string callbackID;

public:
	OwnButton(std::string callbackID, sf::String text, sf::Vector2f pSize, sf::Vector2f pPos, 
		sf::Color backColor = sf::Color::Black, sf::Color textColor = sf::Color::White, sf::RenderWindow* winPtr = &cr::currWin(), unsigned int pCharSize = 25U);
	~OwnButton();

	//void Update(bool click);
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

	bool validClick(bool click);

	void setWinPtr(sf::RenderWindow* ptr) { win = ptr; }

	void setCharSize(unsigned int newSize);

	virtual void update();
	virtual void cleanup();

#pragma region Callbacks
private:
	void LeftMCallback(int x, int y);
	input::mouseCallback lMCB = 
		std::bind(
			&OwnButton::LeftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	void initCallbacks();

public:
	typedef std::function<void(std::string)> buttonCallback;

	void setOnClickCallback(buttonCallback newCallback);

private:
	buttonCallback callback;
#pragma endregion

};
