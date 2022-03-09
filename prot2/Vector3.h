#pragma once
#include <DxLib.h>

class Vector3 : public VECTOR
{
public:
	// �R���X�g���N�^
	Vector3();                          //��x�N�g���Ƃ��Đ���
	Vector3(float x, float y, float z); //x�����Ay�����Az�������w�肵�Đ���

	// �����o�֐�
	float Length() const;                  //�m����(����)�����߂�
	float sqLength() const;                //�m����(����)�̓������߂�
	Vector3& Normalize();                  //���K������
	float Dot(const Vector3& v) const;     //���ς����߂�
	Vector3 Cross(const Vector3& v) const; //�O�ς����߂�

	// �P�����Z�q�I�[�o�[���[�h
	Vector3 operator+() const;
	Vector3 operator-() const;

	// ������Z�q�I�[�o�[���[�h
	Vector3& operator+=(const Vector3& v);
	Vector3& operator-=(const Vector3& v);
	Vector3& operator*=(float s);
	Vector3& operator/=(float s);

	// ��r���Z�q�I�[�o�[���[�h
	bool operator==(const Vector3& v);
	bool operator!=(const Vector3& v);
};

// 2�����Z�q�I�[�o�[���[�h
// �������Ȉ���(�����̌^�Ə���)�̃p�^�[���ɑΉ����邽�߁A�ȉ��̂悤�ɏ������Ă���
const Vector3 operator+(const Vector3& v1, const Vector3& v2);
const Vector3 operator-(const Vector3& v1, const Vector3& v2);
const Vector3 operator*(const Vector3& v, float s);
const Vector3 operator*(float s, const Vector3& v);
const Vector3 operator/(const Vector3& v, float s);

// ��Ԋ֐�
// ���`���(1���֐����)
const Vector3 Lerp(const Vector3& start, const Vector3& end, const float t);
// easeIn���
const Vector3 easeIn(const Vector3& start, const Vector3& end, const float t);
// easeOut���
const Vector3 easeOut(const Vector3& start, const Vector3& end, const float t);
// easeInOut���
const Vector3 easeInOut(const Vector3& start, const Vector3& end, const float t);