#pragma once

class Easing
{
public: //静的メンバ関数
	static float easeout(const float start, const float end, const float time);

public: //メンバ関数
	Easing();
	~Easing();

public: //メンバ変数
	float maxTime;
	float timeRate;
	float addTime;

	bool isMove;
};
