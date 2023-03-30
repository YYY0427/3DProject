#include "DxLib.h"
#include "Game.h"
#include <cmath>
#include <vector>

namespace
{
	// 原点からカメラまでの距離
	constexpr float kCameraDistance = 800.0f;

	// テクスチャ情報
	const char* const kTextureFilename = "texture00.png";

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
void DrawMapPolygon(VECTOR pos, float size, int texture)
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

// グリッド表示
void DrawGrid()
{
	// グリッドの開始位置と終了位置の宣言
	VECTOR start = VGet(0, 0, 0);
	VECTOR end = VGet(0, 0, 0);

	// グリッドの表示
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

// プログラムは WinMain から始まります
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	// windowモード設定
	ChangeWindowMode(Game::kWindowMode);

	// window名設定
	SetMainWindowText(Game::kTitleText);

	// 画面サイズの設定
	SetGraphMode(Game::kScreenWidth, Game::kScreenHeight, Game::kColorDepth);

	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;				// エラーが起きたら直ちに終了
	}

	//ダブルバッファモード
	SetDrawScreen(DX_SCREEN_BACK);

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを行う
	SetWriteZBuffer3D(true);

	// カメラの設定
	SetCameraNearFar(5.0f, 2800.0f);

	// 裏面ポリゴンを表示しない
	SetUseBackCulling(true);

	// 視野角の設定
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

	// カメラの位置と向きの設定
	SetCameraPositionAndTarget_UpVecY(VGet(0, 300, -800), VGet(0.0f, 0.0f, 0.0f));

	// マテリアル設定
	MATERIALPARAM Material;
	Material.Diffuse = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Specular = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Ambient = GetColorF(1.0f, 1.0f, 1.0f, 1.0f);
	Material.Emissive = GetColorF(0.0f, 0.0f, 0.0f, 0.0f);
	Material.Power = 20.0f;
	SetMaterialParam(Material);

	// 視野角
	float pers = 60.0f;

	// 中心から見たカメラの方向
	float cameraAngle = 0.0f;

	// テクスチャのロード
	int texture = LoadGraph(kTextureFilename);

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面のクリア
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

		// 視野角を毎フレーム変更
		SetupCamera_Perspective(pers * DX_PI_F / 180.0f);

#if false
		// カメラの向きを毎フレーム変更
		VECTOR cameraPos = VGet(0, 300, 0);
		cameraPos.x = sinf(cameraAngle) * kCameraDistance;
		cameraPos.z = cosf(cameraAngle) * kCameraDistance;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
#else 
		// カメラの向きを毎フレーム変更
		VECTOR cameraPos = VGet(0, 300, -800);	// カメラ初期位置
		MATRIX mtx = MGetRotY(cameraAngle);		//
		cameraPos = VTransform(cameraPos, mtx);
		SetCameraPositionAndTarget_UpVecY(cameraPos, VGet(0, 0, 0));
#endif
		// グリッドの表示
		DrawGrid();

		// ポリゴンの表示
		DrawMapPolygon(VGet(0, 0, 0), 400.0f, texture);

		// 現在の視野角をデバック表示
		DrawFormatString(0, 0, 0xffffff, "pers = %f", pers);

		COLOR_F ambColor = GetLightAmbColor();
		DrawFormatString(0, 16, 0xffffff, "amb = (%.4f,%.4f,%.4f,%.4f)", ambColor.r, ambColor.g, ambColor.b, ambColor.a);

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		
		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	DeleteGraph(texture);

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
