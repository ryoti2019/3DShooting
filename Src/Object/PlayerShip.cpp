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

	// 回転操作(移動処理前に)
	ProcessTurn();

	VECTOR forward = transform_.GetForward();

	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 移動
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE * delta));

	// カメラ位置とカメラ注視点
	transform_.pos = VAdd(transform_.pos, forward);

	transform_.Update();

}

void PlayerShip::Draw(void)
{
	MV1DrawModel(transform_.modelId);
}

void PlayerShip::Release(void)
{
}

const Transform& PlayerShip::GetTransform(void) const
{
	return transform_;
}

void PlayerShip::ProcessTurn(void)
{

	auto& ins = InputManager::GetInstance();

	// 右旋回
	if (ins.IsNew(KEY_INPUT_RIGHT))
	{
		Turn(SPEED_ROT_DEG_Y,AsoUtility::AXIS_Y);
	}

	// 左旋回
	if (ins.IsNew(KEY_INPUT_LEFT))
	{
		Turn(-SPEED_ROT_DEG_Y, AsoUtility::AXIS_Y);
	}

	// 上旋回
	if (ins.IsNew(KEY_INPUT_UP))
	{
		Turn(-SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

	// 下旋回
	if (ins.IsNew(KEY_INPUT_DOWN))
	{
		Turn(SPEED_ROT_DEG_X, AsoUtility::AXIS_X);
	}

}

void PlayerShip::Turn(float deg, VECTOR axis)
{

	// 引数で指定された回転量・軸分、回転を加える

	// ①デグリーをラジアンに変換する
	float rad = AsoUtility::Deg2RadF(deg);

	// ②ラジアンをクォータニオンに変換
	// (とある関数で、回転量RADと回転軸を使用)
	Quaternion rotPow = Quaternion::AngleAxis(rad, axis);

	// ③今回作成した回転量を、自機の回転量に加える(合成する)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);

}
