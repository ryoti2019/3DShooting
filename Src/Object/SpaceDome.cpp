#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "SpaceDome.h"

SpaceDome::SpaceDome(void)
{
}

SpaceDome::~SpaceDome(void)
{
}

void SpaceDome::Init(void)
{

	// ÉÇÉfÉãêßå‰ÇÃäÓñ{èÓïÒ
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

}

void SpaceDome::Update(void)
{
	transform_.Update();
}

void SpaceDome::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void SpaceDome::Release(void)
{
}
