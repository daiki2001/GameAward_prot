#pragma once
#include <vector>
#include "Vector3.h"
#include "Easing.h"

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
		std::vector<Vector3> leftUpPos = {};
		std::vector<Vector3> rightDownPos = {};
		size_t size = 1;
		unsigned char width = 1;
		unsigned char height = 1;
		unsigned char direction = 0;
		bool isEase = false;
		std::vector<Vector3> startPos = {};
		std::vector<Vector3> endPos = {};
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

public: //定数
	static const int blockSize;
	static const int halfBlockSize;

	Vector3 offset = { 0,0,0 };

private: //静的メンバ変数
	static int startPlayerPosX;
	static int startPlayerPosY;
	static unsigned char initFoldCount[4];

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Updata();
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// ステージファイルの読み込み
	/// </summary>
	/// <param name="foldCount"> プレイヤーが折れる回数(0番から上、下、左、右) </param>
	/// <param name="fileHandle"> ステージファイルのパス </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	int LoadStage(const char* fileHandle, unsigned char playerTileArray[4]);

	// ステージを折る・開く
	int FoldAndOpen(const Vector3& playerPos, unsigned char foldCount[4]);
	// リセット
	void Reset();
	// 内部データ全削除
	void DataClear();

	// プレイヤーのx軸上の開始位置を取得
	inline static int GetStartPlayerPosX() { return startPlayerPosX; }
	// プレイヤーのx軸上の開始位置を取得
	inline static int GetStartPlayerPosY() { return startPlayerPosY; }
	// プレイヤーの折れる回数の初期状態を取得
	static void GetInitFoldCount(unsigned char foldCount[4]);
	// ステージタイルのデータを取得
	inline StageTileData* GetStageTileData(const short& stageNumber, const short& stageTileNumber);
	// ステージのデータを取得
	inline StageData* GetStageData(const short& stageNumber);
	// 全ステージのデータを取得
	inline StageData* GetAllStageData();
	//StageDataのサイズを取得
	size_t GetStageDataSize();
	//StageTileDataのサイズを取得
	size_t GetStageTileDataSize(int i);

	char GetStageWidth(int i);
	char GetStageHeight(int i);

	char GetStageTileWidth(int i, int j);
	char GetStageTileHeight(int i, int j);

	char GetStageMapchip(int i, int j, int mapchipPos);

	bool GetPlayerTile(Vector3 center, int i, int j);

private:
	// ステージを折る
	int Fold(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& onPlayerStageTile, const size_t& moveStageData);
	// ステージを開く
	int Open(unsigned char playerTile[4], const unsigned char& direction, const size_t& onPlayerStage, const size_t& moveStageTile, const size_t& moveStageData);
	// 一番上のステージタイルを探す
	int SearchTopStageTile();

private: //メンバ変数
	std::vector<StageData> stageData;
	std::vector<StageData> initStageData;

	Easing stageEase;
};
