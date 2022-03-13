#include <DxLib.h>
#include "Stage.h"
#include "Player.h"
#include "InputManger.h"
#include "Colors.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "ワガミオリ";

// ウィンドウ横幅
const int WIN_WIDTH = 600;

// ウィンドウ縦幅
const int WIN_HEIGHT = 400;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// ウィンドウモードに設定
	ChangeWindowMode(TRUE);

	// ウィンドウサイズを手動では変更させず、
	// かつウィンドウサイズに合わせて拡大できないようにする
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// タイトルを変更
	SetMainWindowText(TITLE);

	// 画面サイズの最大サイズ、カラービット数を設定(モニターの解像度に合わせる)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// 画面サイズを設定(解像度との比率で設定)
	SetWindowSizeExtendRate(1.0);

	// 画面の背景色を設定する
	SetBackgroundColor(0x00, 0x00, 0xFF);

	// DXlibの初期化
	if (DxLib_Init() == -1) { return -1; }

	// (ダブルバッファ)描画先グラフィック領域は裏面を指定
	SetDrawScreen(DX_SCREEN_BACK);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
	Stage* stage = Stage::Get();
	stage->LoadStage("./Resources/stage1.csv");

	Player* player = Player::Get();

	// ゲームループ
	while (1)
	{
		Input::Update();

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		player->Updata();
		if (Input::isKey(KEY_INPUT_1))
		{
			stage->LoadStage("./Resources/stage1.csv");
		}
		if (Input::isKey(KEY_INPUT_2))
		{
			stage->LoadStage("./Resources/stage2.csv");
		}
		if (InputManger::Reset())
		{
			stage->Reset();
		}
		if (InputManger::Act1() && (InputManger::UpTrigger() || InputManger::DownTrigger() || InputManger::LeftTrigger() || InputManger::RightTrigger()))
		{
			stage->Fold(player->GetPos());
		}

		// 描画処理
		stage->Draw();
		player->Draw();

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		ScreenFlip();	

		// 20ミリ秒待機(疑似60FPS)
		WaitTimer(20);

		// Windowsシステムからくる情報を処理する
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESCキーが押されたらループから抜ける
		if (InputManger::Escape())
		{
			break;
		}
	}
	// Dxライブラリ終了処理
	DxLib_End();

	// 正常終了
	return 0;
}