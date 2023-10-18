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

	// �C�䃍�[�J����]
	localRotAddStand_ = { 0.0f, 0.0f, 0.0f };

	// �C�g���[�J����]
	localRotAddBarrel_ = { 0.0f, 0.0f, 0.0f };

	// �U����������Ԃɂ���
	ChangeState(STATE::ATTACK);

	isStand_ = false;

	isBarrel_ = false;

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

	// �C��̉�]��(deg)
	rotPowStand_ = ROT_POW_STAND;

	// ���[�J����](���ʂ�Z�̐������ɍ��킹��)
	transformStand_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// �e���f���Ɠ���
	SyncParent(transformStand_,{ 0.0f, 0.0f, 0.0f });

#pragma endregion

#pragma region �C�g�̐ݒ�

	// ���f������̊�{���
	transformBarrel_.scl = { SCALE, SCALE, SCALE };

	// ��]�Ȃ�
	transformBarrel_.quaRot = Quaternion();

	// �C�g�̉�]��(deg)
	rotPowBarrel_ = ROT_POW_GUN;

	// ���[�J����](���ʂ�Z�̐������ɍ��킹��)
	transformBarrel_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// �e���f���Ɠ���
	SyncParent(transformBarrel_,{ 0.0f, 0.0f, 0.0f });

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

	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float standDeg;

#pragma region �C��̉�]����

	if (isStand_)
	{
		// �C��̉ғ���]
		localRotAddStand_.y += (rotPowStand_ * deltaTime) ;
	}
	else
	{
		// �C��̉ғ���]
		localRotAddStand_.y += (rotPowStand_ * deltaTime)* -1.0f;
	}

	standDeg = AsoUtility::Rad2DegF(localRotAddStand_.y);
	//�������ōl���Ď������Ă݂܂��傤
	//��]�𔽓] = �͂𔽓]����ɂ́A - 1.0f�Ŋ|���Z������Ɨǂ��ł��傤

	if (ANGLE_Y_MAX_STAND < standDeg)
	{
		isStand_ = false;
	}
	else if (ANGLE_Y_MIN_STAND > standDeg)
	{
		isStand_ = true;
	}

	// �C��̉�]�ƈʒu�𒲐�(�ғ����̉�]��������)
	SyncParent(transformStand_, localRotAddStand_);

#pragma endregion

	float barrelDeg;

#pragma region �C�g�̉�]����

	if (isBarrel_)
	{
		// �C��̉ғ���]
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime);
	}
	else
	{
		// �C��̉ғ���]
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime) * -1.0f;
	}

	// �C�g�̉ғ���]
	barrelDeg = AsoUtility::Rad2DegF(localRotAddBarrel_.x);
	//�������ōl���Ď������Ă݂܂��傤
	//��]�𔽓] = �͂𔽓]����ɂ́A - 1.0f�Ŋ|���Z������Ɨǂ��ł��傤

	if (ANGLE_X_MAX_GUN < barrelDeg)
	{
		isBarrel_ = false;
	}
	else if (ANGLE_X_MIN_GUN > barrelDeg)
	{
		isBarrel_ = true;
	}


	// �C�g�̉�]�ƈʒu�𒲐�(�ғ����̉�]��������)
	//SyncParent(transformBarrel_, localRotAddBarrel_);

#pragma endregion

	// �C�䂪��]�������C�g����]����K�v������̂ł��̉�]�����
	Quaternion stand = Quaternion::Euler(localRotAddStand_);
	Quaternion gun = Quaternion::Euler(localRotAddBarrel_);
	Quaternion mix = stand.Mult(gun);

	// �C��̉�]�ƈʒu�𒲐�(�ғ����̉�]��������)
	SyncParent(transformBarrel_, mix.ToEuler());

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

void Turret::SyncParent(Transform& transform, VECTOR addAxis)
{

	// �e(���)�̉�]�����擾
	Quaternion parentRot = transformParent_.quaRot;

#pragma region �⑫
	// �C��̃��[�J����]
	// Unity Z��X��Y = Y * X * Z
	//axis = Quaternion::AngleAxis(localRotAdd_.y, AsoUtility::AXIS_Y);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.x, AsoUtility::AXIS_X);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.z, AsoUtility::AXIS_Z);
	//localRot = localRot.Mult(axis);

#pragma endregion

	// �e(���)��]����̑��Ή�]
	Quaternion rot = Quaternion::Identity();
	rot = rot.Mult(Quaternion::Euler(localRot_));

	// �ғ����̉�]��������
	rot = rot.Mult(Quaternion::Euler(addAxis));

	// �e(���)�̉�]�Ƒ��Ή�]������
	transform.quaRot = parentRot.Mult(rot);

	// �e(���)�Ƃ̑��΍��W���A�e(���)�̉�]���ɍ��킹�ĉ�]
	VECTOR localRotPos = Quaternion::PosAxis(parentRot, localPos_);

	// ���΍��W�����[���h���W�ɒ���
	transform.pos = VAdd(transformParent_.pos, VScale(localRotPos, SCALE));

	// ���f������̊�{���X�V
	transform.Update();

}

void Turret::Release(void)
{
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
