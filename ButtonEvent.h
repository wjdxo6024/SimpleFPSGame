#pragma once
#include "Event.h"

class ButtonEvent :
	public Event
{
private:
	ButtonEventDefine HasEvent;
public:
	bool InitButtonEvent(ButtonEventDefine Event);
	ButtonEventDefine GetEvent();
};

