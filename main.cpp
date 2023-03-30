#include "DxLib.h"
#include "Game.h"
#include <cmath>
#include <vector>

namespace
{
	// ���_����J�����܂ł̋���
	constexpr float kCameraDistance = 800.0f;

	// �e�N�X�`�����
	const char* const kTextureFilename = "texture00.png";

	// �}�b�v�`�b�v�̕�����(�v�Z�Ɏg�p����̂�float�Œ�`)
	constexpr float kTextureDivNumX_F = 11.0f;
	constexpr float kTextureDivNumY_F = 8.0f;
	
	// �g�p����`�b�v�̔ԍ�(���ʒu�A�c�ʒu���w�肷��)
	constexpr int kUseTextureX = 8;
	constexpr int kUseTextureY = 3;

	// �g�p����`�b�v�̃e�N�X�`�����W
	constexpr float kTextureMinU = 1.0f / kTextureDivNumX_F * kUseTextureX;
	constexpr float kTextureMaxU = kTextureMinU + 1.0f / kTextureDivNumX_F;
	constexpr float kTextureMinV = 1.0f / kTextureDivNumY_F * kUseTextureY;
	constexpr float kTextureMaxV = kTextureMinV + 1.0f / kTextureDivNumY_F;
}

// �}�b�v���\�����闧���̂�1�ʂ�\��
// ���_�𒆐S�Ƃ���Z���������������ʂ�`��

// �w��ʒu�𒆐S�Ƃ����w��T�C�Y�̗����̂�\������
void DrawMapPolygon(VECTOR pos, float size, int texture)
{
	constexpr int polyNum = 2;				// �����`�P�ʕ`�悷��̂ɕK�v�ȃ|���S���̐�
	constexpr int vtxNum = polyNum * 3;		// �|���S���̕`��ɕK�v�Ȓ��_��
	constexpr int cubeVtxNum = vtxNum * 6;	// �����̂P�`�悷��̂ɕK�v�Ȓ��_��

	std::vector<VERTEX3D> drawVtx;

	VERTEX3D polyVtx[vtxNum];				// �����̂P�`�悷��̂ɕK�v�Ȓ��_��

	// ����
	polyVtx[0].pos = VGet(-size / 2, size / 2, -size / 2);
	polyVtx[0].norm = VGet(0, 0, -1);
	polyVtx[0].dif = GetColorU8(255, 255, 255, 255);
	polyVtx[0].spc = GetColorU8(255, 255, 255, 255);
	polyVtx[0].u = kTextureMinU;
	polyVtx[0].v = kTextureMinV;
	polyVtx[0].su = 0.0f;
	polyVtx[0].sv = 0.0f;

	// 0�Ԗڂ̒��_�̏����ȍ~�̒��_�ɃR�s�[����
	for (int i = 0; i < vtxNum; i++)
	{
		polyVtx[i] = polyVtx[0];
	}

	// �E��
	polyVtx[1].pos = VGet(size / 2, size / 2, -size / 2);
	polyVtx[1].u = kTextureMaxU;
	polyVtx[1].v = kTextureMinV;

	// ����
	polyVtx[2].pos = VGet(-size / 2, -size / 2, -size / 2);
	polyVtx[2].u = kTextureMinU;
	polyVtx[2].v = kTextureMaxV;

	// �E��
	polyVtx[3].pos = VGet(size / 2, -size / 2, -size / 2);
	polyVtx[3].u = kTextureMaxU;
	polyVtx[3].v = kTextureMaxV;

	// ����
	polyVtx[4].pos = VGet(-size / 2, -size / 2, -size / 2);
	polyVtx[4].u = kTextureMinU;
	polyVtx[4].v = kTextureMaxV;

	// �E��
	polyVtx[5].pos = VGet(size / 2, size / 2, -size / 2);
	polyVtx[5].u = kTextureMaxU;
	polyVtx[5].v = kTextureMinV;

	// �`��p�̒��_�f�[�^�ɓo�^
	for (auto& vtx : polyVtx)
	{
		drawVtx.push_back(vtx);
	}

	// �|���S���̕\��
	DrawPolygon3D(polyVtx, 2, texture, false);

	// ��]�s��
	MATRIX mtx = MGetRotY(DX_PI_F / 2.0f);

	// ���ʂ̒��_�f�[�^�̓o�^
	for (int i = 0; i < 3; i++)
	{
		for (auto& vtx : polyVtx)
		{
			vtx.pos = VTransform(vtx.pos, mtx);
			vtx.norm = VTransform(vtx.norm, mtx);
			drawVtx.push_back(vtx);
		}
	}

	// ��ʂ̒��_�f�[�^�����A�o�^
	mtx = MGetRotZ(DX_PI_F / 2.0f);
	for (auto& vtx : polyVtx)
	{
		vtx.pos = VTransform(vtx.pos, mtx);
		vtx.norm = VTransform(vtx.norm, mtx);
		drawVtx.push_back(vtx);
	}
	// ���ʃf�[�^�̐����A�o�^
	mtx = MGetRotZ(DX_PI_F);
	for (auto& vtx : polyVtx)
	{
		vtx.pos = VTransform(vtx.pos, mtx);
		vtx.norm = VTransform(vtx.norm, mtx);
		drawVtx.push_back(vtx);
	}

	// �Ō�Ɉʒu���𑫂�
	for (auto& vtx : drawVtx)
	{
		vtx.pos = VAdd(vtx.pos, pos);
	}

	DrawPolygon3D(drawVtx.data(), drawVtx.size() / 3, texture, false);
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
	float cameraAngle = 0.0f;

	// �e�N�X�`���̃��[�h
	int texture = LoadGraph(kTextureFilename);

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// ��ʂ̃N���A
		ClearDrawScreen();

		int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
		if (key & PAD_INPUT_UP)
		{
			
		}
		if (key & PAD_INPUT_DOWN)
		{
			
		}
		if (key & PAD_INPUT_RIGHT)
		{
			cameraAngle += 0.01f;
		}
		if (key & PAD_INPUT_LEFT)
		{
			cameraAngle -= 0.01f;
		}

		// ����p�𖈃t���[���ύX
		SetupCamera_Perspective(pers * DX_PI_F / 180.0f);

#if false
		// �J�����̌����𖈃t���[���ύX
		VECTOR cameraPos = VGet(0, 300, 0);
		cameraPos.x = sinf(cameraAngle) * kCameraDistance;
		cameraPos.z = cosf(cameraAngle) * kCameraDistance;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
#else 
		// �J�����̌����𖈃t���[���ύX
		VECTOR cameraPos = VGet(0, 300, -800);	// �J���������ʒu
		MATRIX mtx = MGetRotY(cameraAngle);		//
		cameraPos = VTransform(cameraPos, mtx);
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
#endif
		// �O���b�h�̕\��
		DrawGrid();

		// �|���S���̕\��
		DrawMapPolygon(VGet(0, 0, 0), 400.0f, texture);

		// ���݂̎���p���f�o�b�N�\��
		DrawFormatString(0, 0, 0xffffff, "pers = %f", pers);

		COLOR_F ambColor = GetLightAmbColor();
		DrawFormatString(0, 16, 0xffffff, "amb = (%.4f,%.4f,%.4f,%.4f)", ambColor.r, ambColor.g, ambColor.b, ambColor.a);

		// ����ʂ�\��ʂ����ւ���
		ScreenFlip();

		// esc�L�[����������I������
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		
		// fps��60�ɌŒ�
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	DeleteGraph(texture);

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}
