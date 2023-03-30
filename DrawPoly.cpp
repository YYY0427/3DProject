#include "DrawPoly.h"
#include <vector>

namespace
{
	// マップチップの分割数(計算に使用するのでfloatで定義)
	constexpr float kTextureDivNumX_F = 11.0f;
	constexpr float kTextureDivNumY_F = 8.0f;

	// 使用するチップの番号(横位置、縦位置を指定する)
	constexpr int kUseTextureX = 8;
	constexpr int kUseTextureY = 3;

	// 使用するチップのテクスチャ座標
	constexpr float kTextureMinU = 1.0f / kTextureDivNumX_F * kUseTextureX;
	constexpr float kTextureMaxU = kTextureMinU + 1.0f / kTextureDivNumX_F;
	constexpr float kTextureMinV = 1.0f / kTextureDivNumY_F * kUseTextureY;
	constexpr float kTextureMaxV = kTextureMinV + 1.0f / kTextureDivNumY_F;
}


// マップを構成する立方体の1面を表示
// 原点を中心としてZ方向を向いた平面を描画

// 指定位置を中心とした指定サイズの立方体を表示する
void Poly::DrawCube(VECTOR pos, float size, int texture)
{
	constexpr int polyNum = 2;				// 正方形１面描画するのに必要なポリゴンの数
	constexpr int vtxNum = polyNum * 3;		// ポリゴンの描画に必要な頂点数
	constexpr int cubeVtxNum = vtxNum * 6;	// 立方体１つ描画するのに必要な頂点数

	std::vector<VERTEX3D> drawVtx;

	VERTEX3D polyVtx[vtxNum];				// 立方体１つ描画するのに必要な頂点数

	// 左上
	polyVtx[0].pos = VGet(-size / 2, size / 2, -size / 2);
	polyVtx[0].norm = VGet(0, 0, -1);
	polyVtx[0].dif = GetColorU8(255, 255, 255, 255);
	polyVtx[0].spc = GetColorU8(255, 255, 255, 255);
	polyVtx[0].u = kTextureMinU;
	polyVtx[0].v = kTextureMinV;
	polyVtx[0].su = 0.0f;
	polyVtx[0].sv = 0.0f;

	// 0番目の頂点の情報を以降の頂点にコピーする
	for (int i = 0; i < vtxNum; i++)
	{
		polyVtx[i] = polyVtx[0];
	}

	// 右上
	polyVtx[1].pos = VGet(size / 2, size / 2, -size / 2);
	polyVtx[1].u = kTextureMaxU;
	polyVtx[1].v = kTextureMinV;

	// 左下
	polyVtx[2].pos = VGet(-size / 2, -size / 2, -size / 2);
	polyVtx[2].u = kTextureMinU;
	polyVtx[2].v = kTextureMaxV;

	// 右下
	polyVtx[3].pos = VGet(size / 2, -size / 2, -size / 2);
	polyVtx[3].u = kTextureMaxU;
	polyVtx[3].v = kTextureMaxV;

	// 左下
	polyVtx[4].pos = VGet(-size / 2, -size / 2, -size / 2);
	polyVtx[4].u = kTextureMinU;
	polyVtx[4].v = kTextureMaxV;

	// 右上
	polyVtx[5].pos = VGet(size / 2, size / 2, -size / 2);
	polyVtx[5].u = kTextureMaxU;
	polyVtx[5].v = kTextureMinV;

	// 描画用の頂点データに登録
	for (auto& vtx : polyVtx)
	{
		drawVtx.push_back(vtx);
	}

	// ポリゴンの表示
	DrawPolygon3D(polyVtx, 2, texture, false);

	// 回転行列
	MATRIX mtx = MGetRotY(DX_PI_F / 2.0f);

	// 側面の頂点データの登録
	for (int i = 0; i < 3; i++)
	{
		for (auto& vtx : polyVtx)
		{
			vtx.pos = VTransform(vtx.pos, mtx);
			vtx.norm = VTransform(vtx.norm, mtx);
			drawVtx.push_back(vtx);
		}
	}

	// 上面の頂点データ生成、登録
	mtx = MGetRotZ(DX_PI_F / 2.0f);
	for (auto& vtx : polyVtx)
	{
		vtx.pos = VTransform(vtx.pos, mtx);
		vtx.norm = VTransform(vtx.norm, mtx);
		drawVtx.push_back(vtx);
	}
	// 下面データの生成、登録
	mtx = MGetRotZ(DX_PI_F);
	for (auto& vtx : polyVtx)
	{
		vtx.pos = VTransform(vtx.pos, mtx);
		vtx.norm = VTransform(vtx.norm, mtx);
		drawVtx.push_back(vtx);
	}

	// 最後に位置情報を足す
	for (auto& vtx : drawVtx)
	{
		vtx.pos = VAdd(vtx.pos, pos);
	}

	DrawPolygon3D(drawVtx.data(), drawVtx.size() / 3, texture, false);
}
