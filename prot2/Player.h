#pragma once
#include "Vector3.h"

class Player final
{
public: //シングルトン化
	static Player* Get();
private:
	Player();
	Player(const Player&) = delete;
	~Player();
	Player operator=(const Player&) = delete;

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Updata();
	// 描画
	void Draw();

	// プレイヤーの座標を取得
	inline Vector3 GetPos() { return pos; }

private: //メンバ変数
	Vector3 pos;
};
