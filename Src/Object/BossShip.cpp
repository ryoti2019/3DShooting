#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Utility/AsoUtility.h"
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

}
void BossShip::Update(void)
{

	VECTOR forward = transform_.GetForward();

	// �ړ�
	transform_.pos = VAdd(transform_.pos,VScale(forward,SPEED_MOVE));

	// ���f������̊�{���X�V
	transform_.Update();

}

void BossShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void BossShip::Release(void)
{
}
