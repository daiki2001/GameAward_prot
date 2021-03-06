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
static size_t i = 0, j = 0; //for文のループカウンタ
static size_t x = 0, y = 0; //マップチップ上の座標

static size_t mapchipPos = 0; //マップチップの要素番号
static size_t reverseMapchipPos = 0; //反転したマップチップの要素番号
}

const int Stage::blockSize = 30;
const int Stage::halfBlockSize = Stage::blockSize / 2;
int Stage::BlockHandle = -1;
int Stage::GoalHandle = -1;
int Stage::EmptyHandle = -1;
int Stage::startPlayerPosX = 0;
int Stage::startPlayerPosY = 0;
unsigned char Stage::initFoldCount[4] = { 0 };

Stage* Stage::Get()
{
	static Stage instance = {};
	Init();
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
	GraphInit();
}

void Stage::GraphInit()
{
	if (BlockHandle == -1)
	{
		BlockHandle = LoadGraph("./Resources/block.png");
	}
	if (GoalHandle == -1)
	{
		GoalHandle = LoadGraph("./Resources/goal.png");
	}
	if (EmptyHandle == -1)
	{
		EmptyHandle = LoadGraph("./Resources/stage_enpty.png");
	}
}

void Stage::GetInitFoldCount(unsigned char foldCount[4])
{
	for (i = 0; i < sizeof(initFoldCount) / sizeof(initFoldCount[0]); i++)
	{
		foldCount[i] = initFoldCount[i];
	}
}

