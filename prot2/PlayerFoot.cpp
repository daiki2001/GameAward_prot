#include "PlayerFoot.h"
#include <DxLib.h>

PlayerFoot::PlayerFoot() :
	FootCenterPosition{},
	ease{},
	FootIsAction(false),
	FootHandle(-1)
{
	Init();
}

PlayerFoot::~PlayerFoot()
{
}

void PlayerFoot::Init()
{
	FootHandle = LoadGraph("./Resources/playerLegs/playerLegs.png");
}

void PlayerFoot::Set()
{
	ease.addTime = 0.1f;
	ease.maxTime = 1.2f;
	ease.timeRate = 0.0f;
	FootIsAction = true;
	IsFootUp = true;
}

void PlayerFoot::Update(Vector3& FaceCenterPos)
{
	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 15.0f;
		ease.timeRate = min(ease.addTime / ease.maxTime, 1.0f);

		FootCenterPosition = { FaceCenterPos.x,ease.easeOut(FaceCenterPos.y,FaceCenterPos.y - 10,ease.timeRate),0.0f };
		if (ease.timeRate >= 1.0f)
		{
			FootIsAction = false;
		}
	}

	if (IsFootUp == false)
	{
		FootCenterPosition = FaceCenterPos;
	}
}

void PlayerFoot::Draw(int offsetX, int offsetY, bool isleft, bool isright)
{
	if (isleft)
	{
		DrawExtendGraph(
			static_cast<int>(FootCenterPosition.x) - 30 + offsetX, static_cast<int>(FootCenterPosition.y) - 30,
			static_cast<int>(FootCenterPosition.x) + 30 + offsetX, static_cast<int>(FootCenterPosition.y) + 30, FootHandle, true);
	}
	if (isright)
	{
		DrawExtendGraph(
			static_cast<int>(FootCenterPosition.x) + 30 + offsetX, static_cast<int>(FootCenterPosition.y) - 30,
			static_cast<int>(FootCenterPosition.x) - 30 + offsetX, static_cast<int>(FootCenterPosition.y) + 30, FootHandle, true);
	}
}