#pragma once
#include <DxLib.h>

class Map;

class Player
{
public:
	Player(Map* pMap);
	virtual ~Player();

	void Init();
	void Update();
	void Draw();

	// プレイヤー始点のカメラを設定する
	void SetCamera();

	// プレイヤーの3D座標を取得する
	VECTOR GetPos() const;

	// プレイヤーの向いている方向を取得する
	VECTOR GetDirection() const; 

private:
	enum class Dir
	{
		minusZ,
		plusX,
		plusZ,
		minusX
	};
private:
	// マップのポインタ
	Map* pMap_;

	// マップ上の位置(マップチップのインデックス)
	int indexX_;
	int indexZ_;

	// プレイヤーの向いている方向
	Dir direction_;

	// カメラ情報
	VECTOR cameraDirection_;	//カメラの向いている方高

	// 前のフレームキー情報
	int lastKey_;
};

