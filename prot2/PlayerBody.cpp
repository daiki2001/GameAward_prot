#include "PlayerBody.h"
#include <DxLib.h>

PlayerBody::PlayerBody() :
	Isactivate(false),
	body_type(),
	Isfold(false),
	Isopen(true),
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
	//開いているとき
	if (Isfold == false && Isopen == true)
	{
		if (ease.ismove == true && Isslide == false)
		{
			ease.addtime += ease.maxtime / 60.0f;
			ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

			if (body_type == left)
			{
				bodyendpos = { center.x - static_cast<float>(30 + 60 * (bodydistance - 1)), center.y + 30.0f, 0.0f };
				bodystartpos.x = ease.easeout(bodyendpos.x + 60, bodyendpos.x - 60, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60;
			}
			if (body_type == up)
			{
				bodyendpos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
				bodystartpos.y = ease.easeout(bodyendpos.y + 60, bodyendpos.y - 60, ease.timerate);
				bodystartpos.x = bodyendpos.x - 60;
			}
			if (body_type == right)
			{
				bodystartpos = { center.x + static_cast<float>(30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f };
				bodyendpos.x = ease.easeout(bodystartpos.x - 60, bodystartpos.x + 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60;
			}
			if (body_type == down)
			{
				bodystartpos = { center.x + 30.0f, center.y + 30.0f, 0.0f };
				bodyendpos.y = ease.easeout(bodystartpos.y + 60, bodystartpos.y - 60, ease.timerate);
				bodyendpos.x = bodystartpos.x - 60;
			}

			if (ease.timerate >= 1.0f)
			{
				ease.ismove = false;
				Isaction = false;
			}
		}
		else if (Isaction == false)
		{
			if (body_type == left)
			{
				bodystartpos = { center.x - static_cast<float>(30 + bodydistance * 60), center.y - 30.0f, 0.0f };
			}
			if (body_type == up)
			{
				bodystartpos = { center.x - 30.0f, center.y - 90.0f, 0.0f };
			}
			if (body_type == right)
			{
				bodystartpos = { center.x + static_cast<float>(30 + (bodydistance - 1) * 60), center.y - 30.0f, 0.0f };
			}
			if (body_type == down)
			{
				bodystartpos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
			}

			bodyendpos = { bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f };
		}
	}
	//折るとき
	else if (Isfold == true && Isopen == false)
	{
		if (ease.ismove == true && Isslide == false)
		{
			ease.addtime += ease.maxtime / 60.0f;
			ease.timerate = min(ease.addtime / ease.maxtime, 1.0f);

			if (body_type == left)
			{
				bodyendpos = { center.x - static_cast<float>(30 + 60 * (bodydistance - 1)), center.y + 30.0f, 0.0f };
				bodystartpos.x = ease.easeout(bodyendpos.x - 60, bodyendpos.x + 60, ease.timerate);
				bodystartpos.y = bodyendpos.y - 60;
			}
			if (body_type == up)
			{
				bodyendpos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
				bodystartpos.y = ease.easeout(bodyendpos.y - 60, bodyendpos.y + 60, ease.timerate);
				bodystartpos.x = bodyendpos.x - 60;
			}
			if (body_type == right)
			{
				bodystartpos = { center.x + static_cast<float>(30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f };
				bodyendpos.x = ease.easeout(bodystartpos.x + 60, bodystartpos.x - 60, ease.timerate);
				bodyendpos.y = bodystartpos.y + 60;
			}
			if (body_type == down)
			{
				bodystartpos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
				bodyendpos.y = ease.easeout(bodystartpos.y + 60, bodystartpos.y - 60, ease.timerate);
				bodyendpos.x = bodystartpos.x + 60;
			}

			if (ease.timerate >= 1.0f)
			{
				ease.ismove = false;
				Isaction = false;
			}
		}
		else
		{
			if (body_type == left)
			{
				bodyendpos = { center.x - static_cast<float>(30 + 60 * (bodydistance - 1)), center.y + 30.0f, 0.0f };
				bodystartpos = { bodyendpos.x + 60.0f, bodyendpos.y - 60.0f, 0.0f };
			}
			else if (body_type == up)
			{
				bodystartpos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
				bodyendpos = { bodystartpos.x + 60.0f, bodystartpos.y - 60.0f, 0.0f };
			}
			else if (body_type == right)
			{
				bodystartpos = { center.x + static_cast<float>(30 + 60 * (bodydistance - 1)), center.y - 30.0f, 0.0f };
				bodyendpos = { bodystartpos.x - 60.0f, bodystartpos.y + 60.0f, 0.0f };
			}
			else if (body_type == down)
			{
				bodystartpos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
				bodyendpos = { bodystartpos.x + 60.0f, bodystartpos.y - 60.0f, 0.0f };
			}
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
			bodystartpos = { ease.easeout(center.x - 90, center.x + 30, ease.timerate), center.y - 30.0f, 0.0f };
			bodyendpos = { bodystartpos.x + static_cast<float>(-120 * Isfold + 60), center.y + 30.0f, 0.0f };
		}
		else if (body_type == right)
		{
			bodystartpos = { ease.easeout(center.x + 30, center.x - 90, ease.timerate), center.y - 30.0f, 0.0f };
			bodyendpos = { bodystartpos.x + static_cast<float>(-120 * Isfold + 60), center.y + 30.0f, 0.0f };
		}
		else if (body_type == up)
		{
			bodystartpos = { center.x - 30.0f, ease.easeout(center.y - 90, center.y + 30, ease.timerate), 0.0f };
			bodyendpos = { center.x + 30.0f, bodystartpos.y + static_cast<float>(-120 * Isfold + 60), 0.0f };
		}
		else if (body_type == down)
		{
			bodystartpos = { center.x - 30.0f, ease.easeout(center.y + 30, center.y - 90, ease.timerate), 0.0f };
			bodyendpos = { center.x + 30.0f, bodystartpos.y + static_cast<float>(-120 * Isfold + 60), 0.0f };
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
				bodystartpos = { ease.easeout(center.x - 90, center.x - 150, ease.timerate), center.y - 30.0f, 0.0f };
			}
			else
			{
				bodystartpos = { ease.easeout(center.x - 150, center.x - 90, ease.timerate), center.y - 30.0f, 0.0f };
			}
		}
		else if (body_type == right)
		{
			if (slidepat == -1)
			{
				bodystartpos = { ease.easeout(center.x + 90, center.x + 30, ease.timerate), center.y - 30.0f, 0.0f };
			}
			else
			{
				bodystartpos = { ease.easeout(center.x + 30, center.x + 90, ease.timerate), center.y - 30.0f, 0.0f };
			}
		}

		bodyendpos = { bodystartpos.x + static_cast<float>(-120 * Isfold + 60), center.y + 30.0f, 0.0f };

		if (ease.timerate >= 1.0f)
		{
			ease.ismove = false;
			Isaction = false;
			Isslide = false;
		}
	}
}

void PlayerBody::Draw()
{
	if (Isactivate == true)
	{
		DrawBox(static_cast<int>(bodystartpos.x), static_cast<int>(bodystartpos.y),
			static_cast<int>(bodyendpos.x), static_cast<int>(bodyendpos.y), bodycolor, true);
	}
}

void PlayerBody::setactivate(Vector3 center)
{
	if (Isactivate == true)
	{
		Isfold = false;
		Isopen = true;
		bodydistance = 1;

		if (body_type == left)
		{
			bodystartpos = { center.x - 90.0f, center.y - 30.0f, 0.0f };
		}
		else if (body_type == right)
		{
			bodystartpos = { center.x + 30.0f, center.y - 30.0f, 0.0f };
		}
		else if (body_type == up)
		{
			bodystartpos = { center.x - 30.0f, center.y - 90.0f, 0.0f };
		}
		else if (body_type == down)
		{
			bodystartpos = { center.x - 30.0f, center.y + 30.0f, 0.0f };
		}

		bodyendpos = { bodystartpos.x + 60.0f, bodystartpos.y + 60.0f, 0.0f };
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
