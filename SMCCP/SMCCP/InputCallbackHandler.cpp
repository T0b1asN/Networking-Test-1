#include "InputCallbackHandler.h"

InputCallbackHandler::InputCallbackHandler(std::string callback_id) :
	callback_id(callback_id)
{

}

InputCallbackHandler::~InputCallbackHandler()
{

}

void InputCallbackHandler::initCallbacks()
{
	input::addLeftMouseCallback(lMCb, callback_id);
	input::addCloseCallback(cCb, callback_id);
	input::addTextEnteredCallback(tECb, callback_id);
	input::addLostFocusCallback(lFCb, callback_id);
}

void InputCallbackHandler::cleanCallbacks()
{
	input::deleteCloseCallback(callback_id);
	input::deleteLMouseCallback(callback_id);
	input::deleteTextEnteredCallback(callback_id);
	input::deleteLostFocusCallback(callback_id);
}