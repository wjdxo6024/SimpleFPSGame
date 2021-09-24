#include "ButtonEvent.h"

bool ButtonEvent::InitButtonEvent(ButtonEventDefine Event)
{
	HasEvent = Event;

	return true;
}

ButtonEventDefine ButtonEvent::GetEvent()
{
	return HasEvent;
}