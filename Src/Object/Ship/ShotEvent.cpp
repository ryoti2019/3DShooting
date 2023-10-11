#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "ShotEvent.h"

ShotEvent::ShotEvent(void)
{
}

ShotEvent::~ShotEvent(void)
{
}

void ShotEvent::SetParam(void)
{

	// �e�̑��x
	speed_ = 20.0f;
	// ��������
	timeAlive_ = 10.0f;
	// �Փ˗p���̔��a
	collisionRadius_ = 10.0f;

}

void ShotEvent::CheckAlive(void)
{

	// �J�����Փ˃`�F�b�N
	VECTOR pos2D = ConvWorldPosToScreenPos(transform_.pos);
	if (pos2D.z <= 0.0f || pos2D.z >= 1.0f)
	{

		// �J�����Ɏʂ�悤�Ɉʒu����ނ�����
		VECTOR velocity = VScale(VScale(dir_, -1.0f), 100.0f);
		VECTOR backPos = VAdd(transform_.pos, velocity);
		transform_.pos = backPos;

		transform_.Update();

		// ������Ԃ�
		ChangeState(STATE::BLAST);

		SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::SHAKE);

	}

}

ShotBase::STATE ShotEvent::GetState(void)
{
	return state_;
}