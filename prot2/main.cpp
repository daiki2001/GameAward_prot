#include <DxLib.h>
#include "Player.h"
#include "Stage.h"
#include "Vector3.h"
#include "Quaternion.h"
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

	SetUseZBuffer3D(TRUE);   //Z�o�b�t�@��L���ɂ���
	SetWriteZBuffer3D(TRUE); //Z�o�b�t�@�ւ̏������݂�L���ɂ���

	const float cameraRange = 625.0f;

	Vector3 cameraPosition(WIN_WIDTH / 2.0f, -WIN_HEIGHT / 2.0f, -cameraRange); //�J�����̈ʒu
	Vector3 cameraTarget(WIN_WIDTH / 2.0f, -WIN_HEIGHT / 2.0f, 0.0f);     //�J�����̒����_
	Vector3 cameraUp(0.0f, 1.0f, 0.0f);             //�J�����̏�����x�N�g��

	float cameraRightAngle = 0.0f; //�o�x
	float cameraUpAngle = -0.3f;    //�ܓx

	Quaternion qCameraRightAngle = quaternion(cameraUp, cameraRightAngle);
	Vector3 cameraSide = cameraUp.Cross(cameraTarget - cameraPosition).Normalize();
	Quaternion qCameraUpAngle = quaternion(cameraSide, cameraUpAngle);

	Quaternion q = qCameraRightAngle * qCameraUpAngle;
	Quaternion qCameraPosition = quaternion(cameraPosition.x, cameraPosition.y, cameraPosition.z, 0.0f);
	Quaternion qq = conjugate(q);

	qCameraPosition = q * qCameraPosition * qq;
	cameraPosition = { qCameraPosition.x, qCameraPosition.y, qCameraPosition.z };

	// ������x�N�g���̉�]
	Quaternion qCameraUp = quaternion(cameraUp.x, cameraUp.y, cameraUp.z, 0.0f);
	qCameraUp = q * qCameraUp * qq;
	cameraUp = getAxis(qCameraUp); //�J�����̏�����x�N�g���̎��̎��o��

	SetCameraNearFar(1.0f, 1000.0f); //�J�����̗L���͈͂�ݒ�
	SetCameraScreenCenter(WIN_WIDTH / 2.0f, WIN_HEIGHT / 2.0f); //��ʂ̒��S���J�����̒��S�ɍ��킹��
	SetCameraPositionAndTargetAndUpVec(cameraPosition, cameraTarget, cameraUp);

	SetUseLighting(false);

	// �摜�Ȃǂ̃��\�[�X�f�[�^�̕ϐ��錾�Ɠǂݍ���

	// �Q�[�����[�v�Ŏg���ϐ��̐錾
	Player* player = Player::Get();
	unsigned char playerTile[4] = { 0 };

	Stage* stage = Stage::Get();
	stage->LoadStage("./Resources/stage1.csv", playerTile);
	player->Init();
	player->bodysetup(playerTile);

	const int drawOffsetX = 320, drawOffsetY = 0;

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

		// �`�揈��
		stage->Draw(drawOffsetX, drawOffsetY/* - WIN_HEIGHT*/);
		player->Draw(drawOffsetX, drawOffsetY);

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