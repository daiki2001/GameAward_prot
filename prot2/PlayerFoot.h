#pragma once
#include "PlayerBody.h"
#include "Vector3.h"

class Foot
{
public:
	Vector3 FootCenterPosition;
	Easing ease;

	bool FootIsAction = false;
	bool IsFootUp = false;

	//‰æ‘œƒnƒ“ƒhƒ‹
	int Foothandle;

	void Init();
	void Set();
	void Update(Vector3& FaceCenterPos);
	void Draw(bool isleft, bool isright);
};