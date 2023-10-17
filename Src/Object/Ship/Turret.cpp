#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "../Common/Transform.h"
#include "Turret.h"

Turret::Turret(const Transform& transformParent,
	VECTOR localPos, VECTOR localRot) : transformParent_(transformParent)
{

	// 親(戦艦)からの相対座標
	localPos_ = localPos;

	// 親(戦艦)からの相対角度
	localRot_ = localRot;

	// ３Ｄモデルの読込
	ResourceManager& ins = ResourceManager::GetInstance();
	transformStand_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_STAND));
	transformBarrel_.SetModel(
		ins.LoadModelDuplicate(ResourceManager::SRC::TURRET_GUN));

	// 攻撃を初期状態にする
	ChangeState(STATE::ATTACK);

}

Turret::~Turret(void)
{
	
}

void Turret::Init(void)
{
	// 耐久力
	hp_ = 2;

#pragma region 砲台の設定
	// モデル制御の基本情報
	transformStand_.scl = { SCALE, SCALE, SCALE };

	// 回転なし
	transformStand_.quaRot = Quaternion();

	// ローカル回転(正面をZの正方向に合わせる)
	transformStand_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(transformStand_);

#pragma endregion
#pragma region 砲身の設定
	// モデル制御の基本情報
	transformBarrel_.scl = { SCALE, SCALE, SCALE };

	// 回転なし
	transformBarrel_.quaRot = Quaternion();

	// ローカル回転(正面をZの正方向に合わせる)
	transformBarrel_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(transformBarrel_);
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
	}

}

void Turret::UpdateAttack(void)
{

	// 砲台の回転と位置を調整
	SyncParent(transformStand_);

	// 砲身の回転と位置を調整
	SyncParent(transformBarrel_);

}

void Turret::UpdateDestroy(void)
{
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
	}

}

void Turret::DrawAttack(void)
{

	// ３Ｄモデルの描画
	MV1DrawModel(transformStand_.modelId);
	MV1DrawModel(transformBarrel_.modelId);

}

void Turret::DrawDestroy(void)
{
}

void Turret::Release(void)
{
}

void Turret::SyncParent(Transform& transform)
{
	// 位置
	transform.pos = VAdd(transformParent_.pos, VScale(localPos_, SCALE));
		// モデル制御の基本情報更新
		transform.Update();
}

void Turret::ChangeState(STATE state)
{

	// 状態の変更
	state_ = state;
	// 状態遷移時の初期化処理
	switch (state_)
	{
	case Turret::STATE::NONE:
		break;
	case Turret::STATE::ATTACK:
		break;
	case Turret::STATE::DESTROY:
		break;
	}

}