void Stage::Updata()
{
	static int posX = 0;
	static int posY = 0;

	EaseingUpdate();

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].stageEase.isMove)
			{
				for (y = 0; y < stageData[i].stageTileData[j].height; y++)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						static size_t easeMapchipPos = 0;

						mapchipPos = y * stageData[i].stageTileData[j].width + x;

						switch ((stageData[i].stageTileData[j].direction - 1) % 4)
						{
						case bodytype::up:
						{
							reverseMapchipPos = (stageData[i].stageTileData[j].height - y - 1) * stageData[i].stageTileData[j].width + x;

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].height - y - 1) + 1 == static_cast<size_t>(stageData[i].stageTileData[j].height))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y + blockSize);
							}
							else
							{
								easeMapchipPos = ((stageData[i].stageTileData[j].height - y - 1) + 1) * stageData[i].stageTileData[j].width + x;

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case bodytype::down:
						{
							reverseMapchipPos = (stageData[i].stageTileData[j].height - y - 1) * stageData[i].stageTileData[j].width + x;

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].height - y - 1) - 1 == static_cast<size_t>(-1))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y - blockSize);
							}
							else
							{
								easeMapchipPos = ((stageData[i].stageTileData[j].height - y - 1) - 1) * stageData[i].stageTileData[j].width + x;

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case bodytype::left:
						{
							reverseMapchipPos = y * stageData[i].stageTileData[j].width + (stageData[i].stageTileData[j].width - x - 1);

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].width - x - 1) + 1 == static_cast<size_t>(stageData[i].stageTileData[j].width))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x + blockSize);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y);
							}
							else
							{
								easeMapchipPos = y * stageData[i].stageTileData[j].width + ((stageData[i].stageTileData[j].width - x - 1) + 1);

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						case bodytype::right:
						{
							reverseMapchipPos = y * stageData[i].stageTileData[j].width + (stageData[i].stageTileData[j].width - x - 1);

							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[reverseMapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							if ((stageData[i].stageTileData[j].width - x - 1) - 1 == static_cast<size_t>(-1))
							{
								posX = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].x - blockSize);
								posY = static_cast<int>(stageData[i].stageTileData[j].startPos[reverseMapchipPos].y);
							}
							else
							{
								easeMapchipPos = y * stageData[i].stageTileData[j].width + ((stageData[i].stageTileData[j].width - x - 1) - 1);

								posX = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].x);
								posY = static_cast<int>(stageData[i].stageTileData[j].easePos[easeMapchipPos].y);
							}

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z);
							break;
						}
						default:
							posX = static_cast<int>(stageData[i].stageTileData[j].easePos[mapchipPos].x);
							posY = static_cast<int>(stageData[i].stageTileData[j].easePos[mapchipPos].y);

							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY);
							stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = stageData[i].stageTileData[j].easePos[mapchipPos].z;

							stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>(posX + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>(posY + blockSize);
							stageData[i].stageTileData[j].drawRightDown[mapchipPos].z =
								static_cast<float>(stageData[i].stageTileData[j].easePos[mapchipPos].z + blockSize);
							break;
						}
					}
				}
			}
			else
			{
				for (y = 0; y < stageData[i].stageTileData[j].height; y++)
				{
					for (x = 0; x < stageData[i].stageTileData[j].width; x++)
					{
						posX = static_cast<int>(x + stageData[i].stageTileData[j].offsetX);
						posY = static_cast<int>(y + stageData[i].stageTileData[j].offsetY);
						mapchipPos = y * stageData[i].stageTileData[j].width + x;

						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x = static_cast<float>(posX * blockSize);
						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y = static_cast<float>(posY * blockSize);
						stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z = 0.0f;
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].x = static_cast<float>((posX + 1) * blockSize);
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].y = static_cast<float>((posY + 1) * blockSize);
						stageData[i].stageTileData[j].drawRightDown[mapchipPos].z = 0.0f;
					}
				}
			}
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

					pos1.x = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].x + static_cast<float>(offsetX);
					pos1.y = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].y + static_cast<float>(offsetY);
					pos1.z = stageData[i].stageTileData[j].drawLeftUp[mapchipPos].z;
					pos2.x = stageData[i].stageTileData[j].drawRightDown[mapchipPos].x + static_cast<float>(offsetX);
					pos2.y = stageData[i].stageTileData[j].drawRightDown[mapchipPos].y + static_cast<float>(offsetY);
					pos2.z = stageData[i].stageTileData[j].drawRightDown[mapchipPos].z;

					switch (stageData[i].stageTileData[j].mapchip[mapchipPos])
					{
					case MapchipData::EMPTY_STAGE:
					{
						continue;
						break;
					}
					case MapchipData::BLOCK:
					{
						//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						//DrawShape::DrawPlane(pos1, pos2, GRAY);
						DrawExtendGraph(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), BlockHandle, true);
						break;
					}
					case MapchipData::GOAL:
					{
						/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0xE0);
						DrawShape::DrawPlane(pos1, pos2, YELLOW);*/
						DrawExtendGraph(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), GoalHandle, true);
						break;
					}
					case MapchipData::NONE:
					case MapchipData::START:
					default:
					{
						/*SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0x80);
						DrawShape::DrawPlane(pos1, pos2, WHITE);*/
						DrawExtendGraph(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), EmptyHandle, true);
						break;
					}
					}
				}
			}
		}

		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0xFF);

		// つなぎ目
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
		// 関数失敗
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

			stageData[i].stageTile[j] = static_cast<char>(stageData[i].stageTileData.size() + 1);
			stageData[i].stageTileData.push_back({});
			stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].stageNumber = static_cast<char>(j);

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
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].drawLeftUp.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].drawRightDown.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].startPos.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].endPos.push_back({});
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].easePos.push_back({});
			}

			if (stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].mapchip == nullptr)
			{
				// 関数失敗
				return EF;
			}

			if (LoadFile::LoadCSV(stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].mapchip,
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].width *
				stageData[i].stageTileData[stageData[i].stageTileData.size() - 1].height, fileHandle, endCharacter) != 0)
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
			// プレイヤーの開始位置の探索
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
		// 開始位置が無い
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
			case bodytype::up: //上入力
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

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}
				else
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}

				isAct = true;

				break;
			}
			case bodytype::down: //下入力
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

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}
				else
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}

				isAct = true;

				break;
			}
			case bodytype::left: //左入力
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

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					Open(playerTile, direction, i, onPlayerStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}
				else
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}

				isAct = true;

				break;
			}
			case bodytype::right: //右入力
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

				if (stageData[i].stageTileData[moveStageData].stageEase.isMove)
				{
					stageData[i].stageTileData[moveStageData].stageEase.isMove = false;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
				}

				if (stageData[i].stageTileData[moveStageData].isFold)
				{
					Open(playerTile, direction, i, moveStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
				}
				else
				{
					Fold(playerTile, direction, i, onPlayerStageTile, moveStageData);

					stageData[i].stageTileData[moveStageData].stageEase.isMove = true;
					stageData[i].stageTileData[moveStageData].stageEase.splineIndex = 0;
					stageData[i].stageTileData[moveStageData].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[moveStageData].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[moveStageData].stageEase.maxTime = 1.2f;
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

int Stage::FoldSimulation(const Vector3& playerPos, const unsigned char& direction, char* returnMapchip)
{
	static size_t onPlayerStageData = 0;
	static size_t moveStageTile = 0;
	static size_t moveStageData = 0;
	static bool isFold = false; //折れる物があるかどうか

	isFold = false;

	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if ((playerPos.x / blockSize >= stageData[i].stageTileData[j].offsetX &&
				playerPos.x / blockSize < stageData[i].stageTileData[j].offsetX + stageData[i].stageTileData[j].width) &&
				(playerPos.y / blockSize >= stageData[i].stageTileData[j].offsetY &&
					playerPos.y / blockSize < stageData[i].stageTileData[j].offsetY + stageData[i].stageTileData[j].height))
			{
				onPlayerStageData = i;
			}
			else
			{
				continue;
			}

			switch (direction)
			{
			case bodytype::up: //上入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) / stageData[i].width <= 0)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber - stageData[i].width;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
			}
			case bodytype::down: //下入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) / stageData[i].width >= stageData[i].height - 1)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber + stageData[i].width;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
			}
			case bodytype::left: //左入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) % stageData[i].width <= 0)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber - 1;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile < 0 ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
			}
			case bodytype::right: //右入力
			{
				if (static_cast<unsigned char>(stageData[i].stageTileData[j].stageNumber) % stageData[i].width >= stageData[i].width - 1)
				{
					// プレイヤーがいるステージタイルが端
					break;
				}

				moveStageTile = stageData[i].stageTileData[j].stageNumber + 1;
				moveStageData = stageData[i].stageTile[moveStageTile] - 1;

				if (moveStageTile >= static_cast<size_t>(stageData[i].width * stageData[i].height) ||
					stageData[i].stageTile[moveStageTile] == MapchipData::EMPTY_STAGE)
				{
					// ステージタイルがその方向に折れない
					break;
				}

				isFold = true;
			}
			default:
			{
				return EF;
				break;
			}
			}

			if (isFold)
			{
				break;
			}
		}

		if (isFold)
		{
			break;
		}
	}

	if (isFold)
	{
		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStageData].stageTileData[moveStageData].height); y++)
		{
			for (x = 0; x < stageData[onPlayerStageData].stageTileData[moveStageData].width; x++)
			{
				if (direction == bodytype::up || direction == bodytype::down)
				{
					mapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
					reverseMapchipPos = (stageData[onPlayerStageData].stageTileData[moveStageData].height - y - 1) * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
				}
				if (direction == bodytype::left || direction == bodytype::right)
				{
					mapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + x;
					reverseMapchipPos = y * stageData[onPlayerStageData].stageTileData[moveStageData].width + (stageData[onPlayerStageData].stageTileData[moveStageData].width - x - 1);
				}

				returnMapchip[mapchipPos] = stageData[onPlayerStageData].stageTileData[moveStageData].mapchip[reverseMapchipPos];
			}
		}

		if (stageData[onPlayerStageData].stageTileData[moveStageData].isFold)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		// 折れる物が無い
		return EF;
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

			stageData[i].stageTileData[j].stageEase.isMove = false;
			stageData[i].stageTileData[j].stageEase.splineIndex = 0;
			stageData[i].stageTileData[j].stageEase.timeRate = 0.0f;
			stageData[i].stageTileData[j].isFold = false;
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

			ContainerClear(stageData[i].stageTileData[j].drawLeftUp);
			ContainerClear(stageData[i].stageTileData[j].drawRightDown);
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

			ContainerClear(initStageData[i].stageTileData[j].drawLeftUp);
			ContainerClear(initStageData[i].stageTileData[j].drawRightDown);
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

