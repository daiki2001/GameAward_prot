#pragma once
#include <DxLib.h>
#include "Vector3.h"
#include "Easing.h"
#include "Colors.h"
#include "Stage.h"

enum bodytype
{
	left,
	up,
	right,
	down
};

class PlayerBody
{
public: //メンバ関数
	PlayerBody();
	~PlayerBody();

	// 初期化
	void Init(Vector3 position, bodytype number);
	// 更新
	void Update(Vector3 center);
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
	void IsHitBody(Stage& stage, Vector3& center, PlayerBody& body_one, PlayerBody& body_two, bool& isfall,bool& isjump);

	/// <summary>
	/// 押し出し処理
	/// </summary>
	/// <param name="center">押し出される側の座標</param>
	/// <param name="extrudepos">押し出す側の座標</param>
	/// <param name="extrudedis">押し出す距離</param>
	/// <param name="extrudetype">どの方向"から"押し出すか</param>
	void Extrude(Vector3& center, Vector3 extrudepos, float extrudedis, bodytype extrudetype, bool& isfall, bool& isjump);

public: //メンバ変数
	//有効化フラグ
	bool Isactivate;

	//顔から見た体の位置
	int body_type;

	//体の座標(初期状態：左上)
	Vector3 bodystartpos;

	//体の座標(初期状態：右下)
	Vector3 bodyendpos;

	//スライドの開始座標
	Vector3 slidestartpos;

	//折る・開くフラグ
	bool Isfold;
	bool Isopen;

	//この体が折られた回数
	int foldcount;

	//スライドフラグ・スライドする方向
	bool Isslide;
	int slidepat;

	//折る・開く・スライドをしている途中かどうか
	bool Isaction;

	//スライドする距離
	int slide_dis;

	//上に重なっている噛みの数
	int overlap;

	//本体からどれだけ離れているか(左右専用)
	int bodydistance;

	//イージング
	Easing ease;

	//体の色
	int bodycolor = WHITE;
};
