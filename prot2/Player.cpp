#include "Player.h"
#include <DxLib.h>
#include "Stage.h"
#include "InputManger.h"
#include "Colors.h"

namespace
{
	static size_t i = 0;
}

Player* Player::Get()
{
	static Player instance = {};
	return &instance;
}

Player::Player() :
	CenterPosition{},
	Body_One{},
	Body_Two{},
	Body_Three{}
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	CenterPosition.x = static_cast<float>(Stage::GetStartPlayerPosX() * Stage::blockSize + Stage::blockSize / 2);
	CenterPosition.y = static_cast<float>(Stage::GetStartPlayerPosY() * Stage::blockSize + Stage::blockSize / 2);

	//for (i = 0; i < sizeof(tile) / sizeof(tile[0]); i++) tile[i] = 0;

	FallSpeed = 3.0f;
	IsFall = true;
	IsJump = false;
	Player_IsAction = false;
	IsColide = false;

	Body_One.Init(CenterPosition, left);
	Body_One.BodyColor = YELLOW;

	Body_Two.Init(CenterPosition, up);
	Body_Two.BodyColor = GREEN;

	Body_Three.Init(CenterPosition, right);
	Body_Three.BodyColor = MAGENTA;

	FaceHandle[0] = LoadGraph("Resources/face.png");
	FaceHandle[1] = LoadGraph("Resources/face_fold.png");

	PlayerFoot.Init();
}

