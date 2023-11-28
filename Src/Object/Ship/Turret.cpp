#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../../Application.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Resource.h"
#include "../../Manager/InputManager.h"
#include "../../Common/Quaternion.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Ship/ShotTurret.h"
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

	// 爆破エフェクト
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// 攻撃を初期状態にする
	ChangeState(STATE::ATTACK);

	// 砲台ローカル回転
	localRotAddStand_ = { 0.0f, 0.0f, 0.0f };

	// 砲身ローカル回転
	localRotAddBarrel_ = { 0.0f, 0.0f, 0.0f };

	isStand_ = false;

	isBarrel_ = false;

	effectTime_ = 0.0f;

	isEffect_ = false;

	isAlive_ = true;

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

	// 砲台の回転量(deg)
	rotPowStand_ = ROT_POW_STAND;

	// ローカル回転(正面をZの正方向に合わせる)
	transformStand_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(0.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(transformStand_,{ 0.0f, 0.0f, 0.0f });

#pragma endregion

#pragma region 砲身の設定

	// モデル制御の基本情報
	transformBarrel_.scl = { SCALE, SCALE, SCALE };

	// 回転なし
	transformBarrel_.quaRot = Quaternion();

	// 砲身の回転量(deg)
	rotPowBarrel_ = ROT_POW_GUN;

	// ローカル回転(正面をZの正方向に合わせる)
	transformBarrel_.quaRotLocal = Quaternion::Euler(
		AsoUtility::Deg2RadF(-5.0f),
		AsoUtility::Deg2RadF(180.0f),
		AsoUtility::Deg2RadF(0.0f)
	);

	// 親モデルと同期
	SyncParent(transformBarrel_,{ 0.0f, 0.0f, 0.0f });

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
	case Turret::STATE::END:
		break;
	}

	for (auto v : shots_)
	{
		v->Update();
	}

}

void Turret::UpdateAttack(void)
{

	ProcessShot();

	auto deltaTime = SceneManager::GetInstance().GetDeltaTime();
	float standDeg;

#pragma region 砲台の回転動作

	if (isStand_)
	{
		// 砲台の稼働回転
		localRotAddStand_.y += (rotPowStand_ * deltaTime) ;
	}
	else
	{
		// 砲台の稼働回転
		localRotAddStand_.y += (rotPowStand_ * deltaTime)* -1.0f;
	}

	standDeg = AsoUtility::Rad2DegF(localRotAddStand_.y);
	//※自分で考えて実装してみましょう
	//回転を反転 = 力を反転するには、 - 1.0fで掛け算をすると良いでしょう

	if (ANGLE_Y_MAX_STAND < standDeg)
	{
		isStand_ = false;
	}
	else if (ANGLE_Y_MIN_STAND > standDeg)
	{
		isStand_ = true;
	}

	// 砲台の回転と位置を調整(稼働分の回転を加える)
	SyncParent(transformStand_, localRotAddStand_);

#pragma endregion

	float barrelDeg;

#pragma region 砲身の回転動作

	if (isBarrel_)
	{
		// 砲台の稼働回転
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime);
	}
	else
	{
		// 砲台の稼働回転
		localRotAddBarrel_.x += (rotPowBarrel_ * deltaTime) * -1.0f;
	}

	// 砲身の稼働回転
	barrelDeg = AsoUtility::Rad2DegF(localRotAddBarrel_.x);
	//※自分で考えて実装してみましょう
	//回転を反転 = 力を反転するには、 - 1.0fで掛け算をすると良いでしょう

	if (ANGLE_X_MAX_GUN < barrelDeg)
	{
		isBarrel_ = false;
	}
	else if (ANGLE_X_MIN_GUN > barrelDeg)
	{
		isBarrel_ = true;
	}


	// 砲身の回転と位置を調整(稼働分の回転を加える)
	//SyncParent(transformBarrel_, localRotAddBarrel_);

#pragma endregion

	// 砲台が回転した分砲身も回転する必要があるのでその回転を作る
	Quaternion stand = Quaternion::Euler(localRotAddStand_);
	Quaternion gun = Quaternion::Euler(localRotAddBarrel_);
	Quaternion mix = stand.Mult(gun);

	// 砲台の回転と位置を調整(稼働分の回転を加える)
	SyncParent(transformBarrel_, mix.ToEuler());

}

