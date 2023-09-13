#pragma once
#include "Common/Transform.h"
class PlayerShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// �R���X�g���N�^
	PlayerShip(void);

	// �f�X�g���N�^
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

	// �e�X�g
};