void Player::Update(Stage& stage)
{
	//左右移動
	if (InputManger::Right() && !InputManger::Act1())
	{
		CenterPosition.x += SideMoveSpeed;
	}
	if (InputManger::Left() && !InputManger::Act1())
	{
		CenterPosition.x -= SideMoveSpeed;
	}

	//ジャンプ入力できるかどうか
	if (IsJump == false && IsFall == false)
	{
		IsInputjump = true;
	}
	else
	{
		IsInputjump = false;
	}

	//ジャンプ
	if (InputManger::UpTrigger() && !InputManger::Act1() && IsInputjump == true)
	{
		//CenterPosition.y -= FallSpeed;
		IsJump = true;
		IsFall = true;
		FallSpeed = -9.0f;
	}
	if (InputManger::Down() && !InputManger::Act1())
	{
		//CenterPosition.y += FallSpeed;
	}

	if (IsJump == true)
	{
		Player_IsAction = true;
		FallSpeed += 0.5f;
		CenterPosition.y += FallSpeed;

		if (FallSpeed > 0)
		{
			Player_IsAction = false;
			IsJump = false;
			IsFall = true;
		}
	}

	//落下判定
	if (IsFall == true)
	{
		CenterPosition.y += FallSpeed;

		if (FallSpeed < 5.0)
		{
			FallSpeed += 0.5f;
		}
	}

	IsHitPlayerBody(stage);

	//折る
	//左
	if (InputManger::SubLeftTrigger() && Body_One.Ease.ismove == false && Body_One.Body_Type == left && Body_One.IsActivate == true)
	{
		Body_One.Ease.addtime = 0.1f;
		Body_One.Ease.maxtime = 1.2f;
		Body_One.Ease.timerate = 0.0f;

		//折る
		if (Body_One.IsFold == false && Body_One.IsOpen == true && Body_One.IsAction == false)
		{
			Body_One.Ease.ismove = true;
			Body_One.IsFold = true;
			Body_One.IsOpen = false;
			Body_One.IsAction = true;

			if (Body_Two.IsFold == true && Body_One.BodyDistance == 1)
			{
				Body_Two.Overlap++;
			}
			if (Body_Three.IsFold == true || Body_One.BodyDistance == 2)
			{
				Body_Three.Overlap++;
			}
		}

		if (Body_One.IsFold == true && Body_One.IsOpen == false && Body_One.FoldCount == 1 && Body_One.IsAction == false && Body_Three.Body_Type == left)
		{
			Body_Three.Ease.addtime = 0.1f;
			Body_Three.Ease.maxtime = 1.2f;
			Body_Three.Ease.timerate = 0.0f;

			Body_Three.Ease.ismove = true;
			Body_Three.IsFold = true;
			Body_Three.IsOpen = false;
			Body_Three.IsAction = true;
			Body_Three.Overlap = 0;

			Body_One.IsAction = true;
			Body_One.Ease.ismove = true;
			Body_One.Overlap = 1;

			if (Body_Two.IsFold == true)
			{
				Body_Two.Overlap = 2;
			}
		}

		if (Body_One.BodyDistance == 2 && Body_Two.IsFold == true)
		{
			IsOpenTwo = false;
		}
	}
	//上
	if (InputManger::SubUpTrigger() && Body_Two.Ease.ismove == false && Body_Two.Body_Type == up && Body_Two.IsActivate == true)
	{
		Body_Two.Ease.addtime = 0.1f;
		Body_Two.Ease.maxtime = 1.2f;
		Body_Two.Ease.timerate = 0.0f;

		//折る
		if (Body_Two.IsFold == false && Body_Two.IsOpen == true && Body_Two.IsAction == false)
		{
			Body_Two.Ease.ismove = true;
			Body_Two.IsFold = true;
			Body_Two.IsOpen = false;
			Body_Two.IsAction = true;

			if (Body_One.FoldCount == 1 && Body_One.Body_Type == left)
			{
				Body_One.Overlap++;
			}
			if (Body_Three.FoldCount == 1 && Body_Three.Body_Type == right)
			{
				Body_Three.Overlap++;
			}

			if (Body_One.FoldCount == 2)
			{
				Body_One.Overlap = 2;
				Body_Three.Overlap = 1;
			}
			if (Body_Three.FoldCount == 2)
			{
				Body_Three.Overlap = 2;
				Body_One.Overlap = 1;
			}
		}
	}
	//右
	if (InputManger::SubRightTrigger() && Body_Three.Ease.ismove == false && Body_Three.Body_Type == right && Body_Three.IsActivate == true)
	{
		Body_Three.Ease.addtime = 0.1f;
		Body_Three.Ease.maxtime = 1.2f;
		Body_Three.Ease.timerate = 0.0f;

		//折る
		if (Body_Three.IsFold == false && Body_Three.IsOpen == true && Body_Three.IsAction == false)
		{
			Body_Three.Ease.ismove = true;
			Body_Three.IsFold = true;
			Body_Three.IsOpen = false;
			Body_Three.IsAction = true;

			if (Body_One.IsFold == true || Body_Three.BodyDistance == 2)
			{
				Body_One.Overlap++;
			}
			if (Body_Two.IsFold == true && Body_Three.BodyDistance == 1)
			{
				Body_Two.Overlap++;
			}
		}

		if (Body_Three.IsFold == true && Body_Three.IsOpen == false && Body_Three.FoldCount == 1 && Body_Three.IsAction == false && Body_One.Body_Type == right)
		{
			Body_One.Ease.addtime = 0.1f;
			Body_One.Ease.maxtime = 1.2f;
			Body_One.Ease.timerate = 0.0f;

			Body_One.Ease.ismove = true;
			Body_One.IsFold = true;
			Body_One.IsOpen = false;
			Body_One.IsAction = true;
			Body_One.Overlap = 0;

			Body_Three.IsAction = true;
			Body_Three.Ease.ismove = true;
			Body_Three.Overlap = 1;

			if (Body_Two.IsFold == true)
			{
				Body_Two.Overlap = 2;
			}
		}

		if (Body_Three.BodyDistance == 2 && Body_Two.IsFold == true)
		{
			IsOpenTwo = false;
		}
	}
	//下
	if (InputManger::SubDownTrigger() && Body_Two.Ease.ismove == false && Body_Two.Body_Type == down && Body_Two.IsActivate == true)
	{
		Body_Two.Ease.addtime = 0.1f;
		Body_Two.Ease.maxtime = 1.2f;
		Body_Two.Ease.timerate = 0.0f;

		//折る
		if (Body_Two.IsFold == false && Body_Two.IsOpen == true && Body_Two.IsAction == false)
		{
			Body_Two.Ease.ismove = true;
			Body_Two.IsFold = true;
			Body_Two.IsOpen = false;
			Body_Two.IsAction = true;

			if (Body_One.FoldCount < 2 && Body_One.BodyDistance == 1)
			{
				Body_One.Overlap++;
			}
			if (Body_Three.FoldCount < 2 && Body_Three.BodyDistance == 1)
			{
				Body_Three.Overlap++;
			}

			if (Body_One.FoldCount == 2)
			{
				Body_One.Overlap++;
			}
			if (Body_Three.FoldCount == 2)
			{
				Body_Three.Overlap++;
			}
		}
	}

	//開く
	if (InputManger::Act1Trigger())
	{
		//左
		if (Body_One.IsFold == true && Body_One.IsAction == false && Body_One.Body_Type == left && Body_One.Overlap == 0 ||
			Body_Three.Body_Type == left && Body_Three.IsFold == true && Body_Three.Overlap == 0)
		{
			Body_One.Ease.addtime = 0.1f;
			Body_One.Ease.maxtime = 1.2f;
			Body_One.Ease.timerate = 0.0f;

			Body_One.Ease.ismove = true;
			Body_One.IsFold = false;
			Body_One.IsOpen = true;
			Body_One.IsAction = true;

			if (Body_One.FoldCount == 2)
			{
				Body_Three.Ease.addtime = 0.1f;
				Body_Three.Ease.maxtime = 1.2f;
				Body_Three.Ease.timerate = 0.0f;

				Body_Three.Ease.ismove = true;
				Body_Three.IsFold = false;
				Body_Three.IsOpen = true;
				Body_Three.IsAction = true;

				Body_Three.Overlap = 1;
				Body_One.Overlap = 0;

				if (Body_Two.IsFold == true)
				{
					Body_Two.Overlap = 0;
				}
			}

			if (Body_Two.IsFold == true && Body_One.BodyDistance == 1)
			{
				Body_Two.Overlap--;
			}
			if (Body_Three.Body_Type == right && Body_Three.IsFold == true || Body_One.FoldCount == 1 && Body_Three.Body_Type == left)
			{
				Body_Three.Overlap--;
			}

			if (Body_One.BodyDistance == 2 && Body_One.FoldCount == 1 && IsOpenTwo == false)
			{
				IsOpenTwo = true;
			}
		}
		//上
		else if (Body_Two.IsFold == true && Body_Two.IsAction == false && Body_Two.Overlap == 0 && IsOpenTwo == true)
		{
			Body_Two.Ease.addtime = 0.1f;
			Body_Two.Ease.maxtime = 1.2f;
			Body_Two.Ease.timerate = 0.0f;

			Body_Two.Ease.ismove = true;
			Body_Two.IsFold = false;
			Body_Two.IsOpen = true;
			Body_Two.IsAction = true;

			if (Body_One.IsFold == true)
			{
				Body_One.Overlap--;
			}
			if (Body_Three.IsFold == true && Body_Three.Overlap > 0)
			{
				Body_Three.Overlap--;
			}
		}
		//右
		else if (Body_Three.IsFold == true && Body_Three.IsAction == false && Body_Three.Body_Type == right && Body_Three.Overlap == 0 ||
			Body_One.Body_Type == right && Body_One.IsFold == true)
		{
			Body_Three.Ease.addtime = 0.1f;
			Body_Three.Ease.maxtime = 1.2f;
			Body_Three.Ease.timerate = 0.0f;

			Body_Three.Ease.ismove = true;
			Body_Three.IsFold = false;
			Body_Three.IsOpen = true;
			Body_Three.IsAction = true;

			if (Body_Three.FoldCount == 2)
			{
				Body_One.Ease.addtime = 0.1f;
				Body_One.Ease.maxtime = 1.2f;
				Body_One.Ease.timerate = 0.0f;

				Body_One.Ease.ismove = true;
				Body_One.IsFold = false;
				Body_One.IsOpen = true;
				Body_One.IsAction = true;

				Body_One.Overlap = 1;
				Body_Three.Overlap = 0;

				if (Body_Two.IsFold == true)
				{
					Body_Two.Overlap = 0;
				}
			}

			if (Body_Two.IsFold == true && Body_Three.BodyDistance == 1)
			{
				Body_Two.Overlap--;
			}
			if (Body_One.Body_Type == left && Body_One.IsFold == true || Body_Three.FoldCount == 1 && Body_One.Body_Type == right)
			{
				Body_One.Overlap--;
			}

			if (Body_Three.BodyDistance == 2 && Body_Three.FoldCount == 1 && IsOpenTwo == false)
			{
				IsOpenTwo = true;
			}
		}
	}

	//体のリセット
	if (Input::isKeyTrigger(KEY_INPUT_R))
	{
		bodysetup(false, left, true, up, true, right);
	}

	//体のスライド
	//左にスライド
	if (Input::isKeyTrigger(KEY_INPUT_Z) && Body_One.BodyDistance < 2 && Body_One.IsAction == false && Body_Three.FoldCount < 2)
	{
		if (Body_One.IsActivate == true && Body_Three.IsActivate == true)
		{
			if (Body_One.Body_Type == right)
			{
				Body_One.Overlap = 0;
				Body_One.setslide(-1, 2);
				Body_Three.BodyDistance = 1;
				Body_Three.setslide(-1, 1);
			}
			if (Body_One.Body_Type == left && Body_One.BodyDistance == 1 && Body_Three.IsFold == false)
			{
				if (Body_One.IsFold == true)
				{
					Body_Three.Overlap = 1;
					Body_One.BodyDistance = 2;
					Body_One.setslide(-1, 1);
					Body_Three.setslide(-1, 2);

					if (Body_Two.IsFold == true)
					{
						Body_Two.Overlap = 0;
					}
				}
				else
				{
					Body_One.BodyDistance = 2;
					Body_One.setslide(-1, 1);
					Body_Three.setslide(-1, 2);
				}
			}
		}
		else if (Body_Three.IsActivate == true && Body_Three.Body_Type == right && Body_Three.IsFold == false && Body_Three.IsAction == false)
		{
			Body_Three.setslide(-1, 2);
		}
		else if (Body_One.IsActivate == true && Body_One.Body_Type == right && Body_One.IsFold == false)
		{
			Body_One.setslide(-1, 2);
		}
	}
	//右にスライド
	if (Input::isKeyTrigger(KEY_INPUT_X) && Body_Three.BodyDistance < 2 && Body_Three.IsAction == false && Body_One.FoldCount < 2)
	{
		if (Body_One.IsActivate == true && Body_Three.IsActivate == true)
		{
			if (Body_Three.Body_Type == left)
			{
				Body_Three.Overlap = 0;
				Body_Three.setslide(1, 2);
				Body_One.BodyDistance = 1;
				Body_One.setslide(1, 1);
			}
			if (Body_Three.Body_Type == right && Body_Three.BodyDistance == 1 && Body_One.IsFold == false)
			{
				if (Body_Three.IsFold == true)
				{
					Body_One.Overlap = 1;
					Body_Three.BodyDistance = 2;
					Body_Three.setslide(1, 1);
					Body_One.setslide(1, 2);

					if (Body_Two.IsFold == true)
					{
						Body_Two.Overlap = 0;
					}
				}
				else
				{
					Body_Three.BodyDistance = 2;
					Body_Three.setslide(1, 1);
					Body_One.setslide(1, 2);
				}
			}
		}
		else if (Body_One.IsActivate == true && Body_One.Body_Type == left && Body_One.IsFold == false && Body_One.IsAction == false)
		{
			Body_One.setslide(1, 2);
		}
		else if (Body_Three.IsActivate == true && Body_Three.Body_Type == left && Body_Three.IsFold == false)
		{
			Body_Three.setslide(1, 2);
		}
	}
	//上下のスライド
	if (Input::isKeyTrigger(KEY_INPUT_C) && Body_Two.IsFold == false && Body_Two.IsAction == false && Body_Two.IsActivate == true)
	{
		if (Body_Two.Body_Type == up)
		{
			Body_Two.setslide(1, 2);
		}
		else
		{
			Body_Two.setslide(-1, 2);
		}
	}

	//足のイージング
	if (Input::isKeyTrigger(KEY_INPUT_F) && PlayerFoot.FootIsAction == false)
	{
		PlayerFoot.Set();
	}

	if (Body_One.Body_Type == down || Body_Two.Body_Type == down || Body_Three.Body_Type == down)
	{
		IsDownBody = true;
	}
	else
	{
		IsDownBody = false;
	}
	PlayerFoot.Update(CenterPosition);

	if (Body_One.IsActivate == true)
	{
		Body_One.IsHitBody(stage, &CenterPosition, FallSpeed, IsFall, IsJump, IsColide);
		Body_One.Update(CenterPosition);
	}
	if (Body_Two.IsActivate == true)
	{
		Body_Two.IsHitBody(stage, &CenterPosition, FallSpeed, IsFall, IsJump, IsColide);
		Body_Two.Update(CenterPosition);
	}
	if (Body_Three.IsActivate == true)
	{
		Body_Three.IsHitBody(stage, &CenterPosition, FallSpeed, IsFall, IsJump, IsColide);
		Body_Three.Update(CenterPosition);
	}

	if (Body_One.IsAction == true || Body_Two.IsAction == true || Body_Three.IsAction == true ||
		(IsColide == true && (InputManger::Right() || InputManger::Left())) ||
		PlayerFoot.FootIsAction == true)
	{
		Player_IsAction = true;
	}
	else
	{
		Player_IsAction = false;
	}
}

