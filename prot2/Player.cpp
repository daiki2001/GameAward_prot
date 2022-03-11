#include "Player.h"
#include <DxLib.h>
#include "Input.h"
#include "Stage.h"
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
	//if (Input::isKeyTrigger(KEY_INPUT_A) && Input::isKeyTrigger(KEY_INPUT_LEFT))
	//{
	//	pos.x -= 1.0f;
	//}
	//if (Input::isKeyTrigger(KEY_INPUT_D) && Input::isKeyTrigger(KEY_INPUT_RIGHT))
	//{
	//	pos.x += 1.0f;
	//}
}

void Player::Draw()
{
	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, WHITE, true);
	DrawBox(int(pos.x) * Stage::blockSize, int(pos.y) * Stage::blockSize,
		(int(pos.x) + 1) * Stage::blockSize, (int(pos.y) + 1) * Stage::blockSize, BLACK, false);
}
