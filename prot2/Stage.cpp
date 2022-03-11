#include "Stage.h"
#include <DxLib.h>
#include "Input.h"
#include "LoadFile.h"

#include "General.h"
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
	initStageData{},
	offsetX{},
	offsetY{}
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
	static size_t mapchipPos = 0;

	for (size_t i = 0; i < stageData.size(); i++)
	{
		for (size_t y = 0; y < stageData[i].height; y++)
		{
			for (size_t x = 0; x < stageData[i].width; x++)
			{
				posX = static_cast<int>(x + stageData[i].offsetX);
				posY = static_cast<int>(y + stageData[i].offsetY);
				mapchipPos = y * stageData[i].height + x;

				switch (stageData[i].mapchip[mapchipPos])
				{
				case MapchipData::EMPTY_STAGE:
				{
					continue;
					break;
				}
				case MapchipData::BLOCK:
				{
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, GRAY, true);
					break;
				}
				case MapchipData::GOAL:
				{
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, YELLOW, true);
					break;
				}
				case MapchipData::NONE:
				case MapchipData::START:
				default:
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
					DrawBox(posX * blockSize, posY * blockSize, (posX + 1) * blockSize, (posY + 1) * blockSize, WHITE, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);
					break;
				}
				}
			}
		}
	}

	for (size_t i = 0; i < stageData.size(); i++)
	{
		if ((stageData[i].stageNumber % stageTile.width) - 1 >= 0 &&
			stageTile.stageTile[stageData[i].stageNumber - 1] != MapchipData::EMPTY_STAGE)
		{
			for (size_t y = 0; y < stageData[i].height; y++)
			{
				posX = static_cast<int>(0 + stageData[i].offsetX);
				posY = static_cast<int>(y + stageData[i].offsetY);
				mapchipPos = y * stageData[i].height + (stageData[i].width - 1);

				DrawBox(posX * blockSize, posY * blockSize + blockSize * 1 / 4,
					posX * blockSize + blockSize * 1 / 4, posY * blockSize + blockSize * 3 / 4, BLACK, true);
			}
		}
		if ((stageData[i].stageNumber % stageTile.width) + 1 < stageTile.width &&
			stageTile.stageTile[stageData[i].stageNumber + 1] != MapchipData::EMPTY_STAGE)
		{
			for (size_t y = 0; y < stageData[i].height; y++)
			{
				posX = static_cast<int>((stageData[i].width - 1) + stageData[i].offsetX);
				posY = static_cast<int>(y + stageData[i].offsetY);
				mapchipPos = y * stageData[i].height + (stageData[i].width - 1);

				DrawBox(posX * blockSize + blockSize * 3 / 4, posY * blockSize + blockSize * 1 / 4,
					(posX + 1) * blockSize, posY * blockSize + blockSize * 3 / 4, BLACK, true);
			}
		}
		if ((stageData[i].stageNumber / stageTile.height) - 1 >= 0 &&
			stageTile.stageTile[stageData[i].stageNumber - stageTile.height] != MapchipData::EMPTY_STAGE)
		{
			for (size_t x = 0; x < stageData[i].width; x++)
			{
				posX = static_cast<int>(x + stageData[i].offsetX);
				posY = static_cast<int>(0 + stageData[i].offsetY);
				mapchipPos = (stageData[i].height - 1) * stageData[i].height + x;

				DrawBox(posX * blockSize + blockSize * 1 / 4, posY * blockSize,
					posX * blockSize + blockSize * 3 / 4, posY * blockSize + blockSize * 1 / 4, BLACK, true);
			}
		}
		if ((stageData[i].stageNumber / stageTile.height) + 1 < stageTile.height &&
			stageTile.stageTile[stageData[i].stageNumber + stageTile.height] != MapchipData::EMPTY_STAGE)
		{
			for (size_t x = 0; x < stageData[i].width; x++)
			{
				posX = static_cast<int>(x + stageData[i].offsetX);
				posY = static_cast<int>((stageData[i].height - 1) + stageData[i].offsetY);
				mapchipPos = (stageData[i].height - 1) * stageData[i].height + x;

				DrawBox(posX * blockSize + blockSize * 1 / 4, posY * blockSize + blockSize * 3 / 4,
					posX * blockSize + blockSize * 3 / 4, (posY + 1) * blockSize, BLACK, true);
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

	static unsigned char size[2] = { 0 };

	for (size_t i = 0; i < sizeof(size) / sizeof(size[0]); i++) size[i] = 0;
	for (size_t i = 0; i < sizeof(string) / sizeof(string[0]); i++) string[i] = 0;
	index = 0;

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
			}
			else if (string[i] >= '0' && string[i] <= '9')
			{
				size[index] *= 10;
				size[index] += string[i] - '0';
			}
		}
	}

	if (size[0] <= 0 || size[1] <= 0)
	{
		// 一つ以上の値が間違ってる
		return EF;
	}

	stageTile.width = size[0];
	stageTile.height = size[1];

	stageTile.stageTile = (char*)malloc(sizeof(char) * stageTile.width * stageTile.height);

	if (LoadFile::LoadCSV(stageTile.stageTile, stageTile.width * stageTile.height, fileHandle))
	{
		// 関数失敗
		return EF;
	}

	for (size_t i = 0; i < size_t(stageTile.width * stageTile.height); i++)
	{
		if (stageTile.stageTile[i] == MapchipData::EMPTY_STAGE)
		{
			continue;
		}

		static char tilePos[2] = { 0 };

		stageTile.stageTile[i] = stageData.size() + 1;
		stageData.push_back({});
		stageData[stageData.size() - 1].stageNumber = i;
		for (size_t j = 0; j < sizeof(tilePos) / sizeof(tilePos[0]); j++) tilePos[j] = 0;
		for (size_t j = 0; j < sizeof(size) / sizeof(size[0]); j++) size[j] = 0;
		for (size_t j = 0; j < sizeof(string) / sizeof(string[0]); j++) string[j] = '\0';
		index = 0;

		if (fgets(string, 256, fileHandle) != nullptr)
		{
			isSlash = false;

			for (int j = 0; string[j] != '\0'; j++)
			{
				if (string[j] == '/')
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
				if (string[j] == ',' || string[j] == '\n')
				{
					// 次の数字へ
					index++;
				}
				else if (string[j] >= '0' && string[j] <= '9')
				{
					tilePos[index] *= 10;
					tilePos[index] += string[j] - '0';
				}
			}
		}

		for (size_t j = 0; j < sizeof(string) / sizeof(string[0]); j++) string[j] = '\0';
		index = 0;

		if (fgets(string, 256, fileHandle) != nullptr)
		{
			isSlash = false;

			for (int j = 0; string[j] != '\0'; j++)
			{
				if (string[j] == '/')
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
				if (string[j] == ',' || string[j] == '\n')
				{
					// 次の数字へ
					index++;
				}
				else if (string[j] >= '0' && string[j] <= '9')
				{
					size[index] *= 10;
					size[index] += string[j] - '0';
				}
			}
		}

		if (size[0] <= 0 || size[1] <= 0)
		{
			// 一つ以上の値が間違ってる
			return EF;
		}

		stageData[stageData.size() - 1].offsetX = tilePos[0];
		stageData[stageData.size() - 1].offsetY = tilePos[1];
		stageData[stageData.size() - 1].width = size[0];
		stageData[stageData.size() - 1].height = size[1];
		stageData[stageData.size() - 1].mapchip = (char*)malloc(sizeof(char) * stageData[stageData.size() - 1].width * stageData[stageData.size() - 1].height);

		if (stageData[stageData.size() - 1].mapchip == nullptr)
		{
			// 関数失敗
			return EF;
		}

		if (LoadFile::LoadCSV(stageData[stageData.size() - 1].mapchip, stageData[stageData.size() - 1].width * stageData[stageData.size() - 1].height, fileHandle) != 0)
		{
			// 関数失敗
			return EF;
		}
	}

	fclose(fileHandle);

	std::vector<char> tileOffsetX;
	std::vector<char> tileOffsetY;
	// オフセット値の計算
	for (size_t i = 0; i < stageData.size(); i++)
	{
		static unsigned char tileNumber = 0;
		static char offsetX = 0;
		static char offsetY = 0;

		tileNumber = stageData[i].stageNumber;
		offsetX = tileNumber % stageTile.width;
		offsetY = tileNumber / stageTile.height;

		if (stageTile.stageTile[offsetY * stageTile.height + offsetX] == MapchipData::EMPTY_STAGE)
		{
			continue;
		}

		if (offsetX <= 0)
		{
			stageData[i].offsetX = 0;
			if (tileOffsetX.size() <= 0)
			{
				tileOffsetX.push_back(0);
			}
		}
		else
		{
			if (offsetX < tileOffsetX.size())
			{
				stageData[i].offsetX = tileOffsetX[offsetX - 1];
				stageData[i].offsetX += stageData[i].width;
				tileOffsetX[offsetX] = stageData[i].offsetX;
			}
			else
			{
				for (size_t j = tileOffsetX.size(); j <= size_t(offsetX); j++)
				{
					tileOffsetX.push_back(tileOffsetX[j - 1]);
				}
				stageData[i].offsetX = tileOffsetX[offsetX - 1];
				stageData[i].offsetX += stageData[i].width;
				tileOffsetX[offsetX] = stageData[i].offsetX;
			}
		}

		if (offsetY <= 0)
		{
			stageData[i].offsetY = 0;
			if (tileOffsetY.size() <= 0)
			{
				tileOffsetY.push_back(0);
			}
		}
		else
		{
			if (offsetY < tileOffsetY.size())
			{
				stageData[i].offsetY = tileOffsetY[offsetY - 1];
				stageData[i].offsetY += stageData[i].height;
				tileOffsetY[offsetY] = stageData[i].offsetY;
			}
			else
			{
				for (size_t j = tileOffsetY.size(); j <= size_t(offsetY); j++)
				{
					tileOffsetY.push_back(tileOffsetY[j - 1]);
				}
				stageData[i].offsetY = tileOffsetY[offsetY - 1];
				stageData[i].offsetY += stageData[i].height;
				tileOffsetY[offsetY] = stageData[i].offsetY;
			}
		}
	}

	static bool end = false;
	end = false;

	for (size_t i = 0; i < stageData.size(); i++)
	{
		// プレイヤーの開始位置の探索
		for (size_t y = 0; y < stageData[i].height; y++)
		{
			for (size_t x = 0; x < stageData[i].width; x++)
			{
				static size_t mapchipPos = 0;
				mapchipPos = y * stageData[i].height + x;
				if (stageData[i].mapchip[mapchipPos] == MapchipData::START)
				{
					startPlayerPos.x = static_cast<float>(x + stageData[i].offsetX);
					startPlayerPos.y = static_cast<float>(y + stageData[i].offsetY);

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

	if (end == false)
	{
		// 開始位置が無い
		return EF;
	}

	initStageData = stageData;

	return 0;
}

int Stage::Fold(const Vector3& playerPos)
{
	static unsigned char direction = 0;
	static size_t onPlayerStageTile = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;

	for (size_t i = 0; i < stageData.size(); i++)
	{
		if (playerPos.x >= stageData[i].offsetX && playerPos.x <= stageData[i].offsetX + stageData[i].width &&
			playerPos.y >= stageData[i].offsetY && playerPos.y <= stageData[i].offsetY + stageData[i].height)
		{
			onPlayerStageTile = stageData[i].stageNumber;
			break;
		}
	}

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

	static size_t mapchipPos = 0;
	static size_t reverseMapchipPos = 0;

	switch (direction)
	{
	case 0: //上入力
	{
		if (onPlayerStageTile / stageTile.height <= 0)
		{
			break;
		}

		moveStageTile = onPlayerStageTile - stageTile.height;
		moveStageData = stageTile.stageTile[moveStageTile] - 1;

		if (moveStageTile < 0 ||
			stageTile.stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		for (size_t y = 0; y < stageData[moveStageData].height / 2; y++)
		{
			if (y == stageData[moveStageData].height - y - 1)
			{
				break;
			}
			for (size_t x = 0; x < stageData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[moveStageData].height + x;
				reverseMapchipPos = (stageData[moveStageData].height - y - 1) * stageData[moveStageData].height + x;

				Swap(&stageData[moveStageData].mapchip[mapchipPos], &stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		stageData[moveStageData].offsetY += stageData[moveStageData].height;
		stageData[moveStageData].stageNumber = onPlayerStageTile;
		stageData[moveStageData].turnFlagY = !stageData[moveStageData].turnFlagY;

		break;
	}
	case 1: //左入力
	{
		if (onPlayerStageTile % stageTile.width <= 0)
		{
			break;
		}

		moveStageTile = onPlayerStageTile - 1;
		moveStageData = stageTile.stageTile[moveStageTile] - 1;

		if (moveStageTile < 0 ||
			stageTile.stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		for (size_t y = 0; y < stageData[moveStageData].height; y++)
		{
			for (size_t x = 0; x < stageData[moveStageData].width / 2; x++)
			{
				if (x == stageData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[moveStageData].height + x;
				reverseMapchipPos = y * stageData[moveStageData].height + (stageData[moveStageData].width - x - 1);

				Swap(&stageData[moveStageData].mapchip[mapchipPos], &stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		stageData[moveStageData].offsetX += stageData[moveStageData].width;
		stageData[moveStageData].stageNumber = onPlayerStageTile;
		stageData[moveStageData].turnFlagX = !stageData[moveStageData].turnFlagX;

		break;
	}
	case 2: //下入力
	{
		if (onPlayerStageTile / stageTile.height >= stageTile.height - 1)
		{
			break;
		}

		moveStageTile = onPlayerStageTile + stageTile.height;
		moveStageData = stageTile.stageTile[moveStageTile] - 1;

		if (moveStageTile >= stageTile.width * stageTile.height ||
			stageTile.stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		for (size_t y = 0; y < stageData[moveStageData].height / 2; y++)
		{
			if (y == stageData[moveStageData].height - y - 1)
			{
				break;
			}
			for (size_t x = 0; x < stageData[moveStageData].width; x++)
			{
				mapchipPos = y * stageData[moveStageData].height + x;
				reverseMapchipPos = (stageData[moveStageData].height - y - 1) * stageData[moveStageData].height + x;

				Swap(&stageData[moveStageData].mapchip[mapchipPos], &stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		stageData[moveStageData].offsetY -= stageData[moveStageData].height;
		stageData[moveStageData].stageNumber = onPlayerStageTile;
		stageData[moveStageData].turnFlagY = !stageData[moveStageData].turnFlagY;

		break;
	}
	case 3: //右入力
	{
		if (onPlayerStageTile % stageTile.width >= stageTile.width - 1)
		{
			break;
		}

		moveStageTile = onPlayerStageTile + 1;
		moveStageData = stageTile.stageTile[moveStageTile] - 1;

		if (moveStageTile >= stageTile.width * stageTile.height ||
			stageTile.stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
		{
			break;
		}

		for (size_t y = 0; y < stageData[moveStageData].height; y++)
		{
			for (size_t x = 0; x < stageData[moveStageData].width / 2; x++)
			{
				if (x == stageData[moveStageData].width - x - 1)
				{
					break;
				}

				mapchipPos = y * stageData[moveStageData].height + x;
				reverseMapchipPos = y * stageData[moveStageData].height + (stageData[moveStageData].width - x - 1);

				Swap(&stageData[moveStageData].mapchip[mapchipPos], &stageData[moveStageData].mapchip[reverseMapchipPos]);
			}
		}

		stageData[moveStageData].offsetX -= stageData[moveStageData].width;
		stageData[moveStageData].stageNumber = onPlayerStageTile;
		stageData[moveStageData].turnFlagX = !stageData[moveStageData].turnFlagX;

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
