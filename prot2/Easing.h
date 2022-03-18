#pragma once

class Easing
{
public: //静的メンバ関数
	static float easeout(const float start, const float end, const float time);

public: //メンバ関数
	Easing();
	~Easing();

public: //メンバ変数
	float maxtime;
	float timerate;
	float addtime;

	bool ismove;
};
