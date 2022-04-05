#include "Player.h"
#include <DxLib.h>
#include "Stage.h"
#include "InputManger.h"
#include "DrawShape.h"
#include "Colors.h"

namespace
{
static size_t i = 0;
}

Stage* Player::stage = Stage::Get();

Player* Player::Get()
{
	static Player instance = {};
	return &instance;
}

Player::Player() :
	FloorHeight(640.0f),
	CenterPosition{ 100.0f, 100.0f, 0.0f },
	IsLeft(),
	IsRight(),
	Body_One{},
	Body_Two{},
	Body_Three{},
	Body_Four{},
	IsOpenTwo(true),
	IsLeftFold(),
	IsUpFold(),
	IsRightFold(),
	IsDownFold(),
	SideMoveSpeed(3.0f),
	IsJump(false),
	JumpSpeed(3.0f),
	FallSpeed(3.0f),
	IsFaceFall(),
	IsAllFall(false),
	IsInputjump(false),
	Player_IsAction(false),
	FaceHandle{},
	IsGoal(false),
	IsColide(false),
	IsDownBody(false),
	leg{}
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

	FallSpeed = 3.0f;
	IsAllFall = true;
	IsJump = false;
	Player_IsAction = false;
	IsColide = false;
	IsGoal = false;

	Body_One.Init(CenterPosition, left);
	Body_One.BodyColor = YELLOW;

	Body_Two.Init(CenterPosition, up);
	Body_Two.BodyColor = GREEN;

	Body_Three.Init(CenterPosition, right);
	Body_Three.BodyColor = MAGENTA;

	Body_Four.Init(CenterPosition, down);
	Body_Four.BodyColor = MAGENTA;

	FaceHandle[0] = LoadGraph("Resources/player.png");
	FaceHandle[1] = LoadGraph("Resources/playerBody/playerBody02.png");

	leg.Init();
	IsLeft = true;
	IsRight = false;
}

