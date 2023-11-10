#include <DxLib.h>
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Application.h"
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

	isBreak_ = true;

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

	// 中心座標
	VECTOR centerPos;
	centerPos.x = Application::SCREEN_SIZE_X / 2;
	centerPos.y = Application::SCREEN_SIZE_Y / 2;

	// スクリーンサイズよりも大きめの線(ベクトル)
	VECTOR vec;
	vec.y = -1000 - centerPos.y;

	// このベクトルを回転させる
	float radRot = GetRand(10.0f) + 10.0f;
	MATRIX mat = MGetRotZ(radRot);
	VECTOR epos = VTransform(vec, mat);

	// 座標が画面サイズを出ないようにする
	VECTOR sPos = { 0.0f,0.0f,0.0f };
	if (sPos.y <= 0)
	{
		sPos.y = 0;
	}
	if (sPos.y >= Application::SCREEN_SIZE_Y)
	{
		sPos.y = Application::SCREEN_SIZE_Y;
	}

	// この３つの座標をそれぞれ頂点座標、１ポリゴンとする
	for (auto vertex : vertexs_)
	{
		vertex = { sPos.x, sPos.y, 0.0f };
		vertex.u = sPos.x / Application::SCREEN_SIZE_X;
		vertex.v = sPos.y / Application::SCREEN_SIZE_Y;
		vertex.dif = GetColorU8(255, 255, 255, 255);
		vertex.rhw = 1.0f;
		vertexs_.emplace_back(vertex);
	}

	breakScreen_ =
		MakeScreen(
			Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);


	if (isBreak_)
	{
		SetDrawScreen(breakScreen_);
		ClearDrawScreen();
		SceneManager::GetInstance().GetCamera()->SetBeforeDraw();
	}

	if (isBreak_)
	{
		SetDrawScreen(DX_SCREEN_BACK);
		DrawPolygon2D(
			vertexs_.data(),
			(int)vertexs_.size() / 3,
			breakScreen_, true);
	}

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