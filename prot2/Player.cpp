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
	center_position{},
	body_one{},
	body_two{},
	body_three{}
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	center_position.x = static_cast<float>(Stage::GetStartPlayerPosX() * Stage::blockSize + Stage::blockSize / 2);
	center_position.y = static_cast<float>(Stage::GetStartPlayerPosY() * Stage::blockSize + Stage::blockSize / 2);

	//for (i = 0; i < sizeof(tile) / sizeof(tile[0]); i++) tile[i] = 0;

	body_one.Init(center_position, left);
	body_one.bodycolor = YELLOW;

	body_two.Init(center_position, up);
	body_two.bodycolor = GREEN;

	body_three.Init(center_position, right);
	body_three.bodycolor = MAGENTA;
}

void Player::Updata()
{
	//���E�ړ�
	if (InputManger::Right() && !InputManger::Act1())
	{
		center_position.x += 2.0f;
	}
	if (InputManger::Left() && !InputManger::Act1())
	{
		center_position.x -= 2.0f;
	}

	//�W�����v
	if (InputManger::Up() && !InputManger::Act1() && IsJump == false)
	{
		IsJump = true;
		fallspeed = 8.0f;
	}

	if (IsJump == true)
	{
		center_position.y -= fallspeed;
		fallspeed -= 0.5f;

		if (center_position.y >= floorHeight - 30)
		{
			center_position.y = floorHeight - 30;
			IsJump = false;
		}
	}

	//��������
	if (center_position.y < floorHeight - 31 && IsJump == false)
	{
		center_position.y += fallspeed;

		if (fallspeed <= 5.0)
		{
			fallspeed += 0.1f;
		}
	}

	//�܂�E�J��
	//��
	if (InputManger::SubLeftTrigger() && body_one.ease.ismove == false && body_one.body_type == left)
	{
		body_one.ease.addtime = 0.1f;
		body_one.ease.maxtime = 1.5f;
		body_one.ease.timerate = 0.0f;

		//�܂�
		if (body_one.Isfold == false && body_one.Isopen == true && body_one.Isaction == false)
		{
			body_one.ease.ismove = true;
			body_one.Isfold = true;
			body_one.Isopen = false;
			body_one.Isaction = true;

			if (body_two.Isfold == true)
			{
				body_two.overlap++;
			}
			if (body_three.Isfold == true)
			{
				body_three.overlap++;
			}
		}
	}
	//��
	if (InputManger::SubUpTrigger() && body_two.ease.ismove == false && body_two.body_type == up)
	{
		body_two.ease.addtime = 0.1f;
		body_two.ease.maxtime = 1.5f;
		body_two.ease.timerate = 0.0f;

		//�܂�
		if (body_two.Isfold == false && body_two.Isopen == true && body_two.Isaction == false)
		{
			body_two.ease.ismove = true;
			body_two.Isfold = true;
			body_two.Isopen = false;
			body_two.Isaction = true;

			if (body_one.Isfold == true)
			{
				body_one.overlap++;
			}
			if (body_three.Isfold == true)
			{
				body_three.overlap++;
			}
		}
	}
	//�E
	if (InputManger::SubRightTrigger() && body_three.ease.ismove == false && body_three.body_type == right)
	{
		body_three.ease.addtime = 0.1f;
		body_three.ease.maxtime = 1.5f;
		body_three.ease.timerate = 0.0f;

		//�܂�
		if (body_three.Isfold == false && body_three.Isopen == true && body_three.Isaction == false)
		{
			body_three.ease.ismove = true;
			body_three.Isfold = true;
			body_three.Isopen = false;
			body_three.Isaction = true;

			if (body_one.Isfold == true)
			{
				body_one.overlap++;
			}
			if (body_two.Isfold == true)
			{
				body_two.overlap++;
			}
		}


	}
	//��
	if (InputManger::SubDownTrigger() && body_two.ease.ismove == false && body_two.body_type == down)
	{
		body_two.ease.addtime = 0.1f;
		body_two.ease.maxtime = 1.5f;
		body_two.ease.timerate = 0.0f;

		//�܂�
		if (body_two.Isfold == false && body_two.Isopen == true && body_two.Isaction == false)
		{
			body_two.ease.ismove = true;
			body_two.Isfold = true;
			body_two.Isopen = false;
			body_two.Isaction = true;

			if (body_one.Isfold == true)
			{
				body_one.overlap++;
			}
			if (body_three.Isfold == true)
			{
				body_three.overlap++;
			}
		}
	}

	//�J��
	if (InputManger::Act1Trigger())
	{
		//��
		if (body_one.Isfold == true && body_one.Isopen == false && body_one.Isaction == false && body_one.overlap == 0)
		{
			body_one.ease.addtime = 0.1f;
			body_one.ease.maxtime = 1.5f;
			body_one.ease.timerate = 0.0f;

			body_one.ease.ismove = true;
			body_one.Isfold = false;
			body_one.Isopen = true;
			body_one.Isaction = true;

			if (body_two.Isfold == true)
			{
				body_two.overlap--;
			}
			if (body_three.Isfold == true || (body_three.Isopen == true && body_three.body_type == left))
			{
				body_three.overlap--;
			}
		}
		//��
		else if (body_two.Isfold == true && body_two.Isopen == false && body_two.Isaction == false && body_two.overlap == 0)
		{
			body_two.ease.addtime = 0.1f;
			body_two.ease.maxtime = 1.5f;
			body_two.ease.timerate = 0.0f;

			body_two.ease.ismove = true;
			body_two.Isfold = false;
			body_two.Isopen = true;
			body_two.Isaction = true;

			if (body_one.Isfold == true)
			{
				body_one.overlap--;
			}
			if (body_three.Isfold == true)
			{
				body_three.overlap--;
			}
		}
		//�E
		else if (body_three.Isfold == true && body_three.Isopen == false && body_three.Isaction == false && body_three.overlap == 0)
		{
			body_three.ease.addtime = 0.1f;
			body_three.ease.maxtime = 1.5f;
			body_three.ease.timerate = 0.0f;

			body_three.ease.ismove = true;
			body_three.Isfold = false;
			body_three.Isopen = true;
			body_three.Isaction = true;

			if (body_one.Isfold == true)
			{
				body_one.overlap--;
			}
			if (body_two.Isfold == true || (body_one.Isopen == true && body_one.body_type == right))
			{
				body_two.overlap--;
			}
		}
	}


	//�̂̃X���C�h
	//���ɃX���C�h
	if (Input::isKeyTrigger(KEY_INPUT_Z) && body_one.bodydistance < 2 && body_one.Isaction == false)
	{
		if (body_one.body_type == right)
		{
			body_one.setslide(-1, 2);
			body_three.bodydistance = 1;
			body_three.setslide(-1, 1);
		}
		if (body_one.body_type == left && body_one.bodydistance == 1 && body_three.Isfold == false)
		{
			if (body_one.Isfold == true)
			{
				body_three.overlap = 1;
				body_one.bodydistance = 2;
				body_one.setslide(-1, 1);
				body_three.setslide(-1, 2);
			}
			else
			{
				body_one.bodydistance = 2;
				body_one.setslide(-1, 1);
				body_three.setslide(-1, 2);
			}
		}
	}
	//�E�ɃX���C�h
	if (Input::isKeyTrigger(KEY_INPUT_X) && body_three.bodydistance < 2 && body_three.Isaction == false)
	{
		if (body_three.body_type == left)
		{
			body_three.setslide(1, 2);
			body_one.bodydistance = 1;
			body_one.setslide(1, 1);
		}
		if (body_three.body_type == right && body_three.bodydistance == 1 && body_one.Isfold == false)
		{
			if (body_three.Isfold == true)
			{
				body_one.overlap = 1;
				body_three.bodydistance = 2;
				body_three.setslide(1, 1);
				body_one.setslide(1, 2);
			}
			else
			{
				body_three.bodydistance = 2;
				body_three.setslide(1, 1);
				body_one.setslide(1, 2);
			}
		}
	}
	//�㉺�̃X���C�h
	if (Input::isKeyTrigger(KEY_INPUT_C) && body_two.Isaction == false)
	{
		if (body_two.body_type == up)
		{
			body_two.setslide(1, 2);
		}
		else
		{
			body_two.setslide(-1, 2);
		}
	}

	if (body_one.Isactivate == true)
	{
		body_one.Update(center_position);
	}
	if (body_two.Isactivate == true)
	{
		body_two.Update(center_position);
	}
	if (body_three.Isactivate == true)
	{
		body_three.Update(center_position);
	}
}

