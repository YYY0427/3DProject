#include "Map.h"
#include "DrawPoly.h"
#include <DxLib.h>

namespace
{
	// テクスチャ情報
	const char* const kTextureFilename = "texture00.png";

	// マップ情報
	constexpr int kMapSizeX = 8;
	constexpr int kMapSizeZ = 6;

	constexpr int kMapData[kMapSizeZ][kMapSizeX] = 
	{
		{1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 1, 0, 0, 0, 1},
		{1, 1, 0, 1, 0, 1, 0, 1},
		{1, 1, 0, 1, 0, 1, 0, 1},
		{1, 1, 0, 0, 0, 1, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1},
	};

	// ブロック一つのサイズ
	constexpr float kCubeSize = 100.0f;
}

Map::Map() :
	texture_(-1)
{
	
}

Map::~Map()
{
	DeleteGraph(texture_);
}

void Map::Init()
{
	texture_ = LoadGraph(kTextureFilename);
}

void Map::Update()
{
}

void Map::Draw()
{
	for (int x = 0; x < kMapSizeX; x++)
	{
		for (int z = 0; z < kMapSizeZ; z++)
		{
			if (kMapData[z][x] == 0)	continue;
			Poly::DrawCube(GetChipPos(x, z), kCubeSize, texture_);
		}
	}
}

float Map::GetSizeX() const
{
	return kCubeSize * kMapSizeX;
}

float Map::GetSizeY() const
{
	return kCubeSize;
}

float Map::GetSizeZ() const
{
	return kCubeSize * kMapSizeZ;
}

VECTOR Map::GetCenter() const
{
	VECTOR result;
	result.x = (GetSizeX() / 2.0f) - (kCubeSize / 2.0f);
	result.y = 0.0f;
	result.z = (-GetSizeZ() / 2.0f) + (kCubeSize / 2.0f);
	return result;
}

int Map::GetChipNumX() const
{
	return kMapSizeX;
}

int Map::GetChipNumZ() const
{
	return kMapSizeZ;
}

bool Map::IsBlock(int x, int z) const
{
	if (kMapData[z][x] == 0)	return false;
	return true;
}

VECTOR Map::GetChipPos(int x, int z) const
{
	float posX = kCubeSize * x;
	float posZ = -kCubeSize * z;
	VECTOR result = VGet(posX, 0.0f, posZ);
	return result;
}
