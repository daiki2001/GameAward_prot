#pragma once
#include "PlayerBody.h"
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
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// どの体を有効化するか
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type);

public: //メンバ変数
	//床の高さ
	float floorHeight = 500;

	//体の構成要素
	Vector3 center_position = { 100.0f, 100.0f, 0.0f };

	//体(折るほう)
	PlayerBody body_one;
	PlayerBody body_two;
	PlayerBody body_three;

	//ジャンプ
	bool IsJump = false;
	float jumpspeed = 3.0f;
	float fallspeed = 3.0f;
	bool IsLand = false;
};
