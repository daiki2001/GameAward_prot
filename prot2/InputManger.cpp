#include "InputManger.h"
#include <DxLib.h>

bool InputManger::Escape()
{
	return Input::isKey(KEY_INPUT_ESCAPE);
}

bool InputManger::EscapeTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_ESCAPE);
}

bool InputManger::EscapeReturn()
{
	return Input::isKeyReturn(KEY_INPUT_ESCAPE);
}

bool InputManger::Up()
{
	return Input::isKey(KEY_INPUT_W) || Input::isKey(KEY_INPUT_UP);
}

bool InputManger::UpTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_W) || Input::isKeyTrigger(KEY_INPUT_UP);
}

bool InputManger::UpReturn()
{
	return Input::isKeyReturn(KEY_INPUT_W) || Input::isKeyReturn(KEY_INPUT_UP);
}

bool InputManger::Down()
{
	return Input::isKey(KEY_INPUT_S) || Input::isKey(KEY_INPUT_DOWN);
}

bool InputManger::DownTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_S) || Input::isKeyTrigger(KEY_INPUT_DOWN);
}

bool InputManger::DownReturn()
{
	return Input::isKeyReturn(KEY_INPUT_S) || Input::isKeyReturn(KEY_INPUT_DOWN);
}

bool InputManger::Left()
{
	return Input::isKey(KEY_INPUT_A) || Input::isKey(KEY_INPUT_LEFT);
}

bool InputManger::LeftTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_A) || Input::isKeyTrigger(KEY_INPUT_LEFT);
}

bool InputManger::LeftReturn()
{
	return Input::isKeyReturn(KEY_INPUT_A) || Input::isKeyReturn(KEY_INPUT_LEFT);
}

bool InputManger::Right()
{
	return Input::isKey(KEY_INPUT_D) || Input::isKey(KEY_INPUT_RIGHT);
}

bool InputManger::RightTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_D) || Input::isKeyTrigger(KEY_INPUT_RIGHT);
}

bool InputManger::RightReturn()
{
	return Input::isKeyReturn(KEY_INPUT_D) || Input::isKeyReturn(KEY_INPUT_RIGHT);
}

bool InputManger::Reset()
{
	return Input::isKey(KEY_INPUT_R);
}

bool InputManger::ResetTrigger()
{
	return Input::isKeyTrigger(KEY_INPUT_R);
}

bool InputManger::ResetReturn()
{
	return Input::isKeyReturn(KEY_INPUT_R);
}

bool InputManger::Act1()
{
	return Input::isKey(KEY_INPUT_SPACE);
}

bool InputManger::Act1Trigger()
{
	return Input::isKeyTrigger(KEY_INPUT_SPACE);
}

bool InputManger::Act1Return()
{
	return Input::isKeyReturn(KEY_INPUT_SPACE);
}
