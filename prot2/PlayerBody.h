#pragma once
#include "Vector3.h"
#include "Easing.h"
#include "Colors.h"
#include "Stage.h"

enum bodytype
{
	left,
	up,
	right,
	down
};

class PlayerBody
{
public: //�����o�֐�
	PlayerBody();
	~PlayerBody();

	// ������
	void Init(Vector3 position, bodytype number);
	// �X�V
	void Update(Vector3& center);
	/// <summary>
	// �`��
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// �̂�L�����������̐ݒ�
	/// </summary>
	/// <param name="center">�L�����������̍��W�Q�Ɛ�</param>
	void setactivate(Vector3 center);
	/// �̂̃X���C�h�̃Z�b�g�A�b�v
	/// </summary>
	/// <param name="slidepat">�X���C�h�������(����:-1 �E��:1)</param>
	/// <param name="move_dis">�X���C�h���鋗��(��:1 ����܂���:2)</param>
	void setslide(int slidepat, int move_dis);

	/// <summary>
	/// �v���C���[�̓����蔻��
	/// </summary>
	/// <param name="stage">�X�e�[�W�f�[�^</param>
	/// <param name="center">�v���C���[�̒��S</param>
	/// <returns></returns>
	void IsHitBody(Stage& stage, Vector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide);

	/// <summary>
	/// �����o������
	/// </summary>
	/// <param name="center">�����o����鑤�̍��W</param>
	/// <param name="extrudepos">�����o�����̍��W</param>
	/// <param name="extrudedis">�����o������</param>
	/// <param name="extrudetype">�ǂ̕���"����"�����o����</param>
	void Extrude(Vector3* center, Vector3 extrudepos, float extrudedis, bodytype extrudetype, bool& isfall, bool& isjump, bool& iscolide);

public: //�����o�ϐ�
	//�L�����t���O
	bool IsActivate;

	//�炩�猩���̂̈ʒu
	int Body_Type;

	//�̂̍��W(������ԁF����)
	Vector3 BodyStartPos;

	//�̂̍��W(������ԁF�E��)
	Vector3 BodyEndPos;

	//�̂̒��S���W
	Vector3 BodyCenterPos;

	//�X���C�h�̊J�n���W
	Vector3 SlideStartPos;

	//�܂�E�J���t���O
	bool IsFold;
	bool IsOpen;

	//���̑̂��܂�ꂽ��
	int FoldCount = 0;

	//�X���C�h�t���O�E�X���C�h�������
	bool IsSlide;
	int SlidePat;

	//�܂�E�J���E�X���C�h�����Ă���r�����ǂ���
	bool IsAction;

	//�X���C�h���鋗��
	int SlideDis;

	//��ɏd�Ȃ��Ă��銚�݂̐�
	int Overlap;

	//�{�̂���ǂꂾ������Ă��邩(���E��p)
	int BodyDistance;

	//�C�[�W���O
	Easing Ease;

	//�̂̐F
	int BodyColor = WHITE;

	//�̂̑傫��
	float BodySize = 60.0f;

	//�摜�n���h��
	int Bodyhandle = LoadGraph("Resources/Body.png");
};