size_t Stage::GetStageDataSize() const
{
	return stageData.size();
}

size_t Stage::GetStageTileDataSize(int i) const
{
	return stageData[i].stageTileData.size();
}

char Stage::GetStageWidth(int i) const
{
	return stageData[i].width;
}

char Stage::GetStageHeight(int i) const
{
	return stageData[i].height;
}

char Stage::GetStageTileWidth(int i, int j) const
{
	return stageData[i].stageTileData[j].width;
}

char Stage::GetStageTileHeight(int i, int j) const
{
	return stageData[i].stageTileData[j].height;
}

char Stage::GetStageMapchip(int i, int j, int mapchipPos) const
{
	return stageData[i].stageTileData[j].mapchip[mapchipPos];
}

bool Stage::GetPositionTile(Vector3 center, int i, int j) const
{
	float left = (float)stageData[i].stageTileData[j].offsetX * blockSize;
	float up = (float)stageData[i].stageTileData[j].offsetY * blockSize;
	float right = left + blockSize * (float)stageData[i].stageTileData[j].width;
	float down = up + blockSize * (float)stageData[i].stageTileData[j].height;

	if (center.x >= left && center.x < right && center.y >= up && center.y < down)
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Stage::GetStageTileOffsetX(int i, int j) const
{
	return stageData[i].stageTileData[j].offsetX;
}

int Stage::GetStageTileOffsetY(int i, int j) const
{
	return stageData[i].stageTileData[j].offsetY;
}

int Stage::Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] <= 0)
	{
		return EF;
	}

	if (direction == bodytype::up || direction == bodytype::down)
	{
		EaseingInit(onPlayerStage, moveStageData, direction);

		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			if (y >= stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
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
		EaseingInit(onPlayerStage, moveStageData, direction);

		for (y = 0; y < stageData[onPlayerStage].stageTileData[moveStageData].height; y++)
		{
			for (x = 0; x < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].width / 2); x++)
			{
				if (x >= stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1)
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
	stageData[onPlayerStage].stageTileData[moveStageData].isFold = true;
	playerTile[direction]--;

	return 0;
}

int Stage::Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData)
{
	if (playerTile[direction] > 0)
	{
		return EF;
	}

	if (direction == bodytype::up || direction == bodytype::down)
	{
		EaseingInit(onPlayerStage, moveStageData, direction + 2);

		for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height / 2); y++)
		{
			if (y >= stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1)
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
		EaseingInit(onPlayerStage, moveStageData, direction + 2);

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
	stageData[onPlayerStage].stageTileData[moveStageData].direction = direction + 1 + 2;
	stageData[onPlayerStage].stageTileData[moveStageData].isFold = false;
	playerTile[direction]++;

	return 0;
}

