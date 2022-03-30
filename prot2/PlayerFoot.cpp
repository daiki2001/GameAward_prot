#include"PlayerFoot.h"

void Foot::Init()
{
	Foothandle = LoadGraph("Resources/Foot.png");
}

void Foot::Set()
{
	ease.addtime = 0.1f;
	ease.maxtime = 1.2f;
	ease.timerate = 0.0f;
	FootIsAction = true;
}

void Foot::Update(Vector3& FaceCenterPos)
{
	if (FootIsAction == true)
	{
		ease.addtime += ease.maxtime / 15.0f;
		ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

		FootCenterPosition = { FaceCenterPos.x,ease.easeout(FaceCenterPos.y,FaceCenterPos.y - 10,ease.timerate),0.0f };
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

void Foot::Draw()
{
	DrawExtendGraph(FootCenterPosition.x - 30, FootCenterPosition.y - 30, FootCenterPosition.x + 30, FootCenterPosition.y + 30, Foothandle, true);
}