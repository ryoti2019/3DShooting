#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/Camera.h"
#include "../Object/SpaceDome.h"
#include "../Object/Common/Transform.h"
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

	// カメラモード：定点カメラ
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// タイトルロゴ
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;

	spaceDomeTrans_.pos = AsoUtility::VECTOR_ZERO;
	spaceDome_ = new SpaceDome(spaceDomeTrans_);
	spaceDome_->Init();

}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}

}

void TitleScene::Draw(void)
{

	// スカイドーム描画
	spaceDome_->Draw();

	// ロゴ描画
	DrawLogo();

}

void TitleScene::Release(void)
{

	// スカイドーム解放
	spaceDome_->Release();

}

void TitleScene::DrawLogo(void)
{

	int cx = Application::SCREEN_SIZE_X / 2;
	int cy = Application::SCREEN_SIZE_Y / 2;

	// タイトルロゴ
	DrawRotaGraph(
		cx, cy - 200,
		1.0f, 0.0f, imgTitleLogo_, true);

	// Pushメッセージ
	std::string msg = "Push Space";
	SetFontSize(28);
	int len = (int)strlen(msg.c_str());
	int width = GetDrawStringWidth(msg.c_str(), len);
	DrawFormatString(cx - (width / 2), 200, 0x87cefa, msg.c_str());
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	SetFontSize(16);

}
