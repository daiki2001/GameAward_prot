#pragma once

class Easing
{
public: //�ÓI�����o�֐�
	static float easeout(const float start, const float end, const float time);

public: //�����o�֐�
	Easing();
	~Easing();

public: //�����o�ϐ�
	float maxTime;
	float timeRate;
	float addTime;

	bool isMove;
};
