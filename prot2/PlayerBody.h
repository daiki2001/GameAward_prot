#pragma once
#include "Stage.h"
#include "Vector3.h"
#include "Easing.h"

enum bodytype
{
	left,
	up,
	right,
	down
};

class PlayerBody
{
public: //定数
	//体の大きさ
	static const float BodySize;
	static const float HalfBodySize;

public: //静的メンバ関数
	// 画像の初期化
	static void GraphInit();

public: //静的メンバ変数
	static Stage* stage;

	//画像ハンドル
	static int BodyHandle;
	static int BodyHandle_fold;

public: //メンバ関数
	PlayerBody();
	~PlayerBody();

	// 初期化
	void Init(Vector3 position, bodytype number);
	// 更新
	void Update(Vector3& center);
	/// <summary>
	// 描画
	void Draw(int offsetX, int offsetY);

	/// <summary>
	/// 体を有効化した時の設定
	/// </summary>
	/// <param name="center">有効化した時の座標参照先</param>
	void setactivate(Vector3 center);
	/// 体のスライドのセットアップ
	/// </summary>
	/// <param name="slidepat">スライドする向き(左上:-1 右下:1)</param>
	/// <param name="move_dis">スライドする距離(隣:1 顔をまたぐ:2)</param>
	void setslide(int slidepat, int move_dis);

	/// <summary>
	/// プレイヤーの当たり判定
	/// </summary>
	/// <param name="stage">ステージデータ</param>
	/// <param name="center">プレイヤーの中心</param>
	/// <returns></returns>
	void IsHitBody(Vector3* center, float& FallSpeed, bool& isfall, bool& isjump, bool& iscolide);

	/// <summary>
	/// 押し出し処理
	/// </summary>
	/// <param name="center">押し出される側の座標</param>
	/// <param name="extrudepos">押し出す側の座標</param>
	/// <param name="extrudedis">押し出す距離</param>
	/// <param name="extrudetype">どの方向"から"押し出すか</param>
	void Extrude(Vector3* center, Vector3 extrudepos, float extrudedis, bodytype extrudetype, bool& isfall, bool& isjump, bool& iscolide);

public: //メンバ変数
	//有効化フラグ
	bool IsActivate;

	//顔から見た体の位置
	int Body_Type;

	//体の座標(初期状態：左上)
	Vector3 BodyStartPos;

	//体の座標(初期状態：右下)
	Vector3 BodyEndPos;

	//スライドの開始座標
	Vector3 SlideStartPos;

	//折る・開くフラグ
	bool IsFold;
	bool IsOpen;

	//この体が折られた回数
	int FoldCount;

	//スライドフラグ・スライドする方向
	bool IsSlide;
	int SlidePat;

	//折る・開く・スライドをしている途中かどうか
	bool IsAction;

	//上下左右それぞれの当たり判定
	bool IsHitLeft;
	bool IsHitUp;
	bool IsHitRight;
	bool IsHitDown;

	//body別落下判定
	bool BodyIsFall;

	//スライドする距離
	int SlideDis;

	//上に重なっている噛みの数
	int Overlap;

	//本体からどれだけ離れているか(左右専用)
	int BodyDistance;

	//イージング
	Easing Ease;

	//体の色
	int BodyColor;
};
