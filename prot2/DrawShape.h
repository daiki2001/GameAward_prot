#pragma once
#include <DxLib.h>
#include "Vector3.h"

class DrawShape final
{
public: //�V���O���g����
	static DrawShape* Get();
private:
	DrawShape();
	DrawShape(const DrawShape&) = delete;
	~DrawShape() {}
	DrawShape operator=(const DrawShape&) = delete;

private: //�萔
	static const WORD rectIndex[6];

public: //�ÓI�����o�֐�
	static void Init();

	static void DrawPlane(const Vector3& pos1, const Vector3& pos2, const unsigned int& color);
	static void DrawPlane(const Vector3& pos1, const Vector3& pos2, const COLOR_U8& color);

private: //�ÓI�����o�ϐ�
	static VERTEX3D rectVertex[4];

};
