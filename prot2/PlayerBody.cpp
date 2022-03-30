#include "PlayerBody.h"
#include <DxLib.h>
#include "InputManger.h"

PlayerBody::PlayerBody() :
	IsActivate(false),
	Body_Type(),
	IsFold(false),
	IsOpen(true),
	IsSlide(false),
	SlidePat(0),
	IsAction(false),
	SlideDis(),
	Overlap(0),
	BodyDistance(1)
{
}

PlayerBody::~PlayerBody()
{
}

void PlayerBody::Init(Vector3 position, bodytype number)
{
	Body_Type = number;

	if (Body_Type == left)
	{
		BodyStartPos = { position.x - 90.0f, position.y - 30.0f, 0.0f };
	}
	else if (Body_Type == up)
	{
		BodyStartPos = { position.x - 30.0f, position.y - 90.0f, 0.0f };
	}
	else if (Body_Type == right)
	{
		BodyStartPos = { position.x + 30.0f, position.y - 30.0f, 0.0f };
	}
	else if (Body_Type == down)
	{
		BodyStartPos = { position.x - 30.0f, position.y + 30.0f, 0.0f };
	}

	BodyEndPos = { BodyStartPos.x + 60.0f, BodyStartPos.y + 60.0f, 0.0f };

	IsOpen = true;
	IsFold = false;
}

