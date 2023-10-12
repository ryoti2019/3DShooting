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

	// ���@�j�󎞂���̃��X�^�[�g
	static constexpr float TIME_RESTART = 2.0f;

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

	// ���@�̔j�󉉏o����
	float stepShipDestroy_;

	// �Փ˔���
	void Collision(void);

};

