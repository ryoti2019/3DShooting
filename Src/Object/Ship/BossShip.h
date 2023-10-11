#pragma once
#include "../Common/Transform.h"
class GameScene;

class BossShip
{

public:

	// �ړ��X�s�[�h
	static constexpr float SPEED_MOVE = 4.0f;

	// ����X�s�[�h
	static constexpr float ROT_POW_DEG = 3.0f;

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

private:

	// ���f������̊�{���
	Transform transform_;

	// ���
	STATE state_;
	
	// ��ԑJ��
	void ChangeState(STATE state);

	// �O���ړ�
	void MoveForward(void);

	// ����
	void Rotation(void);

};


