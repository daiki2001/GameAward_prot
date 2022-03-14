#pragma once
#include <vector>
#include "Vector3.h"

/*メモリが大きくなりすぎる懸念がある箇所はchar型にしています*/

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
public: //シングルトン化
	static Stage* Get();
private:
	Stage();
	Stage(const Stage&) = delete;
	~Stage();
	Stage operator=(const Stage&) = delete;

public: //サブクラス
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

public: //定数
	static const int stageSize = 5;
	static const int blockSize;

public: //静的メンバ変数
	static int foldGraph;
private:
	static int startPlayerPosX;
	static int startPlayerPosY;

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Updata();
	// 描画
	void Draw();

	/// <summary>
	/// ステージファイルの読み込み
	/// </summary>
	/// <param name="playerTileArray"> プレイヤーが折れる回数(0番から上、下、左、右) </param>
	/// <param name="fileHandle"> ステージファイルのパス </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// ステージを折る
	int Fold(const Vector3& playerPos, unsigned char playerTile[4]);
	// リセット
	inline void Reset() { stageData = initStageData; }
	// 内部データ全削除
	void DataClear();

	// プレイヤーのx軸上の開始位置を取得
	static int GetStartPlayerPosX() { return startPlayerPosX; }
	// プレイヤーのx軸上の開始位置を取得
	static int GetStartPlayerPosY() { return startPlayerPosY; }
	// ステージタイルのデータを取得
	inline StageTileData* GetStageTileData(const short& stageNumber, const short& stageTileNumber);
	// ステージのデータを取得
	inline StageData* GetStageData(const short& stageNumber);
	// 全ステージのデータを取得
	inline StageData* GetAllStageData();

private: //メンバ変数
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;
};
