#pragma once
#include <DxLib.h>

class Map
{
public:
	Map();
	virtual ~Map();

	void Init();
	void Update();
	void Draw();

	// マップのサイズを取得する
	float GetSizeX() const;
	float GetSizeY() const;
	float GetSizeZ() const;
	
	// マップの中心座標を取得する
	VECTOR GetCenter() const;

	// マップチップ情報を取得する
	int GetChipNumX() const;
	int GetChipNumZ() const;

	// 指定した場所がブロックかどうか取得する
	bool IsBlock(int x, int z) const;

	// 指定したチップの3D座標を取得する(中心位置)
	VECTOR GetChipPos(int x, int z) const;

private:
	int texture_;
};