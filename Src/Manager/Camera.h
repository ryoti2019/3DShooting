#pragma once
#include <DxLib.h>
#include "../Common/Quaternion.h"

class Transform;

class Camera
{

public:

	// カメラスピード：NEAR
	static constexpr float SPEED = 20.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_NEAR = 40.0f;

	// カメラクリップ：NEAR
	static constexpr float CAMERA_FAR = 15000.0f;

	// シェイク：時間
	static constexpr float TIME_SHAKE = 2.0f;
	// シェイク：幅
	static constexpr float WIDTH_SHAKE = 3.0f;
	// シェイク：スピード
	static constexpr float SPEED_SHAKE = 30.0f;

	// カメラの初期座標
	static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

	// カメラ位置から注視点までの相対座標
	static constexpr VECTOR RELATIVE_C2T_POS = { 0.0f, -100.0f, 500.0f };

	// 追従対象からカメラ位置までの相対座標(完全追従)
	static constexpr VECTOR RELATIVE_F2C_POS_FOLLOW = { 0.0f, 25.0f, -80.0f };

	// 追従対象からカメラ位置までの相対座標(ばね付き)
	static constexpr VECTOR RELATIVE_F2C_POS_SPRING = { 0.0f, 40.0f, 150.0f };

	// カメラモード
	enum class MODE
	{
		NONE,
		FIXED_POINT,	// 定点カメラ
		FREE,			// フリーモード
		FOLLOW,			// 追従モード
		FOLLOW_SPRING,	// ばね
		SHAKE // カメラ揺らし
	};

	Camera(void);
	~Camera(void);

	void Init(void);
	void Update(void);
	void SetBeforeDraw(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawFollowSprnig(void);
	void SetBeforeDrawShake(void);
	void Draw(void);
	void Release(void);

	VECTOR GetPos(void) const;

	// 追従対象の設定
	void SetFollow(const Transform* follow);

	// カメラモードの変更
	void ChangeMode(MODE mode);

	// カメラモードの取得
	MODE GetMode(void);

private:

	// カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラの注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	// カメラの速度
	VECTOR velocity_;

	// カメラの回転
	Quaternion rot_;

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 追従対象
	const Transform* followTransform_;
	
	// 画面揺らし用
	float stepShake_;
	VECTOR defaultPos_;
	VECTOR shakeDir_;

};

