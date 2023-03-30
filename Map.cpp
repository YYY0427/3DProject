#include "Map.h"
#include "DrawPoly.h"
#include <DxLib.h>

namespace
{
	// テクスチャ情報
	const char* const kTextureFilename = "texture00.png";
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
	// ポリゴンの表示
	Poly::DrawCube(VGet(0, 0, 0), 400.0f, texture_);
}