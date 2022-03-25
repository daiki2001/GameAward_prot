#pragma once
#include "Vector3.h"

class BaseBlock
{
public: //�萔
	static const int blockSize;
	static const int halfBlockSize;

public: //�����o�֐�
	BaseBlock();
	virtual ~BaseBlock();

	virtual void Init(const Vector3& pos);
	virtual void Update(const Vector3& pos);
	virtual void Draw(int offsetX, int offsetY);

public: //�����o�ϐ�
	Vector3 pos;
private:
	Vector3 pos1, pos2;
};
