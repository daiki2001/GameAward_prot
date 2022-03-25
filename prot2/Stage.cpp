#include "Stage.h"
#include <DxLib.h>
#include "LoadFile.h"
#include "InputManger.h"
#include "DrawShape.h"
#include "General.h"
#include "Colors.h"

#define EF (-1) //Error Function

#define ContainerClear(container)\
container.clear();\
container.shrink_to_fit();

enum bodytype
{
	left,
	up,
	right,
	down
};

namespace
{
static size_t i = 0, j = 0; //for���̃��[�v�J�E���^
static size_t x = 0, y = 0; //�}�b�v�`�b�v��̍��W

static size_t mapchipPos = 0; //�}�b�v�`�b�v�̗v�f�ԍ�
}

const int Stage::blockSize = 60;
const int Stage::halfBlockSize = Stage::blockSize / 2;
int Stage::startPlayerPosX = 0;
int Stage::startPlayerPosY = 0;
unsigned char Stage::initFoldCount[4] = { 0 };

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

Stage::Stage() :
	stageData{},
	initStageData{},
	stageEase{}
{
	Init();
}

Stage::~Stage()
{
	DataClear();
}

void Stage::Init()
{
}

void Stage::Updata()
{
	static int posX = 0;
	static int posY = 0;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
					posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);
					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					stageData[i].stageTileData[j].leftUpPos[mapchipPos].x = static_cast<float>(posX * blockSize);
					stageData[i].stageTileData[j].leftUpPos[mapchipPos].y = static_cast<float>(posY * blockSize);
					stageData[i].stageTileData[j].rightDownPos[mapchipPos].x = static_cast<float>((posX + 1) * blockSize);
					stageData[i].stageTileData[j].rightDownPos[mapchipPos].y = static_cast<float>((posY + 1) * blockSize);
				}
			}
		}
	}

	if (stageEase.ismove)
	{
		if (stageEase.timerate >= 1.0f)
		{
			stageEase.ismove = false;
		}
	}
}

void Stage::Draw(int offsetX, int offsetY)
{
	static int posX = 0;
	static int posY = 0;

	static Vector3 pos1, pos2;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					pos1.x = stageData[i].stageTileData[j].leftUpPos[mapchipPos].x + static_cast<float>(offsetX);
					pos1.y = stageData[i].stageTileData[j].leftUpPos[mapchipPos].y + static_cast<float>(offsetY);
					pos2.x = stageData[i].stageTileData[j].rightDownPos[mapchipPos].x + static_cast<float>(offsetX);
					pos2.y = stageData[i].stageTileData[j].rightDownPos[mapchipPos].y + static_cast<float>(offsetY);

					switch (stageData[i].stageTileData[j].mapchip[mapchipPos])
					{
					case MapchipData::EMPTY_STAGE:
					{
						continue;
						break;
					}
					case MapchipData::BLOCK:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DrawShape::DrawPlane(pos1, pos2, GRAY);
						break;
					}
					case MapchipData::GOAL:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DrawShape::DrawPlane(pos1, pos2, YELLOW);
						break;
					}
					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
						DrawShape::DrawPlane(pos1, pos2, WHITE);
						break;
					}
					}
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);

		// �Ȃ���
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			static char sideStageTile = 0;
			static char sideStageData = 0;

			if ((stageData[i].stageTileData[j].stageNumber % stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageTileData[j].height; y++)
					{
						posX = static_cast<int>(0 + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);

						DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if ((stageData[i].stageTileData[j].stageNumber % stageData[i].width) + 1 < stageData[i].width)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageTileData[j].height; y++)
					{
						posX = static_cast<int>((stageData[i].stageTileData[j].width - 1) + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos2.x = static_cast<float>((posX + 1) * blockSize + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);

						DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if ((stageData[i].stageTileData[j].stageNumber / stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber - stageData[i].width;
				sideStageData = stageData[i].stageTile[sideStageTile];

				if (sideStageData != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageTileData[sideStageData].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[sideStageData].offsetX);
						posY = static_cast<int>(0 + stageData[i].stageTileData[sideStageData].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + offsetY);
						pos2.y = static_cast<float>(posY * blockSize + blockSize * 1 / 4 + offsetY);

						DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
			if ((stageData[i].stageTileData[j].stageNumber / stageData[i].width) + 1 < stageData[i].height)
			{
				sideStageTile = stageData[i].stageTileData[j].stageNumber + stageData[i].width;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>((stageData[i].stageTileData[j].height - 1) + stageData[i].stageTileData[j].offsetY);

						pos1.x = static_cast<float>(posX * blockSize + blockSize * 1 / 4 + offsetX);
						pos2.x = static_cast<float>(posX * blockSize + blockSize * 3 / 4 + offsetX);
						pos1.y = static_cast<float>(posY * blockSize + blockSize * 3 / 4 + offsetY);
						pos2.y = static_cast<float>((posY + 1) * blockSize + offsetY);

						DrawShape::DrawPlane(pos1, pos2, BLACK);
					}
				}
			}
		}
	}
}

