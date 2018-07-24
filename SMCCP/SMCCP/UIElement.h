#pragma once

class UIElement
{
protected:
	void addToList();
	void deleteFromList();

public:
	virtual void update() = 0;

};
#include "curr.h"

inline void UIElement::addToList() { cr::addUIElement(this); }
inline void UIElement::deleteFromList() { cr::deleteUIElement(this); }
