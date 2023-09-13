#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
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

	// モデル制御の基本情報
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

}

void PlayerShip::Update(void)
{

	auto& ins = InputManager::GetInstance();

	VECTOR moveDir = { 0.0f,0.0f,1.0f };

	// 押下された移動ボタンの方向に移動
	VECTOR direction = VNorm(rot_.PosAxis(moveDir)); // 回転させる必要がある

	// 移動量(方向 * スピード)
	VECTOR movePow = VScale(direction, SPEED_MOVE);

	// 移動(座標 + 移動量)
	// カメラ位置とカメラ注視点
	transform_.pos = VAdd(transform_.pos, movePow);

	transform_.Update();

}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerShip::Release(void)
{
}
