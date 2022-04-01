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
	FootHandle = LoadGraph("Resources/Foot.png");
}

void PlayerFoot::Set()
{
	ease.addTime = 0.1f;
	ease.maxTime = 1.2f;
	ease.timerate = 0.0f;
	FootIsAction = true;
}

void PlayerFoot::Update(Vector3& FaceCenterPos)
{
	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 20.0f;
		ease.timerate = min(ease.addTime / ease.maxTime, 1.0f);

		FootCenterPosition = { FaceCenterPos.x,ease.easeOut(FaceCenterPos.y,FaceCenterPos.y - 10,ease.timerate),0.0f };
		if (ease.timerate >= 1.0f)
		{
			FootIsAction = false;
		}
	}
	else
	{
		FootCenterPosition = FaceCenterPos;
	}
}

void PlayerFoot::Draw(int offsetX, int offsetY)
{
	DrawExtendGraph(static_cast<int>(FootCenterPosition.x) - 30 + offsetX, static_cast<int>(FootCenterPosition.y) - 30 + offsetY,
		static_cast<int>(FootCenterPosition.x) + 30 + offsetX, static_cast<int>(FootCenterPosition.y) + 30 + offsetY, FootHandle, true);
}
