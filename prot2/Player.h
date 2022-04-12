#pragma once
#include "PlayerBody.h"
#include "Vector3.h"
#include"PlayerFoot.h"

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
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void bodysetup(const unsigned char foldCount[4]);

	
	void IsHitPlayerBody(Stage& stage);
	
	void ExtrudePlayer(Vector3 ExtrudePos, float ExtrudeDis, bodytype ExtrudeType);

	bool IsFall();
public: //�����o�ϐ�
	//���̍���
	float FloorHeight = 640;

	//�̂̍\���v�f
	Vector3 CenterPosition = { 100.0f, 100.0f, 0.0f };

	//�����Ă������
	bool IsLeft;
	bool IsRight;

	//��(�܂�ق�)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//body_two��D��I�ɊJ����
	bool IsOpenTwo = true;

	//�ǂ̕�����܂邩(4����)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//�ړ����x
	float SideMoveSpeed = 3.0f;

	//�W�����v
	bool IsJump = false;
	float JumpSpeed = 3.0f;
	float FallSpeed = 3.0f;

	//��������(��̂�
	bool IsFaceFall;

	//�̂Ɗ炷�ׂĂ��l��������������
	bool IsAllFall = false;

	//�������E�W�����v���ɃW�����v���͂������Ă��邩�ǂ���
	bool IsInputjump = false;

	//�ǂꂩ��ł��̂𓮂����Ă�����true
	bool Player_IsAction = false;

	//�摜�n���h��(��)
	int FaceHandle[2];

	//�S�[���ɐG��Ă��邩�ǂ���
	bool IsGoal = false;

	//�u���b�N�ɓ������Ă��邩�ǂ���
	bool IsColide = false;

	//���ɑ̂����邩�ǂ���
	bool IsDownBody = false;

	//��
	Foot PlayerFoot;
};
