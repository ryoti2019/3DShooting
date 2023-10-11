#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SpaceDome;
class BossShip;
class ShotEvent;

class EventScene : public SceneBase
{

public:

	// コンストラクタ
	EventScene(void);

	// デストラクタ
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// スカイドーム用
	Transform spaceDomeTran_;

	// スカイドーム
	SpaceDome* spaceDome_;

	// ボス
	BossShip* bossShip_;

	// イベント用
	ShotEvent* shotEvent_;
};

