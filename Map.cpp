#include "Map.h"
#include "DrawPoly.h"
#include <DxLib.h>

namespace
{
	// �e�N�X�`�����
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
	// �|���S���̕\��
	Poly::DrawCube(VGet(0, 0, 0), 400.0f, texture_);
}