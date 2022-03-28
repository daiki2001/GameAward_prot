#include "Easing.h"
#include "Vector3.h"

Easing::Easing() :
	maxTime(2.0f),
	timerate(0.0f),
	addTime(0.1f),
	isMove(false)
{
}

Easing::~Easing()
{
}

float Easing::easeout(const float start, const float end, const float time)
{
	float position = time * (2 - time);
	return start * (1.0f - position) + end * position;
}
