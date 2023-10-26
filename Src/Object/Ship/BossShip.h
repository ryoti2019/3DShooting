#pragma once
#include <vector>
#include "../Common/Transform.h"
class GameScene;
class Turret;

class BossShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 0.1f;

	// ����X�s�[�h
	static constexpr float ROT_POW_DEG = 3.0f;

	// �t�F�[�h����
	static constexpr float TIME_FADE = 8.0f;

	// �t�F�[�h�J�n�E�I���F
	static constexpr COLOR_F FADE_C_FROM = { 1.0f, 1.0f, 1.0f, 1.0f };
	static constexpr COLOR_F FADE_C_TO = { 0.8f, 0.1f, 0.1f, 0.0f };

	// ���
	enum class STATE
	{
		NONE,
		EVENT,
		BATTLE,
		DESTROY,
		END,
	};

	// �R���X�g���N�^
	BossShip(void);

	// �f�X�g���N�^
	~BossShip(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	Transform GetTransform(void);

	// ��������
	bool IsAlive(void) const;

	// �X�e�[�W�̎擾
	int GetModelIdBossShip(void);

	// �^���b�g
	const std::vector<Turret*>& GetTurrets(void) const;

	// �j�󔻒�
	bool IsDestroy(void) const;

	// �j�󉉏o�I������
	bool IsEnd(void) const;

private:

	// �^���b�g
	std::vector<Turret*> turrets_;

	// ���f������̊�{���
	Transform transform_;

	// ���
	STATE state_;

	// �^���b�g�̑S�Ńt���O
	bool isAllDestroyedTurrets_;

	// �t�F�[�h����
	float stepFade_;

	// ��ԑJ��
	void ChangeState(STATE state);

	// �O���ړ�
	void MoveForward(void);

	// ����
	void Rotation(void);

	// �^���b�g
	void UpdateTurret(void);
	void DrawTurret(void);

	// �^���b�g�̍쐬
	void MakeTurret(VECTOR localPos, VECTOR localAddAxis);
};