void PlayerBody::Update(Vector3& center)
{
	//開いている途中
	if (IsFold == false && IsOpen == true && IsAction == true && IsSlide == false)
	{
		Ease.addtime += Ease.maxtime / 25.0f;
		Ease.timerate = min(Ease.addtime / Ease.maxtime, 1.0f);

		if (Body_Type == left)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = { center.x - static_cast<float>(30 + BodySize * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
		}
		if (Body_Type == up)
		{
			BodyEndPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
			BodyStartPos.y = Ease.easeout(BodyEndPos.y + BodySize, BodyEndPos.y - BodySize, Ease.timerate);
			BodyStartPos.x = BodyEndPos.x - BodySize;
			if (Ease.timerate < 0.5)
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
		}
		if (Body_Type == right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + static_cast<float>(30 + BodySize * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = { center.x + 30,center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x + 30.0f, center.y + 30.0f, 0.0f };
			BodyEndPos.y = Ease.easeout(BodyStartPos.y - BodySize, BodyStartPos.y + BodySize, Ease.timerate);
			BodyEndPos.x = BodyStartPos.x - BodySize;
			if (Ease.timerate < 0.5)
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
		}

		if (Ease.timerate >= 1.0f)
		{
			Ease.ismove = false;
			IsAction = false;
			FoldCount--;
		}
	}
	//開いた後
	if (IsFold == false && IsOpen == true && IsAction == false)
	{
		if (Body_Type == left)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x - (30 + BodyDistance * BodySize),center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x - 30,BodyStartPos.y + 30 ,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30.0f,center.y - 90.0f,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30,0.0f };
		}
		if (Body_Type == right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + (30 + (BodyDistance - 1) * BodySize),center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30 ,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 30,center.y + 30,0.0f };
				BodyStartPos = { BodyEndPos.x + BodySize,BodyEndPos.y - BodySize,0.0f };
				BodyCenterPos = { BodyEndPos.x + 30,BodyEndPos.y - 30,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y + BodySize ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y + 30 ,0.0f };
		}
	}
	//折っている途中
	if (IsFold == true && IsOpen == false && IsAction == true && IsSlide == false)
	{
		Ease.addtime += Ease.maxtime / 25.0f;
		Ease.timerate = min(Ease.addtime / Ease.maxtime, 1.0f);

		if (Body_Type == left)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = { center.x - static_cast<float>(30 + BodySize * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x - BodySize, BodyEndPos.x + BodySize, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x - BodySize, BodyStartPos.x + BodySize, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
		}
		if (Body_Type == up)
		{
			BodyEndPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
			BodyStartPos.y = Ease.easeout(BodyEndPos.y - BodySize, BodyEndPos.y + BodySize, Ease.timerate);
			BodyStartPos.x = BodyEndPos.x - BodySize;
			if (Ease.timerate < 0.5)
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
		}
		if (Body_Type == right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + static_cast<float>(30 + BodySize * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x + BodySize, BodyStartPos.x - BodySize, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + 60;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 30,center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x + BodySize, BodyEndPos.x - BodySize, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - BodySize;
				if (Ease.timerate < 0.5)
				{
					BodyCenterPos.x = BodyEndPos.x + (BodyStartPos.x - BodyEndPos.x) / 2;
				}
				else
				{
					BodyCenterPos.x = BodyStartPos.x + (BodyEndPos.x - BodyStartPos.x) / 2;
				}
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
			BodyEndPos.y = Ease.easeout(BodyStartPos.y + BodySize, BodyStartPos.y - BodySize, Ease.timerate);
			BodyEndPos.x = BodyStartPos.x + BodySize;
			if (Ease.timerate < 0.5)
			{
				BodyCenterPos.y = BodyStartPos.y + (BodyEndPos.y - BodyStartPos.y) / 2;
			}
			else
			{
				BodyCenterPos.y = BodyEndPos.y + (BodyStartPos.y - BodyEndPos.y) / 2;
			}
		}

		if (Ease.timerate >= 1.0f)
		{
			Ease.ismove = false;
			IsAction = false;
			FoldCount++;
		}
	}
	//折った後
	if (IsFold == true && IsOpen == false && IsAction == false)
	{
		if (Body_Type == left)
		{
			if (FoldCount == 1)
			{
				BodyEndPos = { center.x - (30 + BodySize * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos = { BodyEndPos.x + 60, BodyEndPos.y - BodySize ,0.0f };
				BodyCenterPos = { BodyEndPos.x + 30, BodyEndPos.y - 30 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30, BodyStartPos.y + 30 ,0.0f };
			}
		}
		else if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y - 30 ,0.0f };
		}
		else if (Body_Type == right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + (30 + BodySize * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x - BodySize,BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x - 30,BodyStartPos.y + 30 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x + BodySize, BodyStartPos.y + BodySize ,0.0f };
				BodyCenterPos = { BodyStartPos.x + 30, BodyStartPos.y + 30 ,0.0f };
			}
		}
		else if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize,BodyStartPos.y - BodySize ,0.0f };
			BodyCenterPos = { BodyStartPos.x + 30,BodyStartPos.y - 30 ,0.0f };
		}
	}

	//体のスライド
	//顔を挟むとなり移動
	if (IsSlide == true && Ease.ismove == true && SlideDis == 2)
	{
		Ease.addtime += Ease.maxtime / 25.0f;
		Ease.timerate = min(Ease.addtime / Ease.maxtime, 1.0f);

		if (Body_Type == left)
		{
			BodyStartPos = { Ease.easeout(center.x - 90, center.x + 30, Ease.timerate), center.y - 30.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 30.0f, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == right)
		{
			BodyStartPos = { Ease.easeout(center.x + 30, center.x - 90, Ease.timerate), center.y - 30.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + BodySize, center.y + 30.0f, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30.0f, Ease.easeout(center.y - 90, center.y + 30, Ease.timerate), 0.0f };
			BodyEndPos = { center.x + 30.0f, BodyStartPos.y + BodySize, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}
		else if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30.0f, Ease.easeout(center.y + 30, center.y - 90, Ease.timerate), 0.0f };
			BodyEndPos = { center.x + 30.0f, BodyStartPos.y + BodySize, 0.0f };
			BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30.0f, 0.0f };
		}

		if (Ease.timerate >= 1.0f)
		{
			Ease.ismove = false;
			IsAction = false;
			IsSlide = false;

			if (Body_Type == left)
			{
				Body_Type = right;
			}
			else if (Body_Type == right)
			{
				Body_Type = left;
			}
			else if (Body_Type == up)
			{
				Body_Type = down;
			}
			else if (Body_Type == down)
			{
				Body_Type = up;
			}
		}
	}

	//顔を挟まないとなり移動(左右のみ)
	if (IsSlide == true && Ease.ismove == true && SlideDis == 1)
	{
		Ease.addtime += Ease.maxtime / 25.0f;
		Ease.timerate = min(Ease.addtime / Ease.maxtime, 1.0f);

		if (Body_Type == left)
		{
			if (SlidePat == -1)
			{
				BodyEndPos = { Ease.easeout(center.x - 30, center.x - 90, Ease.timerate), center.y - 30.0f, 0.0f };
				BodyCenterPos = { BodyEndPos.x - 30.0f, BodyEndPos.y + 30, 0.0f };
			}
			else
			{
				BodyEndPos = { Ease.easeout(center.x - 90, center.x - 30, Ease.timerate), center.y - 30.0f, 0.0f };
				BodyCenterPos = { BodyEndPos.x - 30.0f, BodyEndPos.y + 30, 0.0f };
			}
			BodyStartPos = { BodyEndPos.x + static_cast<float>(120 * IsFold - BodySize), center.y + 30.0f, 0.0f };
		}
		else if (Body_Type == right)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = { Ease.easeout(center.x + 90, center.x + 30, Ease.timerate), center.y - 30.0f, 0.0f };
				BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30, 0.0f };
			}
			else
			{
				BodyStartPos = { Ease.easeout(center.x + 30, center.x + 90, Ease.timerate), center.y - 30.0f, 0.0f };
				BodyCenterPos = { BodyStartPos.x + 30.0f, BodyStartPos.y + 30, 0.0f };
			}
			BodyEndPos = { BodyStartPos.x + static_cast<float>(-120 * IsFold + BodySize), center.y + 30.0f, 0.0f };
		}

		if (Ease.timerate >= 1.0f)
		{
			Ease.ismove = false;
			IsAction = false;
			IsSlide = false;
		}
	}
}