int Stage::LoadStage(const char* filePath, unsigned char foldCount[4])
{
	DataClear();

	static const char endCharacter = '@';
	static FILE* fileHandle;
	static errno_t err;
	static char string[256];
	static int index;
	static bool isSlash;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	if (LoadFile::LoadCSV((char*)initFoldCount, sizeof(initFoldCount) / sizeof(initFoldCount[0]), fileHandle, endCharacter) == EF)
	{
		// �֐����s
		return EF;
	}

	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
		//foldCount[i] = 1;
		//initFoldCount[i] = 1;
	}

	static char stageCount = 0;

	if (LoadFile::LoadCSV(&stageCount, 1, fileHandle, endCharacter) == EF)
	{
		// �֐����s
		return EF;
	}

	static char size[2] = { 0 };
	static char tilePos[2] = { 0 };

	for (i = 0; i < static_cast<size_t>(stageCount); i++)
	{
		stageData.push_back({});

		if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
		{
			// �֐����s
			return EF;
		}
		if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
		{
			// �֐����s
			return EF;
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// ��ȏ�̒l���Ԉ���Ă�
			return EF;
		}

		stageData[i].offsetX = tilePos[0];
		stageData[i].offsetY = tilePos[1];
		stageData[i].width = size[0];
		stageData[i].height = size[1];

		stageData[i].stageTile = (char*)malloc(sizeof(char) * stageData[i].width * stageData[i].height);

		if (LoadFile::LoadCSV(stageData[i].stageTile, stageData[i].width * stageData[i].height, fileHandle, endCharacter))
		{
			// �֐����s
			return EF;
		}
	}

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < static_cast<size_t>(stageData[i].width * stageData[i].height); j++)
		{
			if (stageData[i].stageTile[j] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			stageData[i].stageTile[j] = static_cast<char>(stageData[i].stageTileData.size() + 1);
			stageData[i].stageTileData.push_back({});
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].stageNumber = static_cast<char>(j);

			if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
			{
				// �֐����s
				return EF;
			}
			if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
			{
				// �֐����s
				return EF;
			}

			if (size[0] <= 0 || size[1] <= 0)
			{
				// ��ȏ�̒l���Ԉ���Ă�
				return EF;
			}

			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].offsetX = tilePos[0];
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].offsetY = tilePos[1];
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].width = size[0];
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].height = size[1];
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].size =
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].width *
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].height;
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].mapchip =
				(char*)malloc(sizeof(char) * stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].size);
			for (size_t k = 0; k < stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].size; k++)
			{
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].leftUpPos.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].rightDownPos.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].startPos.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].endPos.push_back({});
			}

			if (stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].mapchip == nullptr)
			{
				// �֐����s
				return EF;
			}

			if (LoadFile::LoadCSV(stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].mapchip,
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].width *
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].height, fileHandle, endCharacter) != 0)
			{
				// �֐����s
				return EF;
			}
		}
	}

	fclose(fileHandle);

	// �I�t�Z�b�g�l�̌v�Z
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			static unsigned char tileNumber = 0;
			tileNumber = stageData[i].stageTileData[j].stageNumber;

			if (stageData[i].stageTile[tileNumber] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			stageData[i].stageTileData[j].offsetX += stageData[i].offsetX;
			stageData[i].stageTileData[j].offsetY += stageData[i].offsetY;
		}
	}

	static bool end = false;
	end = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			// �v���C���[�̊J�n�ʒu�̒T��
			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageTileData[j].height + x;
					if (stageData[i].stageTileData[j].mapchip[mapchipPos] == MapchipData::START)
					{
						startPlayerPosX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						startPlayerPosY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);

						end = true;
						break;
					}
				}
				if (end)
				{
					break;
				}
			}
			if (end)
			{
				break;
			}
		}
		if (end)
		{
			break;
		}
	}

	if (end == false)
	{
		// �J�n�ʒu������
		return EF;
	}

	for (i = 0; i < stageData.size(); i++)
	{
		initStageData.push_back(stageData[i]);
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			char* initMapchip = static_cast<char*>(malloc(sizeof(char) * stageData[i].stageTileData[j].size));
			memcpy_s(initMapchip, sizeof(char) * stageData[i].stageTileData[j].size,
				stageData[i].stageTileData[j].mapchip, sizeof(char) * stageData[i].stageTileData[j].size);
			initStageData[i].stageTileData[j].mapchip = initMapchip;
		}
	}

	return 0;
}

