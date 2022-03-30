#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
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

	void IsHitPlayerBody(Stage& stage);

	void ExtrudePlayer(Vector3 ExtrudePos, float ExtrudeDis, bodytype ExtrudeType);
public: //メンバ変数
	//床の高さ
	float FloorHeight;

	//体の構成要素
	Vector3 CenterPosition;

	//体(折るほう)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;

	//body_twoを優先的に開くか
	bool IsOpenTwo;

	//どの方向を折ったか(4方向)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//ジャンプ
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;
	bool IsFall;

	//落下中・ジャンプ中にジャンプ入力が入っているかどうか
	bool IsInputjump = false;

	//どれか一つでも体動かしていたらtrue
	bool Player_IsAction;
	//画像ハンドル(顔)
	int FaceHandle[2];

	bool IsGoal;

	bool IsColide;

	//足
	PlayerFoot playerFoot;
};
