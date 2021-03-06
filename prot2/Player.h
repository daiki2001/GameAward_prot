#pragma once
#include "PlayerBody.h"
#include "PlayerFoot.h"
#include "Stage.h"
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

public: //静的メンバ変数
	static Stage* stage;

public: //メンバ関数
	// 初期化
	void Init();
	// 更新
	void Update();
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// どの体を有効化するか
	/// </summary>
	/// <param name="one">body_one</param>
	/// <param name="two">body_two</param>
	/// <param name="three">body_three</param>
	void bodysetup(bool one, int one_type, bool two, int two_type, bool three, int three_type, bool four, int four_type);
	void bodysetup(const unsigned char foldCount[4]);

	void IsHitPlayerBody();

	void ExtrudePlayer(Vector3 ExtrudePos, float ExtrudeDis, bodytype ExtrudeType);

	bool IsFall();
public: //メンバ変数
	//床の高さ
	float FloorHeight;

	//体の構成要素
	Vector3 CenterPosition;

	//向いている方向
	bool IsLeft;
	bool IsRight;

	//体(折るほう)
	PlayerBody Body_One;
	PlayerBody Body_Two;
	PlayerBody Body_Three;
	PlayerBody Body_Four;

	//body_twoを優先的に開くか
	bool IsOpenTwo;

	//どの方向を折るか(4方向)
	bool IsLeftFold;
	bool IsUpFold;
	bool IsRightFold;
	bool IsDownFold;

	//移動速度
	float SideMoveSpeed;

	//ジャンプ
	bool IsJump;
	float JumpSpeed;
	float FallSpeed;

	//落下判定(顔のみ
	bool IsFaceFall;

	//体と顔すべてを考慮した落下判定
	bool IsAllFall;

	//落下中・ジャンプ中にジャンプ入力が入っているかどうか
	bool IsInputjump;

	//どれか一つでも体を動かしていたらtrue
	bool Player_IsAction;

	//画像ハンドル(顔)
	int FaceHandle[2];

	//ゴールに触れているかどうか
	bool IsGoal;

	//ブロックに当たっているかどうか
	bool IsColide;

	//下に体があるかどうか
	bool IsDownBody;

	//足
	PlayerFoot leg;
};
