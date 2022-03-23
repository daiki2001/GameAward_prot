#include "Matrix4.h"
#include "Vector3.h"
#include <math.h>

// �P�ʍs������߂�
Matrix4 Identity()
{
	static const Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

Matrix4 Inverse(const Matrix4& m)
{
	static Matrix4 result{};
	static float determinant = 0.0f; //�s��

	determinant = 0.0f;
	determinant += m.m[0][0] * (
		m.m[1][1] * (m.m[2][2] * m.m[3][3] - m.m[3][2] * m.m[2][3]) -
		m.m[2][1] * (m.m[1][2] * m.m[3][3] - m.m[3][2] * m.m[1][3]) +
		m.m[3][1] * (m.m[1][2] * m.m[2][3] - m.m[2][2] * m.m[1][3]));
	determinant -= m.m[0][1] * (
		m.m[1][0] * (m.m[2][2] * m.m[3][3] - m.m[3][2] * m.m[2][3]) -
		m.m[2][0] * (m.m[1][2] * m.m[3][3] - m.m[3][2] * m.m[1][3]) +
		m.m[3][0] * (m.m[1][2] * m.m[2][3] - m.m[2][2] * m.m[1][3]));
	determinant += m.m[0][2] * (
		m.m[1][0] * (m.m[2][1] * m.m[3][3] - m.m[3][1] * m.m[2][3]) -
		m.m[2][0] * (m.m[1][1] * m.m[3][3] - m.m[3][1] * m.m[1][3]) +
		m.m[3][0] * (m.m[1][1] * m.m[2][3] - m.m[2][1] * m.m[1][3]));
	determinant -= m.m[0][3] * (
		m.m[1][0] * (m.m[2][1] * m.m[3][2] - m.m[3][1] * m.m[2][2]) -
		m.m[2][0] * (m.m[1][1] * m.m[3][2] - m.m[3][1] * m.m[1][2]) +
		m.m[3][0] * (m.m[1][1] * m.m[2][2] - m.m[2][1] * m.m[1][2]));

	if (determinant == 0)
	{
#ifdef _DEBUG
		OutputDebugStringA("�t�s�񂪂���܂���\n");
#endif // _DEBUG

		result = {
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f
		};
	}
	else
	{
		for (size_t i = 0; i < 4; i++)
		{
			static float symbol = 0.0f;
			static size_t a = 0, b = 0, c = 0;
			static size_t d = 0, e = 0, f = 0;

			a = (i < 1) ? 1 : 0;
			b = (i < 2) ? 2 : 1;
			c = (i < 3) ? 3 : 2;

			for (size_t j = 0; j < 4; j++)
			{
				if ((i + j) % 2)
				{
					symbol = -1.0f;
				}
				else
				{
					symbol = +1.0f;
				}

				d = (j < 1) ? 1 : 0;
				e = (j < 2) ? 2 : 1;
				f = (j < 3) ? 3 : 2;

				result.m[j][i] = (1.0f / determinant) * symbol * (
					m.m[a][d] * (m.m[b][e] * m.m[c][f] - m.m[c][e] * m.m[b][f]) -
					m.m[b][d] * (m.m[a][e] * m.m[c][f] - m.m[c][e] * m.m[a][f]) +
					m.m[c][d] * (m.m[a][e] * m.m[b][f] - m.m[b][e] * m.m[a][f]));
			}
		}
	}

	return result;
}

// �g��k���s������߂�
Matrix4 scale(const Vector3& s)
{
	Matrix4 result
	{
		+s.x, 0.0f, 0.0f, 0.0f,
		0.0f, +s.y, 0.0f, 0.0f,
		0.0f, 0.0f, +s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// X���܂��̉�]�s������߂�
Matrix4 rotateX(float angle)
{
	float sinX = sinf(angle);
	float cosX = cosf(angle);

	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cosX, sinX, 0.0f,
		0.0f,-sinX, cosX, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// Y���܂��̉�]�s������߂�
Matrix4 rotateY(float angle)
{
	float sinY = sinf(angle);
	float cosY = cosf(angle);

	Matrix4 result
	{
		cosY, 0.0f,-sinY, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sinY, 0.0f, cosY, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// Z���܂��̉�]�s������߂�
Matrix4 rotateZ(float angle)
{
	float sinZ = sinf(angle);
	float cosZ = cosf(angle);

	Matrix4 result
	{
		 cosZ, sinZ, 0.0f, 0.0f,
		-sinZ, cosZ, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		 0.0f, 0.0f, 0.0f, 1.0f
	};

	return result;
}

// ���s�ړ��s������߂�
Matrix4 translate(const Vector3& t)
{
	Matrix4 result
	{
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		+t.x, +t.y, +t.z, 1.0f
	};

	return result;
}

// ���W�ϊ��i�x�N�g���ƍs��̊|���Z�j���s��
Vector3 transform(const Vector3& v, const Matrix4& m)
{
	float w = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

	Vector3 result
	{
		(v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0]) / w,
		(v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1]) / w,
		(v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2]) / w
	};

	return result;
}

// ������Z�q*=�̃I�[�o�[���[�h
Matrix4& operator *= (Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result{ 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result.m[i][j] += m1.m[i][k] * m2.m[k][j];
			}
		}
	}

	m1 = result;
	return m1;
}

// 2�����Z�q*�I�[�o�[���[�h�i�s��ƍs��̐ρj
Matrix4 operator * (const Matrix4& m1, const Matrix4& m2)
{
	Matrix4 result = m1;

	return result *= m2;
}

// 2�����Z�q*�I�[�o�[���[�h�i�x�N�g���ƍs��̐ρj
Vector3 operator * (const Vector3& v, const Matrix4& m)
{
	return transform(v, m);
}
