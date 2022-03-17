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
	pos{},
	tile{}
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	pos.x = static_cast<float>(Stage::GetStartPlayerPosX());
	pos.y = static_cast<float>(Stage::GetStartPlayerPosY());

	for (i = 0; i < sizeof(tile) / sizeof(tile[0]); i++) tile[i] = 0;
}

void Player::Updata()
{
	if (!InputManger::Act1())
	{
		if (InputManger::UpTrigger())
		{
			pos.y -= 1.0f;
		}
		if (InputManger::DownTrigger())
		{
			pos.y += 1.0f;
		}
		if (InputManger::LeftTrigger())
		{
			pos.x -= 1.0f;
		}
		if (InputManger::RightTrigger())
		{
			pos.x += 1.0f;
		}
	}
}

void Player::Draw()
{
	static size_t j = 0;

	static int drawPosX = 0, drawPosY = 0;

	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, WHITE, true);
	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, BLACK, false);

	for (i = 0; i < sizeof(tile) / sizeof(tile[0]); i++)
	{
		for (j = 0; j < static_cast<size_t>(tile[i]); j++)
		{
			switch (i)
			{
			case 0:
				drawPosX = static_cast<int>(pos.x);
				drawPosY = static_cast<int>(pos.y - (j + 1));
				break;
			case 1:
				drawPosX = static_cast<int>(pos.x);
				drawPosY = static_cast<int>(pos.y + (j + 1));
				break;
			case 2:
				drawPosX = static_cast<int>(pos.x - (j + 1));
				drawPosY = static_cast<int>(pos.y);
				break;
			case 3:
				drawPosX = static_cast<int>(pos.x + (j + 1));
				drawPosY = static_cast<int>(pos.y);
				break;
			default:
				break;
			}

			DrawBox(drawPosX * Stage::blockSize, drawPosY * Stage::blockSize,
				(drawPosX + 1) * Stage::blockSize, (drawPosY + 1) * Stage::blockSize, WHITE, true);
			DrawBox(drawPosX * Stage::blockSize, drawPosY * Stage::blockSize,
				(drawPosX + 1) * Stage::blockSize, (drawPosY + 1) * Stage::blockSize, BLACK, false);
		}
	}
}
