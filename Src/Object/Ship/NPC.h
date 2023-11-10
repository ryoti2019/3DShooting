#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Transform.h"

class PlayerShip;

class NPC
{
public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 10.0f;

	// 加速時のスピード
	static constexpr float BOOST_SPEED = 15.0f;

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_POS = { 0.0f, 0.0f, -20.0f };

	// 追従対象からエフェクトまでの相対座標(完全追従)
	static constexpr VECTOR LOCAL_DESTROY_POS = { 0.0f, 0.0f, -50.0f };

	// 状態
	enum class STATE
	{
		NONE,
		RUN, // 走行状態
		DESTROY// 破壊
	};

	// 軌跡データ
	struct OrbitData
	{

		// どの方向に
		VECTOR dir;

		// 旋回する時間
		float turnTime;

		// 前進する時間
		float advanceTime;

		// 旋回する時間の比率
		float stepTime;

	};

	// コンストラクタ
	NPC(void);

	// デストラクタ
	~NPC(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// プレイヤーのTransformの取得
	const Transform& GetTransform(void) const;

private:

	// モデル制御の基本情報
	Transform transform_;

	// プレイヤー
	PlayerShip* playerShip_;

	// 前の方向
	VECTOR preOrbitDir_;

	// 状態
	STATE state_;

	// 軌跡データ
	std::vector<OrbitData> orbitDatas_;

	// データの添え字
	int data_;

	// 噴射エフェクト
	int effectJetResId_;
	int effectJetPlayId_;

	VECTOR effectPos_;

	// 自機破壊エフェクト
	int effectDestroyResId_;
	int effectDestroyPlayId_;

	VECTOR effectDestroyPos_;

	// エフェクトの再生時間
	float effectTime_;

	// エフェクトのフラグ
	bool isEffect_;

	// エフェクト初期化
	void InitEffect(void);

	// エフェクト制御
	void SyncJetEffect(void);

	// 爆破エフェクト制御
	void SyncDestroyEffect(void);

	// 状態遷移
	void ChangeState(STATE state);

	// 状態別更新ステップ
	void UpdateRun(void);
	void UpdateDestroy(void);

};

