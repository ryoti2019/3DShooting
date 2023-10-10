#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class SpaceDome;
class BossShip;

class EventScene : public SceneBase
{

public:

	// �R���X�g���N�^
	EventScene(void);

	// �f�X�g���N�^
	~EventScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �X�J�C�h�[���p
	Transform spaceDomeTran_;

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// �{�X
	BossShip* bossShip_;

};

