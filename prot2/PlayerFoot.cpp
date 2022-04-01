#include"PlayerFoot.h"

void Foot::Init()
{
	Foothandle = LoadGraph("Resources/playerLegs/playerLegs.png");
}

void Foot::Set()
{
	ease.addTime = 0.1f;
	ease.maxTime = 1.2f;
	ease.timeRate = 0.0f;
	FootIsAction = true;
	IsFootUp = true;
}

void Foot::Update(Vector3& FaceCenterPos)
{
	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 15.0f;
		ease.timeRate = min(ease.addTime / ease.maxTime, 1.0f);

		FootCenterPosition = { FaceCenterPos.x,ease.easeout(FaceCenterPos.y,FaceCenterPos.y - 10,ease.timeRate),0.0f };
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

void Foot::Draw()
{
	DrawExtendGraph(FootCenterPosition.x - 30, FootCenterPosition.y - 30, FootCenterPosition.x + 30, FootCenterPosition.y + 30, Foothandle, true);
}