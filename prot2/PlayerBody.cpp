#include "PlayerBody.h"
#include <DxLib.h>
#include "DrawShape.h"

PlayerBody::PlayerBody() :
	Isactivate(false),
	body_type(),
	Isfold(false),
	Isopen(true),
	foldcount(0),
	Isslide(false),
	slidepat(0),
	Isaction(false),
	slide_dis(),
	overlap(0),
	bodydistance(1)
{
}

PlayerBody::~PlayerBody()
{
}

void PlayerBody::Init(Vector3 position, bodytype number)
{
	body_type = number;

	if (body_type == left)
	{
		bodystartpos = { position.x - 90.0f, position.y - 30.0f, 0.0f };
	}
	else if (body_type == up)
	{
		bodystartpos = { position.x - 30.0f, position.y - 90.0f, 0.0f };
	}
	else if (body_type == right)
	{
		bodystartpos = { position.x + 30.0f, position.y - 30.0f, 0.0f };
	}
	else if (body_type == down)
	{
		bodystartpos = { position.x - 30.0f, position.y + 30.0f, 0.0f };
	}

	bodyendpos = { bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f };

	Isopen = true;
	Isfold = false;
}

void PlayerBody::Update(Vector3 center)
{
	//開いている途中
	if (Isfold == false && Isopen == true && Isaction == true && Isslide == false)
	{
		ease.addtime += ease.maxtime / 60.0f;
		ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

		if (body_type == left)
		{
			if (foldcount == 1)
			{
				bodyendpos = Vector3(center.x - static_cast<float>(30 + 60 * (bodydistance - 1)), center.y + 30.0f ,0.0f);
				bodystartpos.x = ease.easeout(bodyendpos.x + 60, bodyendpos.x - 60, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60;
			}
			else if (foldcount == 2)
			{
				bodystartpos = Vector3(center.x - 30.0f, center.y - 30.0f, 0.0f);
				bodyendpos.x = ease.easeout(bodystartpos.x + 60, bodystartpos.x - 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60.0f;
			}
		}
		if (body_type == up)
		{
			bodyendpos = Vector3(center.x + 30.0f, center.y - 30.0f, 0.0f);
			bodystartpos.y = ease.easeout(bodyendpos.y + 60, bodyendpos.y - 60, ease.timerate);
			bodystartpos.x = bodyendpos.x - 60.0f;
		}
		if (body_type == right)
		{
			if (foldcount == 1)
			{
				bodystartpos = Vector3(center.x + static_cast<float>(30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f);
				bodyendpos.x = ease.easeout(bodystartpos.x - 60, bodystartpos.x + 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60.0f;
			}
			else if (foldcount == 2)
			{
				bodyendpos = Vector3(center.x + 30.0f, center.y + 30.0f, 0.0f);
				bodystartpos.x = ease.easeout(bodyendpos.x - 60.0f, bodyendpos.x + 60.0f, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60.0f;
			}
		}
		if (body_type == down)
		{
			bodystartpos = Vector3(center.x + 30.0f, center.y + 30.0f, 0.0f);
			bodyendpos.y = ease.easeout(bodystartpos.y - 60.0f, bodystartpos.y + 60.0f, ease.timerate);
			bodyendpos.x = bodystartpos.x - 60.0f;
		}

		if (ease.timerate >= 1.0f)
		{
			ease.ismove = false;
			Isaction = false;
			foldcount--;
		}
	}
	//開いた後
	if (Isfold == false && Isopen == true && Isaction == false)
	{
		if (body_type == left)
		{
			if (foldcount == 0)
			{
				bodystartpos = Vector3(center.x - (30 + bodydistance * 60), center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
			}
			else if (foldcount == 1)
			{
				bodystartpos = Vector3(center.x - 30.0f, center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x - 60.0f, bodystartpos.y + 60.0f, 0.0f);
				Isfold = true;
				Isopen = false;
			}
		}
		if (body_type == up)
		{
			bodystartpos = Vector3(center.x - 30.0f, center.y - 90.0f, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
		}
		if (body_type == right)
		{
			if (foldcount == 0)
			{
				bodystartpos = Vector3(center.x + (30 + (bodydistance - 1) * 60), center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
			}
			else if (foldcount == 1)
			{
				bodyendpos = Vector3(center.x + 30.0f, center.y + 30.0f, 0.0f);
				bodystartpos = Vector3(bodyendpos.x + 60.0f, bodyendpos.y - 60.0f, 0.0f);
				Isfold = true;
				Isopen = false;
			}
		}
		if (body_type == down)
		{
			bodystartpos = Vector3(center.x - 30.0f, center.y + 30.0f, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
		}
	}
	//折るとき
	if (Isfold == true && Isopen == false && Isaction == true && Isslide == false)
	{
		ease.addtime += ease.maxtime / 60.0f;
		ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

		if (body_type == left)
		{
			if (foldcount == 0)
			{
				bodyendpos = Vector3(center.x - static_cast<float>(30 + 60 * (bodydistance - 1)), center.y + 30.0f, 0.0f);
				bodystartpos.x = ease.easeout(bodyendpos.x - 60, bodyendpos.x + 60, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60;
			}
			else if (foldcount == 1)
			{
				bodystartpos = Vector3(center.x - 30.0f, center.y - 30.0f, 0.0f);
				bodyendpos.x = ease.easeout(bodystartpos.x - 60, bodystartpos.x + 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60;
			}
		}
		if (body_type == up)
		{
			bodyendpos = Vector3(center.x + 30.0f, center.y - 30.0f, 0.0f);
			bodystartpos.y = ease.easeout(bodyendpos.y - 60, bodyendpos.y + 60, ease.timerate);
			bodystartpos.x = bodyendpos.x - 60;
		}
		if (body_type == right)
		{
			if (foldcount == 0)
			{
				bodystartpos = Vector3(center.x + static_cast<float>(30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f);
				bodyendpos.x = ease.easeout(bodystartpos.x + 60, bodystartpos.x - 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60;
			}
			else if (foldcount == 1)
			{
				bodyendpos = Vector3(center.x + 30.0f, center.y + 30.0f ,0.0f);
				bodystartpos.x = ease.easeout(bodyendpos.x + 60, bodyendpos.x - 60, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60;
			}
		}
		if (body_type == down)
		{
			bodystartpos = Vector3(center.x - 30.0f, center.y + 30.0f, 0.0f);
			bodyendpos.y = ease.easeout(bodystartpos.y + 60, bodystartpos.y - 60, ease.timerate);
			bodyendpos.x = bodystartpos.x + 60;
		}

		if (ease.timerate >= 1.0f)
		{
			ease.ismove = false;
			Isaction = false;
			foldcount++;
		}
	}
	//折った後
	if (Isfold == true && Isopen == false && Isaction == false)
	{
		if (body_type == left)
		{
			if (foldcount == 1)
			{
				bodyendpos = Vector3(center.x - (30 + 60 * (bodydistance - 1)), center.y + 30.0f ,0.0f);
				bodystartpos = Vector3(bodyendpos.x + 60.0f, bodyendpos.y - 60.0f, 0.0f);
			}
			else if (foldcount == 2)
			{
				bodystartpos = Vector3(center.x - 30.0f, center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
			}
		}
		else if (body_type == up)
		{
			bodystartpos = Vector3(center.x - 30.0f, center.y + 30.0f, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y - 60.0f, 0.0f);
		}
		else if (body_type == right)
		{
			if (foldcount == 1)
			{
				bodystartpos = Vector3(center.x + (30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x - 60.0f, bodystartpos.y + 60.0f, 0.0f);
			}
			else if (foldcount == 2)
			{
				bodystartpos = Vector3(center.x - 30.0f, center.y - 30.0f, 0.0f);
				bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f);
			}
		}
		else if (body_type == down)
		{
			bodystartpos = Vector3(center.x - 30.0f, center.y + 30.0f, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60.0f, bodystartpos.y - 60.0f, 0.0f);
		}
	}

	//体のスライド
	//顔を挟むとなり移動
	if (Isslide == true && ease.ismove == true && slide_dis == 2)
	{
		ease.addtime += ease.maxtime / 60.0f;
		ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

		if (body_type == left)
		{
			bodystartpos = Vector3(ease.easeout(center.x - 90, center.x + 30, ease.timerate), center.y - 30, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60, center.y + 30, 0.0f);
		}
		else if (body_type == right)
		{
			bodystartpos = Vector3(ease.easeout(center.x + 30, center.x - 90, ease.timerate), center.y - 30, 0.0f);
			bodyendpos = Vector3(bodystartpos.x + 60, center.y + 30, 0.0f);
		}
		else if (body_type == up)
		{
			bodystartpos = Vector3(center.x - 30, ease.easeout(center.y - 90, center.y + 30, ease.timerate), 0.0f);
			bodyendpos = Vector3(center.x + 30, bodystartpos.y + 60, 0.0f);
		}
		else if (body_type == down)
		{
			bodystartpos = Vector3(center.x - 30, ease.easeout(center.y + 30, center.y - 90, ease.timerate), 0.0f);
			bodyendpos = Vector3(center.x + 30, bodystartpos.y + 60, 0.0f);
		}

		if (ease.timerate >= 1.0f)
		{
			ease.ismove = false;
			Isaction = false;
			Isslide = false;

			if (body_type == left)
			{
				body_type = right;
			}
			else if (body_type == right)
			{
				body_type = left;
			}
			else if (body_type == up)
			{
				body_type = down;
			}
			else if (body_type == down)
			{
				body_type = up;
			}
		}
	}

	//顔を挟まないとなり移動(左右のみ)
	if (Isslide == true && ease.ismove == true && slide_dis == 1)
	{
		ease.addtime += ease.maxtime / 60.0f;
		ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

		if (body_type == left)
		{
			if (slidepat == -1)
			{
				bodyendpos = Vector3(ease.easeout(center.x - 30, center.x - 90, ease.timerate), center.y - 30, 0.0f);
			}
			else
			{
				bodyendpos = Vector3(ease.easeout(center.x - 90, center.x - 30, ease.timerate), center.y - 30, 0.0f);
			}
			bodystartpos = Vector3(bodyendpos.x + (120 * Isfold - 60), center.y + 30, 0.0f);
		}
		else if (body_type == right)
		{
			if (slidepat == -1)
			{
				bodystartpos = Vector3(ease.easeout(center.x + 90, center.x + 30, ease.timerate), center.y - 30, 0.0f);
			}
			else
			{
				bodystartpos = Vector3(ease.easeout(center.x + 30, center.x + 90, ease.timerate), center.y - 30, 0.0f);
			}
			bodyendpos = Vector3(bodystartpos.x + (-120 * Isfold + 60), center.y + 30, 0.0f);
		}

		if (ease.timerate >= 1.0f)
		{
			ease.ismove = false;
			Isaction = false;
			Isslide = false;
		}
	}
}

void PlayerBody::Draw(int offsetX, int offsetY)
{
	if (Isactivate == true)
	{
		DrawShape::DrawPlane(
			Vector3(bodystartpos.x + offsetX, bodystartpos.y + offsetY, 0.0f),
			Vector3(bodyendpos.x + offsetX, bodyendpos.y + offsetY, 0.0f),
			bodycolor);
	}
}

void PlayerBody::setactivate(Vector3 center)
{
	if (Isactivate == true)
	{
		Isfold = false;
		Isopen = true;
		Isslide = false;
		bodydistance = 1;
		overlap = 0;
		foldcount = 0;

		if (body_type == left)
		{
			bodystartpos = Vector3(center.x - 90, center.y - 30, 0.0f);
		}
		else if (body_type == right)
		{
			bodystartpos = Vector3(center.x + 30, center.y - 30, 0.0f);
		}
		else if (body_type == up)
		{
			bodystartpos = Vector3(center.x - 30, center.y - 90, 0.0f);
		}
		else if (body_type == down)
		{
			bodystartpos = Vector3(center.x - 30, center.y + 30, 0.0f);
		}

		bodyendpos = Vector3(bodystartpos.x + 60, bodystartpos.y + 60, 0.0f);
	}
}

void PlayerBody::setslide(int slidepat, int move_dis)
{
	Isaction = true;
	Isslide = true;
	ease.ismove = true;

	ease.addtime = 0.1f;
	ease.maxtime = 1.5f;
	ease.timerate = 0.0f;

	this->slidepat = slidepat;

	slide_dis = move_dis;
}

void PlayerBody::IsHitBody(Stage& stage, Vector3& center, PlayerBody& body_one, PlayerBody& body_two, bool& isfall, bool& isjump)
{
	//全体的なマップチップの座標
	int center_x_mapchip = static_cast<int>(center.x - stage.offset.x) / 60;
	int center_y_mapchip = static_cast<int>(center.y - stage.offset.y) / 60;

	//今いるタイル内でのプレイヤーのマップチップ座標
	int center_x_mapchip_tile = (int)center_x_mapchip % 5;
	int center_y_mapchip_tile = (int)center_y_mapchip % 5;

	//ステージの数
	size_t i = 0;
	//タイルの数
	size_t j = 0;

	//マップチップの座標
	size_t mapchipPos = 0;

	//プレイヤーがいるタイルのみ判定
	bool istile = false;

	if (center.x - 30 <= stage.offset.x || body_type == left && Isfold == true)
	{
		Extrude(center, stage.offset, 30, left, isfall, isjump);
	}
	if (center.y - 30 <= stage.offset.y || body_type == up && Isfold == true)
	{
		Extrude(center, stage.offset, 30, up, isfall, isjump);
	}

	for (i = 0; i < stage.GetStageDataSize(); i++)
	{
		for (j = 0; j < stage.GetStageTileDataSize(i); j++)
		{
			if (stage.GetPlayerTile(center, i, j))
			{
				if (Isfold == false)
				{
					switch (body_type)
					{
					case left:
						//左側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance - 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center,
								Vector3(60.0f + (center_x_mapchip - bodydistance - 1) * 60.0f, center_y_mapchip * 60.0f, 0.0f),
								30.0f + bodydistance * 60.0f, left, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance - 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { 60 + (center_x_mapchip - bodydistance - 1) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance - 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { 60 + (center_x_mapchip - bodydistance - 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}

						if (bodydistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, right, isfall, isjump);
							}
						}
						break;
					case right:
						//右側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance + 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center,
								Vector3((center_x_mapchip + bodydistance + 1) * 60.0f, center_y_mapchip * 60.0f, 0.0f),
								30.0f + bodydistance * 60.0f, right, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance + 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + bodydistance + 1) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance + 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + bodydistance + 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}

						if (bodydistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, left, isfall, isjump);
							}
						}
						break;
					case up:
						//上
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,60 + (center_y_mapchip - 2) * 60.0f,0.0f }, 90, up, isfall, isjump);
						}
						//左
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip - 1) * 60.0f,60 + (center_y_mapchip - 1) * 60.0f,0.0f }, 30, left, isfall, isjump);
						}
						//右
						mapchipPos = (center_y_mapchip_tile - 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + 1) * 60.0f,60 + (center_y_mapchip - 1) * 60.0f,0.0f }, 30, right, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}
						break;
					case down:
						//下
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 2) * 60.0f,0.0f }, 90, down, isfall, isjump);
						}
						//左
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip - 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, left, isfall, isjump);
						}
						//右
						mapchipPos = (center_y_mapchip_tile + 2) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { (center_x_mapchip + 1) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, right, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
						}
						break;
					}
				}
				else
				{
					switch (body_type)
					{
					case left:
						//左側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center,
								Vector3((center_x_mapchip - bodydistance) * 60.0f, center_y_mapchip * 60.0f, 0.0f),
								30.0f + (bodydistance - 1) * 60.0f, left, isfall, isjump);
						}
						if (foldcount < 2)
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - bodydistance) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - bodydistance);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - bodydistance) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						else
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						if (bodydistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + 1);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, right, isfall, isjump);
							}
						}
						break;
					case right:
						//右側
						mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance);
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center,
								Vector3((center_x_mapchip + bodydistance) * 60.0f, center_y_mapchip * 60.0f, 0.0f),
								30.0f + (bodydistance - 1), right, isfall, isjump);
						}
						if (foldcount < 2)
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + bodydistance) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile + bodydistance);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip + bodydistance) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						else
						{
							//上
							mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 30, up, isfall, isjump);
							}
							//下
							mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip) * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
							}
						}
						if (bodydistance == 2)
						{
							mapchipPos = center_y_mapchip_tile * stage.GetStageTileWidth(i, j) + (center_x_mapchip_tile - 1);
							if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
							{
								Extrude(center, { (center_x_mapchip - 1) * 60.0f,center_y_mapchip * 60.0f,0.0f }, 30, left, isfall, isjump);
							}
						}
						break;
					case up:
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip - 1) * 60.0f,0.0f }, 90, up, isfall, isjump);
						}
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 30, down, isfall, isjump);
						}
						break;
					case down:
						//下
						mapchipPos = (center_y_mapchip_tile + 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
						{
							Extrude(center, { center_x_mapchip * 60.0f,(center_y_mapchip + 1) * 60.0f,0.0f }, 90, down, isfall, isjump);
						}
						//上
						mapchipPos = (center_y_mapchip_tile - 1) * stage.GetStageTileWidth(i, j) + center_x_mapchip_tile;
						if (stage.GetStageMapchip(i, j, mapchipPos) == MapchipData::BLOCK)
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
