#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "InputManger.h"
#include "Colors.h"

// �E�B���h�E�̃^�C�g���ɕ\�����镶����
const char TITLE[] = "���K�~�I��";

// �E�B���h�E����
const int WIN_WIDTH = 1280;

// �E�B���h�E�c��
const int WIN_HEIGHT = 720;

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	// �E�B���h�E���[�h�ɐݒ�
	ChangeWindowMode(TRUE);

	// �E�B���h�E�T�C�Y���蓮�ł͕ύX�������A
	// ���E�B���h�E�T�C�Y�ɍ��킹�Ċg��ł��Ȃ��悤�ɂ���
	SetWindowSizeChangeEnableFlag(FALSE, FALSE);

	// �^�C�g����ύX
	SetMainWindowText(TITLE);

	// ��ʃT�C�Y�̍ő�T�C�Y�A�J���[�r�b�g����ݒ�(���j�^�[�̉𑜓x�ɍ��킹��)
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);

	// ��ʃT�C�Y��ݒ�(�𑜓x�Ƃ̔䗦�Őݒ�)
	SetWindowSizeExtendRate(1.0);

	// ��ʂ̔w�i�F��ݒ肷��
	SetBackgroundColor(0x00, 0x00, 0xFF);

	// DXlib�̏�����
	if (DxLib_Init() == -1) { return -1; }

	// (�_�u���o�b�t�@)�`���O���t�B�b�N�̈�͗��ʂ��w��
	SetDrawScreen(DX_SCREEN_BACK);

	SetUseLighting(false);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	Player* player = Player::Get();
	unsigned char playerTile[4] = { 0 };

	Stage* stage = Stage::Get();
	stage->LoadStage("./Resources/CSV's/stage4.csv", playerTile);
	player->Init();
	player->bodysetup(false, left, true, up, false, right, false, down);

	int Back = LoadGraph("Resources/backSin.png");

	const int drawOffsetX = 340, drawOffsetY = 60;

	// �Q�[�����[�v
	while (1)
	{
		Input::Update();

		// ��ʃN���A
		ClearDrawScreen();
		//---------  ��������v���O�������L�q  ----------//

		// �X�V����
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

		// �`�揈��
		DrawGraph(0, 0, Back, true);
		//DrawBox(0, 0, 1280, 720, GetColor(0, 0, 0), true);
		stage->Draw(drawOffsetX, drawOffsetY);
		player->Draw(drawOffsetX, drawOffsetY);

		//DrawFormatString(5, 200, GetColor(255, 255, 255), "R:���Z�b�g");
		//DrawFormatString(5, 220, GetColor(255, 255, 255), "1�F�X�e�[�W1�Đݒ�");
		//DrawFormatString(5, 240, GetColor(255, 255, 255), "2�F�X�e�[�W2�Đݒ�");
		//DrawFormatString(5, 260, GetColor(255, 255, 255), "3�F�X�e�[�W3");

		//DrawFormatString(5, 280, GetColor(255, 255, 255), "�����L�[�F�X�e�[�W�̐܂�E�J��");

		//---------  �����܂łɃv���O�������L�q  ---------//
		// (�_�u���o�b�t�@)����
		DxLib::ScreenFlip();

		// 20�~���b�ҋ@(�^��60FPS)
		DxLib::WaitTimer(20);

		// Windows�V�X�e�����炭�������������
		if (ProcessMessage() == -1)
		{
			break;
		}

		// ESC�L�[�������ꂽ�烋�[�v���甲����
		if (InputManger::Escape())
		{
			break;
		}
	}
	// Dx���C�u�����I������
	DxLib::DxLib_End();

	// ����I��
	return 0;
}