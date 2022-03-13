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
		bool turnFlagX = false;
		bool turnFlagY = false;
	};
	struct StageData
	{
		char* stageTile = nullptr;
		unsigned char width = 1;
		unsigned char height = 1;
	};

public: //定数
	static const int stageSize = 5;
	static const int blockSize;

public: //静的メンバ変数
	static int foldGraph;
private:
	static Vector3 startPlayerPos;

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
	/// <param name="fileHandle"> ステージファイルのパス </param>
	/// <returns> 0で成功、0以外で失敗 </returns>
	int LoadStage(const char* fileHandle);

	// ステージを折る
	int Fold(const Vector3& playerPos);
	// リセット
	inline void Reset() { stageData = initStageData; }
	// 内部データ全削除
	void DataClear();

	// ステージの枚数を取得
	static Vector3 GetStartPlayerPos();
	// 各ステージのデータを取得
	inline StageTileData* GetStageData(const short& stageTileNumber);
	// 全ステージのデータを取得
	inline StageTileData* GetAllStageData();

private: //メンバ変数
	StageData stageTile;
	std::vector<StageTileData> stageData;
	std::vector<StageTileData> initStageData;
	std::vector<char> offsetX, offsetY;
};