void PlayerBody::Draw(int offsetX, int offsetY)
{
	if (IsActivate == true)
	{
		/*DrawBox(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
			static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY, BodyColor, true);*/
		DrawExtendGraph(static_cast<int>(BodyStartPos.x) + offsetX, static_cast<int>(BodyStartPos.y) + offsetY,
			static_cast<int>(BodyEndPos.x) + offsetX, static_cast<int>(BodyEndPos.y) + offsetY, Bodyhandle, true);
		DrawCircle(BodyStartPos.x, BodyStartPos.y, 1, GetColor(255, 255, 255));
		DrawCircle(BodyStartPos.x, BodyEndPos.y, 1, GetColor(255, 255, 255));
		DrawCircle(BodyEndPos.x, BodyStartPos.y, 1, GetColor(255, 255, 255));
		DrawCircle(BodyEndPos.x, BodyEndPos.y, 1, GetColor(255, 255, 255));
		DrawCircle(BodyCenterPos.x, BodyCenterPos.y, 1, GetColor(255, 255, 255));
	}
}

void PlayerBody::setactivate(Vector3 center)
{
	if (IsActivate == true)
	{
		IsFold = false;
		IsOpen = true;
		IsSlide = false;
		BodyDistance = 1;
		Overlap = 0;
		FoldCount = 0;

		if (Body_Type == left)
		{
			BodyStartPos = { center.x - 90.0f, center.y - 30.0f, 0.0f };
		}
		else if (Body_Type == right)
		{
			BodyStartPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
		}
		else if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30.0f, center.y - 90.0f, 0.0f };
		}
		else if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
		}

		BodyEndPos = { BodyStartPos.x + 60.0f, BodyStartPos.y + 60.0f, 0.0f };
	}
}

void PlayerBody::setslide(int slidepat, int move_dis)
{
	IsAction = true;
	IsSlide = true;
	Ease.ismove = true;

	Ease.addtime = 0.1f;
	Ease.maxtime = 1.2f;
	Ease.timerate = 0.0f;

	this->SlidePat = slidepat;

	SlideDis = move_dis;
}

