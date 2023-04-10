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

	// �v���C���[�n�_�̃J������ݒ肷��
	void SetCamera();

	// �v���C���[��3D���W���擾����
	VECTOR GetPos() const;

	// �v���C���[�̌����Ă���������擾����
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
	// �}�b�v�̃|�C���^
	Map* pMap_;

	// �}�b�v��̈ʒu(�}�b�v�`�b�v�̃C���f�b�N�X)
	int indexX_;
	int indexZ_;

	// �v���C���[�̌����Ă������
	Dir direction_;

	// �J�������
	VECTOR cameraDirection_;	//�J�����̌����Ă������

	// �O�̃t���[���L�[���
	int lastKey_;
};

