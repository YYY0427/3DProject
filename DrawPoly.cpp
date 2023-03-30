#include "DrawPoly.h"
#include <vector>

namespace
{
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
void Poly::DrawCube(VECTOR pos, float size, int texture)
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
