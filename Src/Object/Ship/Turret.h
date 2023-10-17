#pragma once
#include <vector>
#include "../../Object/Common/Transform.h"

class Turret
{

public:

	// �^���b�g�̑傫��
	static constexpr float SCALE = 80.0f;

	// ���
	enum class STATE
	{
		NONE,
		ATTACK,
		DESTROY
	};

	// �R���X�g���N�^
	Turret(const Transform& transformParent,
		VECTOR localPos, VECTOR localRot);

	// �f�X�g���N�^
	~Turret(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

private:

	// ���f������̊�{���
	const Transform& transformParent_;

	// ���f������̊�{���(�C��)
	Transform transformStand_;

	// ���f������̊�{���(�C�g)
	Transform transformBarrel_;

	// ���
	STATE state_;

	// �e(���)����̑��΍��W
	VECTOR localPos_;

	// �e(���)����̑��Ίp�x
	VECTOR localRot_;

	// �ϋv��
	int hp_;

	// ��ԑJ��
	void ChangeState(STATE state);

	void UpdateAttack(void);
	void UpdateDestroy(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	// �e(���)�Ƃ̉�]�ƈʒu�̓���
	void SyncParent(Transform& transform);

};

