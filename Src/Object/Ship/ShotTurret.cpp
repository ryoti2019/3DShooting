#include "ShotTurret.h"

ShotTurret::ShotTurret(void)
{
}

ShotTurret::~ShotTurret(void)
{
}

ShotTurret::STATE ShotTurret::GetState(void)
{
	return state_;
}

void ShotTurret::SetState(ShotTurret::STATE state)
{
	ChangeState(state);
}

void ShotTurret::SetParam(void)
{

	// �e�̑��x
	speed_ = 1.0f;

	// ��������
	timeAlive_ = 3.0f;

	// �Փ˗p���̔��a
	collisionRadius_ = 1000.0f;

}
