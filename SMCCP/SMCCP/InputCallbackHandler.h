#pragma once
#include "InputHandler.h"
#include "util.h"

#include <Windows.h>

class InputCallbackHandler
{
protected:
	InputCallbackHandler(std::string callback_id);
	~InputCallbackHandler();

	void initCallbacks();
	void cleanCallbacks();

public:
	const std::string callback_id;

	virtual void LeftMCallback(int x, int y) = 0;
	// handle for leftMouseCallback
	input::mouseCallback lMCb =
		std::bind(
			&InputCallbackHandler::LeftMCallback, this,
			std::placeholders::_1,
			std::placeholders::_2
		);

	virtual void CloseCallback() = 0;
	// handle for closeCallback
	input::closeCallback cCb =
		std::bind(&InputCallbackHandler::CloseCallback, this);

	virtual void TextEnteredCallback(sf::Event::TextEvent text) = 0;
	// handle for textEnteredCallback
	input::textEnteredCallback tECb =
		std::bind(
			&InputCallbackHandler::TextEnteredCallback, this,
			std::placeholders::_1
		);

	virtual void LostFocusCallback() = 0;
	input::lostFocusCallback lFCb =
		std::bind(&InputCallbackHandler::LostFocusCallback, this);
};
