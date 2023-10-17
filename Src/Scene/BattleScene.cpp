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

	// �w�i(�X�J�C�h�[��)
	spaceDome_ = new SpaceDome(trans);
	spaceDome_->Init();

	// �J������Ǐ]���[�h�ɂ���
	manager.GetCamera()->SetFollow(&trans);
	manager.GetCamera()->ChangeMode(Camera::MODE::FOLLOW_SPRING);
	rockManager_ = new RockManager(trans);
	rockManager_->Init();

	// �{�X
	bossShip_ = new BossShip();
	bossShip_->Init();

	// ���@�̔j�󉉏o����
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

	// �Փ˔���
	Collision();

}

void BattleScene::Draw(void)
{

	// �s����
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
	// ���@�ƃ{�X�̓����蔻��
	if (playerShip_->IsDestroy())
	{
		stepShipDestroy_ += sceneIns.GetDeltaTime();
		if (stepShipDestroy_ > TIME_RESTART)
		{
			// �o�g�����X�^�[�g
			sceneIns.ChangeScene(SceneManager::SCENE_ID::BATTLE);
		}
	}
	else
	{
		// �_���W����(��)
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1)
		{
			playerShip_->Destroy();
		}
		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);
	}

	// ���@�̋��ƃ{�X�̓����蔻��
	for (auto v : playerShip_->GetShots())
	{
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			v->GetPos(), v->GetCollisionRadius());
		if (info.HitNum >= 1)
		{
			v->SetState(ShotBase::STATE::BLAST);
		}
		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);
	}
}
