#pragma once
#include <vector>
#include "Vector3.h"

/*���������傫���Ȃ肷���錜�O������ӏ���char�^�ɂ��Ă��܂�*/

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE = 1,
	BLOCK = 2,
	GOAL = 3,
	START = 4
};

class Stage final
{
public: //�V���O���g����
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

public: //�T�u�N���X
	struct StageTileData
	{
		char* mapchip = nullptr;
		char stageNumber = 0;
		char offsetX = 0;
		char offsetY = 0;
		unsigned char width = 1;
		unsigned char height = 1;
		bool turnFlagX = false;
		bool turnFlagY = false;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		unsigned char width = 1;
		unsigned char height = 1;
	};

public: //�萔
	static const int stageSize = 5;
	static const int blockSize;

public: //�ÓI�����o�ϐ�
	static int foldGraph;
private:
	static Vector3 startPlayerPos;

public: //�����o�֐�
	// ������
	void Init();
	// �X�V
	void Updata();
	// �`��
	void Draw();

	/// <summary>
	/// �X�e�[�W�t�@�C���̓ǂݍ���
	/// </summary>
	/// <param name="fileHandle"> �X�e�[�W�t�@�C���̃p�X </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	int LoadStage(const char* fileHandle);

	// �X�e�[�W��܂�
	int Fold(const Vector3& playerPos);
	// ���Z�b�g
	inline void Reset() { stageData = initStageData; }
	// �����f�[�^�S�폜
	void DataClear();

	// �X�e�[�W�̖������擾
	static Vector3 GetStartPlayerPos();
	// �e�X�e�[�W�̃f�[�^���擾
	inline StageTileData* GetStageData(const short& stageTileNumber);
	// �S�X�e�[�W�̃f�[�^���擾
	inline StageTileData* GetAllStageData();

private: //�����o�ϐ�
	StageData stageTile;
	std::vector<StageTileData> stageData;
	std::vector<StageTileData> initStageData;
	std::vector<char> offsetX, offsetY;
};
