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
	void bodysetup(const unsigned char foldCount[4]);

public: //�����o�ϐ�
	//���̍���
	float floorHeight;

	//�̂̍\���v�f
	Vector3 center_position;

	//��
	PlayerBody body_one;
	PlayerBody body_two;
	PlayerBody body_three;

	//�܂����̂̏���
	int foldlist[3];

	//body_two��D��I�ɊJ����
	bool isopentwo;

	//�W�����v
	bool IsJump;
	float jumpspeed;
	float fallspeed;
	bool IsFall;
};
