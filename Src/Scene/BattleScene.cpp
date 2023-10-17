#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/RockManager.h"
#include "../Object/Ship/PlayerShip.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/ShotPlayer.h"
#include "BattleScene.h"

BattleScene::BattleScene(void)
{

	rockManager_ = nullptr;
	playerShip_ = nullptr;
	bossShip_ = nullptr;
	spaceDome_ = nullptr;

}

BattleScene::~BattleScene(void)
{
}

void BattleScene::Init(void)
{

	auto& manager = SceneManager::GetInstance();
	playerShip_ = new PlayerShip();
	playerShip_->Init();
	const Transform& trans = playerShip_->GetTransform();

	// 背景(スカイドーム)
	spaceDome_ = new SpaceDome(trans);
	spaceDome_->Init();

	// カメラを追従モードにする
	manager.GetCamera()->SetFollow(&trans);
	manager.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	rockManager_ = new RockManager(trans);
	rockManager_->Init();

	// ボス
	bossShip_ = new BossShip();
	bossShip_->Init();

	// 自機の破壊演出時間
	stepShipDestroy_ = 0.0f;

}

void BattleScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE);
	}

	bossShip_->Update();
	playerShip_->Update();
	rockManager_->Update();
	spaceDome_->Update();

	// 衝突判定
	Collision();

}

void BattleScene::Draw(void)
{

	// 不透明
	spaceDome_->Draw();
	rockManager_->Draw();
	bossShip_->Draw();
	playerShip_->Draw();

}

void BattleScene::Release(void)
{

	playerShip_->Release();
	delete playerShip_;
	bossShip_->Release();
	delete bossShip_;
	spaceDome_->Release();
	delete spaceDome_;
	rockManager_->Release();
	delete rockManager_;

}

void BattleScene::Collision(void)
{

	SceneManager& sceneIns = SceneManager::GetInstance();
	// 自機とボスの当たり判定
	if (playerShip_->IsDestroy())
	{
		stepShipDestroy_ += sceneIns.GetDeltaTime();
		if (stepShipDestroy_ > TIME_RESTART)
		{
			// バトルリスタート
			sceneIns.ChangeScene(SceneManager::SCENE_ID::BATTLE);
		}
	}
	else
	{
		// ダンジョン(岩)
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1)
		{
			playerShip_->Destroy();
		}
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
	}

	// 自機の球とボスの当たり判定
	for (auto v : playerShip_->GetShots())
	{
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			v->GetPos(), v->GetCollisionRadius());
		if (info.HitNum >= 1)
		{
			v->SetState(ShotBase::STATE::BLAST);
		}
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
	}
}
