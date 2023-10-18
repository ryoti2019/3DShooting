#pragma once
#include <vector>
#include "../../Object/Common/Transform.h"

class Turret
{

public:

	// �^���b�g�̑傫��
	static constexpr float SCALE = 80.0f;

	// �C���]���쐧���p(deg)
	static constexpr float ANGLE_Y_MIN_STAND = -30.0f;
	static constexpr float ANGLE_Y_MAX_STAND = 30.0f;

	// �C��̉�]��(deg)
	static constexpr float ROT_POW_STAND = 0.2f;

	// �C�g��]���쐧���p(deg)
	static constexpr float ANGLE_X_MIN_GUN = -10.0f;
	static constexpr float ANGLE_X_MAX_GUN = 20.0f;
	
	// �C�g�̉�]��(deg)
	static constexpr float ROT_POW_GUN = 0.2f;

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

	// �C��̉�]��(deg)
	float rotPowStand_;

	// �C�g�̉�]��(deg)
	float rotPowBarrel_;

	// �C��̉�]����~�ϗp
	VECTOR localRotAddStand_;

	VECTOR localRotAddBarrel_;

	bool isStand_;

	bool isBarrel_;

	// ��ԑJ��
	void ChangeState(STATE state);

	void UpdateAttack(void);
	void UpdateDestroy(void);
	void DrawAttack(void);
	void DrawDestroy(void);

	// �e(���)�Ƃ̉�]�ƈʒu�̓���
	void SyncParent(Transform& transform, VECTOR addAxis);

};

