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

	// ’e‚Ì‘¬“x
	speed_ = 20.0f;

	// ¶‘¶ŠÔ
	timeAlive_ = 5.0f;

	// Õ“Ë—p‹…‘Ì”¼Œa
	collisionRadius_ = 10.0f;

}
