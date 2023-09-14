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

	// �v���C���[�̃g�����X�t�H�[��
	const Transform& GetTransform(void) const;

private:

	// ���f������̊�{���
	Transform transform_;

};

