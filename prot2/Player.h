#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
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

	void IsHitPlayerBody(Stage& stage);

	void ExtrudePlayer(Vector3 ExtrudePos, float ExtrudeDis, bodytype ExtrudeType);
public: //�����o�ϐ�
	//���̍���
	float FloorHeight;

	//�̂̍\���v�f
	Vector3 CenterPosition;

	//��(�܂�ق�)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;

	//body_two��D��I�ɊJ����
	bool IsOpenTwo;

	//�ǂ̕�����܂�����(4����)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//�W�����v
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;
	bool IsFall;

	//�������E�W�����v���ɃW�����v���͂������Ă��邩�ǂ���
	bool IsInputjump = false;

	//�ǂꂩ��ł��̓������Ă�����true
	bool Player_IsAction;
	//�摜�n���h��(��)
	int FaceHandle[2];

	bool IsGoal;

	bool IsColide;

	//��
	PlayerFoot playerFoot;
};
