#pragma once
#include "../../Object/Ship/ShotBase.h"

class ShotTurret : public ShotBase
{

public:

	// コンストラクタ
	ShotTurret(void);

	// デストラクタ
	~ShotTurret(void);

	ShotTurret::STATE GetState(void);

	void SetState(ShotTurret::STATE state);

protected:

	// パラメータ設定
	void SetParam(void) override;

};