void PlayerBody::IsHitBody(Stage& stage, Vector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide)
{
	//体の四辺
	float BodyLeft;
	float BodyRight;
	float BodyUp;
	float BodyDown;

	//StartPosとEndPosの位置関係によって上下左右の設定を変える
	if (BodyStartPos.x < BodyEndPos.x)
	{
		BodyLeft = BodyStartPos.x;
		BodyRight = BodyEndPos.x - 1.0f;
	}
	else
	{
		BodyLeft = BodyEndPos.x;
		BodyRight = BodyStartPos.x - 1.0f;
	}

	if (BodyStartPos.y < BodyEndPos.y)
	{
		BodyUp = BodyStartPos.y;
		BodyDown = BodyEndPos.y - 1.0f;
	}
	else
	{
		BodyUp = BodyEndPos.y;
		BodyDown = BodyStartPos.y - 1.0f;
	}

	//四辺をブロックサイズで割った数
	int BodyLeft_mapchip = (int)(BodyLeft - stage.offset.x) / 60;
	int BodyUp_mapchip = (int)(BodyUp - stage.offset.y) / 60;
	int BodyRight_mapchip = (int)(BodyRight - stage.offset.x) / 60;
	int BodyDown_mapchip = (int)(BodyDown - stage.offset.y) / 60;

	//タイル内のマップチップ座標
	int BodyLeft_mapchip_tile;
	int BodyUp_mapchip_tile;
	int BodyRight_mapchip_tile;
	int BodyDown_mapchip_tile;

	//押し出す方向を決めるための距離
	float BuriedX = 0;
	float BuriedY = 0;

	//ステージの数
	int i = 0;
	//タイルの数
	int j = 0;

	//マップチップの座標
	int mapchipPos = 0;

	IsHitLeft = false;
	IsHitUp = false;
	IsHitRight = false;
	IsHitDown = false;

	if (center->x - 30 <= stage.offset.x || Body_Type == left && IsFold == true)
	{
		Extrude(center, stage.offset, 30, left, isfall, isjump, iscolide);
		iscolide = true;
	}
	if (center->y - 30 <= stage.offset.y || Body_Type == up && IsFold == true)
	{
		Extrude(center, stage.offset, 30, up, isfall, isjump, iscolide);
		iscolide = true;
	}

	for (i = 0; i < stage.GetStageDataSize(); i++)
	{
		for (j = 0; j < stage.GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage.GetPositionTile({ BodyLeft,BodyUp,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage.GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage.GetStageTileHeight(i, j);

				//今いる座標のマップチップを確認
				mapchipPos = BodyUp_mapchip_tile * stage.GetStageTileWidth(i, j) + BodyLeft_mapchip_tile;
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							center->y = (BodyUp_mapchip + 1) * 60 + (center->y - BodyUp);
							IsHitUp = true;
						}
						FallSpeed = 0.0f;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							center->x = (BodyLeft_mapchip + 1) * 60 + (center->x - BodyLeft);
							IsHitLeft = true;
						}
					}
				}
			}
			//左下
			if (stage.GetPositionTile({ BodyLeft,BodyDown,0.0f }, i, j))
			{
				BodyLeft_mapchip_tile = BodyLeft_mapchip % stage.GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyLeft_mapchip_tile);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyLeft_mapchip * 60) - BodyLeft;
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						if (IsHitDown == false)
						{
							Extrude(center, { center->x,(BodyDown_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, (BodyDown - center->y), down, isfall, isjump, iscolide);
							IsHitDown = true;
						}
						IsFallLeft = false;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitLeft == false)
						{
							center->x = (BodyLeft_mapchip + 1) * 60 + (center->x - BodyLeft);
							IsHitLeft = true;
						}
						IsFallLeft = true;
					}
				}
				else
				{
					IsFallLeft = true;
				}
			}
			//右上
			if (stage.GetPositionTile({ BodyRight,BodyUp,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage.GetStageTileWidth(i, j);
				BodyUp_mapchip_tile = BodyUp_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyUp_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyUp_mapchip * 60) - BodyUp;

					if (BuriedX > BuriedY)
					{
						if (IsHitUp == false)
						{
							center->y = (BodyUp_mapchip + 1) * 60 + (center->y - BodyUp);
							IsHitUp = true;
						}
						FallSpeed = 0.0f;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							Extrude(center, { (BodyRight_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,center->y,0.0f }, (BodyRight - center->x), right, isfall, isjump, iscolide);
							IsHitRight = true;
						}
					}
				}
			}
			//右下
			if (stage.GetPositionTile({ BodyRight,BodyDown,0.0f }, i, j))
			{
				BodyRight_mapchip_tile = BodyRight_mapchip % stage.GetStageTileWidth(i, j);
				BodyDown_mapchip_tile = BodyDown_mapchip % stage.GetStageTileHeight(i, j);

				mapchipPos = (BodyDown_mapchip_tile)*stage.GetStageTileWidth(i, j) + (BodyRight_mapchip_tile);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (BodyRight - 60) - (BodyRight_mapchip * 60);
					BuriedY = (BodyDown - 60) - (BodyDown_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						if (IsHitDown == false)
						{
							Extrude(center, { center->x,(BodyDown_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, (BodyDown - center->y), down, isfall, isjump, iscolide);
							IsHitDown = true;
						}
						IsFallRight = false;
					}
					else if (BuriedX < BuriedY)
					{
						if (IsHitRight == false)
						{
							Extrude(center, { (BodyRight_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,center->y,0.0f }, (BodyRight - center->x), right, isfall, isjump, iscolide);
							IsHitRight = true;
						}
						IsFallRight = true;
					}
				}
				else
				{
					IsFallRight = true;
				}
			}
		}
	}

	if (IsFallLeft == false || IsFallRight == false)
	{
		isfall = false;
	}
}

void PlayerBody::Extrude(Vector3* center, Vector3 extrudepos, float extrudedis, bodytype extrudetype, bool& isfall, bool& isjump, bool& iscolide)
{
	switch (extrudetype)
	{
	case left:
		if (center->x - extrudepos.x < extrudedis)
		{
			center->x = extrudepos.x + extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case right:
		if (extrudepos.x - center->x < extrudedis)
		{
			center->x = extrudepos.x - extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case up:
		if (center->y - extrudepos.y < extrudedis)
		{
			center->y = extrudepos.y + extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	case down:
		if (extrudepos.y - center->y < extrudedis)
		{
			center->y = extrudepos.y - extrudedis;
			iscolide = true;
		}
		else
		{
			iscolide = false;
		}
		break;
	default:
		break;
	}
}