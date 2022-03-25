#pragma once
#include <vector>
#include "Vector3.h"
#include "Easing.h"

/*���������傫���Ȃ肷���錜�O������ӏ���char�^�ɂ��Ă��܂�*/

enum MapchipData
{
	EMPTY_STAGE = 0,
	NONE,
	BLOCK,
	GOAL,
	START
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
		size_t size = 1;
		unsigned char width = 1;
		unsigned char height = 1;
		unsigned char direction = 0;
		bool isTop = true;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		char offsetX = 0;
		char offsetY = 0;
		unsigned char width = 1;
		unsigned char height = 1;
		std::vector<StageTileData> stageTileData;
		std::vector<char> stageOffsetX;
		std::vector<char> stageOffsetY;
	};

public: //�萔
	static const int blockSize;

	Vector3 offset = { 0,0,0 };

public: //�ÓI�����o�ϐ�
	static int foldGraph;
private:
	static int startPlayerPosX;
	static int startPlayerPosY;
	static unsigned char initFoldCount[4];

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
	/// <param name="foldCount"> �v���C���[���܂���(0�Ԃ����A���A���A�E) </param>
	/// <param name="fileHandle"> �X�e�[�W�t�@�C���̃p�X </param>
	/// <returns> 0�Ő����A0�ȊO�Ŏ��s </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// �X�e�[�W��܂�E�J��
	int FoldAndOpen(const Vector3& playerPos, unsigned char foldCount[4]);
	// ���Z�b�g
	void Reset();
	// �����f�[�^�S�폜
	void DataClear();

	// �v���C���[��x����̊J�n�ʒu���擾
	inline static int GetStartPlayerPosX() { return startPlayerPosX; }
	// �v���C���[��x����̊J�n�ʒu���擾
	inline static int GetStartPlayerPosY() { return startPlayerPosY; }
	// �v���C���[�̐܂��񐔂̏�����Ԃ��擾
	static void GetInitFoldCount(unsigned char foldCount[4]);
	// �X�e�[�W�^�C���̃f�[�^���擾
	inline StageTileData* GetStageTileData(const short& stageNumber, const short& stageTileNumber);
	// �X�e�[�W�̃f�[�^���擾
	inline StageData* GetStageData(const short& stageNumber);
	// �S�X�e�[�W�̃f�[�^���擾
	inline StageData* GetAllStageData();
	//Stagedata��size���擾
	size_t GetStageDataSize();
	//Stagetiledata��size���擾
	size_t GetStageTileDataSize(int i);
	//�C�ӂ�StageTile�̍������擾
	char GetStageTileHeight(int i, int j);
	//�C�ӂ�StageTile�̕����擾
	char GetStageTileWidth(int i, int j);
	//�C�ӂ̏ꏊ�̃}�b�v�`�b�v�����擾
	char GetStageMapChip(int i, int j, int mapchipPos);
	//�C�ӂ̍��W����ǂ�StageTile�ɂ��邩���擾
	bool GetPositionTile(Vector3 center, int i, int j);

private:
	// �X�e�[�W��܂�
	int Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData);
	// �X�e�[�W���J��
	int Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData);
	// ��ԏ�̃X�e�[�W�^�C����T��
	int SearchTopStageTile();

private: //�����o�ϐ�
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;

	Easing stageEase;
};
