#pragma once
#include "Input.h"

// 入力をまとめたクラス
class InputManger final
{
private: //シングルトン化
	InputManger() {}
	InputManger(const InputManger&) = delete;
	~InputManger() {}
	InputManger operator=(const InputManger&) = delete;

public: //静的メンバ関数
	// 終了
	static bool Escape();
	// 終了(押した瞬間の判定)
	static bool EscapeTrigger();
	// 終了(離した瞬間の判定)
	static bool EscapeReturn();
	// 上
	static bool Up();
	// 上(押した瞬間の判定)
	static bool UpTrigger();
	// 上(離した瞬間の判定)
	static bool UpReturn();
	// 下
	static bool Down();
	// 下(押した瞬間の判定)
	static bool DownTrigger();
	// 下(離した瞬間の判定)
	static bool DownReturn();
	// 左
	static bool Left();
	// 左(押した瞬間の判定)
	static bool LeftTrigger();
	// 左(離した瞬間の判定)
	static bool LeftReturn();
	// 右
	static bool Right();
	// 右(押した瞬間の判定)
	static bool RightTrigger();
	// 右(離した瞬間の判定)
	static bool RightReturn();
	// リセット
	static bool Reset();
	// リセット(押した瞬間の判定)
	static bool ResetTrigger();
	// リセット(離した瞬間の判定)
	static bool ResetReturn();
	// アクション1
	static bool Act1();
	// アクション1(押した瞬間の判定)
	static bool Act1Trigger();
	// アクション1(離した瞬間の判定)
	static bool Act1Return();
};
