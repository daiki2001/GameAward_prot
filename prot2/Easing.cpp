#include "Easing.h"

Easing::Easing() :
	maxtime(2.0f),
	timerate(0.0f),
	addtime(0.1f),
	ismove(false)
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
