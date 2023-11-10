#pragma once
#include <vector>
#include "../Common/Transform.h"
class GameScene;
class Turret;

class BossShip
{

public:

	// 移動スピード
	static constexpr float SPEED_MOVE = 4.0f;

	// 旋回スピード
	static constexpr float ROT_POW_DEG = 3.0f;

	// フェード時間
	static constexpr float TIME_FADE = 8.0f;

	// フェード開始・終了色
	static constexpr COLOR_F FADE_C_FROM = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr COLOR_F FADE_C_TO = { 0.8f, 0.1f, 0.1f, 0.0f };

	// 衝突判定：楕円体半径
	static constexpr float COLLISION_ELLIPSOID_RADIUS_X = 800.0f;
	static constexpr float COLLISION_ELLIPSOID_RADIUS_Y = 800.0f;
	static constexpr float COLLISION_ELLIPSOID_RADIUS_Z = 2000.0f;

	// 爆発間隔
	static constexpr float TIME_EXPLOSION = 0.6f;

	// 状態
	enum class STATE
	{
		NONE,
		EVENT,
		BATTLE,
		DESTROY,
		END,
	};

	// コンストラクタ
	BossShip(void);

	// デストラクタ
	~BossShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform GetTransform(void);

	// 生存判定
	bool IsAlive(void) const;

	// ステージの取得
	int GetModelIdBossShip(void);

	// タレット
	const std::vector<Turret*>& GetTurrets(void) const;

	// 破壊判定
	bool IsDestroy(void) const;

	// 破壊演出終了判定
	bool IsEnd(void) const;

	// 状態の取得
	BossShip::STATE GetState(void);

private:

	// タレット
	std::vector<Turret*> turrets_;

	// モデル制御の基本情報
	Transform transform_;

	// 状態
	STATE state_;

	// タレットの全滅フラグ
	bool isAllDestroyedTurrets_;

	// フェード時間
	float stepFade_;

	// 爆発間隔
	float stepExplosion_;

	// 破壊エフェクト
	int effectDestroyResId_;

	// 破壊エフェクトの再生ハンドルID
	std::vector<int> effectDestroyPlayIds_;

	// 状態遷移
	void ChangeState(STATE state);

	// 前方移動
	void MoveForward(void);

	// 旋回
	void Rotation(void);

	// タレット
	void UpdateTurret(void);
	void DrawTurret(void);

	// タレットの作成
	void MakeTurret(VECTOR localPos, VECTOR localAddAxis);

	// ボス破壊時の爆発エフェクト作成
	void CreateRandomExplosion(void);

};


