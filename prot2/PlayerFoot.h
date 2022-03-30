#pragma once
#include "Vector3.h"
#include "Easing.h"

class PlayerFoot
{
public: //メンバ関数
	PlayerFoot();
	~PlayerFoot();

	void Init();
	void Set();
	void Update(Vector3& FaceCenterPos);
	void Draw(int offsetX, int offsetY);

public: //メンバ変数
	Vector3 FootCenterPosition;
	Easing ease;

	bool FootIsAction;

	//画像ハンドル
	int FootHandle;
};
