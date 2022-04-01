#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "Vector3.h"
#include "Quaternion.h"
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

	SetUseZBuffer3D(TRUE);   //Zバッファを有効にする
	SetWriteZBuffer3D(TRUE); //Zバッファへの書き込みを有効にする

	const float cameraRange = 625.0f;

	Vector3 cameraPosition(WIN_WIDTH / 2.0f, -WIN_HEIGHT / 2.0f, -cameraRange); //カメラの位置
	Vector3 cameraTarget(WIN_WIDTH / 2.0f, -WIN_HEIGHT / 2.0f, 0.0f);     //カメラの注視点
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);             //カメラの上方向ベクトル

	float cameraRightAngle = 0.0f; //経度
	float cameraUpAngle = -0.3f;    //緯度

	Quaternion qCameraRightAngle = quaternion(cameraUp, cameraRightAngle);
	Vector3 cameraSide = cameraUp.Cross(cameraTarget - cameraPosition).Normalize();
	Quaternion qCameraUpAngle = quaternion(cameraSide, cameraUpAngle);

	Quaternion q = qCameraRightAngle * qCameraUpAngle;
	Quaternion qCameraPosition = quaternion(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f);
	Quaternion qq = conjugate(q);

	qCameraPosition = q * qCameraPosition * qq;
	cameraPosition = { qCameraPosition.x, qCameraPosition.y, qCameraPosition.z };

	// 上方向ベクトルの回転
	Quaternion qCameraUp = quaternion(cameraUp.x, cameraUp.y, cameraUp.z, 0.0f);
	qCameraUp = q * qCameraUp * qq;
	cameraUp = getAxis(qCameraUp); //カメラの上方向ベクトルの軸の取り出し

	SetCameraNearFar(1.0f, 1000.0f); //カメラの有効範囲を設定
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f); //画面の中心をカメラの中心に合わせる
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

	SetUseLighting(false);

	// 画像などのリソースデータの変数宣言と読み込み

	// ゲームループで使う変数の宣言
	Player* player = Player::Get();
	unsigned char playerTile[4] = { 0 };

	Stage* stage = Stage::Get();
	stage->LoadStage("./Resources/stage1.csv", playerTile);
	player->Init();
	player->bodysetup(playerTile);

	const int drawOffsetX = 320, drawOffsetY = 0;

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
			stage->LoadStage("./Resources/stage1.csv", playerTile);
			player->Init();
			player->bodysetup(playerTile);
		}
		if (Input::isKey(KEY_INPUT_2))
		{
			stage->LoadStage("./Resources/stage2.csv", playerTile);
			player->Init();
			player->bodysetup(playerTile);
		}
		if (Input::isKey(KEY_INPUT_3))
		{
			stage->LoadStage("./Resources/stage3.csv", playerTile);
			player->Init();
			player->bodysetup(playerTile);
		}
		if (InputManger::Reset())
		{
			stage->Reset();
			stage->GetInitFoldCount(playerTile);
			player->Init();
			player->bodysetup(playerTile);
		}

		if (InputManger::SubUpTrigger() || InputManger::SubDownTrigger() || InputManger::SubLeftTrigger() || InputManger::SubRightTrigger())
		{
			stage->FoldAndOpen(player->CenterPosition, playerTile);
		}

		stage->Updata();
		player->Update(*stage);

		// 描画処理
		stage->Draw(drawOffsetX, drawOffsetY/* - WIN_HEIGHT*/);
		player->Draw(drawOffsetX, drawOffsetY);

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