void Player::Draw(int offsetX, int offsetY)
{
	if (Body_One.IsSlide == false && Body_Two.IsSlide == false && Body_Three.IsSlide == false)
	{
		//DrawBox(CenterPosition.x - 30 + offsetX, CenterPosition.y - 30 + offsetY, CenterPosition.x + 30 + offsetX, CenterPosition.y + 30 + offsetY, GetColor(255, 0, 0), true);
		PlayerFoot.Draw();
		DrawExtendGraph(CenterPosition.x - 30, CenterPosition.y - 30, CenterPosition.x + 30, CenterPosition.y + 30, FaceHandle[Player_IsAction], true);
	}

#pragma region 重なっている枚数ごとに順番に描画
	if (Body_One.Overlap == 2)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.Overlap == 2)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.Overlap == 2)
	{
		Body_Three.Draw(offsetX, offsetY);
	}

	if (Body_One.Overlap == 1)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.Overlap == 1)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.Overlap == 1)
	{
		Body_Three.Draw(offsetX, offsetY);
	}

	if (Body_One.Overlap == 0)
	{
		Body_One.Draw(offsetX, offsetY);
	}
	if (Body_Two.Overlap == 0)
	{
		Body_Two.Draw(offsetX, offsetY);
	}
	if (Body_Three.Overlap == 0)
	{
		Body_Three.Draw(offsetX, offsetY);
	}
