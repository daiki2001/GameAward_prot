#pragma once
#include "PlayerBody.h"
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
	void Update(Stage& stage);
	// �`��
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// �ǂ̑̂�L�������邩
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type);

public: //�����o�ϐ�
	//���̍���
	float floorHeight = 640;

	//�̂̍\���v�f
	Vector3 center_position = { 100.0f, 100.0f, 0.0f };

	//��(�܂�ق�)
	PlayerBody body_one;
	PlayerBody body_two;
	PlayerBody body_three;

	//body_two��D��I�ɊJ����
	bool isopentwo = true;

	//�W�����v
	bool IsJump = false;
	float jumpspeed = 3.0f;
	float fallspeed = 3.0f;
	bool IsFall = false;
};
