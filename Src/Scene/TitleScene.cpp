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

	// �J�������[�h�F��_�J����
	SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::FIXED_POINT);

	// �^�C�g�����S
	imgTitleLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::START_LOGO).handleId_;

	spaceDomeTrans_.pos = AsoUtility::VECTOR_ZERO;
	spaceDome_ = new SpaceDome(spaceDomeTrans_);
	spaceDome_->Init();

	// �e�L�X�g�̓ǂݍ���
	for (int i = 0; i < NUM_TEXT; i++)
	{
		text_.handle.emplace_back(ResourceManager::GetInstance().Load(ResourceManager::SRC::ALPHABET).handleIds_[i]);
	}

	// �e�L�X�g�̍��W
	text_.pos = { -300.0f,0.0f,0.0f };

}

void TitleScene::Update(void)
{

	// �V�[���J��
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

	// �X�J�C�h�[���`��
	spaceDome_->Draw();

	// ���S�`��
	DrawLogo();

	// �e�L�X�g�̕`��
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

	// �X�J�C�h�[�����
	spaceDome_->Release();

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