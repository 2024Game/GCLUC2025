#pragma once
#include "CharaBase.h"
#include "UI.h" // UI�A�C�R����ύX���邽��

class Player : public CharaBase
{
public:
	// �R���X�g���N�^// UI ���Q�ƂŎ󂯎��
	Player(const CVector3D& pos);
	// �f�X�g���N�^
	~Player();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	void TakeDamage(int damage) override; // TakeDamage ���I�[�o�[���C�h

private:
	// ���
	enum class EState
	{
		Idle,	// �ҋ@���
		Jump,	// �W�����v��
		Attack,	// �U����
		Death,	// ���S
		Sliding,// �X���C�f�B���O
		Damage, // ��_��
	};
	void ChangeState(EState state);

	// �ړ������̍X�V
	bool UpdateMove();

	// �ҋ@���̍X�V����
	void StateIdle();
	// �W�����v���̍X�V����
	void StateJump();
	// �U�����̍X�V����
	void StateAttack();
	// ���S���̍X�V����
	void StateDeath();
	// �X���C�f�B���O���̍X�V����
	void StateSliding();
	// ��_�����̍X�V����
	void StateDamage();

	EState m_state;
	int m_stateStep;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		Dash,	// �_�b�V��
		Jump,	// �W�����v
		Death,	// ���S
		Attack,	// �U��
		Sliding,// �X���C�f�B���O
		Damage, // ��_��
		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// �v���C���[�̉摜

	float m_moveSpeedY;	// Y���i�����j�̈ړ����x
};