int Stage::FoldAndOpen(const Vector3& playerPos, unsigned char playerTile[4])
{
	static unsigned char direction = 0;
	static size_t onPlayerStageTile = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	if (InputManger::SubUp())
	{
		direction = bodytype::up;
	}
	else if (InputManger::SubDown())
	{
		direction = bodytype::down;
	}
	else if (InputManger::SubLeft())
	{
		direction = bodytype::left;
	}
	else if (InputManger::SubRight())
	{
		direction = bodytype::right;
	}

	static size_t reverseMapchipPos = 0;
	static bool isAct = false;

	isAct = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= stageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= stageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				onPlayerStageTile = stageData[i].stageTileData[j].stageNumber;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case bodytype::up: //�����
			{
				if (onPlayerStageTile / stageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - stageData[i].width;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].direction == 0)
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);
				}
				else
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);
				}

				isAct = true;

				break;
			}
			case bodytype::down: //������
			{
				if (onPlayerStageTile / stageData[i].width >= static_cast<size_t>(stageData[i].height - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + stageData[i].width;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].direction == 0)
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);
				}
				else
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);
				}

				isAct = true;

				break;
			}
			case bodytype::left: //������
			{
				if (onPlayerStageTile % stageData[i].width <= 0)
				{
					break;
				}

				moveStageTile = onPlayerStageTile - 1;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].direction == 0)
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);
				}
				else
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);
				}

				isAct = true;

				break;
			}
			case bodytype::right: //�E����
			{
				if (onPlayerStageTile % stageData[i].width >= static_cast<size_t>(stageData[i].width - 1))
				{
					break;
				}

				moveStageTile = onPlayerStageTile + 1;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					break;
				}

				if (stageData[i].stageTileData[moveStageData].direction == 0)
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);
				}
				else
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);
				}

				isAct = true;

				break;
			}
			default:
			{
				return EF;
				break;
			}
			}

			if (isAct)
			{
				break;
			}
		}

		if (isAct)
		{
			break;
		}
	}

	return 0;
}

void Stage::Reset()
{
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			stageData[i].stageTileData[j].offsetX = initStageData[i].stageTileData[j].offsetX;
			stageData[i].stageTileData[j].offsetY = initStageData[i].stageTileData[j].offsetY;
			stageData[i].stageTileData[j].stageNumber = initStageData[i].stageTileData[j].stageNumber;
			stageData[i].stageTileData[j].direction = initStageData[i].stageTileData[j].direction;
			for (size_t k = 0; k < stageData[i].stageTileData[j].size; k++)
			{
				stageData[i].stageTileData[j].mapchip[k] = initStageData[i].stageTileData[j].mapchip[k];
			}
		}
	}
}

void Stage::DataClear()
{
	for (i = 0; i < stageData.size(); i++)
	{
		if (stageData[i].stageTile != nullptr)
		{
			free(stageData[i].stageTile);
			stageData[i].stageTile = nullptr;
		}
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].mapchip != nullptr)
			{
				free(stageData[i].stageTileData[j].mapchip);
				stageData[i].stageTileData[j].mapchip = nullptr;
			}

			ContainerClear(stageData[i].stageTileData[j].leftUpPos);
			ContainerClear(stageData[i].stageTileData[j].rightDownPos);
			ContainerClear(stageData[i].stageTileData[j].startPos);
			ContainerClear(stageData[i].stageTileData[j].endPos);
		}

		ContainerClear(stageData[i].stageTileData);
		ContainerClear(stageData[i].stageOffsetX);
		ContainerClear(stageData[i].stageOffsetY);
	}

	for (i = 0; i < initStageData.size(); i++)
	{
		//if (initStageData[i].stageTile != nullptr)
		//{
		//	free(initStageData[i].stageTile);
		//	initStageData[i].stageTile = nullptr;
		//}
		for (j = 0; j < initStageData[i].stageTileData.size(); j++)
		{
			if (initStageData[i].stageTileData[j].mapchip != nullptr)
			{
				free(initStageData[i].stageTileData[j].mapchip);
				initStageData[i].stageTileData[j].mapchip = nullptr;
			}

			ContainerClear(initStageData[i].stageTileData[j].leftUpPos);
			ContainerClear(initStageData[i].stageTileData[j].rightDownPos);
			ContainerClear(initStageData[i].stageTileData[j].startPos);
			ContainerClear(initStageData[i].stageTileData[j].endPos);
		}

		ContainerClear(initStageData[i].stageTileData);
		ContainerClear(initStageData[i].stageOffsetX);
		ContainerClear(initStageData[i].stageOffsetY);
	}

	ContainerClear(stageData);
	ContainerClear(initStageData);
}

