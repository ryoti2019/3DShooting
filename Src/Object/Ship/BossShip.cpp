#include <EffekseerForDXLib.h>
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Ship/Turret.h"
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
		// 当たり判定(コライダ)作成
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// ×処理が遅い ↑↑↑
		MV1RefreshCollInfo(transform_.modelId, -1);
		ChangeState(STATE::BATTLE);
		break;
	}

	// 破壊エフェクト
	effectDestroyResId_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::PLAYER_SHIP_EXPLOSION).handleId_;

	// タレットの全滅フラグ
	isAllDestroyedTurrets_ = false;

	// フェード時間
	stepFade_ = TIME_FADE;

	// 前方
	MakeTurret(
		{ 4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(-18.0f) });
	MakeTurret(
		{ -4.5f, 5.5f, 7.8f },
		{ 0.0f, 0.0f, AsoUtility::Deg2RadF(18.0f) });

	// 横
	MakeTurret(
		{ 4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(90.0f), 0.0f });
	MakeTurret(
		{ -4.5f, 5.5f, 0.0f },
		{ AsoUtility::Deg2RadF(20.0f), AsoUtility::Deg2RadF(-90.0f), 0.0f });

	// 後方
	MakeTurret(
		{ 3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(18.0f) });
	MakeTurret(
	{ -3.5f, 5.0f, -17.8f },
		{ 0.0f, AsoUtility::Deg2RadF(180.0f), AsoUtility::Deg2RadF(-18.0f) });

}
void BossShip::Update(void)
{

	VECTOR forward = transform_.GetForward();

	// 移動
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

	// モデル制御の基本情報更新
	transform_.Update();

	switch (state_)
	{
	case BossShip::STATE::NONE:
		break;
	case BossShip::STATE::EVENT:
		MoveForward();
		// タレット
		UpdateTurret();
		break;
	case BossShip::STATE::BATTLE:
		Rotation();
		MoveForward();
		// 当たり判定(コライダ)作成
		//int ret = MV1SetupCollInfo(transform_.modelId, -1, 1, 1, 1);
		// ×処理が遅い ↑↑↑
		MV1RefreshCollInfo(transform_.modelId, -1);
		// タレット
		UpdateTurret();
		if (isAllDestroyedTurrets_)
		{
			// タレットが全滅したら、ボスも破壊する
			ChangeState(STATE::DESTROY);
			return;
		}
		break;
	case BossShip::STATE::DESTROY:
		// タレット
		UpdateTurret();
		// 破壊時の機体フェード
		stepFade_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepFade_ < 0.0f)
		{
			ChangeState(STATE::END);
			return;
		}
		// 破壊時のエフェクト
		stepExplosion_ -= SceneManager::GetInstance().GetDeltaTime();
		if (stepExplosion_ < 0.0f)
		{
			CreateRandomExplosion();
			stepExplosion_ = TIME_EXPLOSION;
		}
		break;
	case BossShip::STATE::END:
		break;
	}

}

void BossShip::Draw(void)
{

	switch (state_)
	{
	case BossShip::STATE::NONE:
		break;
	case BossShip::STATE::EVENT:
	case BossShip::STATE::BATTLE:
		MV1DrawModel(transform_.modelId);
		DrawTurret();
		break;
	case BossShip::STATE::DESTROY:
	{
		// 時間による色の線形補間
		float diff = TIME_FADE - stepFade_;
		auto c = AsoUtility::Lerp(FADE_C_FROM, FADE_C_TO, (diff / TIME_FADE));
		// モデルのマテリアルを取得
		int num = MV1GetMaterialNum(transform_.modelId);
		for (int i = 0; i < num; i++)
		{
			// モデルのディフューズカラーを変更
			MV1SetMaterialDifColor(transform_.modelId, i, c);
		}
		// モデルの描画
		MV1DrawModel(transform_.modelId);
		break;
	}
	case BossShip::STATE::END:
		break;
	}
}

void BossShip::Release(void)
{

	for (auto& t : turrets_)
	{
		t->Release();
		delete t;
	}
	turrets_.clear();

	for (int playId : effectDestroyPlayIds_)
	{
		// 破壊エフェクト停止
		StopEffekseer3DEffect(playId);
	}

}

Transform BossShip::GetTransform(void)
{
	return transform_;
}

bool BossShip::IsAlive(void) const
{
	return state_ == STATE::BATTLE;
}

int BossShip::GetModelIdBossShip(void)
{
	return transform_.modelId;
}

