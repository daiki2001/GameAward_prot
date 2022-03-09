#pragma once
#include "Vector3.h"

class Player final
{
public: //�V���O���g����
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //�����o�֐�
	// ������
	void Init();
	// �X�V
	void Updata();
	// �`��
	void Draw();

	// �v���C���[�̍��W���擾
	inline Vector3 GetPos() { return pos; }

private: //�����o�ϐ�
	Vector3 pos;
};
