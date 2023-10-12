#include "ShotPlayer.h"

ShotPlayer::ShotPlayer(void)
{
}

ShotPlayer::~ShotPlayer(void)
{
}

ShotPlayer::STATE ShotPlayer::GetState(void)
{
	return state_;
}

void ShotPlayer::SetParam(void)
{

	// ’e‚Ì‘¬“x
	speed_ = 35.0f;

	// ¶‘¶ŠÔ
	timeAlive_ = 5.0f;

	// Õ“Ë—p‹…‘Ì”¼Œa
	collisionRadius_ = 10.0f;

}