void Player::Draw(int offsetX, int offsetY)
{
	if (body_one.Isslide == false && body_two.Isslide == false && body_three.Isslide == false)
	{
		DrawBox(center_position.x - 30 + offsetX, center_position.y - 30 + offsetY, center_position.x + 30 + offsetX, center_position.y + 30 + offsetY, GetColor(255, 0, 0), true);
	}

#pragma region �d�Ȃ��Ă��閇�����Ƃɏ��Ԃɕ`��
	if (body_one.overlap == 2)
	{
		body_one.Draw(offsetX, offsetY);
	}
	if (body_two.overlap == 2)
	{
		body_two.Draw(offsetX, offsetY);
	}
	if (body_three.overlap == 2)
	{
		body_three.Draw(offsetX, offsetY);
	}

	if (body_one.overlap == 1)
	{
		body_one.Draw(offsetX, offsetY);
	}
	if (body_two.overlap == 1)
	{
		body_two.Draw(offsetX, offsetY);
	}
	if (body_three.overlap == 1)
	{
		body_three.Draw(offsetX, offsetY);
	}

	if (body_one.overlap == 0)
	{
		body_one.Draw(offsetX, offsetY);
	}
	if (body_two.overlap == 0)
	{
		body_two.Draw(offsetX, offsetY);
	}
	if (body_three.overlap == 0)
	{
		body_three.Draw(offsetX, offsetY);
	}
#pragma endregion �d�Ȃ��Ă��閇�����Ƃɏ��Ԃɕ`��

	if (body_one.Isslide == true || body_two.Isslide == true || body_three.Isslide == true)
	{
		if (body_three.slide_dis == 2)
		{
			//DrawBox(center_position.x - 30 + offsetX, center_position.y - 30 + offsetY, body_one.bodyendpos.x + offsetX, body_one.bodyendpos.y + offsetY, body_three.bodycolor, true);
		}
		if (body_one.slide_dis == 2)
		{
			DrawBox(center_position.x + 30 + offsetX, center_position.y - 30 + offsetY, body_three.bodystartpos.x + offsetX, body_three.bodyendpos.y + offsetY, body_one.bodycolor, true);
		}

		DrawBox(center_position.x - 30 + offsetX, center_position.y - 30 + offsetY, center_position.x + 30 + offsetX, center_position.y + 30 + offsetY, GetColor(255, 0, 0), true);
	}

	DrawLine(0, floorHeight + offsetY, 1280, floorHeight + offsetY, WHITE, true);

#pragma region UI
	DrawFormatString(0, 0, WHITE, "AD:���E�ړ�");
	DrawFormatString(0, 20, WHITE, "W:�W�����v");
	DrawFormatString(0, 40, WHITE, "������:�܂�E�J��");
	DrawFormatString(0, 60, WHITE, "SPACE:�J��");
	//DrawFormatString(0, 80, WHITE, "�d�Ȃ��Ă��閇��\n���F%d\n��F%d\n�E�F%d", body_one.overlap, body_two.overlap, body_three.overlap);
	//DrawFormatString(0, 160, WHITE, "���E�X���C�h�FZ or X\n�㉺�X���C�h�FC or V");
	//DrawFormatString(0, 200, WHITE, "%f   %f   %f", body_one.bodystartpos.x, body_one.bodyendpos.x, center_position.x);
#pragma endregion
}

void Player::bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type)
{
	if (one == true)
	{
		body_one.Isactivate = true;
		body_one.body_type = one_type;
	}

	if (two == true)
	{
		body_two.Isactivate = true;
		body_two.body_type = two_type;
	}

	if (three == true)
	{
		body_three.Isactivate = true;
		body_three.body_type = three_type;
	}

	body_one.setactivate(center_position);
	body_two.setactivate(center_position);
	body_three.setactivate(center_position);
}
