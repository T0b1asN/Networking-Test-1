#pragma once
#include "SFML.h"
#include "LogUtil.h"
#include "curr.h"

class CheckBox
{
private:
	bool checked;
	sf::RenderWindow* win;

	sf::Vector2f pos;

	float size;	
	sf::RectangleShape mainBody;
	sf::RectangleShape frontBody;

	sf::Color backColor;
	sf::Color frontColor;

	sf::Text infoText;

public:
	CheckBox(sf::Vector2f _pos, float _size, bool initialState = false, sf::RenderWindow* _win = &cr::currWin());
	~CheckBox();

	void setBackColor(sf::Color c) { 
		backColor = c; 	
		mainBody.setFillColor(backColor);
	}
	void setFrontColor(sf::Color c) { 
		frontColor = c; 
		frontBody.setFillColor(sf::Color(frontColor.r, frontColor.g, frontColor.b, checked * 255));
	}

	void setInfo(std::string newInfo);

	void display();

	bool isChecked() { return checked; }

	//returns if value is changed
	bool CheckClick();

	//TODO: Getters
	//TODO: setPos -> should also change position of all Drawables
};

