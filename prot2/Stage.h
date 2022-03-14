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
		unsigned char direction = 0;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		char offsetX = 0;
		char offsetY = 0;
		unsigned char width = 1;
		unsigned char height = 1;
		std::vector<StageTileData> stageData;
		std::vector<char> stageOffsetX;
		std::vector<char> stageOffsetY;
	};

public: //�萔
	static const int stageSize = 5;
	static const int blockSize;

public: //�ÓI�����o�ϐ�
	static int foldGraph;
private:
	static int startPlayerPosX;
	static int startPlayerPosY;

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
	/// <param name="playerTileArray"> �v���C���[���܂���(0�Ԃ����A���A���A�E) </param>
	/// <param name="fileHandle"> �X�e�[�W�t�@�C���̃p�X </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// �X�e�[�W��܂�
	int Fold(const Vector3& playerPos, unsigned char playerTile[4]);
	// ���Z�b�g
	inline void Reset() { stageData = initStageData; }
	// �����f�[�^�S�폜
	void DataClear();

	// �v���C���[��x����̊J�n�ʒu���擾
	static int GetStartPlayerPosX() { return startPlayerPosX; }
	// �v���C���[��x����̊J�n�ʒu���擾
	static int GetStartPlayerPosY() { return startPlayerPosY; }
	// �X�e�[�W�^�C���̃f�[�^���擾
	inline StageTileData* GetStageTileData(const short& stageNumber, const short& stageTileNumber);
	// �X�e�[�W�̃f�[�^���擾
	inline StageData* GetStageData(const short& stageNumber);
	// �S�X�e�[�W�̃f�[�^���擾
	inline StageData* GetAllStageData();

private: //�����o�ϐ�
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;
};
