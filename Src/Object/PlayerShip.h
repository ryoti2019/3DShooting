#pragma once
#include "Common/Transform.h"

class PlayerShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 1.0f;

	// 自機の回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// エフェクトから注視点までの相対座標
	static constexpr VECTOR RELATIVE_F2T_POS = { 0.0f, 0.0f, 0.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_POS_L = { -5.0f, 0.0f, -20.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_POS_R = { 5.0f, 0.0f, -20.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_BOOST_POS = { 0.0f, 0.0f, 500.0f };

	// コンストラクタ
	PlayerShip(void);

	// デストラクタ
	~PlayerShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	// 噴射エフェクト
	int effectJetResId_;
	int effectJetLPlayId_;
	int effectJetRPlayId_;

	VECTOR effectLPos_;
	VECTOR effectRPos_;

	// ブースト時の加算速度
	float speedBoost_;

	// ブースト使用間隔
	float deleyBoost_;

	// ブーストエフェクト
	int effectBoostResId_;
	int effectBoostPlayId_;

	VECTOR effectBoostPos_;

	// 操作：移動ブースト
	void ProcessBoost(void);

	// エフェクト制御
	void SyncBoostEffect(void);

	// エフェクト初期化
	void InitEffect(void);

	// エフェクト制御
	void SyncJetEffect(void);

	// 操作
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

};

