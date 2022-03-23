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
	void Update(Stage& stage);
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// どの体を有効化するか
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type);
	void bodysetup(const unsigned char foldCount[4]);

public: //メンバ変数
	//床の高さ
	float floorHeight;

	//体の構成要素
	Vector3 center_position;

	//体
	PlayerBody body_one;
	PlayerBody body_two;
	PlayerBody body_three;

	//折った体の順番
	int foldlist[3];

	//body_twoを優先的に開くか
	bool isopentwo;

	//ジャンプ
	bool IsJump;
	float jumpspeed;
	float fallspeed;
	bool IsFall;
};
