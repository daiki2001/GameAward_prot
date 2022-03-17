#pragma once
#include "Vector3.h"
#include "Colors.h"

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
	void Update(Vector3 center);
	//1 or -1(1:right&down -1:left&up)
	void setslide(int slidepat, Vector3 startpos);
	// �`��
	void Draw();

public: //�����o�ϐ�
	int body_type;

	//�̂̍��W
	Vector3 bodystartpos;

	//�[�̍��W
	Vector3 bodyendpos;

	//�X���C�h�̊J�n���W
	Vector3 slidestartpos;

	//�܂�E�J���t���O
	bool Isfold;
	bool Isopen;

	//�X���C�h�t���O�E�X���C�h�������
	bool Isslide;
	int slidepat;

	//�܂�E�J���E�X���C�h�����Ă���r�����ǂ���
	bool Isaction;

	//��ɏd�Ȃ��Ă��銚�݂̐�
	int overlap;

	//�{�̂���ǂꂾ������Ă��邩(���E��p)
	int bodydistance;

	//�C�[�W���O
	//easing ease;

	//�̂̐F
	int bodycolor = WHITE;
};
