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

void Foot::Update(Vector3& FaceCenterPos, bool IsDownBody, int BodyDis)
{
	if (FootIsAction == true)
	{
		ease.addTime += ease.maxTime / 20.0f;
		ease.timeRate = min(ease.addTime / ease.maxTime, 1.0f);

		FootLeftUpPosition = { FaceCenterPos.x - 30,ease.easeout(FaceCenterPos.y + (IsDownBody * 50 * BodyDis) + 25,FaceCenterPos.y + (IsDownBody * 50 * BodyDis) + 15,ease.timeRate),0.0f };
		if (ease.timeRate >= 1.0f)
		{
			FootIsAction = false;
		}
	}

	if (IsFootUp == false)
	{
		FootLeftUpPosition = { FaceCenterPos.x - 30,FaceCenterPos.y + (IsDownBody * 50 * BodyDis) + 25,0.0f };
	}
}

void Foot::Draw(bool isleft, bool isright, int offsetX, int offsetY)
{
	if (isleft)
	{
		DrawExtendGraph(FootLeftUpPosition.x + offsetX, FootLeftUpPosition.y + offsetY, FootLeftUpPosition.x + 60 + offsetX, FootLeftUpPosition.y + 8 + offsetY, Foothandle, true);
	}
	if (isright)
	{
		DrawExtendGraph(FootLeftUpPosition.x + 60 + offsetX, FootLeftUpPosition.y + offsetY, FootLeftUpPosition.x + offsetX, FootLeftUpPosition.y + 8 + offsetY, Foothandle, true);
	}
}