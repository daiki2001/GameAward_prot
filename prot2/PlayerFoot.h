#pragma once
#include "PlayerBody.h"
#include "Vector3.h"
#include "Easing.h"

class PlayerFoot
{
public: //�����o�֐�
	PlayerFoot();
	~PlayerFoot();

	void Init();
	void Set();
	void Update(Vector3& FaceCenterPos, bool IsDownBody, int BodyDis);
	void Draw(bool isleft, bool isright, int offsetX, int offsetY);

public: //�����o�ϐ�
	Vector3 FootLeftUpPosition;
	Easing ease;

	bool FootIsAction;
	bool IsFootUp;

	//�摜�n���h��
	int FootHandle;
};