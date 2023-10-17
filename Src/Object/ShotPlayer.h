#pragma once
#include "../Object/Ship/ShotBase.h"

class ShotPlayer : public ShotBase
{

public:

	// コンストラクタ
	ShotPlayer(void);

	// デストラクタ
	~ShotPlayer(void);

	ShotPlayer::STATE GetState(void);

	void SetState(ShotPlayer::STATE state);

protected:

	// パラメータ設定
	void SetParam(void) override;

};