void Player::Update()
{
	//左右移動
	if (InputManger::Right() && !InputManger::Act1() && Player_IsAction == false)
	{
		CenterPosition.x += SideMoveSpeed;
		IsLeft = false;
		IsRight = true;
	}
	if (InputManger::Left() && !InputManger::Act1() && Player_IsAction == false)
	{
		CenterPosition.x -= SideMoveSpeed;
		IsLeft = true;
		IsRight = false;
	}

	//ジャンプ入力できるかどうか
	if (IsJump == false && IsFall() == false)
	{
		IsInputjump = true;
		FallSpeed = 0.0f;
	}
	else
	{
		IsInputjump = false;
	}

	//ジャンプ
	if (InputManger::UpTrigger() && !InputManger::Act1() && IsInputjump == true)
	{
		IsJump = true;
		FallSpeed = -9.5f;
	}

	if (IsJump == true)
	{
		if (Player_IsAction == false)
		{
			FallSpeed += 0.5f;
		}

		if (FallSpeed > 0)
		{
			IsJump = false;
			IsAllFall = true;
		}
	}

	//落下判定
	if (IsJump == false && IsAllFall == true && Player_IsAction == false)
	{
		if (FallSpeed < 5.0)
		{
			FallSpeed += 0.5f;
		}
	}

	if (IsAllFall == true && Player_IsAction == false)
	{
		CenterPosition.y += FallSpeed;
	}
	IsHitPlayerBody();


	if (InputManger::SubLeftTrigger() && Player_IsAction == false)
	{
		Player_IsAction = true;
		IsLeftFold = true;
		leg.Set();
	}
	if (InputManger::SubUpTrigger() && Player_IsAction == false)
	{
		Player_IsAction = true;
		IsUpFold = true;
		leg.Set();
	}
	if (InputManger::SubRightTrigger() && Player_IsAction == false)
	{
		Player_IsAction = true;
		IsRightFold = true;
		leg.Set();
	}
	if (InputManger::SubDownTrigger() && Player_IsAction == false)
	{
		Player_IsAction = true;
		IsDownFold = true;
		leg.Set();
	}

	if (leg.FootIsAction == false)
	{
		if (IsLeftFold == true)
		{
			if (Body_One.Ease.isMove == false && Body_One.Body_Type == left && Body_One.IsActivate == true)
			{
				Body_One.Ease.addTime = 0.1f;
				Body_One.Ease.maxTime = 1.2f;
				Body_One.Ease.timeRate = 0.0f;

				//折る
				if (Body_One.IsFold == false && Body_One.IsOpen == true && Body_One.IsAction == false)
				{
					Body_One.Ease.isMove = true;
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
					Body_Three.Ease.addTime = 0.1f;
					Body_Three.Ease.maxTime = 1.2f;
					Body_Three.Ease.timeRate = 0.0f;

					Body_Three.Ease.isMove = true;
					Body_Three.IsFold = true;
					Body_Three.IsOpen = false;
					Body_Three.IsAction = true;
					Body_Three.Overlap = 0;

					Body_One.IsAction = true;
					Body_One.Ease.isMove = true;
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
			else if (Body_Three.Ease.isMove == false && Body_Three.Body_Type == left && Body_Three.IsActivate == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				//折る
				if (Body_Three.IsFold == false && Body_Three.IsOpen == true && Body_Three.IsAction == false)
				{
					Body_Three.Ease.isMove = true;
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

				if (Body_Three.IsFold == true && Body_Three.IsOpen == false && Body_Three.FoldCount == 1 && Body_Three.IsAction == false && Body_One.Body_Type == left)
				{
					Body_One.Ease.addTime = 0.1f;
					Body_One.Ease.maxTime = 1.2f;
					Body_One.Ease.timeRate = 0.0f;

					Body_One.Ease.isMove = true;
					Body_One.IsFold = true;
					Body_One.IsOpen = false;
					Body_One.IsAction = true;
					Body_One.Overlap = 0;

					Body_Three.IsAction = true;
					Body_Three.Ease.isMove = true;
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
			IsLeftFold = false;
		}
		if (IsUpFold == true)
		{
			if (Body_Two.Ease.isMove == false && Body_Two.Body_Type == up && Body_Two.IsActivate == true)
			{
				Body_Two.Ease.addTime = 0.1f;
				Body_Two.Ease.maxTime = 1.2f;
				Body_Two.Ease.timeRate = 0.0f;

				//折る
				if (Body_Two.IsFold == false && Body_Two.IsOpen == true && Body_Two.IsAction == false)
				{
					Body_Two.Ease.isMove = true;
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
			IsUpFold = false;
		}
		if (IsRightFold == true)
		{
			if (Body_One.Ease.isMove == false && Body_One.Body_Type == right && Body_One.IsActivate == true)
			{
				Body_One.Ease.addTime = 0.1f;
				Body_One.Ease.maxTime = 1.2f;
				Body_One.Ease.timeRate = 0.0f;

				//折る
				if (Body_One.IsFold == false && Body_One.IsOpen == true && Body_One.IsAction == false)
				{
					Body_One.Ease.isMove = true;
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

				if (Body_One.IsFold == true && Body_One.IsOpen == false && Body_One.FoldCount == 1 && Body_One.IsAction == false && Body_Three.Body_Type == right)
				{
					Body_Three.Ease.addTime = 0.1f;
					Body_Three.Ease.maxTime = 1.2f;
					Body_Three.Ease.timeRate = 0.0f;

					Body_Three.Ease.isMove = true;
					Body_Three.IsFold = true;
					Body_Three.IsOpen = false;
					Body_Three.IsAction = true;
					Body_Three.Overlap = 0;

					Body_One.IsAction = true;
					Body_One.Ease.isMove = true;
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
			if (Body_Three.Ease.isMove == false && Body_Three.Body_Type == right && Body_Three.IsActivate == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				//折る
				if (Body_Three.IsFold == false && Body_Three.IsOpen == true && Body_Three.IsAction == false)
				{
					Body_Three.Ease.isMove = true;
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
					Body_One.Ease.addTime = 0.1f;
					Body_One.Ease.maxTime = 1.2f;
					Body_One.Ease.timeRate = 0.0f;

					Body_One.Ease.isMove = true;
					Body_One.IsFold = true;
					Body_One.IsOpen = false;
					Body_One.IsAction = true;
					Body_One.Overlap = 0;

					Body_Three.IsAction = true;
					Body_Three.Ease.isMove = true;
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
			IsRightFold = false;
		}
		if (IsDownFold == true)
		{
			if (Body_Two.Ease.isMove == false && Body_Two.Body_Type == down && Body_Two.IsActivate == true)
			{
				Body_Two.Ease.addTime = 0.1f;
				Body_Two.Ease.maxTime = 1.2f;
				Body_Two.Ease.timeRate = 0.0f;

				//折る
				if (Body_Two.IsFold == false && Body_Two.IsOpen == true && Body_Two.IsAction == false)
				{
					Body_Two.Ease.isMove = true;
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
			IsDownFold = false;
		}
	}

	if (Body_One.IsAction == false && Body_Two.IsAction == false && Body_Three.IsAction == false && leg.FootIsAction == false)
	{
		Player_IsAction = false;
		leg.IsFootUp = false;
	}

	//開く
	if (InputManger::Act1Trigger())
	{
		//左
		if (Body_One.IsFold == true && Body_One.IsAction == false && Body_One.Body_Type == left && Body_One.Overlap == 0 ||
			Body_Three.Body_Type == left && Body_Three.IsFold == true && Body_Three.Overlap == 0)
		{
			Body_One.Ease.addTime = 0.1f;
			Body_One.Ease.maxTime = 1.2f;
			Body_One.Ease.timeRate = 0.0f;

			Body_One.Ease.isMove = true;
			Body_One.IsFold = false;
			Body_One.IsOpen = true;
			Body_One.IsAction = true;

			if (Body_One.FoldCount == 2 || Body_One.IsActivate == false && Body_Three.Body_Type == left && Body_Three.IsFold == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				Body_Three.Ease.isMove = true;
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
		//上・下
		else if (Body_Two.IsFold == true && Body_Two.IsAction == false && Body_Two.Overlap == 0 && IsOpenTwo == true)
		{
			Body_Two.Ease.addTime = 0.1f;
			Body_Two.Ease.maxTime = 1.2f;
			Body_Two.Ease.timeRate = 0.0f;

			Body_Two.Ease.isMove = true;
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
			Body_One.Body_Type == right && Body_One.IsFold == true && Body_One.Overlap == 0)
		{
			Body_Three.Ease.addTime = 0.1f;
			Body_Three.Ease.maxTime = 1.2f;
			Body_Three.Ease.timeRate = 0.0f;

			Body_Three.Ease.isMove = true;
			Body_Three.IsFold = false;
			Body_Three.IsOpen = true;
			Body_Three.IsAction = true;

			if (Body_Three.FoldCount == 2)
			{
				Body_One.Ease.addTime = 0.1f;
				Body_One.Ease.maxTime = 1.2f;
				Body_One.Ease.timeRate = 0.0f;

				Body_One.Ease.isMove = true;
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

	if (Body_One.Body_Type == down || Body_Two.Body_Type == down || Body_Three.Body_Type == down)
	{
		IsDownBody = true;
	}
	else
	{
		IsDownBody = false;
	}
	leg.Update(CenterPosition, (IsDownBody && !(Body_Two.IsFold)), 1);

	if (Body_One.IsActivate == true)
	{
		Body_One.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_One.Update(CenterPosition);
	}
	if (Body_Two.IsActivate == true)
	{
		Body_Two.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_Two.Update(CenterPosition);
	}
	if (Body_Three.IsActivate == true)
	{
		Body_Three.IsHitBody(&CenterPosition, FallSpeed, IsAllFall, IsJump, IsColide);
		Body_Three.Update(CenterPosition);
	}
}

void Player::Draw(int offsetX, int offsetY)
{
	if (leg.FootIsAction == false)
	{
		if (Body_Two.IsActivate == true && CenterPosition.y + 25 < Body_Two.BodyEndPos.y)
		{
			leg.FootLeftUpPosition.y = Body_Two.BodyEndPos.y;
		}
		else
		{
			leg.FootLeftUpPosition.y = CenterPosition.y + 25;
		}
	}

	if (Body_One.IsSlide == false && Body_Two.IsSlide == false && Body_Three.IsSlide == false)
	{
		leg.Draw(IsLeft, IsRight, offsetX, offsetY);
		if (IsLeft)
		{
			DrawExtendGraph(
				static_cast<int>(CenterPosition.x) - 25 + offsetX, static_cast<int>(CenterPosition.y) - 25 + offsetY,
				static_cast<int>(CenterPosition.x) + 25 + offsetX, static_cast<int>(CenterPosition.y) + 25 + offsetY, FaceHandle[Player_IsAction], true);
		}
		if (IsRight)
		{
			DrawExtendGraph(
				static_cast<int>(CenterPosition.x) + 25 + offsetX, static_cast<int>(CenterPosition.y) - 25 + offsetY,
				static_cast<int>(CenterPosition.x) - 25 + offsetX, static_cast<int>(CenterPosition.y) + 25 + offsetY, FaceHandle[Player_IsAction], true);
		}
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
		leg.Draw(IsLeft, IsRight, offsetX, offsetY);
		if (IsLeft)
		{
			DrawExtendGraph(
				static_cast<int>(CenterPosition.x) - 25 + offsetX, static_cast<int>(CenterPosition.y) - 25 + offsetY,
				static_cast<int>(CenterPosition.x) + 25 + offsetX, static_cast<int>(CenterPosition.y) + 25 + offsetY, FaceHandle[Player_IsAction], true);
		}
		if (IsRight)
		{
			DrawExtendGraph(
				static_cast<int>(CenterPosition.x) + 25 + offsetX, static_cast<int>(CenterPosition.y) - 25 + offsetY,
				static_cast<int>(CenterPosition.x) - 25 + offsetX, static_cast<int>(CenterPosition.y) + 25 + offsetY, FaceHandle[Player_IsAction], true);
		}
	}

#pragma region debug
	DrawFormatString(5, 5, WHITE, "AD:左右移動");
	DrawFormatString(5, 25, WHITE, "W:ジャンプ");
	DrawFormatString(5, 45, WHITE, "方向キー:折る・開く");
	DrawFormatString(5, 65, WHITE, "SPACE:開く");
	DrawFormatString(5, 125, WHITE, "Z or X : 左右スライド");
	DrawFormatString(5, 145, WHITE, "C : 上下スライド");
	//DrawFormatString(5, 165, WHITE, "%f", leg.FootLeftUpPosition.y - CenterPosition.y);
	//DrawFormatString(5, 185, WHITE, "fall:%d", IsFaceFall);
	//DrawFormatString(5, 205, WHITE, "isleft:%d", IsLeft);
	//DrawFormatString(5, 225, WHITE, "isright:%d", IsRight);
	//DrawFormatString(5, 245, WHITE, "3_fall:%d", Body_Three.BodyIsFall);
	//DrawFormatString(5, 265, WHITE, "jump:%d", IsJump);
	//DrawFormatString(5, 285, WHITE, "%f", FallSpeed);
	//DrawFormatString(5, 305, WHITE, "IsAllFall:%d", IsFall());
	//DrawFormatString(5, 325, WHITE, "%d", Player_IsAction);
	//DrawFormatString(5, 345, WHITE, "IsInputjump:%d", IsInputjump);
	if (IsGoal == true)
	{
		DrawFormatString(630, 100, YELLOW, "GOAL");
	}
	else
	{
		DrawFormatString(630, 100, YELLOW, "NO GOAL");
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
	else
	{
		Body_One.IsActivate = false;
	}

	if (two == true)
	{
		Body_Two.IsActivate = true;
		Body_Two.Body_Type = two_type;
	}
	else
	{
		Body_Two.IsActivate = false;
	}

	if (three == true)
	{
		Body_Three.IsActivate = true;
		Body_Three.Body_Type = three_type;
	}
	else
	{
		Body_Three.IsActivate = false;
	}

	Body_One.setactivate(CenterPosition);
	Body_Two.setactivate(CenterPosition);
	Body_Three.setactivate(CenterPosition);

	CenterPosition.y += 1;
}

void Player::bodysetup(const unsigned char foldCount[4])
{
	static int bodyTile[3] = { 0 };
	static size_t j = 0;

	j = 0;

	for (size_t i = 0; i < 3; i++)
	{
		bodyTile[i] = -1;

		for (; j < 4; j++)
		{
			if (foldCount[j] != 0)
			{
				bodyTile[i] = j;
				j++;
				break;
			}
		}
	}

	bodysetup(
		bodyTile[0] != -1, bodyTile[0],
		bodyTile[1] != -1, bodyTile[1],
		bodyTile[2] != -1, bodyTile[2]);
}

void Player::IsHitPlayerBody()
{
	//ステージの数
	size_t i = 0;
	//タイルの数
	size_t j = 0;

	//上下左右(プレイヤーの顔)
	int left_mapchip = (int)((CenterPosition.x - 25) - stage->offset.x) / 60;
	int up_mapchip = (int)((CenterPosition.y - 25) - stage->offset.y) / 60;
	int right_mapchip = (int)((CenterPosition.x + 25) - stage->offset.x) / 60;
	int down_mapchip = (int)((CenterPosition.y + 33) - stage->offset.y) / 60;

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

	int FallCount = 0;

	if (CenterPosition.x - 25 <= stage->offset.x)
	{
		CenterPosition.x = 25;
	}
	if (CenterPosition.y - 25 <= stage->offset.y)
	{
		CenterPosition.y = 25;
	}

	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//左上
			if (stage->GetPositionTile({ CenterPosition.x - 25,CenterPosition.y - 25,0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * 60.0f + 25.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * 60.0f + 25.0f;
					}
				}
			}
			//左下
			if (stage->GetPositionTile({ CenterPosition.x - 25,CenterPosition.y + 33,0.0f }, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = (left_mapchip * 60) - (CenterPosition.x - 30);
					BuriedY = ((CenterPosition.y + 29) - 60) - (down_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(down_mapchip * 60) - 33.0f;
						FallCount++;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * 60.0f + 25.0f;
					}
				}
			}
			//右上
			if (stage->GetPositionTile({ CenterPosition.x + 25,CenterPosition.y - 25,0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (up_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = ((CenterPosition.x + 29) - 60) - (right_mapchip * 60);
					BuriedY = (up_mapchip * 60) - (CenterPosition.y - 30);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * 60.0f + 25.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(right_mapchip * 60) - 25.0f;
					}
				}
			}
			//右下
			if (stage->GetPositionTile({ CenterPosition.x + 25,CenterPosition.y + 33,0.0f }, i, j))
			{
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				MapchipPos = (down_mapchip_tile)*stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::BLOCK)
				{
					BuriedX = ((CenterPosition.x + 29) - 60) - (right_mapchip * 60);
					BuriedY = ((CenterPosition.y + 29) - 60) - (down_mapchip * 60);

					if (BuriedX > BuriedY)
					{
						CenterPosition.y = static_cast<float>(down_mapchip * 60) - 33.0f;
						FallCount++;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(right_mapchip * 60) - 25.0f;
					}
				}
			}

			//ゴール判定
			if (stage->GetPositionTile(CenterPosition, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				//左上
				MapchipPos = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[0] = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				MapchipPos_Goal[1] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[2] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//右上
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[0]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//左下
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[1]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//右下
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[2]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				else
				{
					//IsGoal = false;
				}
			}
		}
	}

	if (FallCount > 0)
	{
		IsFaceFall = false;
		//FallSpeed = 0.0f;
	}
	else
	{
		IsFaceFall = true;
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

bool Player::IsFall()
{
	int FallCount = 0;

	if (Body_One.IsActivate == true && Body_One.BodyIsFall == false)
	{
		FallCount++;
	}
	if (Body_Two.IsActivate == true && Body_Two.BodyIsFall == false)
	{
		FallCount++;
	}
	if (Body_Three.IsActivate == true && Body_Three.BodyIsFall == false)
	{
		FallCount++;
	}
	if (IsFaceFall == false)
	{
		FallCount++;
	}

	if (FallCount > 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}
