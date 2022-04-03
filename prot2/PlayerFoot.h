#pragma once
#include "PlayerBody.h"
#include "Vector3.h"

class Foot
{
public:
	Vector3 FootLeftUpPosition;
	Easing ease;

	bool FootIsAction = false;
	bool IsFootUp = false;

	//画像ハンドル
	int Foothandle;

	void Init();
	void Set();
	void Update(Vector3& FaceCenterPos, bool IsDownBody, int BodyDis);
	void Draw(bool isleft, bool isright, int offsetX, int offsetY);
};