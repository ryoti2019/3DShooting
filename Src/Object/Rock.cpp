#include "Rock.h"

Rock::Rock(void)
{
}

Rock::~Rock(void)
{
}

void Rock::Init(int modelId, VECTOR pos, VECTOR angle, VECTOR scale)
{

	// ���f������̊�{���
	transform_.modelId = modelId;
	transform_.scl = scale;
	transform_.pos = pos;
	transform_.quaRot = Quaternion();
	transform_.quaRotLocal = Quaternion::Euler(angle);
	transform_.Update();

}

void Rock::Update(void)
{
}

void Rock::Draw(void)
{

	// �J�����N���b�v�O�ɂȂ�����`�悵�Ȃ�
	if (!CheckCameraViewClip(transform_.pos))
	{
		MV1DrawModel(transform_.modelId);
	}

}

void Rock::Release(void)
{
}