void Stage::GetInitFoldCount(unsigned char foldCount[4])
{
	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
	}
}

inline Stage::StageTileData* Stage::GetStageTileData(const short& stageNumber, const short& stageTileNumber)
{
	if (stageNumber < 0 || size_t(stageNumber) >= stageData.size())
	{
		return nullptr;
	}
	if (stageTileNumber < 0 || size_t(stageTileNumber) >= stageData[stageNumber].stageTileData.size())
	{
		return nullptr;
	}
	return &stageData[stageNumber].stageTileData[stageTileNumber];
}

inline Stage::StageData* Stage::GetStageData(const short& stageNumber)
{
	if (stageNumber < 0 || size_t(stageNumber) >= stageData.size())
	{
		return nullptr;
	}
	return &stageData[stageNumber];
}

inline Stage::StageData* Stage::GetAllStageData()
{
	return stageData.data();
}

size_t Stage::GetStageDataSize()
{
	return stageData.size();
}

size_t Stage::GetStageTileDataSize(int i)
{
	return stageData[i].stageTileData.size();
}

char Stage::GetStageWidth(int i)
{
	return stageData[i].width;
}

char Stage::GetStageHeight(int i)
{
	return stageData[i].height;
}

char Stage::GetStageTileWidth(int i, int j)
{
	return stageData[i].stageTileData[j].width;
}

char Stage::GetStageTileHeight(int i, int j)
{
	return stageData[i].stageTileData[j].height;
}

char Stage::GetStageMapchip(int i, int j, int mapchipPos)
{
	return stageData[i].stageTileData[j].mapchip[mapchipPos];
}

bool Stage::GetPlayerTile(Vector3 center, int i, int j)
{
	int center_x_mapchip = static_cast<int>(center.x - stageData[i].offsetX) / blockSize;
	int center_y_mapchip = static_cast<int>(center.y - stageData[i].offsetY) / blockSize;

	float left = (float)stageData[i].stageTileData[j].offsetX * blockSize;
	float up = (float)stageData[i].stageTileData[j].offsetY * blockSize;
	float right = left + blockSize * stageData[i].stageTileData[j].width;
	float down = up + blockSize * stageData[i].stageTileData[j].height;

	if (center.x - halfBlockSize >= left && center.x + halfBlockSize <= right && center.y - halfBlockSize >= up && center.y + halfBlockSize <= down)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Stage::Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] <= 0)
	{
		return EF;
	}

	static size_t reverseMapchipPos = 0;

	if (direction == bodytype::up || direction == bodytype::down)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			if (y == stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
			{
				break;
			}
			for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY += stageData[onPlayerStage].stageTileData[moveStageData].height;
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY -= stageData[onPlayerStage].stageTileData[moveStageData].height;
		}
	}
	if (direction == bodytype::left || direction == bodytype::right)
	{
		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				if (x == stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + (stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX += stageData[onPlayerStage].stageTileData[moveStageData].width;
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX -= stageData[onPlayerStage].stageTileData[moveStageData].width;
		}
	}

	stageData[onPlayerStage].stageTileData[moveStageData].stageNumber = static_cast<char>(onPlayerStageTile);
	stageData[onPlayerStage].stageTileData[moveStageData].direction = direction + 1;
	playerTile[direction]--;

	return 0;
}

int Stage::Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] > 0)
	{
		return EF;
	}

	static size_t reverseMapchipPos = 0;

	if (direction == bodytype::up || direction == bodytype::down)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			if (y == stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
			{
				break;
			}
			for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY -= stageData[onPlayerStage].stageTileData[moveStageData].height;
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetY += stageData[onPlayerStage].stageTileData[moveStageData].height;
		}
	}
	if (direction == bodytype::left || direction == bodytype::right)
	{
		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				if (x == stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + (stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageTileData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageTileData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction / 2 == 0)
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX -= stageData[onPlayerStage].stageTileData[moveStageData].width;
		}
		else
		{
			stageData[onPlayerStage].stageTileData[moveStageData].offsetX += stageData[onPlayerStage].stageTileData[moveStageData].width;
		}
	}

	stageData[onPlayerStage].stageTileData[moveStageData].stageNumber = static_cast<char>(moveStageTile);
	stageData[onPlayerStage].stageTileData[moveStageData].direction = 0;
	playerTile[direction]++;

	return 0;
}

int Stage::SearchTopStageTile()
{
	std::vector<Vector3> topStageTile; //x�̒l��stageData�̗v�f�ԍ��Ay�̒l��stageData[i].stageData�̗v�f�ԍ����i�[����
	static bool isTop = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			isTop = false;

			for (size_t k = 0; k < topStageTile.size(); k++)
			{

			}
		}
	}

	return 0;
}
