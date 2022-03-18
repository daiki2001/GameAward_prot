#include "Stage.h"
#include <DxLib.h>
#include "LoadFile.h"
#include "InputManger.h"
#include "General.h"
#include "Colors.h"

#define EF (-1) //Error Function

#define ContainerClear(container)\
container.clear();\
container.shrink_to_fit();

namespace
{
static size_t i = 0;
static size_t j = 0;
static size_t x = 0;
static size_t y = 0;

static size_t mapchipPos = 0;
}

const int Stage::blockSize = 40;
int Stage::foldGraph = -1;
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
	//foldGraph = LoadGraph("./Resources/fold.png");
}

void Stage::Updata()
{
}

void Stage::Draw(int offsetX, int offsetY)
{
	static int posX = 0;
	static int posY = 0;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			for (y = 0; y < stageData[i].stageData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageData[j].width; x++)
				{
					posX = static_cast<int>(x + stageData[i].stageData[j].offsetX);
					posY = static_cast<int>(y + stageData[i].stageData[j].offsetY);
					mapchipPos = y * stageData[i].stageData[j].width + x;

					switch (stageData[i].stageData[j].mapchip[mapchipPos])
					{
					case MapchipData::EMPTY_STAGE:
					{
						continue;
						break;
					}
					case MapchipData::BLOCK:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DxLib::DrawBox(posX * blockSize + offsetX, posY * blockSize + offsetY,
							(posX + 1) * blockSize + offsetX, (posY + 1) * blockSize + offsetY, GRAY, true);
						break;
					}
					case MapchipData::GOAL:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DxLib::DrawBox(posX * blockSize + offsetX, posY * blockSize + offsetY,
							(posX + 1) * blockSize + offsetX, (posY + 1) * blockSize + offsetY, YELLOW, true);
						break;
					}
					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
						DxLib::DrawBox(posX * blockSize + offsetX, posY * blockSize + offsetY,
							(posX + 1) * blockSize + offsetX, (posY + 1) * blockSize + offsetY, WHITE, true);
						break;
					}
					}
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);

		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			static char sideStageTile = 0;
			static char sideStageData = 0;

			if ((stageData[i].stageData[j].stageNumber % stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageData[j].stageNumber - 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageData[j].height; y++)
					{
						posX = static_cast<int>(0 + stageData[i].stageData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageData[j].offsetY);

						DxLib::DrawBox(posX * blockSize + offsetX, posY * blockSize + blockSize * 1 / 4 + offsetY,
							posX * blockSize + blockSize * 1 / 4 + offsetX, posY * blockSize + blockSize * 3 / 4 + offsetY, BLACK, true);
					}
				}
			}
			if ((stageData[i].stageData[j].stageNumber % stageData[i].width) + 1 < stageData[i].width)
			{
				sideStageTile = stageData[i].stageData[j].stageNumber + 1;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (y = 0; y < stageData[i].stageData[j].height; y++)
					{
						posX = static_cast<int>((stageData[i].stageData[j].width - 1) + stageData[i].stageData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageData[j].offsetY);

						DxLib::DrawBox(posX * blockSize + blockSize * 3 / 4 + offsetX, posY * blockSize + blockSize * 1 / 4 + offsetY,
							(posX + 1) * blockSize + offsetX, posY * blockSize + blockSize * 3 / 4 + offsetY, BLACK, true);
					}
				}
			}
			if ((stageData[i].stageData[j].stageNumber / stageData[i].width) - 1 >= 0)
			{
				sideStageTile = stageData[i].stageData[j].stageNumber - stageData[i].width;
				sideStageData = stageData[i].stageTile[sideStageTile];

				if (sideStageData != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageData[sideStageData].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageData[sideStageData].offsetX);
						posY = static_cast<int>(0 + stageData[i].stageData[sideStageData].offsetY);

						DxLib::DrawBox(posX * blockSize + blockSize * 1 / 4 + offsetX, posY * blockSize + offsetY,
							posX * blockSize + blockSize * 3 / 4 + offsetX, posY * blockSize + blockSize * 1 / 4 + offsetY, BLACK, true);
					}
				}
			}
			if ((stageData[i].stageData[j].stageNumber / stageData[i].width) + 1 < stageData[i].height)
			{
				sideStageTile = stageData[i].stageData[j].stageNumber + stageData[i].width;

				if (stageData[i].stageTile[sideStageTile] != MapchipData::EMPTY_STAGE)
				{
					for (x = 0; x < stageData[i].stageData[j].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageData[j].offsetX);
						posY = static_cast<int>((stageData[i].stageData[j].height - 1) + stageData[i].stageData[j].offsetY);

						DxLib::DrawBox(posX * blockSize + blockSize * 1 / 4 + offsetX, posY* blockSize + blockSize * 3 / 4 + offsetY,
							posX* blockSize + blockSize * 3 / 4 + offsetX, (posY + 1)* blockSize + offsetY, BLACK, true);
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
		// 関数失敗
		return EF;
	}

	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		//foldCount[i] = initFoldCount[i];
		foldCount[i] = 1;
		initFoldCount[i] = 1;
	}

	static char stageCount = 0;

	if (LoadFile::LoadCSV(&stageCount, 1, fileHandle, endCharacter) == EF)
	{
		// 関数失敗
		return EF;
	}

	static char size[2] = { 0 };
	static char tilePos[2] = { 0 };

	for (i = 0; i < static_cast<size_t>(stageCount); i++)
	{
		stageData.push_back({});

		if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
		{
			// 関数失敗
			return EF;
		}
		if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
		{
			// 関数失敗
			return EF;
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// 一つ以上の値が間違ってる
			return EF;
		}

		stageData[i].offsetX = tilePos[0];
		stageData[i].offsetY = tilePos[1];
		stageData[i].width = size[0];
		stageData[i].height = size[1];

		stageData[i].stageTile = (char*)malloc(sizeof(char) * stageData[i].width * stageData[i].height);

		if (LoadFile::LoadCSV(stageData[i].stageTile, stageData[i].width * stageData[i].height, fileHandle, endCharacter))
		{
			// 関数失敗
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

			stageData[i].stageTile[j] = static_cast<char>(stageData[i].stageData.size() + 1);
			stageData[i].stageData.push_back({});
			stageData[i].stageData[stageData[i].stageData.size() - 1].stageNumber = static_cast<char>(j);

			if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
			{
				// 関数失敗
				return EF;
			}
			if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
			{
				// 関数失敗
				return EF;
			}

			if (size[0] <= 0 || size[1] <= 0)
			{
				// 一つ以上の値が間違ってる
				return EF;
			}

			stageData[i].stageData[stageData[i].stageData.size() - 1].offsetX = tilePos[0];
			stageData[i].stageData[stageData[i].stageData.size() - 1].offsetY = tilePos[1];
			stageData[i].stageData[stageData[i].stageData.size() - 1].width = size[0];
			stageData[i].stageData[stageData[i].stageData.size() - 1].height = size[1];
			stageData[i].stageData[stageData[i].stageData.size() - 1].size =
				stageData[i].stageData[stageData[i].stageData.size() - 1].width *
				stageData[i].stageData[stageData[i].stageData.size() - 1].height;
			stageData[i].stageData[stageData[i].stageData.size() - 1].mapchip =
				(char*)malloc(sizeof(char) * stageData[i].stageData[stageData[i].stageData.size() - 1].size);

			if (stageData[i].stageData[stageData[i].stageData.size() - 1].mapchip == nullptr)
			{
				// 関数失敗
				return EF;
			}

			if (LoadFile::LoadCSV(stageData[i].stageData[stageData[i].stageData.size() - 1].mapchip,
				stageData[i].stageData[stageData[i].stageData.size() - 1].width *
				stageData[i].stageData[stageData[i].stageData.size() - 1].height, fileHandle, endCharacter) != 0)
			{
				// 関数失敗
				return EF;
			}
		}
	}

	fclose(fileHandle);

	// オフセット値の計算
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			static unsigned char tileNumber = 0;
			tileNumber = stageData[i].stageData[j].stageNumber;

			if (stageData[i].stageTile[tileNumber] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			stageData[i].stageData[j].offsetX += stageData[i].offsetX;
			stageData[i].stageData[j].offsetY += stageData[i].offsetY;
		}
	}

	static bool end = false;
	end = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			// プレイヤーの開始位置の探索
			for (y = 0; y < stageData[i].stageData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageData[j].width; x++)
				{
					mapchipPos = y * stageData[i].stageData[j].height + x;
					if (stageData[i].stageData[j].mapchip[mapchipPos] == MapchipData::START)
					{
						startPlayerPosX = static_cast<int>(x + stageData[i].stageData[j].offsetX);
						startPlayerPosY = static_cast<int>(y + stageData[i].stageData[j].offsetY);

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
		// 開始位置が無い
		return EF;
	}

	for (i = 0; i < stageData.size(); i++)
	{
		initStageData.push_back(stageData[i]);
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			char* initMapchip = static_cast<char*>(malloc(sizeof(char) * stageData[i].stageData[j].size));
			memcpy_s(initMapchip, sizeof(char)* stageData[i].stageData[j].size,
				stageData[i].stageData[j].mapchip, sizeof(char)* stageData[i].stageData[j].size);
			initStageData[i].stageData[j].mapchip = initMapchip;
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
		direction = 0;
	}
	else if (InputManger::SubDown())
	{
		direction = 1;
	}
	else if (InputManger::SubLeft())
	{
		direction = 2;
	}
	else if (InputManger::SubRight())
	{
		direction = 3;
	}

	static size_t reverseMapchipPos = 0;
	static bool isAct = false;

	isAct = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			if ((playerPos.x >= stageData[i].stageData[j].offsetX &&
				playerPos.x < stageData[i].stageData[j].offsetX + stageData[i].stageData[j].width) &&
				(playerPos.y >= stageData[i].stageData[j].offsetY &&
				playerPos.y < stageData[i].stageData[j].offsetY + stageData[i].stageData[j].height))
			{
				onPlayerStageTile = stageData[i].stageData[j].stageNumber;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case 0: //上入力
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

				if (playerPos.x < stageData[i].stageData[moveStageData].offsetX ||
					playerPos.x >= stageData[i].stageData[moveStageData].offsetX + stageData[i].stageData[moveStageData].width)
				{
					break;
				}

				if (stageData[i].stageData[moveStageData].direction == 0)
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
			case 1: //下入力
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

				if (playerPos.x < stageData[i].stageData[moveStageData].offsetX ||
					playerPos.x >= stageData[i].stageData[moveStageData].offsetX + stageData[i].stageData[moveStageData].width)
				{
					break;
				}

				if (stageData[i].stageData[moveStageData].direction == 0)
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
			case 2: //左入力
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

				if (stageData[i].stageData[moveStageData].direction == 0)
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
			case 3: //右入力
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

				if (stageData[i].stageData[moveStageData].direction == 0)
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
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			stageData[i].stageData[j].offsetX = initStageData[i].stageData[j].offsetX;
			stageData[i].stageData[j].offsetY = initStageData[i].stageData[j].offsetY;
			stageData[i].stageData[j].stageNumber = initStageData[i].stageData[j].stageNumber;
			stageData[i].stageData[j].direction = initStageData[i].stageData[j].direction;
			for (size_t k = 0; k < stageData[i].stageData[j].size; k++)
			{
				stageData[i].stageData[j].mapchip[k] = initStageData[i].stageData[j].mapchip[k];
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
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			if (stageData[i].stageData[j].mapchip != nullptr)
			{
				free(stageData[i].stageData[j].mapchip);
				stageData[i].stageData[j].mapchip = nullptr;
			}
		}

		ContainerClear(stageData[i].stageData);
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
		for (j = 0; j < initStageData[i].stageData.size(); j++)
		{
			if (initStageData[i].stageData[j].mapchip != nullptr)
			{
				free(initStageData[i].stageData[j].mapchip);
				initStageData[i].stageData[j].mapchip = nullptr;
			}
		}

		ContainerClear(initStageData[i].stageData);
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
	if (stageTileNumber < 0 || size_t(stageTileNumber) >= stageData[stageNumber].stageData.size())
	{
		return nullptr;
	}
	return &stageData[stageNumber].stageData[stageTileNumber];
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

int Stage::Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] <= 0)
	{
		return EF;
	}

	static size_t reverseMapchipPos = 0;

	if (direction == 0 || direction == 1)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageData[moveStageData].height / 2); y++)
		{
			if (y == stageData[onPlayerStage].stageData[moveStageData].height - y - 1)
			{
				break;
			}
			for (x = 0; x < stageData[onPlayerStage].stageData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction % 2 == 0)
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetY += stageData[onPlayerStage].stageData[moveStageData].height;
		}
		else
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetY -= stageData[onPlayerStage].stageData[moveStageData].height;
		}
	}
	if (direction == 2 || direction == 3)
	{
		for (y = 0; y < stageData[onPlayerStage].stageData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageData[moveStageData].width / 2); x++)
			{
				if (x == stageData[onPlayerStage].stageData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + (stageData[onPlayerStage].stageData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction % 2 == 0)
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetX += stageData[onPlayerStage].stageData[moveStageData].width;
		}
		else
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetX -= stageData[onPlayerStage].stageData[moveStageData].width;
		}
	}

	stageData[onPlayerStage].stageData[moveStageData].stageNumber = static_cast<char>(onPlayerStageTile);
	stageData[onPlayerStage].stageData[moveStageData].direction = direction + 1;
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

	if (direction == 0 || direction == 1)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageData[moveStageData].height / 2); y++)
		{
			if (y == stageData[onPlayerStage].stageData[moveStageData].height - y - 1)
			{
				break;
			}
			for (x = 0; x < stageData[onPlayerStage].stageData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageData[moveStageData].width + x;

				Swap(&stageData[onPlayerStage].stageData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction % 2 == 0)
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetY -= stageData[onPlayerStage].stageData[moveStageData].height;
		}
		else
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetY += stageData[onPlayerStage].stageData[moveStageData].height;
		}
	}
	if (direction == 2 || direction == 3)
	{
		for (y = 0; y < stageData[onPlayerStage].stageData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageData[moveStageData].width / 2); x++)
			{
				if (x == stageData[onPlayerStage].stageData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].width + (stageData[onPlayerStage].stageData[moveStageData].width - x - 1);

				Swap(&stageData[onPlayerStage].stageData[moveStageData].mapchip[mapchipPos], &stageData[onPlayerStage].stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		if (direction % 2 == 0)
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetX -= stageData[onPlayerStage].stageData[moveStageData].width;
		}
		else
		{
			stageData[onPlayerStage].stageData[moveStageData].offsetX += stageData[onPlayerStage].stageData[moveStageData].width;
		}
	}

	stageData[onPlayerStage].stageData[moveStageData].stageNumber = static_cast<char>(moveStageTile);
	stageData[onPlayerStage].stageData[moveStageData].direction = 0;
	playerTile[direction]++;

	return 0;
}

int Stage::SearchTopStageTile()
{
	std::vector<Vector3> topStageTile; //xの値にstageDataの要素番号、yの値にstageData[i].stageDataの要素番号を格納する
	static bool isTop = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			isTop = false;

			for (size_t k = 0; k < topStageTile.size(); k++)
			{

			}
		}
	}

	return 0;
}