#pragma endregion 重なっている枚数ごとに順番に描画

	if (Body_One.IsSlide == true || Body_Two.IsSlide == true || Body_Three.IsSlide == true)
	{
		if (Body_Three.SlideDis == 2)
		{
			//DrawBox(center_position.x - 30 + offsetX, center_position.y - 30 + offsetY, body_one.bodyendpos.x + offsetX, body_one.bodyendpos.y + offsetY, body_three.bodycolor, true);
		}
		if (Body_One.SlideDis == 2)
		{
			//DrawBox(CenterPosition.x + 30 + offsetX, CenterPosition.y - 30 + offsetY, Body_Three.BodyStartPos.x + offsetX, Body_Three.BodyEndPos.y + offsetY, Body_One.BodyColor, true);
		}

		//DrawBox(CenterPosition.x - 30 + offsetX, CenterPosition.y - 30 + offsetY, CenterPosition.x + 30 + offsetX, CenterPosition.y + 30 + offsetY, GetColor(255, 0, 0), true);
		DrawExtendGraph(CenterPosition.x - 30, CenterPosition.y - 30, CenterPosition.x + 30, CenterPosition.y + 30, FaceHandle[Player_IsAction], true);

	}

	DrawLine(0, FloorHeight + offsetY, 1280, FloorHeight + offsetY, WHITE, true);
	DrawLine(300, 0, 300, 720, RED, true);

