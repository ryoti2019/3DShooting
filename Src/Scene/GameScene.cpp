#include "../Manager/ResourceManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Object/RockManager.h"
#include "../Object/Grid.h"
#include "../Object/SpaceDome.h"
#include "../Object/PlayerShip.h"
#include "GameScene.h"

GameScene::GameScene(void)
{
}

GameScene::~GameScene(void)
{
}

void GameScene::Init(void)
{

	//// �J�������[�h�F�t���[�J����
	//SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FREE);

	grid_ = new Grid();
	grid_->Init();

	playerShip_ = new PlayerShip();
	playerShip_->Init();

	spaceDome_ = new SpaceDome(playerShip_->GetTransform());
	spaceDome_->Init();

	// �J�������[�h�F�Ǐ]
	Camera* camera = SceneManager::GetInstance().GetCamera();
	camera->SetFollow(&playerShip_->GetTransform());
	camera->ChangeMode(Camera::MODE::FOLLOW_SPRING);

	// �X�y�[�X�f�u��
	rockManager_ = new RockManager(playerShip_->GetTransform());
	rockManager_->Init();

}

void GameScene::Update(void)
{

	grid_->Update();

	spaceDome_->Update();

	playerShip_->Update();

	rockManager_->Update();

}

void GameScene::Draw(void)
{

	grid_->Draw();

	// �X�J�C�h�[��(�w�i��s�������ŏ��ɕ`��)
	spaceDome_->Draw();

	// �X�y�[�X�f�u��
	rockManager_->Draw();

	// �L�����N�^�[�n
	playerShip_->Draw();

}

void GameScene::Release(void)
{

	grid_->Release();
	delete grid_;

	spaceDome_->Release();
	delete spaceDome_;

	playerShip_->Release();
	delete playerShip_;

	rockManager_->Release();
	delete rockManager_;

}
