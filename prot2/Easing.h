#pragma once
#include <vector>
#include "Vector3.h"

class Easing
{
public: //�ÓI�����o�֐�
	static float easeout(const float start, const float end, const float time);
	static Vector3 SplineCurve(const std::vector<Vector3>& points, const size_t& startIndex, const float time);
	static Vector3 SplineLoop(const std::vector<Vector3>& points, const size_t& startIndex, const float time);

public: //�����o�֐�
	Easing();
	~Easing();

public: //�����o�ϐ�
	float maxTime;
	float timeRate;
	float addTime;

	bool isMove;

	size_t splineIndex;
};