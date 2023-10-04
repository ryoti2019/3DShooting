#include <EffekseerForDXLib.h>
#include "../Utility/AsoUtility.h"
#include "../Manager/InputManager.h"
#include "../Manager/SceneManager.h"
#include "../Object/Common/Transform.h"
#include "Camera.h"

Camera::Camera(void)
{
	mode_ = MODE::NONE;
	pos_ = { 0.0f, 0.0f, 0.0f };
	targetPos_ = { 0.0f, 0.0f, 0.0f };
	rot_ = Quaternion::Identity();
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{
	// カメラの初期設定
	SetDefault();
}

void Camera::Update(void)
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;
	case Camera::MODE::FOLLOW_SPRING:
		SetBeforeDrawFollowSprnig();
		break;
	}

	// カメラの設定(位置と注視点による制御)
	SetCameraPositionAndTargetAndUpVec(
		pos_, 
		targetPos_, 
		cameraUp_
	);

	// DXライブラリのカメラとEffekseerのカメラを同期する
	Effekseer_Sync3DSetting();

}

void Camera::SetBeforeDrawFixedPoint(void)
{
	// 何もしない
}

void Camera::SetBeforeDrawFree(void)
{

	auto& ins = InputManager::GetInstance();

	// 回転
	//-------------------------------------
	VECTOR axisDeg = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_UP)) { axisDeg.x += -1.0f; }
	if (ins.IsNew(KEY_INPUT_DOWN)) { axisDeg.x += 1.0f; }
	if (ins.IsNew(KEY_INPUT_LEFT)) { axisDeg.y += -1.0f; }
	if (ins.IsNew(KEY_INPUT_RIGHT)) { axisDeg.y += 1.0f; }

	// 移動
	//--------------------------------------
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (ins.IsNew(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (ins.IsNew(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (ins.IsNew(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (ins.IsNew(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }
	//---------------------------------------

	if (!AsoUtility::EqualsVZero(axisDeg))
	{

		// カメラ座標を中心として、注視点を回転させる

		// 加えたい回転量
		Quaternion rotPow;
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.z), AsoUtility::AXIS_Z));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.x), AsoUtility::AXIS_X));
		rotPow = rotPow.Mult(
			Quaternion::AngleAxis(
				AsoUtility::Deg2RadF(axisDeg.y), AsoUtility::AXIS_Y));

		// カメラの回転に合成
		rot_ = rot_.Mult(rotPow);

		// 注視点の相対座標を回転
		VECTOR localRotPos = rot_.PosAxis(RELATIVE_C2T_POS);

		// 注視点をワールド座標に変換
		targetPos_ = VAdd(pos_, localRotPos);

		// カメラの上方向
		cameraUp_ = rot_.GetUp();
		//cameraUp_ = AsoUtility::DIR_U;

	}

	if (!AsoUtility::EqualsVZero(moveDir))
	{

		// 押下された移動ボタンの方向に移動
		VECTOR direction = VNorm(rot_.PosAxis(moveDir)); // 回転させる必要がある

		// 移動量(方向 * スピード)
		VECTOR movePow = VScale(direction, SPEED);

		// 移動(座標 + 移動量)
		// カメラ位置とカメラ注視点
		pos_ = VAdd(pos_,movePow);
		targetPos_ = VAdd(targetPos_,movePow);

	}

}

void Camera::Draw(void)
{
}

void Camera::Release(void)
{
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

void Camera::SetBeforeDrawFollow(void)
{

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_FOLLOW);

	// カメラ位置の更新
	pos_ = VAdd(followPos, relativeCPos);

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

void Camera::SetBeforeDrawFollowSprnig(void)
{

	// ばね付きの実装
	float POW_SPRING = 24.0f;
	float dampening = 2.0f * sqrt(POW_SPRING);

	// デルタタイム
	float delta = SceneManager::GetInstance().GetDeltaTime();

	// 追従対象(プレイヤー機)の位置
	VECTOR followPos = followTransform_->pos;

	// 追従対象の向き
	Quaternion followRot = followTransform_->quaRot;

	// 追従対象からカメラまでの相対座標
	VECTOR relativeCPos = followRot.PosAxis(RELATIVE_F2C_POS_SPRING);

	// 理想位置
	VECTOR idealPos = VAdd(followPos, relativeCPos);

	// 実際と理想の差
	VECTOR diff = VSub(pos_, idealPos);

	// 力 = -バネの強さ × バネの伸び - 抵抗 × カメラの速度
	VECTOR force = VScale(diff, -POW_SPRING);
	force = VSub(force, VScale(velocity_, dampening));

	// 速度の更新
	velocity_ = VAdd(velocity_, VScale(force, delta));

	// カメラ位置の更新
	pos_ = VAdd(pos_, VScale(velocity_, delta));

	// カメラ位置の更新
	//pos_ = VAdd(followPos, relativeCPos);

	// カメラ位置から注視点までの相対座標
	VECTOR relativeTPos = followRot.PosAxis(RELATIVE_C2T_POS);

	// 注視点の更新
	targetPos_ = VAdd(pos_, relativeTPos);

	// カメラの上方向
	cameraUp_ = followRot.PosAxis(rot_.GetUp());

}

void Camera::SetFollow(const Transform* follow)
{

	followTransform_ = follow;

}

void Camera::ChangeMode(MODE mode)
{

	// カメラの初期設定
	SetDefault();

	// カメラモードの変更
	mode_ = mode;

	// 変更時の初期化処理
	switch (mode_)
	{
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::FOLLOW_SPRING:
		break;
	}

}

void Camera::SetDefault(void)
{

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;

	// 注視点
	targetPos_ = VAdd(pos_, RELATIVE_C2T_POS);

	// カメラの上方向
	cameraUp_ = { 0.0f, 1.0f, 0.0f };

	// カメラはX軸に傾いているが、
	// この傾いた状態を角度ゼロ、傾き無しとする
	rot_ = Quaternion::Identity();

}
