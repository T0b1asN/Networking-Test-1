#include "TextBox.h"

TextBox::TextBox(sf::Vector2f pPos, sf::Vector2f pSize, std::string pStandardText, sf::Color pBackColor, sf::Color pTextColor)
{
	pos = pPos;
	size = pSize;

	fontSize = (unsigned int)size.y;

	standardText = pStandardText;
	actualText = str_to_wstr(standardText);

	text.setFont(cr::currFont());
	text.setFillColor(pTextColor);
	text.setPosition(pos);
	text.setCharacterSize(fontSize);
	text.setString(actualText);

	background = sf::RectangleShape(size);
	background.setPosition(pos);
	background.setFillColor(pBackColor);
}

TextBox::~TextBox()
{

}

void TextBox::display()
{
	cr::currWin().draw(background);
	cr::currWin().draw(text);
}

void TextBox::SelectOrUnselect()
{
	if (cr::currWin().isOpen())
	{
		if (background.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(cr::currWin())))
		{
			selected = true;
			if (deleteStdMsg)
			{
				if (actualText == str_to_wstr(standardText))
					actualText = L"";
				text.setString(actualText);
			}
		}
		else
		{
			selected = false;
		}
	}
}

void TextBox::Update(sf::String add)
{
	if (add != (char)0)
	{
		
		if (selected)
		{
			if (add == (char)8)
			{
				//actualText = actualText.substr(0, actualText.size() - 1);
				if (actualText.getSize() > 0)
					actualText.erase(actualText.getSize() - 1, 1);
			}
			else if(actualText.getSize() < maxChars)
			{
				actualText += add;
				changed = true;
			}
		}
		text.setString(actualText);
	}
}

void TextBox::SetNormal()
{
	actualText = str_to_wstr(standardText);
	text.setString(actualText);
	changed = false;
}

sf::String TextBox::Text()
{ 
	if (canReturnStdText)
		return text.getString();

	if (changed) 
		return text.getString(); 
	else 
	{
		return ""; 
	} 
}
