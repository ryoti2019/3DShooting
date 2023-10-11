#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/Ship/BossShip.h"
#include "../Object/Ship/ShotEvent.h"
#include "EventScene.h"

EventScene::EventScene(void)
{
}

EventScene::~EventScene(void)
{
}

void EventScene::Init(void)
{

	// カメラモード・位置を変える
	SceneManager::GetInstance().GetCamera()->ChangeMode(
		Camera::MODE::FIXED_POINT);

	// 背景(スカイドーム)
	spaceDome_ = new SpaceDome(spaceDomeTran_);
	spaceDome_->Init();

	// ボス戦艦
	bossShip_ = new BossShip();
	bossShip_->Init();

	// イベント用の弾
	shotEvent_ = new ShotEvent();
	VECTOR cameraPos = SceneManager::GetInstance().GetCamera()->GetPos();
	VECTOR bossPos = bossShip_->GetTransform().pos;

	// ボス戦艦からカメラへの方向
	VECTOR dir = VNorm(VSub(cameraPos, bossPos));
	shotEvent_->Create(bossPos, dir);

}

void EventScene::Update(void)
{

	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::BATTLE);
	}

	spaceDome_->Update();
	bossShip_->Update();
	shotEvent_->Update();

	// 弾が終了しており、カメラのシェイクモードが終わったら、
	// バトルシーンへ移行する
	if (shotEvent_->GetState() == ShotEvent::STATE::END
		&& SceneManager::GetInstance().GetCamera()->
		GetMode() != Camera::MODE::SHAKE)
	{
		SceneManager::GetInstance().ChangeScene(
			SceneManager::SCENE_ID::BATTLE);
	}

}

void EventScene::Draw(void)
{

	spaceDome_->Draw();
	bossShip_->Draw();
	shotEvent_->Draw();

}

void EventScene::Release(void)
{

	spaceDome_->Release();
	delete spaceDome_;
	bossShip_->Release();
	delete bossShip_;
	shotEvent_->Release();
	delete shotEvent_;

}