#pragma once
#include "../Object/Common/Transform.h"
class GameScene;

class BossShip
{

public:

	// �ړ��X�s�[�h
		static constexpr float SPEED_MOVE = 4.0f;

	// �R���X�g���N�^
	BossShip(void);

	// �f�X�g���N�^
	~BossShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	Transform transform_;

};


