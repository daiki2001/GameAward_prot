#include "PlayerBody.h"
#include <DxLib.h>

PlayerBody::PlayerBody() :
	Isfold(false),
	Isopen(true),
	Isslide(false),
	slidepat(0),
	Isaction(false),
	overlap(0),
	bodydistance(1)
{
}

PlayerBody::~PlayerBody()
{
}

void PlayerBody::Init(Vector3 position, bodytype number)
{
}

void PlayerBody::Update(Vector3 center)
{
}

void PlayerBody::setslide(int slidepat, Vector3 startpos)
{
}

void PlayerBody::Draw()
{
}