const std::vector<Turret*>& BossShip::GetTurrets(void) const
{
	return turrets_;
}

bool BossShip::IsDestroy(void) const
{
	return state_ == STATE::DESTROY;
}

bool BossShip::IsEnd(void) const
{
	return state_ == STATE::END;
}

BossShip::STATE BossShip::GetState(void)
{
	return state_;
}

void BossShip::ChangeState(STATE state)
{
	state_ = state;
}

void BossShip::MoveForward(void)
{

	// 敵の前方向を取得
	VECTOR forward = transform_.GetForward();

	// 移動
	transform_.pos = VAdd(transform_.pos, VScale(forward, SPEED_MOVE));

}

void BossShip::Rotation(void)
{
	// 引数で指定された回転量・軸分、回転を加える

	// ①デグリーをラジアンに変換する
	float rad = AsoUtility::Deg2RadF(ROT_POW_DEG * SceneManager::GetInstance().GetDeltaTime());

	// ②ラジアンをクォータニオンに変換
	// (とある関数で、回転量RADと回転軸を使用)
	Quaternion rotPow = Quaternion::AngleAxis(rad, AsoUtility::AXIS_Y);

	// ③今回作成した回転量を、自機の回転量に加える(合成する)
	transform_.quaRot = transform_.quaRot.Mult(rotPow);
}

void BossShip::UpdateTurret(void)
{

	int alived = 0;

	for (auto t : turrets_)
	{
		t->Update();

		if (t->IsAlive())
		{
			alived++;
		}

	}

	// タレットが１機も生存していなければ
	if (alived == 0)
	{
		isAllDestroyedTurrets_ = true;
	}

}

void BossShip::DrawTurret(void)
{

	for (auto t : turrets_)
	{
		t->Draw();
	}

}

void BossShip::MakeTurret(VECTOR localPos, VECTOR localAddAxis)
{

	// タレットの実体を作成
	Turret* turret = new Turret(transform_, localPos, localAddAxis);
	// 初期化
	turret->Init();
	turrets_.push_back(turret);

}

void BossShip::CreateRandomExplosion(void)
{

	// 楕円周上にランダムな座標を作成する
	for (int i = 0; i < 2; i++)
	{

		// XYZのランダムな角度
		float x = static_cast<float>(GetRand(360));
		float y = static_cast<float>(GetRand(360));
		float z = static_cast<float>(GetRand(360));

		// ラジアンに変換
		x = AsoUtility::Deg2RadF(x);
		y = AsoUtility::Deg2RadF(y);
		z = AsoUtility::Deg2RadF(z);

		// クォータニオンに変換
		Quaternion randRot = Quaternion::Euler(x, y, z);

		// 方向に変換
		VECTOR dir = randRot.PosAxis({ 0.0f, 0.0f , 1.0f });

		// 方向に対して、楕円のXYZの半径分距離を伸ばす
		// ここで、半径をランダムにすると、楕円周上ではなく、楕円体状の座標になる
		VECTOR pos = { 0.0f, 0.0f, 0.0f };
			   pos = VAdd(pos, VScale({ dir.x, 0.0f, 0.0f }, COLLISION_ELLIPSOID_RADIUS_X));
			   pos = VAdd(pos, VScale({ 0.0f, dir.y, 0.0f }, COLLISION_ELLIPSOID_RADIUS_Y));
			   pos = VAdd(pos, VScale({ 0.0f, 0.0f, dir.z }, COLLISION_ELLIPSOID_RADIUS_Z));

		// 生成された座標はボス戦艦からの相対座標になっているので、
		// 回転させた上で、ワールド座標に直す
		// 回転情報
		Quaternion shipRot = transform_.quaRot;

		// 相対座標を回転
		VECTOR localRotPos = shipRot.PosAxis(pos);

		// ワールド座標に変換(念のため、SCALE 1)
		pos = VAdd(transform_.pos, VScale(localRotPos, 1.0f));

		// 破壊エフェクト生成
		int playId = PlayEffekseer3DEffect(effectDestroyResId_);

		// 大きさ
		SetScalePlayingEffekseer3DEffect(playId, 50.0f, 50.0f, 50.0f);

		// 位置
		SetPosPlayingEffekseer3DEffect(playId, pos.x, pos.y, pos.z);

		// 角度
		VECTOR angles = transform_.quaRot.ToEuler();
		SetRotationPlayingEffekseer3DEffect(playId, angles.x, angles.y, angles.z);

		// 動的配列に追加
		effectDestroyPlayIds_.push_back(playId);

	}

}