void Stage::EaseingInit(const size_t& onPlayerStage, const size_t& moveStageData, const int& direction)
{
	static float keepA = 0.0f, keepB = 0.0f;

	for (y = 0; y < static_cast<size_t>(stageData[onPlayerStage].stageTileData[moveStageData].height); y++)
	{
		for (x = 0; x < stageData[onPlayerStage].stageTileData[moveStageData].width; x++)
		{
			mapchipPos = y * stageData[onPlayerStage].stageTileData[moveStageData].width + x;

			switch (direction % 4)
			{
			case bodytype::up:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetY + stageData[onPlayerStage].stageTileData[moveStageData].height);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = keepA + keepB + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;
				break;
			}
			case bodytype::down:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY) + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].height - y - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetY - stageData[onPlayerStage].stageTileData[moveStageData].height);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = keepA + keepB;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;
				break;
			}
			case bodytype::left:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetX + stageData[onPlayerStage].stageTileData[moveStageData].width);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = keepA + keepB + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[i].stageTileData[j].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;

				break;
			}
			case bodytype::right:
			{
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].x = static_cast<float>(x + stageData[onPlayerStage].stageTileData[moveStageData].offsetX) + 1.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].y = static_cast<float>(y + stageData[onPlayerStage].stageTileData[moveStageData].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].startPos[mapchipPos] *= blockSize;

				keepA = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].width - x - 1);
				keepB = static_cast<float>(stageData[onPlayerStage].stageTileData[moveStageData].offsetX - stageData[onPlayerStage].stageTileData[moveStageData].width);

				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].x = keepA + keepB;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].y = static_cast<float>(y + stageData[i].stageTileData[j].offsetY);
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos].z = 0.0f;
				stageData[onPlayerStage].stageTileData[moveStageData].endPos[mapchipPos] *= blockSize;

				break;
			}
			default:
				break;
			}
		}
	}
}

