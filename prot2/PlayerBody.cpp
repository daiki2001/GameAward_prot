#include "PlayerBody.h"
#include <DxLib.h>

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
	IsGoal = false;
}

void PlayerBody::Update(Vector3 center)
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
				BodyEndPos = { center.x - static_cast<float>(30 + 60 * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x + 60, BodyEndPos.x - 60, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - 60;
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x + 60, BodyStartPos.x - 60, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + 60;
			}
		}
		if (Body_Type == up)
		{
			BodyEndPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
			BodyStartPos.y = Ease.easeout(BodyEndPos.y + 60, BodyEndPos.y - 60, Ease.timerate);
			BodyStartPos.x = BodyEndPos.x - 60;
		}
		if (Body_Type == right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + static_cast<float>(30 + 60 * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x - 60, BodyStartPos.x + 60, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + 60;
			}
			else if (FoldCount == 2)
			{
				BodyEndPos = { center.x + 30,center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x - 60, BodyEndPos.x + 60, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - 60;
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x + 30.0f, center.y + 30.0f, 0.0f };
			BodyEndPos.y = Ease.easeout(BodyStartPos.y - 60, BodyStartPos.y + 60, Ease.timerate);
			BodyEndPos.x = BodyStartPos.x - 60;
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
				BodyStartPos = { center.x - (30 + BodyDistance * 60),center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y + 60,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x - 60,BodyStartPos.y + 60 ,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30,center.y - 90,0.0f };
			BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y + 60,0.0f };
		}
		if (Body_Type == right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + (30 + (BodyDistance - 1) * 60),center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y + 60 ,0.0f };
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 30,center.y + 30,0.0f };
				BodyStartPos = { BodyEndPos.x + 60,BodyEndPos.y - 60,0.0f };
				IsFold = true;
				IsOpen = false;
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y + 60 ,0.0f };
		}
	}
	//折るとき
	if (IsFold == true && IsOpen == false && IsAction == true && IsSlide == false)
	{
		Ease.addtime += Ease.maxtime / 25.0f;
		Ease.timerate = min(Ease.addtime / Ease.maxtime, 1.0f);

		if (Body_Type == left)
		{
			if (FoldCount == 0)
			{
				BodyEndPos = { center.x - static_cast<float>(30 + 60 * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x - 60, BodyEndPos.x + 60, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - 60;
			}
			else if (FoldCount == 1)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x - 60, BodyStartPos.x + 60, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + 60;
			}
		}
		if (Body_Type == up)
		{
			BodyEndPos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
			BodyStartPos.y = Ease.easeout(BodyEndPos.y - 60, BodyEndPos.y + 60, Ease.timerate);
			BodyStartPos.x = BodyEndPos.x - 60;
		}
		if (Body_Type == right)
		{
			if (FoldCount == 0)
			{
				BodyStartPos = { center.x + static_cast<float>(30 + 60 * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos.x = Ease.easeout(BodyStartPos.x + 60, BodyStartPos.x - 60, Ease.timerate);
				BodyEndPos.y = BodyStartPos.y + 60;
			}
			else if (FoldCount == 1)
			{
				BodyEndPos = { center.x + 30,center.y + 30 ,0.0f };
				BodyStartPos.x = Ease.easeout(BodyEndPos.x + 60, BodyEndPos.x - 60, Ease.timerate);
				BodyStartPos.y = BodyEndPos.y - 60;
			}
		}
		if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
			BodyEndPos.y = Ease.easeout(BodyStartPos.y + 60, BodyStartPos.y - 60, Ease.timerate);
			BodyEndPos.x = BodyStartPos.x + 60;
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
				BodyEndPos = { center.x - (30 + 60 * (BodyDistance - 1)),center.y + 30 ,0.0f };
				BodyStartPos = { BodyEndPos.x + 60, BodyEndPos.y - 60 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30,0.0f };
				BodyEndPos = { BodyStartPos.x + 60, BodyStartPos.y + 60 ,0.0f };
			}
		}
		else if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y - 60 ,0.0f };
		}
		else if (Body_Type == right)
		{
			if (FoldCount == 1)
			{
				BodyStartPos = { center.x + (30 + 60 * (BodyDistance - 1)),center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x - 60,BodyStartPos.y + 60 ,0.0f };
			}
			else if (FoldCount == 2)
			{
				BodyStartPos = { center.x - 30,center.y - 30 ,0.0f };
				BodyEndPos = { BodyStartPos.x + 60, BodyStartPos.y + 60 ,0.0f };
			}
		}
		else if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30,center.y + 30 ,0.0f };
			BodyEndPos = { BodyStartPos.x + 60,BodyStartPos.y - 60 ,0.0f };
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
			BodyEndPos = { BodyStartPos.x + 60.0f, center.y + 30.0f, 0.0f };
		}
		else if (Body_Type == right)
		{
			BodyStartPos = { Ease.easeout(center.x + 30, center.x - 90, Ease.timerate), center.y - 30.0f, 0.0f };
			BodyEndPos = { BodyStartPos.x + 60.0f, center.y + 30.0f, 0.0f };
		}
		else if (Body_Type == up)
		{
			BodyStartPos = { center.x - 30.0f, Ease.easeout(center.y - 90, center.y + 30, Ease.timerate), 0.0f };
			BodyEndPos = { center.x + 30.0f, BodyStartPos.y + 60.0f, 0.0f };
		}
		else if (Body_Type == down)
		{
			BodyStartPos = { center.x - 30.0f, Ease.easeout(center.y + 30, center.y - 90, Ease.timerate), 0.0f };
			BodyEndPos = { center.x + 30.0f, BodyStartPos.y + 60.0f, 0.0f };
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
			}
			else
			{
				BodyEndPos = { Ease.easeout(center.x - 90, center.x - 30, Ease.timerate), center.y - 30.0f, 0.0f };
			}
			BodyStartPos = { BodyEndPos.x + static_cast<float>(120 * IsFold - 60), center.y + 30.0f, 0.0f };
		}
		else if (Body_Type == right)
		{
			if (SlidePat == -1)
			{
				BodyStartPos = { Ease.easeout(center.x + 90, center.x + 30, Ease.timerate), center.y - 30.0f, 0.0f };
			}
			else
			{
				BodyStartPos = { Ease.easeout(center.x + 30, center.x + 90, Ease.timerate), center.y - 30.0f, 0.0f };
			}
			BodyEndPos = { BodyStartPos.x + static_cast<float>(-120 * IsFold + 60), center.y + 30.0f, 0.0f };
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

void PlayerBody::IsHitBody(Stage& stage, Vector3& center, PlayerBody& body_one, PlayerBody& body_two, bool& isfall, bool& isjump)
{
	//全体的なマップチップの座標
	int center_x_mapchip = (center.x - stage.offset.x) / 60;
	int center_y_mapchip = (center.y - stage.offset.y) / 60;

	//上下左右(プレイヤーの顔)
	int left_mapchip = ((int)((center.x - 30) - stage.offset.x) / 60) % 5;
	int up_mapchip = ((int)((center.y - 30) - stage.offset.y) / 60) % 5;
	int right_mapchip = ((int)((center.x + 30) - stage.offset.x) / 60) % 5;
	int down_mapchip = ((int)((center.y + 30) - stage.offset.y) / 60) % 5;

	//今いるタイル内でのプレイヤーのマップチップ座標
	int center_x_mapchip_tile = (int)center_x_mapchip % 5;
	int center_y_mapchip_tile = (int)center_y_mapchip % 5;

	//Bodyの上下左右のマップチップ座標
	int Bodyleft_mapchip = ((int)(BodyStartPos.x - stage.offset.x) / 60) % 5;
	int Bodyup_mapchip = ((int)(BodyStartPos.y - stage.offset.y) / 60) % 5;
	int Bodyright_mapchip = ((int)(BodyEndPos.x - stage.offset.x) / 60) % 5;
	int Bodydown_mapchip = ((int)(BodyEndPos.y - stage.offset.y) / 60) % 5;


	//ステージの数
	int i = 0;
	//タイルの数
	int j = 0;

	//マップチップの座標
	int mapchipPos = 0;

	//プレイヤーがいるタイルのみ判定
	bool istile = false;

	if (center.x - 30 <= stage.offset.x || Body_Type == left && IsFold == true)
	{
		Extrude(center, stage.offset, 30, left, isfall, isjump);
	}
	if (center.y - 30 <= stage.offset.y || Body_Type == up && IsFold == true)
	{
		Extrude(center, stage.offset, 30, up, isfall, isjump);
	}



	for (i = 0; i < stage.GetStageDataSize(); i++)
	{
		for (j = 0; j < stage.GetStageTileDataSize(i); j++)
		{
			if (stage.GetPositionTile(center, i, j))
			{
				mapchipPos = up_mapchip * stage.GetStageTileWidth(i, j) + (left_mapchip);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				mapchipPos = up_mapchip * stage.GetStageTileWidth(i, j) + (right_mapchip);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				mapchipPos = down_mapchip * stage.GetStageTileWidth(i, j) + (left_mapchip);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				mapchipPos = down_mapchip * stage.GetStageTileWidth(i, j) + (right_mapchip);
				if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}

				if (IsFold == false)
				{
					switch (Body_Type)
					{
					case left:
						//左側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance - 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { 60 + (center_x_mapchip - BodyDistance - 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30 + BodyDistance * 60, left, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance - 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { 60 + (center_x_mapchip - BodyDistance - 1) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance - 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { 60 + (center_x_mapchip - BodyDistance - 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}

						if (BodyDistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, right, isfall, isjump);
							}
						}
						break;
					case right:
						//右側
						mapchipPos = (center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance + 1)) - 2;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + BodyDistance + 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30 + BodyDistance * 60, right, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance + 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + BodyDistance + 1) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						//下
						mapchipPos = ((center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance + 1)) - 2;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + BodyDistance + 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}

						if (BodyDistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, left, isfall, isjump);
							}
						}
						break;
					case up:
						//上
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,60 + (center_y_mapchip - 2) * 60.0f,0.0f }, 90, up, isfall, isjump);
						}
						//左
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip - 1) * 60.0f,60 + (center_y_mapchip - 1) * 60.0f,0.0f }, 30, left, isfall, isjump);
						}
						//右
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + 1) * 60.0f,60 + (center_y_mapchip - 1) * 60.0f,0.0f }, 30, right, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}
						break;
					case down:
						//下
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 2) * 60.0f,0.0f }, 90, down, isfall, isjump);
						}
						//左
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip - 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, left, isfall, isjump);
						}
						//右
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, right, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						break;
					}
				}
				else
				{
					switch (Body_Type)
					{
					case left:
						//左側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip - BodyDistance) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30 + (BodyDistance - 1) * 60, left, isfall, isjump);
						}
						if (FoldCount < 2)
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - BodyDistance) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - BodyDistance);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - BodyDistance) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						else
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						if (BodyDistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, right, isfall, isjump);
							}
						}
						break;
					case right:
						//右側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance);
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + BodyDistance) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30 + (BodyDistance - 1), right, isfall, isjump);
						}
						if (FoldCount < 2)
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + BodyDistance) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + BodyDistance);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + BodyDistance) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						else
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						if (BodyDistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
							if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, left, isfall, isjump);
							}
						}
						break;
					case up:
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 90, up, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}
						break;
					case down:
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 90, down, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapChip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						break;
					}
				}
			}
		}
	}
}

void PlayerBody::Extrude(Vector3& center, Vector3 extrudepos, float extrudedis, bodytype extrudetype, bool& isfall, bool& isjump)
{
	switch (extrudetype)
	{
	case left:
		if (center.x - extrudepos.x < extrudedis)
		{
			center.x = extrudepos.x + extrudedis;
		}
		break;
	case right:
		if (extrudepos.x - center.x < extrudedis)
		{
			center.x = extrudepos.x - extrudedis;
		}
		break;
	case up:
		if (center.y - extrudepos.y < extrudedis)
		{
			center.y = extrudepos.y + extrudedis;
		}
		break;
	case down:
		if (extrudepos.y - center.y < extrudedis)
		{
			center.y = extrudepos.y - extrudedis;
			isfall = false;
			isjump = false;
		}
		break;
	default:
		break;
	}
}
