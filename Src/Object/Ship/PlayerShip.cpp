#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Scene/GameScene.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/ShotPlayer.h"
#include "PlayerShip.h"

PlayerShip::PlayerShip(void)
{
}

PlayerShip::~PlayerShip(void)
{
}

void PlayerShip::Init(void)
{

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::PLAYER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	// �G�t�F�N�g������
	InitEffect();

	speedBoost_ = 0.0f;

	effectTime_ = 0.0f;

	isEffect_ = false;

	// �������
	ChangeState(STATE::RUN);

	// ���̔��ˊԊu
	delayShot_ = 0.0f;

}

void PlayerShip::Update(void)
{

	//// ��]����(�ړ������O��)
	//ProcessTurn();

	//// �u�[�X�g
	//ProcessBoost();

	//VECTOR forward = transform_.GetForward();

	//// �ړ�
	//transform_.pos =
	//	VAdd(transform_.pos, VScale(forward, SPEED_MOVE + speedBoost_));

	//float delta = SceneManager::GetInstance().GetDeltaTime();

	//// �ړ�
	////transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE * delta));

	//// �J�����ʒu�ƃJ���������_
	////transform_.pos = VAdd(transform_.pos, forward);

	//transform_.Update();

	SyncJetEffect();
	SyncBoostEffect();
	SyncDestroyEffect();

	switch (state_)
	{
	case PlayerShip::STATE::NONE:
		break;
	case PlayerShip::STATE::RUN:
		UpdateRun();
		break;
	case PlayerShip::STATE::DESTROY:
		isEffect_ = true;
		UpdateDestroy();
		break;
	}

	for (auto v : shots_)
	{
		v->Update();
	}
}

void PlayerShip::Draw(void)
{

	switch (state_)
	{
	case PlayerShip::STATE::NONE:
		break;
	case PlayerShip::STATE::RUN:
		// ���@���f��
		MV1DrawModel(transform_.modelId);
		break;
	case PlayerShip::STATE::DESTROY:
		break;
	}

	for (auto v : shots_)
	{
		v->Draw();
	}
}

void PlayerShip::Destroy(void)
{
	// �j���Ԃֈڍs
	ChangeState(STATE::DESTROY);
}

bool PlayerShip::IsDestroy(void) const
{
	return state_ == STATE::DESTROY;
}

void PlayerShip::Release(void)
{

	// �G�t�F�N�g��~
	StopEffekseer3DEffect(effectJetLPlayId_);
	StopEffekseer3DEffect(effectJetRPlayId_);

}

const Transform& PlayerShip::GetTransform(void) const
{
	return transform_;
}

std::vector<ShotPlayer*>& PlayerShip::GetShots(void)
{
	return shots_;
}

void PlayerShip::ProcessBoost(void)
{

	auto& ins = InputManager::GetInstance();

	// �E����
	if (ins.IsTrgDown(KEY_INPUT_B) && speedBoost_ <= 0.0f)
	{

		// �G�t�F�N�g�Đ�
		effectBoostPlayId_ = PlayEffekseer3DEffect(effectBoostResId_);

		float SCALE = 10.0f;
		// �傫��
		SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, SCALE, SCALE, SCALE);

		// �G�t�F�N�g�̈ʒu
		SyncBoostEffect();

		speedBoost_ = 10.0f;

	}
	if (speedBoost_ > 0.0f)
	{
		speedBoost_ -= 0.1f;
	}

}

void PlayerShip::SyncBoostEffect(void)
{

	// �Ǐ]�Ώ�(�v���C���[�@)�̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectPos = followRot.PosAxis(LOCAL_BOOST_POS);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectBoostPos_ = VAdd(followPos, effectPos);

	// �ʒu�̐ݒ�
	SetPosPlayingEffekseer3DEffect(effectBoostPlayId_, effectBoostPos_.x, effectBoostPos_.y, effectBoostPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectBoostPlayId_, rot.x, rot.y, rot.z);
	transform_.Update();

}

void PlayerShip::InitEffect(void)
{

	// ���˃G�t�F�N�g
	effectJetResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).handleId_;

	// �u�[�X�g�G�t�F�N�g
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).handleId_;

	// ���@�j��G�t�F�N�g
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// �G�t�F�N�g�Đ�
	effectJetLPlayId_ = PlayEffekseer3DEffect(effectJetResId_);
	effectJetRPlayId_ = PlayEffekseer3DEffect(effectJetResId_);

	// �傫��
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(effectJetLPlayId_, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(effectJetRPlayId_, SCALE, SCALE, SCALE);

	// �G�t�F�N�g�̈ʒu
	SyncJetEffect();

}

