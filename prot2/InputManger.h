#pragma once
#include "Input.h"

// ���͂��܂Ƃ߂��N���X
class InputManger final
{
private: //�V���O���g����
	InputManger() {}
	InputManger(const InputManger&) = delete;
	~InputManger() {}
	InputManger operator=(const InputManger&) = delete;

public: //�ÓI�����o�֐�
	// �I��
	static bool Escape();
	// �I��(�������u�Ԃ̔���)
	static bool EscapeTrigger();
	// �I��(�������u�Ԃ̔���)
	static bool EscapeReturn();
	// ��
	static bool Up();
	// ��(�������u�Ԃ̔���)
	static bool UpTrigger();
	// ��(�������u�Ԃ̔���)
	static bool UpReturn();
	// ��
	static bool Down();
	// ��(�������u�Ԃ̔���)
	static bool DownTrigger();
	// ��(�������u�Ԃ̔���)
	static bool DownReturn();
	// ��
	static bool Left();
	// ��(�������u�Ԃ̔���)
	static bool LeftTrigger();
	// ��(�������u�Ԃ̔���)
	static bool LeftReturn();
	// �E
	static bool Right();
	// �E(�������u�Ԃ̔���)
	static bool RightTrigger();
	// �E(�������u�Ԃ̔���)
	static bool RightReturn();
	// ���Z�b�g
	static bool Reset();
	// ���Z�b�g(�������u�Ԃ̔���)
	static bool ResetTrigger();
	// ���Z�b�g(�������u�Ԃ̔���)
	static bool ResetReturn();
	// �A�N�V����1
	static bool Act1();
	// �A�N�V����1(�������u�Ԃ̔���)
	static bool Act1Trigger();
	// �A�N�V����1(�������u�Ԃ̔���)
	static bool Act1Return();
};
