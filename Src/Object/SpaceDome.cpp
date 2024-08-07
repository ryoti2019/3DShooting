#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(const Transform& follow) : follow_(follow)
{
}

SpaceDome::~SpaceDome(void)
{
}

void SpaceDome::Init(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::SKYDOME));
	float scale = 1.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 0.0f, 0.0f, 0.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);
	transform_.quaRotLocal = Quaternion();
	transform_.Update();

	MV1SetUseZBuffer(transform_.modelId, false);
	MV1SetWriteZBuffer(transform_.modelId, false);

}

void SpaceDome::Update(void)
{

	// 追従処理
	transform_.pos = follow_.pos;

	transform_.Update();

}

void SpaceDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SpaceDome::Release(void)
{
}