#pragma region UI
	DrawFormatString(0, 0, WHITE, "AD:左右移動");
	DrawFormatString(0, 20, WHITE, "W:ジャンプ");
	DrawFormatString(0, 40, WHITE, "←↑→:折る・開く");
	DrawFormatString(0, 60, WHITE, "SPACE:開く");
	DrawFormatString(0, 120, WHITE, "%f", CenterPosition.x);
	DrawFormatString(0, 140, WHITE, "%f", Body_Three.BodyStartPos.x);
	DrawFormatString(0, 160, WHITE, "%f", Body_Two.BodyEndPos.x - 30);
	DrawFormatString(0, 180, WHITE, "fall:%d", IsFall);
	DrawFormatString(0, 200, WHITE, "jump:%d", IsJump);
	DrawFormatString(0, 220, WHITE, "%f", FallSpeed);
	if (IsGoal == true)
	{
		DrawFormatString(300, 100, YELLOW, "GOAL");
	}
	else
	{
		DrawFormatString(300, 100, YELLOW, "NO GOAL");
	}

#pragma endregion
}

void Player::bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type)
{
	if (one == true)
	{
		Body_One.IsActivate = true;
		Body_One.Body_Type = one_type;
	}

	if (two == true)
	{
		Body_Two.IsActivate = true;
		Body_Two.Body_Type = two_type;
	}

	if (three == true)
	{
		Body_Three.IsActivate = true;
		Body_Three.Body_Type = three_type;
	}

	Body_One.setactivate(CenterPosition);
	Body_Two.setactivate(CenterPosition);
	Body_Three.setactivate(CenterPosition);

	CenterPosition.y += 1;
}

