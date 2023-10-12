#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "BossShip.h"

BossShip::BossShip(void)
{
}

BossShip::~BossShip(void)
{
}

void BossShip::Init(void)
{

	// �������n
	VECTOR pos = { 0.0f, 0.0f, 4000.0f };

	// ���f������̊�{���
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::BOSS_SHIP));

	float scale = 2.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = pos;
	transform_.quaRot = Quaternion::Euler(
		AsoUtility::Deg2RadF(20.0f),
		AsoUtility::Deg2RadF(210.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	transform_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);
	// ���f������̊�{���X�V
	transform_.Update();

	// �V�[���ɂ���ď�Ԃ�؂�ւ���
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetSceneID();
	switch (sceneId)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChangeState(STATE::EVENT);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		// �����蔻��(�R���C�_)�쐬
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// �~�������x�� ������
		MV1RefreshCollInfo(transform_.modelId, -1);
		ChangeState(STATE::BATTLE);
		break;
	}

}
void BossShip::Update(void)
{

	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos = VAdd(transform_.pos,VScale(forward,SPEED_MOVE));

	// ���f������̊�{���X�V
	transform_.Update();

	switch (state_)
	{
	case BossShip::STATE::NONE:
		break;
	case BossShip::STATE::EVENT:
		MoveForward();
		break;
	case BossShip::STATE::BATTLE:
		Rotation();
		MoveForward();
		// �����蔻��(�R���C�_)�쐬
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// �~�������x�� ������
		MV1RefreshCollInfo(transform_.modelId, -1);
		break;
	case BossShip::STATE::DESTROY:
		break;
	case BossShip::STATE::END:
		break;
	}

}

void BossShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void BossShip::Release(void)
{
}

Transform BossShip::GetTransform(void)
{
	return transform_;
}

bool BossShip::IsAlive(void) const
{
	return state_ == STATE::BATTLE;
}

int BossShip::GetModelIdBossShip(void)
{
	return transform_.modelId;
}

void BossShip::ChangeState(STATE state)
{
	state_ = state;
}

void BossShip::MoveForward(void)
{

	// �G�̑O�������擾
	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

}

void BossShip::Rotation(void)
{
	// �����Ŏw�肳�ꂽ��]�ʁE�����A��]��������

	// �@�f�O���[�����W�A���ɕϊ�����
	float rad = AsoUtility::Deg2RadF(ROT_POW_DEG * SceneManager::GetInstance().GetDeltaTime());

	// �A���W�A�����N�H�[�^�j�I���ɕϊ�
	// (�Ƃ���֐��ŁA��]��RAD�Ɖ�]�����g�p)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

	// �B����쐬������]�ʂ��A���@�̉�]�ʂɉ�����(��������)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}