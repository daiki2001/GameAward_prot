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

void Stage::Draw()
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
					mapchipPos = y * stageData[i].stageData[j].height + x;

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
						DxLib::DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, GRAY, true);
						break;
					}
					case MapchipData::GOAL:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DxLib::DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, YELLOW, true);
						break;
					}
					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
						DxLib::DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, WHITE, true);
						break;
					}
					}
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);

		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			if ((stageData[i].stageData[j].stageNumber % stageData[i].width) - 1 >= 0 &&
				stageData[i].stageTile[stageData[i].stageData[j].stageNumber - 1] != MapchipData::EMPTY_STAGE)
			{
				for (y = 0; y < stageData[i].stageData[j].height; y++)
				{
					posX = static_cast<int>(0 + stageData[i].stageData[j].offsetX);
					posY = static_cast<int>(y + stageData[i].stageData[j].offsetY);

					DxLib::DrawBox(posX * blockSize, posY * blockSize + blockSize * 1 / 4,
						posX * blockSize + blockSize * 1 / 4, posY * blockSize + blockSize * 3 / 4, BLACK, true);
				}
			}
			if ((stageData[i].stageData[j].stageNumber % stageData[i].width) + 1 < stageData[i].width &&
				stageData[i].stageTile[stageData[i].stageData[j].stageNumber + 1] != MapchipData::EMPTY_STAGE)
			{
				for (y = 0; y < stageData[i].stageData[j].height; y++)
				{
					posX = static_cast<int>((stageData[i].stageData[j].width - 1) + stageData[i].stageData[j].offsetX);
					posY = static_cast<int>(y + stageData[i].stageData[j].offsetY);

					DxLib::DrawBox(posX * blockSize + blockSize * 3 / 4, posY * blockSize + blockSize * 1 / 4,
						(posX + 1) * blockSize, posY * blockSize + blockSize * 3 / 4, BLACK, true);
				}
			}
			if ((stageData[i].stageData[j].stageNumber / stageData[i].width) - 1 >= 0 &&
				stageData[i].stageTile[stageData[i].stageData[j].stageNumber - stageData[i].height] != MapchipData::EMPTY_STAGE)
			{
				for (x = 0; x < stageData[i].stageData[j].width; x++)
				{
					posX = static_cast<int>(x + stageData[i].stageData[j].offsetX);
					posY = static_cast<int>(0 + stageData[i].stageData[j].offsetY);

					DxLib::DrawBox(posX * blockSize + blockSize * 1 / 4, posY * blockSize,
						posX * blockSize + blockSize * 3 / 4, posY * blockSize + blockSize * 1 / 4, BLACK, true);
				}
			}
			if ((stageData[i].stageData[j].stageNumber / stageData[i].width) + 1 < stageData[i].height &&
				stageData[i].stageTile[stageData[i].stageData[j].stageNumber + stageData[i].height] != MapchipData::EMPTY_STAGE)
			{
				for (x = 0; x < stageData[i].stageData[j].width; x++)
				{
					posX = static_cast<int>(x + stageData[i].stageData[j].offsetX);
					posY = static_cast<int>((stageData[i].stageData[j].height - 1) + stageData[i].stageData[j].offsetY);

					DxLib::DrawBox(posX * blockSize + blockSize * 1 / 4, posY * blockSize + blockSize * 3 / 4,
						posX * blockSize + blockSize * 3 / 4, (posY + 1) * blockSize, BLACK, true);
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
		// ä÷êîé∏îs
		return EF;
	}

	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
	}

	static char stageCount = 0;

	if (LoadFile::LoadCSV(&stageCount, 1, fileHandle, endCharacter) == EF)
	{
		// ä÷êîé∏îs
		return EF;
	}

	static char size[2] = { 0 };
	static char tilePos[2] = { 0 };

	for (i = 0; i < static_cast<size_t>(stageCount); i++)
	{
		stageData.push_back({});

		if (LoadFile::LoadCSV(tilePos, sizeof(tilePos) / sizeof(tilePos[0]), fileHandle, INT_MIN) == EF)
		{
			// ä÷êîé∏îs
			return EF;
		}
		if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
		{
			// ä÷êîé∏îs
			return EF;
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// àÍÇ¬à»è„ÇÃílÇ™ä‘à·Ç¡ÇƒÇÈ
			return EF;
		}

		stageData[i].offsetX = tilePos[0];
		stageData[i].offsetY = tilePos[1];
		stageData[i].width = size[0];
		stageData[i].height = size[1];

		stageData[i].stageTile = (char*)malloc(sizeof(char) * stageData[i].width * stageData[i].height);

		if (LoadFile::LoadCSV(stageData[i].stageTile, stageData[i].width * stageData[i].height, fileHandle, endCharacter))
		{
			// ä÷êîé∏îs
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
				// ä÷êîé∏îs
				return EF;
			}
			if (LoadFile::LoadCSV(size, sizeof(size) / sizeof(size[0]), fileHandle, INT_MIN) == EF)
			{
				// ä÷êîé∏îs
				return EF;
			}

			if (size[0] <= 0 || size[1] <= 0)
			{
				// àÍÇ¬à»è„ÇÃílÇ™ä‘à·Ç¡ÇƒÇÈ
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
				// ä÷êîé∏îs
				return EF;
			}

			if (LoadFile::LoadCSV(stageData[i].stageData[stageData[i].stageData.size() - 1].mapchip,
				stageData[i].stageData[stageData[i].stageData.size() - 1].width *
				stageData[i].stageData[stageData[i].stageData.size() - 1].height, fileHandle, endCharacter) != 0)
			{
				// ä÷êîé∏îs
				return EF;
			}
		}
	}

	fclose(fileHandle);

	// ÉIÉtÉZÉbÉgílÇÃåvéZ
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			static unsigned char tileNumber = 0;
			static char offsetX = 0;
			static char offsetY = 0;

			tileNumber = stageData[i].stageData[j].stageNumber;
			offsetX = tileNumber % stageData[i].width;
			offsetY = tileNumber / stageData[i].width;

			if (stageData[i].stageTile[tileNumber] == MapchipData::EMPTY_STAGE)
			{
				continue;
			}

			if (offsetX <= 0)
			{
				stageData[i].stageData[j].offsetX = 0 + stageData[i].offsetX;
				if (stageData[i].stageOffsetX.size() <= 0)
				{
					stageData[i].stageOffsetX.push_back(0 + stageData[i].offsetX);
				}
			}
			else
			{
				if (static_cast<size_t>(offsetX) < stageData[i].stageOffsetX.size())
				{
					stageData[i].stageData[j].offsetX = stageData[i].stageOffsetX[offsetX - 1];
					stageData[i].stageData[j].offsetX += stageData[i].stageData[j].width;
					stageData[i].stageOffsetX[offsetX] = stageData[i].stageData[j].offsetX;
				}
				else
				{
					for (size_t k = stageData[i].stageOffsetX.size(); k <= size_t(offsetX); k++)
					{
						stageData[i].stageOffsetX.push_back(stageData[i].stageOffsetX[k - 1]);
					}
					stageData[i].stageData[j].offsetX = stageData[i].stageOffsetX[offsetX - 1];
					stageData[i].stageData[j].offsetX += stageData[i].stageData[j].width;
					stageData[i].stageOffsetX[offsetX] = stageData[i].stageData[j].offsetX;
				}
			}

			if (offsetY <= 0)
			{
				stageData[i].stageData[j].offsetY = 0 + stageData[i].offsetY;
				if (stageData[i].stageOffsetY.size() <= 0)
				{
					stageData[i].stageOffsetY.push_back(0 + stageData[i].offsetY);
				}
			}
			else
			{
				if (static_cast<size_t>(offsetY) < stageData[i].stageOffsetY.size())
				{
					stageData[i].stageData[j].offsetY = stageData[i].stageOffsetY[offsetY - 1];
					stageData[i].stageData[j].offsetY += stageData[i].stageData[j].height;
					stageData[i].stageOffsetY[offsetY] = stageData[i].stageData[j].offsetY;
				}
				else
				{
					for (size_t k = stageData[i].stageOffsetY.size(); k <= size_t(offsetY); k++)
					{
						stageData[i].stageOffsetY.push_back(stageData[i].stageOffsetY[k - 1]);
					}
					stageData[i].stageData[j].offsetY = stageData[i].stageOffsetY[offsetY - 1];
					stageData[i].stageData[j].offsetY += stageData[i].stageData[j].height;
					stageData[i].stageOffsetY[offsetY] = stageData[i].stageData[j].offsetY;
				}
			}
		}
	}

	static bool end = false;
	end = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			// ÉvÉåÉCÉÑÅ[ÇÃäJénà íuÇÃíTçı
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
		// äJénà íuÇ™ñ≥Ç¢
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
	static size_t onPlayerStage = 0;
	static size_t onPlayerStageTile = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageData.size(); j++)
		{
			if (playerPos.x >= stageData[i].stageData[j].offsetX &&
				playerPos.x < stageData[i].stageData[j].offsetX + stageData[i].stageData[j].width &&
				playerPos.y >= stageData[i].stageData[j].offsetY &&
				playerPos.y < stageData[i].stageData[j].offsetY + stageData[i].stageData[j].height)
			{
				onPlayerStage = i;
				onPlayerStageTile = stageData[i].stageData[j].stageNumber;
				break;
			}
		}
	}

	if (InputManger::Up())
	{
		direction = 0;
	}
	else if (InputManger::Down())
	{
		direction = 1;
	}
	else if (InputManger::Left())
	{
		direction = 2;
	}
	else if (InputManger::Right())
	{
		direction = 3;
	}

	static size_t reverseMapchipPos = 0;

	switch (direction)
	{
	case 0: //è„ì¸óÕ
	{
		if (onPlayerStageTile / stageData[onPlayerStage].width <= 0)
		{
			break;
		}

		moveStageTile = onPlayerStageTile - stageData[onPlayerStage].width;
		moveStageData = stageData[onPlayerStage].stageTile[moveStageTile] - 1;

		if (moveStageTile < 0 ||
			stageData[onPlayerStage].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		if (stageData[onPlayerStage].stageData[moveStageData].direction == 0)
		{
			Fold(playerTile, direction, onPlayerStage, onPlayerStageTile, moveStageData);
		}
		else
		{
			Open(playerTile, direction, onPlayerStage, moveStageTile, moveStageData);
		}

		break;
	}
	case 1: //â∫ì¸óÕ
	{
		if (onPlayerStageTile / stageData[onPlayerStage].width >= static_cast<size_t>(stageData[onPlayerStage].height - 1))
		{
			break;
		}

		moveStageTile = onPlayerStageTile + stageData[onPlayerStage].width;
		moveStageData = stageData[onPlayerStage].stageTile[moveStageTile] - 1;

		if (moveStageTile >= static_cast<size_t>(stageData[onPlayerStage].width * stageData[onPlayerStage].height) ||
			stageData[onPlayerStage].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		if (stageData[onPlayerStage].stageData[moveStageData].direction == 0)
		{
			Fold(playerTile, direction, onPlayerStage, onPlayerStageTile, moveStageData);
		}
		else
		{
			Open(playerTile, direction, onPlayerStage, moveStageTile, moveStageData);
		}

		break;
	}
	case 2: //ç∂ì¸óÕ
	{
		if (onPlayerStageTile % stageData[onPlayerStage].width <= 0)
		{
			break;
		}

		moveStageTile = onPlayerStageTile - 1;
		moveStageData = stageData[onPlayerStage].stageTile[moveStageTile] - 1;

		if (moveStageTile < 0 ||
			stageData[onPlayerStage].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		if (stageData[onPlayerStage].stageData[moveStageData].direction == 0)
		{
			Fold(playerTile, direction, onPlayerStage, onPlayerStageTile, moveStageData);
		}
		else
		{
			Open(playerTile, direction, onPlayerStage, moveStageTile, moveStageData);
		}

		break;
	}
	case 3: //âEì¸óÕ
	{
		if (onPlayerStageTile % stageData[onPlayerStage].width >= static_cast<size_t>(stageData[onPlayerStage].width - 1))
		{
			break;
		}

		moveStageTile = onPlayerStageTile + 1;
		moveStageData = stageData[onPlayerStage].stageTile[moveStageTile] - 1;

		if (moveStageTile >= static_cast<size_t>(stageData[onPlayerStage].width * stageData[onPlayerStage].height) ||
			stageData[onPlayerStage].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		if (stageData[onPlayerStage].stageData[moveStageData].direction == 0)
		{
			Fold(playerTile, direction, onPlayerStage, onPlayerStageTile, moveStageData);
		}
		else
		{
			Open(playerTile, direction, onPlayerStage, moveStageTile, moveStageData);
		}

		break;
	}
	default:
	{
		return EF;
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
				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageData[moveStageData].height + x;

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

				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + (stageData[onPlayerStage].stageData[moveStageData].width - x - 1);

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
				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + x;
				reverseMapchipPos = (stageData[onPlayerStage].stageData[moveStageData].height - y - 1) * stageData[onPlayerStage].stageData[moveStageData].height + x;

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

				mapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + x;
				reverseMapchipPos = y * stageData[onPlayerStage].stageData[moveStageData].height + (stageData[onPlayerStage].stageData[moveStageData].width - x - 1);

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
