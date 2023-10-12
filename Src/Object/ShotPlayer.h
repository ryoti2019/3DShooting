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

protected:

	// パラメータ設定
	void SetParam(void) override;

};

