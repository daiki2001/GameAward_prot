#pragma once
#include "Vector3.h"
#include "Easing.h"
#include "Colors.h"

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

public: //メンバ変数
	//有効化フラグ
	bool Isactivate;

	//顔から見た体の位置
	int body_type;

	//体の座標
	Vector3 bodystartpos;

	//端の座標
	Vector3 bodyendpos;

	//スライドの開始座標
	Vector3 slidestartpos;

	//折る・開くフラグ
	bool Isfold;
	bool Isopen;

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
