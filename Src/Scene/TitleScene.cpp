//#include <string>
//#include <vector>
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

	// テキストの読み込み
	for (int i = 0; i < NUM_TEXT; i++)
	{
		text_.handle.emplace_back(ResourceManager::GetInstance().Load(ResourceManager::SRC::ALPHABET).handleIds_[i]);
	}

	// テキストの座標
	text_.pos = { -300.0f,0.0f,0.0f };

}

void TitleScene::Update(void)
{

	// シーン遷移
	InputManager& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME);
	}


	Quaternion qua = Quaternion::Euler(AsoUtility::Deg2RadF(-45.0f), AsoUtility::Deg2RadF(0.0f), AsoUtility::Deg2RadF(0.0f));

	auto dir = qua.PosAxis({ 0.0f,0.0f,1.0f });

	text_.pos = VAdd(text_.pos, VScale(dir, 1.0f));

}

void TitleScene::Draw(void)
{

	// スカイドーム描画
	spaceDome_->Draw();

	// ロゴ描画
	DrawLogo();

	// テキストの描画
	std::string msg =
		"       STAFF       "
		"      Ryo Terashi   "
		"                     "
		"       SYSTEM      "
		"      Ryo Terashi   "
		"                     "
		"       GRAPHIC     "
		"      Ryo Terashi   "
		"                     "
		"      TEST PLAY    "
		"      Ryo Terashi   "
		"                     "
		"   SPECIAL THANKS  "
		"      Ryo Terashi    "
		"                     "
		"    PRODICTION    "
		"      Ryo Terashi    "
		"                     ";



	int ascii;
	size_t len = msg.size();
	for (int i = 0; i < len; i++)
	{
		ascii = msg.at(i);
		if (ascii >= 65 && ascii <= 90)
		{
			DrawBillboard3D({ text_.pos.x + (i % 20) * 32 ,text_.pos.y - (i / 20) * 32, text_.pos.z },
				0.0f, 0.0f, 32, 0.0f, text_.handle[ascii - 65], true);
		}
		if (ascii >= 97 && ascii <= 122)
		{
			DrawBillboard3D({ text_.pos.x + (i % 20) * 32 ,text_.pos.y - (i / 20) * 32, text_.pos.z },
				0.0f, 0.0f, 32, 0.0f, text_.handle[ascii - 71], true);
		}
	}

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