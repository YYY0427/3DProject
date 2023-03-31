#include "DxLib.h"
#include "Game.h"
#include "DrawPoly.h"
#include "Map.h"
#include "Player.h"
#include <cmath>
#include <vector>

namespace
{
	// ���_����J�����܂ł̋���
	constexpr float kCameraDistance = 800.0f;
}

// �O���b�h�\��
void DrawGrid()
{
	// �O���b�h�̊J�n�ʒu�ƏI���ʒu�̐錾
	VECTOR start = VGet(0, 0, 0);
	VECTOR end = VGet(0, 0, 0);

	// �O���b�h�̕\��
	for (float z = -1000.0f; z <= 1000.0f; z += 100.0f)
	{
		start = VGet(-1000.0f, 0.0f, z);
		end = VGet(1000.0f, 0.0f, z);
		DrawLine3D(start, end, 0x00ff00);
	}
	for (float x = -1000.0f; x <= 1000.0f; x += 100.0f)
	{
		start = VGet(x, 0.0f, -1000.0f);
		end = VGet(x, 0.0f, 1000.0f);
		DrawLine3D(start, end, 0x00ff00);
	}
}

// �v���O������ WinMain ����n�܂�܂�
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// window���[�h�ݒ�
	ChangeWindowMode(Game::kWindowMode);

	// window���ݒ�
	SetMainWindowText(Game::kTitleText);

	// ��ʃT�C�Y�̐ݒ�
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;				// �G���[���N�����璼���ɏI��
	}

	//�_�u���o�b�t�@���[�h
	SetDrawScreen(DX_SCREEN_BACK);

	// Z�o�b�t�@��L���ɂ���
	SetUseZBuffer3D(true);

	// Z�o�b�t�@�ւ̏������݂��s��
	SetWriteZBuffer3D(true);

	// �J�����̐ݒ�
	SetCameraNearFar(5.0f, 2800.0f);

	// ���ʃ|���S����\�����Ȃ�
	SetUseBackCulling(true);

	// ����p�̐ݒ�
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

	// �J�����̈ʒu�ƌ����̐ݒ�
	SetCameraPositionAndTarget_UpVecY(VGet(0, 300, -800), VGet(0.0f, 0.0f, 0.0f));

	// �}�e���A���ݒ�
	MATERIALPARAM Material;
	Material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Specular = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Ambient = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 20.0f;
	SetMaterialParam(Material);

	// ����p
	float pers = 60.0f;

	// ���S���猩���J�����̕���
	float cameraAngleAxisX = 0.0f;
	float cameraAngleAxisY = DX_PI_F / 12.0f;

	// ���_����J�����܂ł̋���
	float cameraDistance = 800.0f;

	Map* pMap = new Map();
	Player* pPlayer = new Player(pMap);

	pMap->Init();
	pPlayer->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		pMap->Update();
		pPlayer->Update();

		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (key & PAD_INPUT_1)
		{
			cameraDistance += 8.0f;
		}
		if (key & PAD_INPUT_2)
		{
			cameraDistance -= 8.0f;
		}
		if (key & PAD_INPUT_UP)
		{
			cameraAngleAxisX += 0.01f;
		}
		if (key & PAD_INPUT_DOWN)
		{
			cameraAngleAxisX -= 0.01f;
		}
		if (key & PAD_INPUT_RIGHT)
		{
			cameraAngleAxisY += 0.01f;
		}
		if (key & PAD_INPUT_LEFT)
		{
			cameraAngleAxisY -= 0.01f;
		}

		// ����p�𖈃t���[���ύX
		SetupCamera_Perspective(pers * DX_PI_F / 180.0f);

#if false
		// �J�����̌����𖈃t���[���ύX
		VECTOR cameraPos = VGet(0, 300, 0);
		cameraPos.x = sinf(cameraAngle) * kCameraDistance;
		cameraPos.z = cosf(cameraAngle) * kCameraDistance;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VG
			et(0, 0, 0));
#endif
#if false
		// �J�����̌����𖈃t���[���ύX
		VECTOR cameraPos = VGet(0, 0, -1);	// �J���������ʒu
		VECTOR cameraUp = VGet(0, 1, 0);
		MATRIX mtxAixsX = MGetRotX(cameraAngleAxisX);
		MATRIX mtxAixsY = MGetRotY(cameraAngleAxisY);	
		MATRIX mtxScale = MGetScale(VGet(cameraDistance, cameraDistance, cameraDistance));

		MATRIX mtx = MMult(mtxAixsX, mtxAixsY);
		cameraUp = VTransform(cameraUp, mtx);
		mtx = MMult(mtx, mtxScale);
		cameraPos = VTransform(cameraPos, mtx);
		SetCameraPositionAndTargetAndUpVec(cameraPos, VGet(0, 0, 0), cameraUp);
#endif
		VECTOR cameraPos = pMap->GetCenter();		// �J�����ʒu
		cameraPos.y += cameraDistance;
		VECTOR cameraTarget = pMap->GetCenter();	// �J�����̌��Ă���ꏊ
		VECTOR cameraUp = VGet(0, 0, 1);			// �J�����̏����

		SetCameraPositionAndTargetAndUpVec(cameraPos, cameraTarget, cameraUp);

		// �v���C���[���_�̃J�����ɕύX
		pPlayer->SetCamera();

		// �O���b�h�̕\��
		DrawGrid();		

		// �|���S���̕\��
		pMap->Draw();

		// �v���C���[�̕\��
		pPlayer->Draw();

		// ���݂̎���p���f�o�b�N�\��
		DrawFormatString(0, 0, 0xffffff, "pers = %f", pers);

		COLOR_F ambColor = GetLightAmbColor();
		DrawFormatString(0, 16, 0xffffff, "amb = (%.4f,%.4f,%.4f,%.4f)", ambColor.r, ambColor.g, ambColor.b, ambColor.a);

		// 
		DrawFormatString(0, 32, 0xffffff, "MapCenter(%f, %f, %f)", pMap->GetCenter().x, pMap->GetCenter().y, pMap->GetCenter().z);

		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		
		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	delete pMap;

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
