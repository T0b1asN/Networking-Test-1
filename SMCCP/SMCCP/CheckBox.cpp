#include "CheckBox.h"

CheckBox::CheckBox(sf::Vector2f _pos, float _size, bool initialState, sf::RenderWindow* _win)
{
	checked = initialState;
	win = _win;

	pos = _pos;

	backColor = sf::Color::Black;
	frontColor = sf::Color::White;

	size = _size;
	mainBody.setSize(sf::Vector2f(size, size));
	frontBody.setSize(sf::Vector2f(size * 0.7f, size * 0.7f));

	mainBody.setOrigin(mainBody.getSize() / 2.f);
	frontBody.setOrigin(frontBody.getSize() / 2.f);

	mainBody.setPosition(pos);
	frontBody.setPosition(pos);

	mainBody.setFillColor(backColor);
	frontBody.setFillColor(sf::Color(frontColor.r, frontColor.g, frontColor.b, checked * 255));

	//set up infotext
	infoText.setString("");
	infoText.setFont(cr::currFont());
	infoText.setPosition(pos + (sf::Vector2f(size + size / 2.5f, -size) / 2.f));
	infoText.setCharacterSize((unsigned int)size);
}

CheckBox::~CheckBox()
{

}

void CheckBox::setInfo(std::string newInfo)
{
	infoText.setString(newInfo);
}

void CheckBox::display()
{
	win->draw(mainBody);
	win->draw(frontBody);
	win->draw(infoText);
}

bool CheckBox::CheckClick()
{
	if (win->isOpen())
	{
		if (mainBody.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(*win)))
		{
			checked = !checked;
			frontBody.setFillColor(sf::Color(frontColor.r, frontColor.g, frontColor.b, checked * 255));
			return true;
		}
	}
	return false;
}
