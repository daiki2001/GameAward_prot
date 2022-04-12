#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "InputManger.h"
#include "Colors.h"

// ウィンドウのタイトルに表示する文字列
const char TITLE[] = "ワガミオリ";

// ウィンドウ横幅
const int WIN_WIDTH = 1280;

// ウィンドウ縦幅
const int WIN_HEIGHT = 720;

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

	SetUseLighting(false);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
	Player* player = Player::Get();
	unsigned char playerTile[4] = { 0 };

	Stage* stage = Stage::Get();
	stage->LoadStage("./Resources/CSV's/stage4.csv", playerTile);
	player->Init();
	player->bodysetup(false, left, true, up, false, right, false, down);

	int Back = LoadGraph("Resources/backSin.png");

	const int drawOffsetX = 340, drawOffsetY = 60;

	// ゲームループ
	while (1)
	{
		Input::Update();

		// 画面クリア
		ClearDrawScreen();
		//---------  ここからプログラムを記述  ----------//

		// 更新処理
		if (Input::isKey(KEY_INPUT_1))
		{
			stage->LoadStage("./Resources/CSV's/stage1.csv", playerTile);
			player->Init();
			player->bodysetup(false, left, true, up, true, right, false, down);
		}
		if (Input::isKey(KEY_INPUT_2))
		{
			stage->LoadStage("./Resources/CSV's/stage4.csv", playerTile);
			player->Init();
			player->bodysetup(false, left, true, up, false, right, false, down);
		}
		/*if (Input::isKey(KEY_INPUT_3))
		{
			stage->LoadStage("./Resources/stage5.csv", playerTile);
			player->Init();
			player->bodysetup(true, left, false, up, false, right);
		}
		if (InputManger::Reset())
		{
			stage->Reset();
			stage->GetInitFoldCount(playerTile);
			player->Init();
			player->bodysetup(false, left, true, up, false, right);
		}*/
		if (InputManger::SubUpTrigger() || InputManger::SubDownTrigger() || InputManger::SubLeftTrigger() || InputManger::SubRightTrigger())
		{
			stage->FoldAndOpen(player->CenterPosition, playerTile);
		}

		stage->Updata();
		player->Update(*stage);

		// 描画処理
		DrawGraph(0, 0, Back, true);
		//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), true);
		stage->Draw(drawOffsetX, drawOffsetY);
		player->Draw(drawOffsetX, drawOffsetY);

		//DrawFormatString(5, 200, GetColor(255, 255, 255), "R:リセット");
		//DrawFormatString(5, 220, GetColor(255, 255, 255), "1：ステージ1再設定");
		//DrawFormatString(5, 240, GetColor(255, 255, 255), "2：ステージ2再設定");
		//DrawFormatString(5, 260, GetColor(255, 255, 255), "3：ステージ3");

		//DrawFormatString(5, 280, GetColor(255, 255, 255), "方向キー：ステージの折る・開く");

		//---------  ここまでにプログラムを記述  ---------//
		// (ダブルバッファ)裏面
		DxLib::ScreenFlip();

		// 20ミリ秒待機(疑似60FPS)
		DxLib::WaitTimer(20);

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
	DxLib::DxLib_End();

	// 正常終了
	return 0;
}