void Stage::EaseingUpdate()
{
	for (i = 0; i < stageData.size(); i++)
	{
		for (j = 0; j < stageData[i].stageTileData.size(); j++)
		{
			if (stageData[i].stageTileData[j].stageEase.isMove == false)
			{
				continue;
			}
			if ((stageData[i].stageTileData[j].direction - 1) < 0)
			{
				continue;
			}

			static float ease = 0.0f;

			stageData[i].stageTileData[j].stageEase.addTime += stageData[i].stageTileData[j].stageEase.maxTime / 25.0f;
			stageData[i].stageTileData[j].stageEase.timeRate =
				min(stageData[i].stageTileData[j].stageEase.addTime / stageData[i].stageTileData[j].stageEase.maxTime, 1.0f);
			ease = Easing::easeOut(0.0f, 1.0f, stageData[i].stageTileData[j].stageEase.timeRate);

			for (y = 0; y < stageData[i].stageTileData[j].height; y++)
			{
				for (x = 0; x < stageData[i].stageTileData[j].width; x++)
				{
					static Vector3 axisPos = {};
					static Vector3 overPos = {};

					mapchipPos = y * stageData[i].stageTileData[j].width + x;

					switch ((stageData[i].stageTileData[j].direction - 1) % 4)
					{
					case bodytype::up:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[0].y + (stageData[i].stageTileData[j].offsetY * blockSize)),
							static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
						overPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							(stageData[i].stageTileData[j].offsetY * blockSize) - static_cast<float>(stageData[i].stageTileData[j].startPos[2].y),
							0.0f };
						break;
					case bodytype::down:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[0].y - blockSize),
							static_cast<float>(stageData[i].stageTileData[j].height * blockSize) };
						overPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[2].y) + blockSize,
							0.0f };
						break;
					case bodytype::left:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[0].x + (stageData[i].stageTileData[j].offsetX * blockSize)),
							static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
							static_cast<float>(stageData[i].stageTileData[j].width * blockSize) };
						overPos = { (stageData[i].stageTileData[j].offsetX * blockSize) - static_cast<float>(stageData[i].stageTileData[j].startPos[2].x),
							static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
							0.0f };
						break;
					case bodytype::right:
						axisPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[0].x - blockSize),
							static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
							static_cast<float>(stageData[i].stageTileData[j].width * blockSize) };
						overPos = { static_cast<float>(stageData[i].stageTileData[j].startPos[2].x - blockSize),
							static_cast<float>(stageData[i].stageTileData[j].startPos[mapchipPos].y),
							0.0f };
						break;
					default:
						break;
					}

					std::vector<Vector3> pos = {
						stageData[i].stageTileData[j].startPos[mapchipPos],
						axisPos,
						stageData[i].stageTileData[j].endPos[mapchipPos]
					};

					if ((stageData[i].stageTileData[j].direction - 1) % 4 == bodytype::left ||
						(stageData[i].stageTileData[j].direction - 1) % 4 == bodytype::right)
					{
						if (x > 2)
						{
							pos.insert(pos.begin() + 1, overPos);
						}
						else
						{
							pos.insert(pos.end(), stageData[i].stageTileData[j].endPos[mapchipPos]);
						}
					}
					else if ((stageData[i].stageTileData[j].direction - 1) % 4 == bodytype::up ||
						(stageData[i].stageTileData[j].direction - 1) % 4 == bodytype::down)
					{
						if (y > 2)
						{
							pos.insert(pos.begin() + 1, overPos);
						}
						else
						{
							pos.insert(pos.end(), stageData[i].stageTileData[j].endPos[mapchipPos]);
						}
					}

					if (stageData[i].stageTileData[j].stageEase.splineIndex < pos.size() - 2)
					{
						stageData[i].stageTileData[j].easePos[mapchipPos] = Easing::SplineCurve(
							pos,
							stageData[i].stageTileData[j].stageEase.splineIndex,
							stageData[i].stageTileData[j].stageEase.timeRate);
					}
				}
			}

			if (stageData[i].stageTileData[j].stageEase.timeRate >= 1.0f)
			{
				if (stageData[i].stageTileData[j].stageEase.splineIndex < 4 - 2)
				{
					stageData[i].stageTileData[j].stageEase.splineIndex++;
					stageData[i].stageTileData[j].stageEase.timeRate = 0.0f;
					stageData[i].stageTileData[j].stageEase.addTime = 0.1f;
					stageData[i].stageTileData[j].stageEase.maxTime = 1.5f;
				}
				else
				{
					stageData[i].stageTileData[j].stageEase.isMove = false;
					break;
				}
			}
		}
	}
}

int Stage::SearchTopStageTile()
{
	std::vector<Vector3> topStageTile; //xの値にstageDataの要素番号、yの値にstageData[i].stageDataの要素番号を格納する
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
