#include "Player.h"
#include <DxLib.h>
#include "Stage.h"
#include "InputManger.h"
#include "Colors.h"

Player* Player::Get()
{
	static Player instance = {};
	return &instance;
}

Player::Player() :
	pos{}
{
	Init();
}

Player::~Player()
{
}

void Player::Init()
{
	pos = Stage::GetStartPlayerPos();
}

void Player::Updata()
{
	if (InputManger::LeftTrigger())
	{
		pos.x -= 1.0f;
	}
	if (InputManger::RightTrigger())
	{
		pos.x += 1.0f;
	}
}

void Player::Draw()
{
	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, WHITE, true);
	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, BLACK, false);
}
