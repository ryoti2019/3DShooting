#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/SpaceDome.h"
#include "../Object/RockManager.h"
#include "../Object/Ship/PlayerShip.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/Ship/Turret.h"
#include "../Object/Ship/ShotTurret.h"
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

	// ボスの破壊演出時間
	stepBossDestroy_ = TIME_RESTART_BOSS_END;

	// ゲームクリアロゴ
	imgEndLogo_ = ResourceManager::GetInstance().Load(
		ResourceManager::SRC::END_LOGO).handleId_;

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

	//// ボス破壊
	//if (bossShip_->IsDestroy())
	//{
	//	// 余韻
	//	stepBossDestroy_ -= SceneManager::GetInstance().GetDeltaTime();
	//	if (stepBossDestroy_ < 0.0f)
	//	{
	//		// クリアしたらタイトル画面に戻る
	//		SceneManager::GetInstance().ChangeScene(
	//			SceneManager::SCENE_ID::TITLE);
	//	}
	//	return;
	//}

	// ボス破壊
	if (bossShip_->IsEnd())
	{
		// クリアしたらタイトル画面に戻る
		SceneManager::GetInstance().ChangeScene(
		SceneManager::SCENE_ID::TITLE);
		return;
	}

}

void BattleScene::Draw(void)
{

	// 不透明
	spaceDome_->Draw();
	rockManager_->Draw();
	bossShip_->Draw();
	playerShip_->Draw();

	//// ボスが完全に停止
	//if (bossShip_->IsDestroy())
	//{
	//	DrawRotaGraph(
	//		Application::SCREEN_SIZE_X / 2,
	//		Application::SCREEN_SIZE_Y / 2,
	//		1.0f, 0.0f, imgEndLogo_, true);
	//}

	// ボスが完全に停止
	if (bossShip_->IsDestroy() || bossShip_->IsEnd())
	{
		DrawRotaGraph(
			Application::SCREEN_SIZE_X / 2,
			Application::SCREEN_SIZE_Y / 2,
			1.0f, 0.0f, imgEndLogo_, true);
	}

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
	if (playerShip_->IsDestroy() && bossShip_->GetState() != BossShip::STATE::DESTROY)
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
		// ボスと自機との当たり判定
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1 && bossShip_->GetState() != BossShip::STATE::DESTROY)
		{
			playerShip_->Destroy();
		}
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
	}

	// 自機の球とボスの当たり判定
	for (auto v : playerShip_->GetShots())
	{
		// 弾がSHOT状態じゃなかったら衝突判定しない
		if (v->GetState() != ShotPlayer::STATE::SHOT)
		{
			// 爆発中や処理終了後は、以降の処理は実行しない
			continue;
		}
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			v->GetPos(), v->GetCollisionRadius());
		if (info.HitNum >= 1)
		{
			v->SetState(ShotBase::STATE::BLAST);
		}
		// 当たり判定結果ポリゴン配列の後始末をする
		MV1CollResultPolyDimTerminate(info);
	}

	// ボスの球と自機の当たり判定
	for (auto turret : bossShip_->GetTurrets())
	{
		for (auto v : turret->GetShots())
		{
			auto info = AsoUtility::IsHitSpheres(playerShip_->GetTransform().pos, playerShip_->COLLISION_RADIUS,
				v->GetPos(), v->GetCollisionRadius());
			if (info)
			{
				playerShip_->Destroy();
			}
		}
	}

	// 自機の球とタレットの当たり判定
	for (auto turret : bossShip_->GetTurrets())
	{
		// タレットがATTACK状態じゃなかったら衝突判定しない
		if (turret->GetState() != Turret::STATE::ATTACK)
		{
			// 爆発中や処理終了後は、以降の処理は実行しない
			continue;
		}
		for (auto v : playerShip_->GetShots())
		{
			// 弾がSHOT状態じゃなかったら衝突判定しない
			if (v->GetState() != ShotPlayer::STATE::SHOT)
			{
				// 爆発中や処理終了後は、以降の処理は実行しない
				continue;
			}
			auto info = AsoUtility::IsHitSpheres(v->GetPos(), v->GetCollisionRadius(),
				turret->GetTransformBarrel().pos, turret->COLLISION_RADIUS);
			if (info)
			{
				turret->SetHP(-1);
				v->SetState(ShotPlayer::STATE::BLAST);
				if (turret->GetHP() <= 0)
				{
					turret->Destroy();
				}
			}
		}
	}

}
