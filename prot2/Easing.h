#pragma once

class Easing
{
public: //�ÓI�����o�֐�
	static float easeout(const float start, const float end, const float time);

public: //�����o�֐�
	Easing();
	~Easing();

public: //�����o�ϐ�
	float maxtime;
	float timerate;
	float addtime;

	bool ismove;
};
