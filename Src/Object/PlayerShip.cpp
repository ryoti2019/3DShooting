#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
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

}

void PlayerShip::Update(void)
{

	// ��]����(�ړ������O��)
	ProcessTurn();

	// �u�[�X�g
	ProcessBoost();

	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos =
		VAdd(transform_.pos, VScale(forward, SPEED_MOVE + speedBoost_));

	float delta = SceneManager::GetInstance().GetDeltaTime();

	// �ړ�
	//transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE * delta));

	// �J�����ʒu�ƃJ���������_
	//transform_.pos = VAdd(transform_.pos, forward);

	transform_.Update();

	SyncJetEffect();

	SyncBoostEffect();

}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
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

void PlayerShip::ProcessBoost(void)
{

	auto& ins = InputManager::GetInstance();

	// �E����
	if (ins.IsTrgDown(KEY_INPUT_B))
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
	VECTOR effectLPos = followRot.PosAxis(LOCAL_POS_L);

	// �G�t�F�N�g�̈ʒu�̍X�V
	effectBoostPos_ = VAdd(followPos, effectLPos);

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

	//// �΂˕t���̎���
	//float POW_SPRING = 24.0f;
	//float dampening = 2.0f * sqrt(POW_SPRING);

	//// �f���^�^�C��
	//float delta = SceneManager::GetInstance().GetDeltaTime();

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
	SetPosPlayingEffekseer3DEffect(effectJetLPlayId_, effectLPos_.x, effectLPos_.y, effectLPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetLPlayId_, rot.x, rot.y, rot.z);
	SetPosPlayingEffekseer3DEffect(effectJetRPlayId_, effectRPos_.x, effectRPos_.y, effectRPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetRPlayId_, rot.x, rot.y, rot.z);

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
