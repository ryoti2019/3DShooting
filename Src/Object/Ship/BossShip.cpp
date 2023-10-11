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

	// 初期化系
	VECTOR pos = { 0.0f, 0.0f, 4000.0f };

	// モデル制御の基本情報
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
	// モデル制御の基本情報更新
	transform_.Update();

	// シーンによって状態を切り替える
	SceneManager::SCENE_ID sceneId = SceneManager::GetInstance().GetSceneID();
	switch (sceneId)
	{
	case SceneManager::SCENE_ID::EVENT:
		ChangeState(STATE::EVENT);
		break;
	case SceneManager::SCENE_ID::BATTLE:
		ChangeState(STATE::BATTLE);
		break;
	}

}
void BossShip::Update(void)
{

	VECTOR forward = transform_.GetForward();

	// 移動
	transform_.pos = VAdd(transform_.pos,VScale(forward,SPEED_MOVE));

	// モデル制御の基本情報更新
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

void BossShip::ChangeState(STATE state)
{
	state_ = state;
}

void BossShip::MoveForward(void)
{

	VECTOR forward = transform_.GetForward();

	// 移動
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

}

void BossShip::Rotation(void)
{
}
