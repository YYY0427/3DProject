#include "Player.h"
#include "Map.h"
#include <DxLib.h>

Player::Player(Map* pMap) :
	pMap_(pMap),
	indexX_(1),
	indexZ_(1),
	direction_(Dir::plusZ),
	cameraDirection_(VGet(0, 0, 1))
{
}

Player::~Player()
{
}

void Player::Init()
{
}

void Player::Update()
{
	int key = GetJoypadInputState(DX_INPUT_KEY_PAD1);
#if false
	if (key & PAD_INPUT_UP)
	{
		int tempZ = indexZ_ - 1;
		if (!pMap_->IsBlock(indexX_, tempZ))
		{
			indexZ_ = tempZ;
			cameraDirection_ = VGet(0, 0, -1);
		}
	}
	else if (key & PAD_INPUT_DOWN)
	{
		int tempZ = indexZ_ + 1;
		if (!pMap_->IsBlock(indexX_, tempZ))
		{
			indexZ_ = tempZ;
			cameraDirection_ = VGet(-1, 0, 0);
		}
	}
	else if (key & PAD_INPUT_RIGHT)
	{
		int tempX = indexX_ + 1;
		if (!pMap_->IsBlock(tempX, indexZ_))
		{
			indexX_ = tempX;
			cameraDirection_ = VGet(1, 0, 0);
		}
	}
	else if (key & PAD_INPUT_LEFT)
	{
		int tempX = indexX_ - 1;
		if (!pMap_->IsBlock(tempX, indexZ_))
		{
			indexX_ = tempX;
			cameraDirection_ = VGet(-1, 0, 0);
		}
	}
#else
	int tempDir = static_cast<int>(direction_);
	if ((key & PAD_INPUT_UP) && !(lastKey_ & PAD_INPUT_UP))
	{
		// 前に移動する
		int tempX = indexX_;
		int tempZ = indexZ_;
		switch (direction_)
		{
		case Dir::plusZ:	tempZ++;	break;
		case Dir::plusX:	tempX++;	break;
		case Dir::minusZ:	tempZ--;	break;
		case Dir::minusX:	tempX--;	break;
		}
		if (!pMap_->IsBlock(tempX, tempZ))
		{
			indexX_ = tempX;
			indexZ_ = tempZ;
		}
	}
	if (key & PAD_INPUT_DOWN)
	{
	
	}
	if ((key & PAD_INPUT_LEFT) && !(lastKey_ & PAD_INPUT_LEFT))
	{
		// 反時計回り90度
		tempDir++;
		if (tempDir > static_cast<int>(Dir::minusX))
		{
			tempDir = static_cast<int>(Dir::minusZ);
		}
		/*MATRIX mtx = MGetRotY(-DX_PI_F / 2);
		cameraDirection_ = VTransform(cameraDirection_ , mtx);*/
	}
	if ((key & PAD_INPUT_RIGHT) && !(lastKey_ & PAD_INPUT_RIGHT))
	{
		// 時計回り90度
		tempDir--;
		if (tempDir < 0)
		{
			tempDir = static_cast<int>(Dir::minusX);
		}
		/*MATRIX mtx = MGetRotY(DX_PI_F / 2);
		cameraDirection_ = VTransform(cameraDirection_, mtx);*/
	}
	direction_ = static_cast<Dir>(tempDir);
	switch (direction_)
	{
	case Dir::minusZ:
		cameraDirection_ = VGet(0, 0, -1);
		break;
	case Dir ::plusX:
		cameraDirection_ = VGet(1, 0, 0);
		break;
	case Dir::plusZ:
		cameraDirection_ = VGet(0, 0, 1);
		break;
	case Dir::minusX:
		cameraDirection_ = VGet(-1, 0, 0);
		break;
	default:
		break;
	}
	lastKey_ = key;
#endif
}

void Player::Draw()
{
	DrawSphere3D(GetPos(), 24.0f, 8, 0xffffff, 0xffffff, true);
	VECTOR start = GetPos();
	VECTOR end = GetDirection();
	end = VScale(end, 64);
	end = VAdd(start, end);
	DrawCapsule3D(start, end, 6.0f, 16, 0x0000ff, 0x0000ff, true);
}

void Player::SetCamera()
{
	SetCameraPositionAndTargetAndUpVec(GetPos(), VAdd(GetPos(), GetDirection()), VGet(0, 1, 0));
}

VECTOR Player::GetPos() const
{
	return pMap_->GetChipPos(indexX_, indexZ_);
}

VECTOR Player::GetDirection() const
{
	return cameraDirection_;
}
