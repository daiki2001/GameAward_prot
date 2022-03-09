#include "Stage.h"
#include <DxLib.h>
#include "Input.h"
#include "LoadFile.h"

#include "Colors.h"

#define EF (-1) //Error Function

#define ContainerClear(container)\
container.clear();\
container.shrink_to_fit();

const int Stage::blockSize = 20;
int Stage::foldGraph = -1;
Vector3 Stage::startPlayerPos = Vector3();

Stage* Stage::Get()
{
	static Stage instance = {};
	return &instance;
}

Stage::Stage() :
	stageTile{},
	stageData{},
	initStageData{}
{
	Init();
}

Stage::~Stage()
{
	DataClear();
}

void Stage::Init()
{
	foldGraph = LoadGraph("./Resources/fold.png");
}

void Stage::Updata()
{
}

void Stage::Draw()
{
	for (size_t i = 0; i < stageData.size(); i++)
	{
		for (size_t y = 0; y < stageData[i].height; y++)
		{
			for (size_t x = 0; x < stageData[i].height; x++)
			{
				static int posX = 0;
				static int posY = 0;
				static size_t mapchipPos = 0;
				posX = static_cast<int>(x + stageData[i].offsetX);
				posY = static_cast<int>(y + stageData[i].offsetY);
				mapchipPos = y * stageData[i].height + x;

				switch (stageData[i].mapchip[mapchipPos])
				{
				case MapchipData::EMPTY_STAGE:
					continue;
					break;
				case MapchipData::FOLD:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, WHITE, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);
					DrawGraph(posX * blockSize, posY * blockSize, foldGraph, true);
					break;
				case MapchipData::BLOCK:
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, GRAY, true);
					break;
				case MapchipData::GOAL:
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, YELLOW, true);
					break;
				case MapchipData::NONE:
				case MapchipData::START:
				default:
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, WHITE, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);
					break;
				}
			}
		}
	}
}

