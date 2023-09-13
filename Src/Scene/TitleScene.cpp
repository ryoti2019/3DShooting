#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "TitleScene.h"

TitleScene::TitleScene(void)
{
	imgTitleLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
}

void TitleScene::Init(void)
{

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;

	spaceDome_ = new SpaceDome;
	spaceDome_->Init();

}

void TitleScene::Update(void)
{

	// �V�[���J��
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TitleScene::Draw(void)
{

	// �X�J�C�h�[���`��
	spaceDome_->Draw();

	// ���S�`��
	DrawLogo();

}

void TitleScene::Release(void)
{
}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// �^�C�g�����S
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, imgTitleLogo_, true);

	// Push���b�Z�[�W
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}