#pragma once
#include <vector>
#include "SceneBase.h"
class RockManager;
class SpaceDome;
class PlayerShip;
class BossShip;

class BattleScene : public SceneBase
{

public:

	// �R���X�g���N�^
	BattleScene(void);

	// �f�X�g���N�^
	~BattleScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �w�i��
	RockManager* rockManager_;

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// ���@
	PlayerShip* playerShip_;

	// �{�X���
	BossShip * bossShip_;

};