void Player::IsHitPlayerBody(Stage& stage)
{
	//ステージの数
	int i = 0;
	//タイルの数
	int j = 0;

	//上下左右(プレイヤーの顔)
	int left_mapchip = (int)((CenterPosition.x - 30) - stage.offset.x) / 60;
	int up_mapchip = (int)((CenterPosition.y - 30) - stage.offset.y) / 60;
	int right_mapchip = (int)((CenterPosition.x + 29) - stage.offset.x) / 60;
	int down_mapchip = (int)((CenterPosition.y + 29) - stage.offset.y) / 60;

	//タイル内のマップチップ座標
	int left_mapchip_tile;
	int up_mapchip_tile;
	int right_mapchip_tile;
	int down_mapchip_tile;
	//マップチップの場所(共通)
	int MapchipPos = 0;

	//マップチップの場所(ゴール用)
	int MapchipPos_Goal[3];

	//押し出す方向を決めるための距離
	float BuriedX = 0;
	float BuriedY = 0;

	for (i = 0; i < stage.GetStageDataSize(); i++)
	{
		for (j = 0; j < stage.GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage.GetPositionTile({ CenterPosition.x - 30,CenterPosition.y - 30,0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage.GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage.GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage.GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage.GetStageMapChip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX >= BuriedY)
					{
						ExtrudePlayer({ CenterPosition.x,60 + (up_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, 30, up);
					}
					else
					{
						ExtrudePlayer({ 60 + (left_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,CenterPosition.y,0.0f }, 30, left);
					}
				}
			}
			//左下
			if (stage.GetPositionTile({ CenterPosition.x - 30,CenterPosition.y + 30,0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage.GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage.GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage.GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage.GetStageMapChip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = ((CenterPosition.y + 30) - 60) - (down_mapchip * 60);

					if (BuriedX >= BuriedY)
					{
						ExtrudePlayer({ CenterPosition.x,(down_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, 30, down);
						IsFallLeft = false;
					}
					else
					{
						ExtrudePlayer({ 60 + (left_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,CenterPosition.y,0.0f }, 30, left);
						IsFallLeft = true;
					}
				}
				else if (IsDownBody == false)
				{
					IsFallLeft = true;
				}
			}
			//右上
			if (stage.GetPositionTile({ CenterPosition.x + 30,CenterPosition.y - 30,0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage.GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage.GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage.GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage.GetStageMapChip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = ((CenterPosition.x + 30) - 60) - (right_mapchip * 60);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX >= BuriedY)
					{
						ExtrudePlayer({ CenterPosition.x,(up_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, 30, up);
					}
					else
					{
						ExtrudePlayer({ (right_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,CenterPosition.y,0.0f }, 30, right);
					}
				}
			}
			//右下
			if (stage.GetPositionTile({ CenterPosition.x + 30,CenterPosition.y + 30,0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage.GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage.GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage.GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage.GetStageMapChip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = ((CenterPosition.x + 30) - 60) - (right_mapchip * 60);
					BuriedY = ((CenterPosition.y + 30) - 60) - (down_mapchip * 60);

					if (BuriedX >= BuriedY)
					{
						ExtrudePlayer({ CenterPosition.x,(down_mapchip_tile + stage.GetStageTileOffsetY(i,j)) * 60.0f,0.0f }, 30, down);
						IsFallRight = false;
					}
					else
					{
						ExtrudePlayer({ (right_mapchip_tile + stage.GetStageTileOffsetX(i,j)) * 60.0f,CenterPosition.y,0.0f }, 30, right);
						IsFallRight = true;
					}
				}
				else if (IsDownBody == false)
				{
					IsFallRight = true;
				}
			}

			//ゴール判定
			if (stage.GetPositionTile(CenterPosition, i, j))
			{
				left_mapchip_tile = left_mapchip % stage.GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage.GetStageTileHeight(i, j);
				right_mapchip_tile = right_mapchip % stage.GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage.GetStageTileHeight(i, j);

				//左上
				MapchipPos = up_mapchip_tile * stage.GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[0] = up_mapchip_tile * stage.GetStageTileWidth(i, j) + (right_mapchip_tile);
				MapchipPos_Goal[1] = down_mapchip_tile * stage.GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[2] = down_mapchip_tile * stage.GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (stage.GetStageMapChip(i, j, MapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//右上
				else if (stage.GetStageMapChip(i, j, MapchipPos_Goal[0]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//左下
				else if (stage.GetStageMapChip(i, j, MapchipPos_Goal[1]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//右下
				else if (stage.GetStageMapChip(i, j, MapchipPos_Goal[2]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				else
				{
					IsGoal = false;
				}
			}
		}
	}

	if (IsFallLeft == false || IsFallRight == false)
	{
		IsFall = false;
	}

}

void Player::ExtrudePlayer(Vector3 ExtrudePos, float ExtrudeDis, bodytype ExtrudeType)
{
	switch (ExtrudeType)
	{
	case left:
		if (CenterPosition.x - ExtrudePos.x < ExtrudeDis)
		{
			CenterPosition.x = ExtrudePos.x + ExtrudeDis;
			IsColide = true;
		}
		else
		{
			IsColide = false;
		}
		break;
	case right:
		if (ExtrudePos.x - CenterPosition.x < ExtrudeDis)
		{
			CenterPosition.x = ExtrudePos.x - ExtrudeDis;
			IsColide = true;
		}
		else
		{
			IsColide = false;
		}
		break;
	case up:
		if (CenterPosition.y - ExtrudePos.y < ExtrudeDis)
		{
			CenterPosition.y = ExtrudePos.y + ExtrudeDis;
			IsColide = true;
		}
		else
		{
			IsColide = false;
		}
		break;
	case down:
		if (ExtrudePos.y - CenterPosition.y < ExtrudeDis)
		{
			CenterPosition.y = ExtrudePos.y - ExtrudeDis;
			IsColide = true;
		}
		else
		{
			IsColide = false;
		}
		break;
	default:
		break;
	}
}