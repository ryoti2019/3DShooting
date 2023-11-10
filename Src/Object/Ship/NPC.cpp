#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Ship/PlayerShip.h"
#include "../../Utility/AsoUtility.h"
#include "NPC.h"

NPC::NPC(void)
{
}

NPC::~NPC(void)
{
}

void NPC::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::LEADER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 100.0f, 100.0f, 500.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	transform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	transform_.Update();

	// �G�t�F�N�g������
	InitEffect();

	// �f�[�^���i�[����ϐ�
	OrbitData data;

	// �f�[�^�̓���
	data = { { 0, 0, 1 }, 1.0f, 14.0f , 0.0f};
	orbitDatas_.emplace_back(data);

	data = { { 1, 1, 1 }, 1.0f, 3.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0.3, 1 }, 1.0f, 1.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0, 1 }, 1.0f, 1.5f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0, 0 }, 1.0f, 1.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, -0.3, -1 }, 1.0f, 10.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	// �Y����
	data_ = 0;

	// �������
	ChangeState(STATE::RUN);

}

void NPC::InitEffect(void)
{

	// ���˃G�t�F�N�g
	effectJetResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).handleId_;

	// ���@�j��G�t�F�N�g
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// �G�t�F�N�g�Đ�
	effectJetPlayId_ = PlayEffekseer3DEffect(effectJetResId_);

	// �傫��
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(effectJetPlayId_, SCALE, SCALE, SCALE);

	// �G�t�F�N�g�̈ʒu
	SyncJetEffect();

}

void NPC::Update(void)
{

	//SyncJetEffect();


	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
		UpdateRun();
		break;
	case NPC::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	transform_.Update();

}

void NPC::Draw(void)
{

	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
	{
		// ���[�_�[�@�̕`��
		MV1DrawModel(transform_.modelId);
	}
		break;
	case NPC::STATE::DESTROY:
		break;
	}

}

void NPC::Release(void)
{
	// �G�t�F�N�g��~
	StopEffekseer3DEffect(effectJetPlayId_);
}

const Transform& NPC::GetTransform(void) const
{
	return transform_;
}

void NPC::SyncJetEffect(void)
{

	// �Ǐ]�Ώ�(�v���C���[�@)�̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis(LOCAL_POS);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectJetPlayId_, effectPos_.x, effectPos_.y, effectPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void NPC::SyncDestroyEffect(void)
{

	// �Ǐ]�Ώ�(�v���C���[�@)�̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis(LOCAL_DESTROY_POS);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectDestroyPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectDestroyPlayId_, effectDestroyPos_.x, effectDestroyPos_.y, effectDestroyPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectDestroyPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void NPC::ChangeState(STATE state)
{

	// ��Ԃ̍X�V
	state_ = state;

	// ��ԕʂ̏���������
	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
		break;
	case NPC::STATE::DESTROY:
		// �G�t�F�N�g�Đ�
		effectDestroyPlayId_ = PlayEffekseer3DEffect(effectDestroyResId_);

		float SCALE = 10.0f;
		// �傫��
		SetScalePlayingEffekseer3DEffect(effectDestroyPlayId_, SCALE, SCALE, SCALE);

		// �G�t�F�N�g�̈ʒu
		SyncDestroyEffect();

		effectTime_ = 2.0f;

		isEffect_ = true;
		break;
	}

}

void NPC::UpdateRun(void)
{

	// ���W�̍X�V
	if (data_ == 0)
	{
		transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED_MOVE));
	}

	if (data_ >= 1)
	{
		transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), BOOST_SPEED));
	}

	// ���񎞊Ԃ̊Ԃ������񂷂�
	if (orbitDatas_[data_].stepTime <= 1.0f)
	{
		// ���ʕ�Ԃ��s��
		Quaternion dirRot = Quaternion::Slerp(
			Quaternion::LookRotation(preOrbitDir_),
			Quaternion::LookRotation(orbitDatas_[data_].dir), orbitDatas_[data_].stepTime / orbitDatas_[data_].turnTime);

		// NPC�̌�����ς���
		transform_.quaRot = dirRot;
	}

	// ���񎞊Ԃ��f���^�^�C��������������
	orbitDatas_[data_].stepTime += SceneManager::GetInstance().GetDeltaTime();

	// �Ō�̃f�[�^�܂Ői�񂾂�NPC��j�󂷂�
	if (data_ == 4 && orbitDatas_[data_].advanceTime <= 0.0f)
	{
		ChangeState(STATE::DESTROY);
	}

	// ���񎞊Ԃ�0�ɂȂ�����O�i����
	if (orbitDatas_[data_].stepTime >= 1.0f)
	{
		orbitDatas_[data_].advanceTime -= SceneManager::GetInstance().GetDeltaTime();
	}

	// �O�i���Ԃ�0�ȉ��ɂȂ����玟�̃f�[�^�Ɉڍs����
	if (orbitDatas_[data_].advanceTime <= 0.0f)
	{
		// �O�̐������ۊ�
		preOrbitDir_ = orbitDatas_[data_].dir;

		// ���̃f�[�^�Ɉڍs
		data_ += 1;
	}

	// �G�t�F�N�g����
	SyncJetEffect();

}

void NPC::UpdateDestroy(void)
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
	}

	SyncDestroyEffect();

}
