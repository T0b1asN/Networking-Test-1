#pragma once
#include "TextBox.h"
#include "CheckBox.h"
#include "OwnButton.h"

#include "InputHandler.h"

#include <Windows.h>

#include "InputCallbackHandler.h"

class NamePrompt : public InputCallbackHandler
{
public:
	enum class Result
	{
		Default = -1,
		HasName = 0,
		Close = 1,
		Unexpected = 2,
	};

private:
	TextBox nameBox;
	std::string name;

	OwnButton okButton;

	sf::RenderWindow prompt;

	int returnVal = -1;
	Result result = Result::Default;

	bool loseFocus;

	static const std::string okButton_id;

	const std::string callback_id_base;
	std::string callback_id_own;

public:
	NamePrompt(bool dontLoseFocus = true);
	~NamePrompt();


	//returns errorCode
	//	0 = OK
	//	1 = Closed
	//	2 = reached end of function (bad)
	int run_int();

	Result run();

	std::string getName() { return name; }


	void display();
	void nextWindow();

	// Callback stuff
	virtual void LeftMCallback(int x, int y);
	virtual void CloseCallback();
	virtual void TextEnteredCallback(sf::Event::TextEvent text);

	virtual void LostFocusCallback();

};

