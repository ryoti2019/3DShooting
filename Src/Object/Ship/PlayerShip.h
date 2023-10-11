#pragma once
#include "../Common/Transform.h"
class GameScene;

class PlayerShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 1.0f;

	// 自機の回転量
	static constexpr float SPEED_ROT_DEG_Y = 1.0f;
	static constexpr float SPEED_ROT_DEG_X = 1.0f;

	// 自機破壊時からのリスタート
	static constexpr float TIME_RESTART = 2.0f;

	// エフェクトから注視点までの相対座標
	static constexpr VECTOR RELATIVE_F2T_POS = { 0.0f, 0.0f, 0.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_POS_L = { -5.0f, 0.0f, -20.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_POS_R = { 5.0f, 0.0f, -20.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_BOOST_POS = { 0.0f, 0.0f, 500.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_DESTROY_POS = { 0.0f, 0.0f, -50.0f };

	// 衝突判定：球体半径
	static constexpr float COLLISION_RADIUS = 20.0f;

	// 状態
	enum class STATE
	{
		NONE,
		RUN, // 走行状態
		DESTROY// 破壊
	};

	// 自機破壊状態へ遷移
	void Destroy(void);

	// 破壊状態か判定
	bool IsDestroy(void) const;

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

	// 状態
	STATE state_;

	// ゲームシーン
	GameScene* gameScene_;

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

	// 自機破壊エフェクト
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	VECTOR effectDestroyPos_;

	// エフェクトの再生時間
	float effectTime_;

	// エフェクトのフラグ
	bool isEffect_;

	// 操作：移動ブースト
	void ProcessBoost(void);

	// エフェクト制御
	void SyncBoostEffect(void);

	// エフェクト初期化
	void InitEffect(void);

	// エフェクト制御
	void SyncJetEffect(void);

	// 爆破エフェクト制御
	void SyncDestroyEffect(void);

	// 操作
	void ProcessTurn(void);

	void Turn(float deg, VECTOR axis);

	// 状態遷移
	void ChangeState(STATE state);

	// 状態別更新ステップ
	void UpdateRun(void);
	void UpdateDestroy(void);

};

