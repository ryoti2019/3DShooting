#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Ship/ShotTurret.h"
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

	// ���j�G�t�F�N�g
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// �U����������Ԃɂ���
	ChangeState(STATE::ATTACK);

	// �C�䃍�[�J����]
	localRotAddStand_ = { 0.0f, 0.0f, 0.0f };

	// �C�g���[�J����]
	localRotAddBarrel_ = { 0.0f, 0.0f, 0.0f };

	isStand_ = false;

	isBarrel_ = false;

	effectTime_ = 0.0f;

	isEffect_ = false;

	isAlive_ = true;

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
	case Turret::STATE::END:
		break;
	}

	for (auto v : shots_)
	{
		v->Update();
	}

}

void Turret::UpdateAttack(void)
{

	ProcessShot();

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

	if (effectTime_ > 0.0f)
	{
		effectTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	if (effectTime_ < 0.0f)
	{
		isEffect_ = false;
	}

	if (!isEffect_)
	{
		StopEffekseer3DEffect(effectDestroyPlayId_);
		ChangeState(STATE::END);
	}

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
	case Turret::STATE::END:
		break;
	}

	for (auto v : shots_)
	{
		v->Draw();
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

void Turret::SyncDestroyEffect(void)
{

	// �Ǐ]�Ώۂ̈ʒu
	VECTOR followPos = transformBarrel_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transformBarrel_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectDestroyPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectDestroyPlayId_, effectDestroyPos_.x, effectDestroyPos_.y, effectDestroyPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectDestroyPlayId_, rot.x, rot.y, rot.z);

	transformBarrel_.Update();

}

void Turret::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();

	if (delayShot_ <= 0.0f)
	{
		CreateShot();
		delayShot_ = TIME_DELAY_SHOT;
	}

}

void Turret::CreateShot(void)
{

	// �e�̐����t���O
	bool isCreate = false;

	for (auto v : shots_)
	{
		if (v->GetState() == ShotTurret::STATE::END)
		{
			// �ȑO�ɐ��������C���X�^���X���g����
			v->Create({ transformBarrel_.pos.x,
				transformBarrel_.pos.y + 150,
				transformBarrel_.pos.z, },
				transformBarrel_.GetForward());
			isCreate = true;
			break;
		}
	}
	if (!isCreate)
	{
		// ���@�̑O������
		auto dir = transformBarrel_.GetForward();
		// �V�����C���X�^���X�𐶐�
 		ShotTurret* newShot = new ShotTurret();
		newShot->Create({ transformBarrel_.pos.x,
			transformBarrel_.pos.y + 150 ,
			transformBarrel_.pos.z },
			transformBarrel_.GetForward());

		// �e�̊Ǘ��z��ɒǉ�
		shots_.push_back(newShot);
	}

}

void Turret::Destroy(void)
{
	// �j���Ԃֈڍs
	ChangeState(STATE::DESTROY);
}

bool Turret::IsAlive(void)
{	
	return isAlive_;
}

void Turret::Release(void)
{

	for (auto& s : shots_)
	{
		s->Release();
		delete s;
	}
	shots_.clear();

}

std::vector<ShotTurret*>& Turret::GetShots(void)
{
	return shots_;
}

const Transform& Turret::GetTransformBarrel(void) const
{
	return transformBarrel_;
}

void Turret::SetState(Turret::STATE state)
{
	ChangeState(state);
}

Turret::STATE Turret::GetState(void)
{
	return state_;
}

void Turret::SetHP(int hp)
{
	hp_ += hp;
}

int Turret::GetHP(void)
{
	return hp_;
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
	{
		isEffect_ = true;
		// �G�t�F�N�g�Đ�
		effectDestroyPlayId_ = PlayEffekseer3DEffect(effectDestroyResId_);
		// �傫��
		float SCALE = 100.0f;
		SetScalePlayingEffekseer3DEffect(effectDestroyPlayId_, SCALE, SCALE, SCALE);
		// �G�t�F�N�g�̈ʒu
		SyncDestroyEffect();
		effectTime_ = TIME_DESTROY;
	}
		break;
	case Turret::STATE::END:
		isAlive_ = false;
		break;
	}

}
