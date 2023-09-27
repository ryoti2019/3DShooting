#pragma once
#include "Common/Transform.h"

class PlayerShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 10.0f;

	// ���@�̉�]��
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// �R���X�g���N�^
	PlayerShip(void);

	// �f�X�g���N�^
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// �v���C���[��Transform�̎擾
	const Transform& GetTransform(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

	// ����
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

};