void PlayerShip::SyncJetEffect(void)
{

	// �Ǐ]�Ώ�(�v���C���[�@)�̈ʒu
	VECTOR followPos = transform_.pos;

	// �Ǐ]�Ώۂ̌���
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// �Ǐ]�Ώۂ��玩�@�܂ł̑��΍��W
	VECTOR effectLPos = followRot.PosAxis(LOCAL_POS_L);
	VECTOR effectRPos = followRot.PosAxis(LOCAL_POS_R);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectLPos_ = VAdd(followPos, effectLPos);
	effectRPos_ = VAdd(followPos, effectRPos);

	// �ʒu�̐ݒ�
	// ��
	SetPosPlayingEffekseer3DEffect(effectJetLPlayId_, effectLPos_.x, effectLPos_.y, effectLPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetLPlayId_, rot.x, rot.y, rot.z);

	// �E
	SetPosPlayingEffekseer3DEffect(effectJetRPlayId_, effectRPos_.x, effectRPos_.y, effectRPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetRPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void PlayerShip::SyncDestroyEffect(void)
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

void PlayerShip::ProcessTurn(void)
{

	auto& ins = InputManager::GetInstance();

	// �E����
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		Turn(SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}

	// ������
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	// �����
	if (ins.IsNew(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

	// ������
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::Turn(float deg, VECTOR axis)
{

	// �����Ŏw�肳�ꂽ��]�ʁE�����A��]��������

	// �@�f�O���[�����W�A���ɕϊ�����
	float rad = AsoUtility::Deg2RadF(deg);

	// �A���W�A�����N�H�[�^�j�I���ɕϊ�
	// (�Ƃ���֐��ŁA��]��RAD�Ɖ�]�����g�p)
	Quaternion rotPow = Quaternion::AngleAxis(rad, axis);

	// �B����쐬������]�ʂ��A���@�̉�]�ʂɉ�����(��������)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);

}

void PlayerShip::ChangeState(STATE state)
{

	// ��Ԃ̍X�V
	state_ = state;

	// ��ԕʂ̏���������
	switch (state_)
	{
	case PlayerShip::STATE::NONE:
		break;
	case PlayerShip::STATE::RUN:
		break;
	case PlayerShip::STATE::DESTROY:
		break;
	}

}

void PlayerShip::UpdateRun(void)
{

	// ��]����(�ړ��̑O��)
	ProcessTurn();

	// �u�[�X�g
	ProcessBoost();

	// �O�������擾
	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos =
		VAdd(transform_.pos, VScale(forward, SPEED_MOVE + speedBoost_));

	// ���f������̊�{���X�V
	transform_.Update();

	// �G�t�F�N�g����
	SyncJetEffect();

	ProcessShot();

}

void PlayerShip::UpdateDestroy(void)
{

	if (effectTime_ <= 0.0f)
	{
		// �G�t�F�N�g�Đ�
		effectDestroyPlayId_ = PlayEffekseer3DEffect(effectDestroyResId_);

		float SCALE = 10.0f;
		// �傫��
		SetScalePlayingEffekseer3DEffect(effectDestroyPlayId_, SCALE, SCALE, SCALE);

		// �G�t�F�N�g�̈ʒu
		SyncDestroyEffect();

		effectTime_ = 2.0f;
	}

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

}

void PlayerShip::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();
	if (delayShot_ <= 0.0f)
	{
		delayShot_ = 0.0f;
	}

	if (ins.IsTrgDown(KEY_INPUT_N) && delayShot_ == 0.0f)
	{
		CreateShot();
		delayShot_ = TIME_DELAY_SHOT;
	}

}

void PlayerShip::CreateShot(void)
{
	// �e�̐����t���O
	bool isCreate = false;

	for (auto v : shots_)
	{
		if (v->GetState() == ShotPlayer::STATE::END)
		{
			// �ȑO�ɐ��������C���X�^���X���g����
			v->Create(transform_.pos, transform_.GetForward());
			isCreate = true;
			break;
		}
	}
	if (!isCreate)
	{
		// ���@�̑O������
		auto dir = transform_.GetForward();
		// �V�����C���X�^���X�𐶐�
		ShotPlayer* newShot = new ShotPlayer();
		newShot->Create(transform_.pos, transform_.GetForward());

		// �e�̊Ǘ��z��ɒǉ�
		shots_.push_back(newShot);
	}

}