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
	//���E�ړ�
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

	//�W�����v���͂ł��邩�ǂ���
	if (IsJump == false && IsFall() == false)
	{
		IsInputjump = true;
		FallSpeed = 0.0f;
	}
	else
	{
		IsInputjump = false;
	}

	//�W�����v
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

	//��������
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

				//�܂�
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
			IsLeftFold = false;
		}
		if (IsUpFold == true)
		{
			if (Body_Two.Ease.isMove == false && Body_Two.Body_Type == up && Body_Two.IsActivate == true)
			{
				Body_Two.Ease.addTime = 0.1f;
				Body_Two.Ease.maxTime = 1.2f;
				Body_Two.Ease.timeRate = 0.0f;

				//�܂�
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
			if (Body_Three.Ease.isMove == false && Body_Three.Body_Type == right && Body_Three.IsActivate == true)
			{
				Body_Three.Ease.addTime = 0.1f;
				Body_Three.Ease.maxTime = 1.2f;
				Body_Three.Ease.timeRate = 0.0f;

				//�܂�
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

				//�܂�
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

	//�J��
	if (InputManger::Act1Trigger())
	{
		//��
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

			if (Body_One.FoldCount == 2)
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
		//��E��
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
		//�E
		else if (Body_Three.IsFold == true && Body_Three.IsAction == false && Body_Three.Body_Type == right && Body_Three.Overlap == 0 ||
			Body_One.Body_Type == right && Body_One.IsFold == true)
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

	//�̂̃X���C�h
	//���ɃX���C�h
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
	//�E�ɃX���C�h
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
	//�㉺�̃X���C�h
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
	if (IsDownBody == true && Body_Two.IsFold == false)
	{
		//PlayerFoot.FootLeftUpPosition.y = CenterPosition.y + 60;
	}

	if (Body_One.IsSlide == false && Body_Two.IsSlide == false && Body_Three.IsSlide == false)
	{
		leg.Draw(offsetX, offsetY, IsLeft, IsRight);
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

#pragma region �d�Ȃ��Ă��閇�����Ƃɏ��Ԃɕ`��
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
#pragma endregion �d�Ȃ��Ă��閇�����Ƃɏ��Ԃɕ`��

	if (Body_One.IsSlide == true || Body_Two.IsSlide == true || Body_Three.IsSlide == true)
	{
		leg.Draw(offsetX, offsetY, IsLeft, IsRight);
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
	DrawFormatString(0, 0, WHITE, "AD:���E�ړ�");
	DrawFormatString(0, 20, WHITE, "W:�W�����v");
	DrawFormatString(0, 40, WHITE, "������:�܂�");
	DrawFormatString(0, 60, WHITE, "SPACE:�J��");
	DrawFormatString(0, 120, WHITE, "%f", CenterPosition.y);
	DrawFormatString(0, 140, WHITE, "%f", leg.FootLeftUpPosition.y);
	DrawFormatString(0, 160, WHITE, "%f", static_cast<double>(leg.FootLeftUpPosition.y) - static_cast<double>(CenterPosition.y));
	//DrawFormatString(0, 180, WHITE, "fall:%d", IsFaceFall);
	DrawFormatString(0, 200, WHITE, "isleft:%d", IsLeft);
	DrawFormatString(0, 220, WHITE, "isright:%d", IsRight);
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
	//�X�e�[�W�̐�
	size_t i = 0;
	//�^�C���̐�
	size_t j = 0;

	//�㉺���E(�v���C���[�̊�)
	int left_mapchip = (int)((CenterPosition.x - 25) - stage->offset.x) / 60;
	int up_mapchip = (int)((CenterPosition.y - 25) - stage->offset.y) / 60;
	int right_mapchip = (int)((CenterPosition.x + 25) - stage->offset.x) / 60;
	int down_mapchip = (int)((CenterPosition.y + 33) - stage->offset.y) / 60;

	//�^�C�����̃}�b�v�`�b�v���W
	int left_mapchip_tile;
	int up_mapchip_tile;
	int right_mapchip_tile;
	int down_mapchip_tile;
	//�}�b�v�`�b�v�̏ꏊ(����)
	int MapchipPos = 0;

	//�}�b�v�`�b�v�̏ꏊ(�S�[���p)
	int MapchipPos_Goal[3];

	//�����o�����������߂邽�߂̋���
	float BuriedX = 0;
	float BuriedY = 0;

	int FallCount = 0;

	if (CenterPosition.x - 30 <= stage->offset.x)
	{
		CenterPosition.x = 30;
	}
	if (CenterPosition.y - 30 <= stage->offset.y)
	{
		CenterPosition.y = 30;
	}

	for (i = 0; i < stage->GetStageDataSize(); i++)
	{
		for (j = 0; j < stage->GetStageTileDataSize(i); j++)
		{
			//����
			if (stage->GetPositionTile({ CenterPosition.x - 25,CenterPosition.y - 30,0.0f }, i, j))
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
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * 60.0f + 30.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * 60.0f + 30.0f;
					}
				}
			}
			//����
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
						CenterPosition.x = static_cast<float>(left_mapchip + 1) * 60.0f + 30.0f;
					}
				}
			}
			//�E��
			if (stage->GetPositionTile({ CenterPosition.x + 25,CenterPosition.y - 30,0.0f }, i, j))
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
						CenterPosition.y = static_cast<float>(up_mapchip + 1) * 60.0f + 30.0f;
					}
					else if (BuriedX < BuriedY)
					{
						CenterPosition.x = static_cast<float>(right_mapchip * 60) - 30.0f;
					}
				}
			}
			//�E��
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
						CenterPosition.x = static_cast<float>(right_mapchip * 60) - 30.0f;
					}
				}
			}

			//�S�[������
			if (stage->GetPositionTile(CenterPosition, i, j))
			{
				left_mapchip_tile = left_mapchip % stage->GetStageTileWidth(i, j);
				up_mapchip_tile = up_mapchip % stage->GetStageTileHeight(i, j);
				right_mapchip_tile = right_mapchip % stage->GetStageTileWidth(i, j);
				down_mapchip_tile = down_mapchip % stage->GetStageTileHeight(i, j);

				//����
				MapchipPos = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[0] = up_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);
				MapchipPos_Goal[1] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (left_mapchip_tile);
				MapchipPos_Goal[2] = down_mapchip_tile * stage->GetStageTileWidth(i, j) + (right_mapchip_tile);

				if (stage->GetStageMapchip(i, j, MapchipPos) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//�E��
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[0]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//����
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[1]) == MapchipData::GOAL)
				{
					IsGoal = true;
				}
				//�E��
				else if (stage->GetStageMapchip(i, j, MapchipPos_Goal[2]) == MapchipData::GOAL)
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
