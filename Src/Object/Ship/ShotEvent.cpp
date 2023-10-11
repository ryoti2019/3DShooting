#include "../../Manager/SceneManager.h"
#include "../../Manager/Camera.h"
#include "ShotEvent.h"

ShotEvent::ShotEvent(void)
{
}

ShotEvent::~ShotEvent(void)
{
}

void ShotEvent::SetParam(void)
{

	// 弾の速度
	speed_ = 20.0f;
	// 生存時間
	timeAlive_ = 10.0f;
	// 衝突用球体半径
	collisionRadius_ = 10.0f;

}

void ShotEvent::CheckAlive(void)
{

	// カメラ衝突チェック
	VECTOR pos2D = ConvWorldPosToScreenPos(transform_.pos);
	if (pos2D.z <= 0.0f || pos2D.z >= 1.0f)
	{

		// カメラに写るように位置を後退させる
		VECTOR velocity = VScale(VScale(dir_, -1.0f), 100.0f);
		VECTOR backPos = VAdd(transform_.pos, velocity);
		transform_.pos = backPos;

		transform_.Update();

		// 爆発状態へ
		ChangeState(STATE::BLAST);

		SceneManager::GetInstance().GetCamera()->ChangeMode(Camera::MODE::SHAKE);

	}

}

ShotBase::STATE ShotEvent::GetState(void)
{
	return state_;
}