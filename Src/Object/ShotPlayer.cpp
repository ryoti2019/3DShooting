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

	// �e�̑��x
	speed_ = 35.0f;

	// ��������
	timeAlive_ = 5.0f;

	// �Փ˗p���̔��a
	collisionRadius_ = 10.0f;

}
