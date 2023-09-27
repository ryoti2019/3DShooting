#pragma once
#include "SceneBase.h"

class Grid;
class SpaceDome;
class PlayerShip;
class RockManager;

class GameScene : public SceneBase
{

public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	void Init(void) override;
	void Update(void) override;
	void Draw(void) override;
	void Release(void) override;

private:

	// �X�J�C�h�[��
	SpaceDome* spaceDome_;

	// �O���b�h��
	Grid* grid_;

	// �v���C���[
	PlayerShip* playerShip_;

	// �w�i�̃X�y�[�X�f�u��
	RockManager* rockManager_;

};