int Stage::LoadStage(const char* filePath)
{
	static FILE* fileHandle;
	static errno_t err;
	static char string[256];
	static int index;
	static bool isMinus;
	static bool isSlash;

	err = fopen_s(&fileHandle, filePath, "r");
	if (err != 0)
	{
		return err;
	}

	//line = LoadFile::LoadCSV((int*)playerTileArray, 4, fileHandle);
	//if (line == EF)
	//{
	//	// 関数失敗
	//	return EF;
	//}

	static char size[2] = { 0 };

	for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = 0;
	index = 0;
	isMinus = false;

	if (fgets(string, 256, fileHandle) != nullptr)
	{
		isSlash = false;

		for (int i = 0; string[i] != '\0'; i++)
		{
			if (string[i] == '/')
			{
				// コメントアウト
				if (isSlash)
				{
					break;
				}
				else
				{
					isSlash = true;
				}
			}
			else
			{
				isSlash = false;
			}
			if (string[i] == ',' || string[i] == '\n')
			{
				// 次の数字へ
				index++;
				isMinus = false;
			}
			else if (string[i] == '-')
			{
				isMinus = true;
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				size[index] *= 10;

				if (isMinus == true)
				{
					size[index] -= string[i] - '0';
				}
				else
				{
					size[index] += string[i] - '0';
				}
			}
		}
	}

	stageTile.width = size[0];
	stageTile.height = size[1];

	if (size[0] <= 0 || size[1] <= 0)
	{
		// 一つ以上の値が間違ってる
		return EF;
	}

	stageTile.stageTile = (char*)malloc(sizeof(char) * stageTile.width * stageTile.height);

	if (LoadFile::LoadCSV(stageTile.stageTile, stageTile.width * stageTile.height, fileHandle))
	{
		// 関数失敗
		return EF;
	}

	for (size_t i = 0; i < size_t(stageTile.width * stageTile.height); i++)
	{
		static char offset[2] = { 0 };

		stageData.push_back({});
		for (size_t i = 0; i < sizeof(offset) / sizeof(offset[0]); i++) offset[i] = 0;
		for (size_t i = 0; i < sizeof(size) / sizeof(size[0]); i++) size[i] = 0;

		for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
		index = 0;
		isMinus = false;

		if (fgets(string, 256, fileHandle) != nullptr)
		{
			isSlash = false;

			for (int i = 0; string[i] != '\0'; i++)
			{
				if (string[i] == '/')
				{
					// コメントアウト
					if (isSlash)
					{
						break;
					}
					else
					{
						isSlash = true;
					}
				}
				else
				{
					isSlash = false;
				}
				if (string[i] == ',' || string[i] == '\n')
				{
					// 次の数字へ
					index++;
					isMinus = false;
				}
				else if (string[i] == '-')
				{
					isMinus = true;
				}
				else if (string[i] >= '0' && string[i] <= '9')
				{
					offset[index] *= 10;

					if (isMinus == true)
					{
						offset[index] -= string[i] - '0';
					}
					else
					{
						offset[index] += string[i] - '0';
					}
				}
			}
		}

		for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = '\0';
		index = 0;
		isMinus = false;

		if (fgets(string, 256, fileHandle) != nullptr)
		{
			isSlash = false;

			for (int i = 0; string[i] != '\0'; i++)
			{
				if (string[i] == '/')
				{
					// コメントアウト
					if (isSlash)
					{
						break;
					}
					else
					{
						isSlash = true;
					}
				}
				else
				{
					isSlash = false;
				}
				if (string[i] == ',' || string[i] == '\n')
				{
					// 次の数字へ
					index++;
					isMinus = false;
				}
				else if (string[i] == '-')
				{
					isMinus = true;
				}
				else if (string[i] >= '0' && string[i] <= '9')
				{
					size[index] *= 10;

					if (isMinus == true)
					{
						size[index] -= string[i] - '0';
					}
					else
					{
						size[index] += string[i] - '0';
					}
				}
			}
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// 一つ以上の値が間違ってる
			return EF;
		}

		stageData[i].offsetX = offset[0];
		stageData[i].offsetY = offset[1];
		stageData[i].width = size[0];
		stageData[i].height = size[1];
		stageData[i].mapchip = (char*)malloc(sizeof(char) * stageData[i].width * stageData[i].height);

		if (stageData[i].mapchip == nullptr)
		{
			// 関数失敗
			return EF;
		}

		if (LoadFile::LoadCSV(stageData[i].mapchip, stageData[i].width * stageData[i].height, fileHandle) != 0)
		{
			// 関数失敗
			return EF;
		}
	}

	fclose(fileHandle);

	initStageData = stageData;

	return 0;
}

int Stage::Fold(const Vector3& playerPos)
{
	static unsigned char direction = 0;

	if (Input::isKey(KEY_INPUT_W))
	{
		direction = 0;
	}
	else if (Input::isKey(KEY_INPUT_A))
	{
		direction = 1;
	}
	else if (Input::isKey(KEY_INPUT_S))
	{
		direction = 2;
	}
	else if (Input::isKey(KEY_INPUT_D))
	{
		direction = 3;
	}

	switch (direction)
	{
	case 0:

		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	default:
		return EF;
		break;
	}

	return 0;
}

void Stage::DataClear()
{
	if (stageTile.stageTile != nullptr)
	{
		free(stageTile.stageTile);
		stageTile.stageTile = nullptr;
	}
	for (size_t i = 0; i < stageData.size(); i++)
	{
		if (stageData[i].mapchip != nullptr)
		{
			free(stageData[i].mapchip);
			stageData[i].mapchip = nullptr;
		}
	}

	ContainerClear(stageData);
	ContainerClear(initStageData);
}

Vector3 Stage::GetStartPlayerPos()
{
	return startPlayerPos;
}

inline Stage::StageTileData* Stage::GetStageData(const short& stageTileNumber)
{
	if (stageTileNumber < 0 || size_t(stageTileNumber) >= stageData.size())
	{
		static StageTileData emptyStageData = {};
		return &emptyStageData;
	}
	return &stageData[stageTileNumber];
}

inline Stage::StageTileData* Stage::GetAllStageData()
{
	return stageData.data();
}
