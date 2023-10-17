#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"
#include "Turret.h"

Turret::Turret(const Transform& transformParent,
	VECTOR localPos, VECTOR localRot) : transformParent_(transformParent)
{

	// �e(���)����̑��΍��W
	localPos_ = localPos;

	// �e(���)����̑��Ίp�x
	localRot_ = localRot;

	// �R�c���f���̓Ǎ�
	ResourceManager& ins = ResourceManager::GetInstance();
	transformStand_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_STAND));
	transformBarrel_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_GUN));

	// �U����������Ԃɂ���
	ChangeState(STATE::ATTACK);

}

Turret::~Turret(void)
{
	
}

void Turret::Init(void)
{
	// �ϋv��
	hp_ = 2;

#pragma region �C��̐ݒ�
	// ���f������̊�{���
	transformStand_.scl = { SCALE, SCALE, SCALE };

	// ��]�Ȃ�
	transformStand_.quaRot = Quaternion();

	// ���[�J����](���ʂ�Z�̐������ɍ��킹��)
	transformStand_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// �e���f���Ɠ���
	SyncParent(transformStand_);

#pragma endregion
#pragma region �C�g�̐ݒ�
	// ���f������̊�{���
	transformBarrel_.scl = { SCALE, SCALE, SCALE };

	// ��]�Ȃ�
	transformBarrel_.quaRot = Quaternion();

	// ���[�J����](���ʂ�Z�̐������ɍ��킹��)
	transformBarrel_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// �e���f���Ɠ���
	SyncParent(transformBarrel_);
#pragma endregion
}

void Turret::Update(void)
{

	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		UpdateAttack();
		break;
	case Turret::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

}

void Turret::UpdateAttack(void)
{

	// �C��̉�]�ƈʒu�𒲐�
	SyncParent(transformStand_);

	// �C�g�̉�]�ƈʒu�𒲐�
	SyncParent(transformBarrel_);

}

void Turret::UpdateDestroy(void)
{
}

void Turret::Draw(void)
{

	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		DrawAttack();
		break;
	case Turret::STATE::DESTROY:
		DrawDestroy();
		break;
	}

}

void Turret::DrawAttack(void)
{

	// �R�c���f���̕`��
	MV1DrawModel(transformStand_.modelId);
	MV1DrawModel(transformBarrel_.modelId);

}

void Turret::DrawDestroy(void)
{
}

void Turret::Release(void)
{
}

void Turret::SyncParent(Transform& transform)
{
	// �ʒu
	transform.pos = VAdd(transformParent_.pos, VScale(localPos_, SCALE));
		// ���f������̊�{���X�V
		transform.Update();
}

void Turret::ChangeState(STATE state)
{

	// ��Ԃ̕ύX
	state_ = state;
	// ��ԑJ�ڎ��̏���������
	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
		break;
	}

}