void Turret::UpdateDestroy(void)
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
		ChangeState(STATE::END);
	}

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
	case Turret::STATE::END:
		break;
	}

	for (auto v : shots_)
	{
		v->Draw();
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

void Turret::SyncParent(Transform& transform, VECTOR addAxis)
{

	// 親(戦艦)の回転情報を取得
	Quaternion parentRot = transformParent_.quaRot;

#pragma region 補足
	// 砲台のローカル回転
	// Unity Z→X→Y = Y * X * Z
	//axis = Quaternion::AngleAxis(localRotAdd_.y, AsoUtility::AXIS_Y);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.x, AsoUtility::AXIS_X);
	//localRot = localRot.Mult(axis);

	//axis = Quaternion::AngleAxis(localRotAdd_.z, AsoUtility::AXIS_Z);
	//localRot = localRot.Mult(axis);

#pragma endregion

	// 親(戦艦)回転からの相対回転
	Quaternion rot = Quaternion::Identity();
	rot = rot.Mult(Quaternion::Euler(localRot_));

	// 稼働分の回転を加える
	rot = rot.Mult(Quaternion::Euler(addAxis));

	// 親(戦艦)の回転と相対回転を合成
	transform.quaRot = parentRot.Mult(rot);

	// 親(戦艦)との相対座標を、親(戦艦)の回転情報に合わせて回転
	VECTOR localRotPos = Quaternion::PosAxis(parentRot, localPos_);

	// 相対座標をワールド座標に直す
	transform.pos = VAdd(transformParent_.pos, VScale(localRotPos, SCALE));

	// モデル制御の基本情報更新
	transform.Update();

}

void Turret::SyncDestroyEffect(void)
{

	// 追従対象の位置
	VECTOR followPos = transformBarrel_.pos;

	// 追従対象の向き
	Quaternion followRot = transformBarrel_.quaRot;

	VECTOR rot = Quaternion::ToEuler(followRot);

	// 追従対象から自機までの相対座標
	VECTOR effectPos = followRot.PosAxis({ 0.0f,0.0f,0.0f });

	// エフェクトの位置の更新
	effectDestroyPos_ = VAdd(followPos, effectPos);

	// 位置の設定
	SetPosPlayingEffekseer3DEffect(effectDestroyPlayId_, effectDestroyPos_.x, effectDestroyPos_.y, effectDestroyPos_.z);
	SetRotationPlayingEffekseer3DEffect(effectDestroyPlayId_, rot.x, rot.y, rot.z);

	transformBarrel_.Update();

}

void Turret::ProcessShot(void)
{

	auto& ins = InputManager::GetInstance();

	delayShot_ -= SceneManager::GetInstance().GetDeltaTime();

	if (delayShot_ <= 0.0f)
	{
		CreateShot();
		delayShot_ = TIME_DELAY_SHOT;
	}

}

void Turret::CreateShot(void)
{

	// 弾の生成フラグ
	bool isCreate = false;

	for (auto v : shots_)
	{
		if (v->GetState() == ShotTurret::STATE::END)
		{
			// 以前に生成したインスタンスを使い回し
			v->Create({ transformBarrel_.pos.x,
				transformBarrel_.pos.y + 150,
				transformBarrel_.pos.z, },
				transformBarrel_.GetForward());
			isCreate = true;
			break;
		}
	}
	if (!isCreate)
	{
		// 自機の前方方向
		auto dir = transformBarrel_.GetForward();
		// 新しいインスタンスを生成
 		ShotTurret* newShot = new ShotTurret();
		newShot->Create({ transformBarrel_.pos.x,
			transformBarrel_.pos.y + 150 ,
			transformBarrel_.pos.z },
			transformBarrel_.GetForward());

		// 弾の管理配列に追加
		shots_.push_back(newShot);
	}

}

void Turret::Destroy(void)
{
	// 破壊状態へ移行
	ChangeState(STATE::DESTROY);
}

bool Turret::IsAlive(void)
{	
	return isAlive_;
}

void Turret::Release(void)
{

	for (auto& s : shots_)
	{
		s->Release();
		delete s;
	}
	shots_.clear();

}

std::vector<ShotTurret*>& Turret::GetShots(void)
{
	return shots_;
}

const Transform& Turret::GetTransformBarrel(void) const
{
	return transformBarrel_;
}

void Turret::SetState(Turret::STATE state)
{
	ChangeState(state);
}

Turret::STATE Turret::GetState(void)
{
	return state_;
}

void Turret::SetHP(int hp)
{
	hp_ += hp;
}

int Turret::GetHP(void)
{
	return hp_;
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
	{
		isEffect_ = true;
		// エフェクト再生
		effectDestroyPlayId_ = PlayEffekseer3DEffect(effectDestroyResId_);
		// 大きさ
		float SCALE = 100.0f;
		SetScalePlayingEffekseer3DEffect(effectDestroyPlayId_, SCALE, SCALE, SCALE);
		// エフェクトの位置
		SyncDestroyEffect();
		effectTime_ = TIME_DESTROY;
	}
		break;
	case Turret::STATE::END:
		isAlive_ = false;
		break;
	}

}
