#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../Ship/PlayerShip.h"
#include "../../Utility/AsoUtility.h"
#include "NPC.h"

NPC::NPC(void)
{
}

NPC::~NPC(void)
{
}

void NPC::Init(void)
{

	// モデル制御の基本情報
	transform_.SetModel(
		ResourceManager::GetInstance().LoadModelDuplicate(
			ResourceManager::SRC::LEADER_SHIP));
	float scale = 10.0f;
	transform_.scl = { scale, scale, scale };
	transform_.pos = { 100.0f, 100.0f, 500.0f };
	transform_.quaRot = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(0.0f),
		0.0f
	);

	transform_.quaRotLocal = Quaternion::Euler(
		0.0f,
		AsoUtility::Deg2RadF(180.0f),
		0.0f
	);

	transform_.Update();

	// エフェクト初期化
	InitEffect();

	// データを格納する変数
	OrbitData data;

	// データの入力
	data = { { 0, 0, 1 }, 1.0f, 14.0f , 0.0f};
	orbitDatas_.emplace_back(data);

	data = { { 1, 1, 1 }, 1.0f, 3.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0.3, 1 }, 1.0f, 1.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0, 1 }, 1.0f, 1.5f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, 0, 0 }, 1.0f, 1.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	data = { { 1, -0.3, -1 }, 1.0f, 10.0f ,0.0f };
	orbitDatas_.emplace_back(data);

	// 添え字
	data_ = 0;

	// 初期状態
	ChangeState(STATE::RUN);

}

void NPC::InitEffect(void)
{

	// 噴射エフェクト
	effectJetResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::JET).handleId_;

	// 自機破壊エフェクト
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// エフェクト再生
	effectJetPlayId_ = PlayEffekseer3DEffect(effectJetResId_);

	// 大きさ
	float SCALE = 5.0f;
	SetScalePlayingEffekseer3DEffect(effectJetPlayId_, SCALE, SCALE, SCALE);

	// エフェクトの位置
	SyncJetEffect();

}

void NPC::Update(void)
{

	//SyncJetEffect();


	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
		UpdateRun();
		break;
	case NPC::STATE::DESTROY:
		UpdateDestroy();
		break;
	}

	transform_.Update();

}

void NPC::Draw(void)
{

	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
	{
		// リーダー機の描画
		MV1DrawModel(transform_.modelId);
	}
		break;
	case NPC::STATE::DESTROY:
		break;
	}

}

void NPC::Release(void)
{
	// エフェクト停止
	StopEffekseer3DEffect(effectJetPlayId_);
}

const Transform& NPC::GetTransform(void) const
{
	return transform_;
}

void NPC::SyncJetEffect(void)
{

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis(LOCAL_POS);

	// エフェクトの位置の更新
	effectPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectJetPlayId_, effectPos_.x, effectPos_.y, effectPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectJetPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void NPC::SyncDestroyEffect(void)
{

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = transform_.pos;

	// 追従対象の向き
	Quaternion followRot = transform_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis(LOCAL_DESTROY_POS);

	// エフェクトの位置の更新
	effectDestroyPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectDestroyPlayId_, effectDestroyPos_.x, effectDestroyPos_.y, effectDestroyPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectDestroyPlayId_, rot.x, rot.y, rot.z);

	transform_.Update();

}

void NPC::ChangeState(STATE state)
{

	// 状態の更新
	state_ = state;

	// 状態別の初期化処理
	switch (state_)
	{
	case NPC::STATE::NONE:
		break;
	case NPC::STATE::RUN:
		break;
	case NPC::STATE::DESTROY:
		// エフェクト再生
		effectDestroyPlayId_ = PlayEffekseer3DEffect(effectDestroyResId_);

		float SCALE = 10.0f;
		// 大きさ
		SetScalePlayingEffekseer3DEffect(effectDestroyPlayId_, SCALE, SCALE, SCALE);

		// エフェクトの位置
		SyncDestroyEffect();

		effectTime_ = 2.0f;

		isEffect_ = true;
		break;
	}

}

void NPC::UpdateRun(void)
{

	// 座標の更新
	if (data_ == 0)
	{
		transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), SPEED_MOVE));
	}

	if (data_ >= 1)
	{
		transform_.pos = VAdd(transform_.pos, VScale(transform_.GetForward(), BOOST_SPEED));
	}

	// 旋回時間の間だけ旋回する
	if (orbitDatas_[data_].stepTime <= 1.0f)
	{
		// 球面補間を行う
		Quaternion dirRot = Quaternion::Slerp(
			Quaternion::LookRotation(preOrbitDir_),
			Quaternion::LookRotation(orbitDatas_[data_].dir), orbitDatas_[data_].stepTime / orbitDatas_[data_].turnTime);

		// NPCの向きを変える
		transform_.quaRot = dirRot;
	}

	// 旋回時間をデルタタイム分引き続ける
	orbitDatas_[data_].stepTime += SceneManager::GetInstance().GetDeltaTime();

	// 最後のデータまで進んだらNPCを破壊する
	if (data_ == 4 && orbitDatas_[data_].advanceTime <= 0.0f)
	{
		ChangeState(STATE::DESTROY);
	}

	// 旋回時間が0になったら前進時間
	if (orbitDatas_[data_].stepTime >= 1.0f)
	{
		orbitDatas_[data_].advanceTime -= SceneManager::GetInstance().GetDeltaTime();
	}

	// 前進時間が0以下になったら次のデータに移行する
	if (orbitDatas_[data_].advanceTime <= 0.0f)
	{
		// 前の旋回情報を保管
		preOrbitDir_ = orbitDatas_[data_].dir;

		// 次のデータに移行
		data_ += 1;
	}

	// エフェクト制御
	SyncJetEffect();

}

void NPC::UpdateDestroy(void)
{

	if (effectTime_ > 0.0f)
	{
		effectTime_ -= SceneManager::GetInstance().GetDeltaTime();
	}

	if (effectTime_ < 0.0f)
	{
		isEffect_ = false;
	}

	if (!isEffect_)
	{
		StopEffekseer3DEffect(effectDestroyPlayId_);
	}

	SyncDestroyEffect();

}
