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

	// エフェクト初期化
	InitEffect();

	speedBoost_ = 0.0f;

}

void PlayerShip::Update(void)
{

	// 回転操作(移動処理前に)
	ProcessTurn();

	// ブースト
	ProcessBoost();

	VECTOR forward = transform_.GetForward();

	// 移動
	transform_.pos =
		VAdd(transform_.pos, VScale(forward, SPEED_MOVE + speedBoost_));

	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 移動
	//transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE * delta));

	// カメラ位置とカメラ注視点
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

	// エフェクト停止
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

	// 右旋回
	if (ins.IsTrgDown(KEY_INPUT_B))
	{

		// エフェクト再生
		effectBoostPlayId_ = PlayEffekseer3DEffect(effectBoostResId_);

		float SCALE = 10.0f;
		// 大きさ
		SetScalePlayingEffekseer3DEffect(effectBoostPlayId_, SCALE, SCALE, SCALE);

		// エフェクトの位置
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

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectLPos = followRot.PosAxis(LOCAL_POS_L);

	// エフェクトの位置の更新
	effectBoostPos_ = VAdd(followPos, effectLPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectBoostPlayId_, effectBoostPos_.x, effectBoostPos_.y, effectBoostPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectBoostPlayId_, rot.x, rot.y, rot.z);
	transform_.Update();

}

void PlayerShip::InitEffect(void)
{

	// 噴射エフェクト
	effectJetResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).handleId_;

	// ブーストエフェクト
	effectBoostResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::BOOST).handleId_;

	// エフェクト再生
	effectJetLPlayId_ = PlayEffekseer3DEffect(effectJetResId_);
	effectJetRPlayId_ = PlayEffekseer3DEffect(effectJetResId_);

	// 大きさ
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(effectJetLPlayId_, SCALE, SCALE, SCALE);
	SetScalePlayingEffekseer3DEffect(effectJetRPlayId_, SCALE, SCALE, SCALE);

	// エフェクトの位置
	SyncJetEffect();

}

void PlayerShip::SyncJetEffect(void)
{

	//// ばね付きの実装
	//float POW_SPRING = 24.0f;
	//float dampening = 2.0f * sqrt(POW_SPRING);

	//// デルタタイム
	//float delta = SceneManager::GetInstance().GetDeltaTime();

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectLPos = followRot.PosAxis(LOCAL_POS_L);
	VECTOR effectRPos = followRot.PosAxis(LOCAL_POS_R);

	// エフェクトの位置の更新
	effectLPos_ = VAdd(followPos, effectLPos);
	effectRPos_ = VAdd(followPos, effectRPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectJetLPlayId_, effectLPos_.x, effectLPos_.y, effectLPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetLPlayId_, rot.x, rot.y, rot.z);
	SetPosPlayingEffekseer3DEffect(effectJetRPlayId_, effectRPos_.x, effectRPos_.y, effectRPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetRPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

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
