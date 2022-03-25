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

	Body_One.Init(CenterPosition, left);
	Body_One.BodyColor = YELLOW;

	Body_Two.Init(CenterPosition, up);
	Body_Two.BodyColor = GREEN;

	Body_Three.Init(CenterPosition, right);
	Body_Three.BodyColor = MAGENTA;

	IsFall = false;
}

void Player::Update(Stage& stage)
{
	//左右移動
	if (InputManger::Right() && !InputManger::Act1())
	{
		CenterPosition.x += 2.0f;
	}
	if (InputManger::Left() && !InputManger::Act1())
	{
		CenterPosition.x -= 2.0f;
	}

	//ジャンプ
	if (InputManger::Up() && !InputManger::Act1() && IsJump == false && IsFall == false)
	{
		IsJump = true;
		IsFall = true;
		FallSpeed = -8.0f;
	}

	if (IsJump == true)
	{
		CenterPosition.y += FallSpeed;
		FallSpeed += 0.5f;

		if (FallSpeed > 0)
		{
			IsJump = false;
		}
	}

	//落下判定
	if (IsFall == true && IsJump == false)
	{
		CenterPosition.y += FallSpeed;

		if (FallSpeed <= 8.0)
		{
			FallSpeed += 0.1f;
		}
	}

	//折る
	//左
	if (InputManger::SubLeftTrigger() && Body_One.Ease.ismove == false && Body_One.Body_Type == left)
	{
		Body_One.Ease.addtime = 0.1f;
		Body_One.Ease.maxtime = 1.5f;
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
			Body_Three.Ease.maxtime = 1.5f;
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
	if (InputManger::SubUpTrigger() && Body_Two.Ease.ismove == false && Body_Two.Body_Type == up)
	{
		Body_Two.Ease.addtime = 0.1f;
		Body_Two.Ease.maxtime = 1.5f;
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
	if (InputManger::SubRightTrigger() && Body_Three.Ease.ismove == false && Body_Three.Body_Type == right)
	{
		Body_Three.Ease.addtime = 0.1f;
		Body_Three.Ease.maxtime = 1.5f;
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
			Body_One.Ease.maxtime = 1.5f;
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
	if (InputManger::SubDownTrigger() && Body_Two.Ease.ismove == false && Body_Two.Body_Type == down)
	{
		Body_Two.Ease.addtime = 0.1f;
		Body_Two.Ease.maxtime = 1.5f;
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
			Body_One.Ease.maxtime = 1.5f;
			Body_One.Ease.timerate = 0.0f;

			Body_One.Ease.ismove = true;
			Body_One.IsFold = false;
			Body_One.IsOpen = true;
			Body_One.IsAction = true;

			if (Body_One.FoldCount == 2)
			{
				Body_Three.Ease.addtime = 0.1f;
				Body_Three.Ease.maxtime = 1.5f;
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
			Body_Two.Ease.maxtime = 1.5f;
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
			Body_Three.Ease.maxtime = 1.5f;
			Body_Three.Ease.timerate = 0.0f;

			Body_Three.Ease.ismove = true;
			Body_Three.IsFold = false;
			Body_Three.IsOpen = true;
			Body_Three.IsAction = true;

			if (Body_Three.FoldCount == 2)
			{
				Body_One.Ease.addtime = 0.1f;
				Body_One.Ease.maxtime = 1.5f;
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

	if (Body_One.IsActivate == true)
	{
		Body_One.IsHitBody(stage, CenterPosition, Body_Two, Body_Three, IsFall, IsJump);
		Body_One.Update(CenterPosition);
	}
	if (Body_Two.IsActivate == true)
	{
		Body_Two.IsHitBody(stage, CenterPosition, Body_One, Body_Three, IsFall, IsJump);
		Body_Two.Update(CenterPosition);
	}
	if (Body_Three.IsActivate == true)
	{
		Body_Three.IsHitBody(stage, CenterPosition, Body_One, Body_Two, IsFall, IsJump);
		Body_Three.Update(CenterPosition);
	}
}

void Player::Draw(int offsetX, int offsetY)
{
	if (Body_One.IsSlide == false && Body_Two.IsSlide == false && Body_Three.IsSlide == false)
	{
		DrawBox(CenterPosition.x - 30 + offsetX, CenterPosition.y - 30 + offsetY, CenterPosition.x + 30 + offsetX, CenterPosition.y + 30 + offsetY, GetColor(255, 0, 0), true);
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
			DrawBox(CenterPosition.x + 30 + offsetX, CenterPosition.y - 30 + offsetY, Body_Three.BodyStartPos.x + offsetX, Body_Three.BodyEndPos.y + offsetY, Body_One.BodyColor, true);
		}

		DrawBox(CenterPosition.x - 30 + offsetX, CenterPosition.y - 30 + offsetY, CenterPosition.x + 30 + offsetX, CenterPosition.y + 30 + offsetY, GetColor(255, 0, 0), true);
	}

	DrawLine(0, FloorHeight + offsetY, 1280, FloorHeight + offsetY, WHITE, true);

#pragma region UI
	DrawFormatString(0, 0, WHITE, "AD:左右移動");
	DrawFormatString(0, 20, WHITE, "W:ジャンプ");
	DrawFormatString(0, 40, WHITE, "←↑→:折る・開く");
	DrawFormatString(0, 60, WHITE, "SPACE:開く");
	//DrawFormatString(0, 80, WHITE, "重なっている枚数\n左：%d\n上：%d\n右：%d", body_one.overlap, body_two.overlap, body_three.overlap);
	//DrawFormatString(0, 160, WHITE, "左右スライド：Z or X\n上下スライド：C or V");
	//DrawFormatString(0, 200, WHITE, "%f   %f   %f", body_one.bodystartpos.x, body_one.bodyendpos.x, center_position.x);
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
}
