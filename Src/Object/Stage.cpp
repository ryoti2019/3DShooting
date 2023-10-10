#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Stage.h"

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
}

void Stage::Init(void)
{

	// ステージ
	stage_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::DUNGEON));
	stage_.scl = AsoUtility::VECTOR_ONE;
	stage_.quaRot =
		Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(233.0f), 0.0f);
	stage_.pos = { 1000.0f, 400.0f, 1300.0f };
	stage_.Update();

	// ボス戦艦
	bossShip_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::BOSS_SHIP));
	bossShip_.scl = { 1.5f, 1.5f, 1.5f };
	bossShip_.quaRot =
		Quaternion::Euler(0.0f, AsoUtility::Deg2RadF(233.0f), 0.0f);
	bossShip_.pos = { 17500.0f, -800.0f, 9500.0f };
	bossShip_.Update();

	// ワイヤーフレーム表示
	//MV1SetWireFrameDrawFlag(stage_.modelId, true);

}

void Stage::Update(void)
{
}

void Stage::Draw(void)
{

	// 3Dモデル表示
	MV1DrawModel(stage_.modelId);
	MV1DrawModel(bossShip_.modelId);

}

void Stage::Release(void)
{
}

int Stage::GetModelIdStage(void)
{
	return stage_.modelId;
}

Transform Stage::GetBossTransform(void)
{
	return bossShip_;
}
