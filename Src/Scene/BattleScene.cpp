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

	// �{�X�̔j�󉉏o����
	stepBossDestroy_ = TIME_RESTART_BOSS_END;

	// �Q�[���N���A���S
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

	// �Փ˔���
	Collision();

	//// �{�X�j��
	//if (bossShip_->IsDestroy())
	//{
	//	// �]�C
	//	stepBossDestroy_ -= SceneManager::GetInstance().GetDeltaTime();
	//	if (stepBossDestroy_ < 0.0f)
	//	{
	//		// �N���A������^�C�g����ʂɖ߂�
	//		SceneManager::GetInstance().ChangeScene(
	//			SceneManager::SCENE_ID::TITLE);
	//	}
	//	return;
	//}

	// �{�X�j��
	if (bossShip_->IsEnd())
	{
		// �N���A������^�C�g����ʂɖ߂�
		SceneManager::GetInstance().ChangeScene(
		SceneManager::SCENE_ID::TITLE);
		return;
	}

}

void BattleScene::Draw(void)
{

	// �s����
	spaceDome_->Draw();
	rockManager_->Draw();
	bossShip_->Draw();
	playerShip_->Draw();

	//// �{�X�����S�ɒ�~
	//if (bossShip_->IsDestroy())
	//{
	//	DrawRotaGraph(
	//		Application::SCREEN_SIZE_X / 2,
	//		Application::SCREEN_SIZE_Y / 2,
	//		1.0f, 0.0f, imgEndLogo_, true);
	//}

	// �{�X�����S�ɒ�~
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
	// ���@�ƃ{�X�̓����蔻��
	if (playerShip_->IsDestroy() && bossShip_->GetState() != BossShip::STATE::DESTROY)
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
		// �{�X�Ǝ��@�Ƃ̓����蔻��
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			playerShip_->GetTransform().pos, PlayerShip::COLLISION_RADIUS);
		if (info.HitNum >= 1 && bossShip_->GetState() != BossShip::STATE::DESTROY)
		{
			playerShip_->Destroy();
		}
		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);
	}

	// ���@�̋��ƃ{�X�̓����蔻��
	for (auto v : playerShip_->GetShots())
	{
		// �e��SHOT��Ԃ���Ȃ�������Փ˔��肵�Ȃ�
		if (v->GetState() != ShotPlayer::STATE::SHOT)
		{
			// �������⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
			continue;
		}
		auto info = MV1CollCheck_Sphere(bossShip_->GetModelIdBossShip(), -1,
			v->GetPos(), v->GetCollisionRadius());
		if (info.HitNum >= 1)
		{
			v->SetState(ShotBase::STATE::BLAST);
		}
		// �����蔻�茋�ʃ|���S���z��̌�n��������
		MV1CollResultPolyDimTerminate(info);
	}

	// �{�X�̋��Ǝ��@�̓����蔻��
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

	// ���@�̋��ƃ^���b�g�̓����蔻��
	for (auto turret : bossShip_->GetTurrets())
	{
		// �^���b�g��ATTACK��Ԃ���Ȃ�������Փ˔��肵�Ȃ�
		if (turret->GetState() != Turret::STATE::ATTACK)
		{
			// �������⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
			continue;
		}
		for (auto v : playerShip_->GetShots())
		{
			// �e��SHOT��Ԃ���Ȃ�������Փ˔��肵�Ȃ�
			if (v->GetState() != ShotPlayer::STATE::SHOT)
			{
				// �������⏈���I����́A�ȍ~�̏����͎��s���Ȃ�
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
