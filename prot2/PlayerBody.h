#pragma once
#include "Vector3.h"
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
	//1 or -1(1:right&down -1:left&up)
	void setslide(int slidepat, Vector3 startpos);
	// 描画
	void Draw();

public: //メンバ変数
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

	//上に重なっている噛みの数
	int overlap;

	//本体からどれだけ離れているか(左右専用)
	int bodydistance;

	//イージング
	//easing ease;

	//体の色
	int bodycolor = WHITE;
};
