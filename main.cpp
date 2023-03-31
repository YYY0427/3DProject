#include "DxLib.h"
#include "Game.h"
#include "DrawPoly.h"
#include "Map.h"
#include "Player.h"
#include <cmath>
#include <vector>

namespace
{
	// 原点からカメラまでの距離
	constexpr float kCameraDistance = 800.0f;
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
	float cameraAngleAxisX = 0.0f;
	float cameraAngleAxisY = DX_PI_F / 12.0f;

	// 原点からカメラまでの距離
	float cameraDistance = 800.0f;

	Map* pMap = new Map();
	Player* pPlayer = new Player(pMap);

	pMap->Init();
	pPlayer->Init();

	while (ProcessMessage() == 0)
	{
		LONGLONG time = GetNowHiPerformanceCount();

		// 画面のクリア
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

		// 視野角を毎フレーム変更
		SetupCamera_Perspective(pers * DX_PI_F / 180.0f);

#if false
		// カメラの向きを毎フレーム変更
		VECTOR cameraPos = VGet(0, 300, 0);
		cameraPos.x = sinf(cameraAngle) * kCameraDistance;
		cameraPos.z = cosf(cameraAngle) * kCameraDistance;
		SetCameraPositionAndTarget_UpVecY(cameraPos, VG
			et(0, 0, 0));
#endif
#if false
		// カメラの向きを毎フレーム変更
		VECTOR cameraPos = VGet(0, 0, -1);	// カメラ初期位置
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
		VECTOR cameraPos = pMap->GetCenter();		// カメラ位置
		cameraPos.y += cameraDistance;
		VECTOR cameraTarget = pMap->GetCenter();	// カメラの見ている場所
		VECTOR cameraUp = VGet(0, 0, 1);			// カメラの上方向

		SetCameraPositionAndTargetAndUpVec(cameraPos, cameraTarget, cameraUp);

		// プレイヤー視点のカメラに変更
		pPlayer->SetCamera();

		// グリッドの表示
		DrawGrid();		

		// ポリゴンの表示
		pMap->Draw();

		// プレイヤーの表示
		pPlayer->Draw();

		// 現在の視野角をデバック表示
		DrawFormatString(0, 0, 0xffffff, "pers = %f", pers);

		COLOR_F ambColor = GetLightAmbColor();
		DrawFormatString(0, 16, 0xffffff, "amb = (%.4f,%.4f,%.4f,%.4f)", ambColor.r, ambColor.g, ambColor.b, ambColor.a);

		// 
		DrawFormatString(0, 32, 0xffffff, "MapCenter(%f, %f, %f)", pMap->GetCenter().x, pMap->GetCenter().y, pMap->GetCenter().z);

		// 裏画面を表画面を入れ替える
		ScreenFlip();

		// escキーを押したら終了する
		if (CheckHitKey(KEY_INPUT_ESCAPE))	break;
		
		// fpsを60に固定
		while (GetNowHiPerformanceCount() - time < 16667)
		{

		}
	}

	delete pMap;

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}
