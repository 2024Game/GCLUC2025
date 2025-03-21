#pragma once
#include "CharaBase.h"
#include "UI.h"			// UI�A�C�R����ύX���邽��

class Boss : public CharaBase
{
public:
	// �R���X�g���N�^// UI ���Q�ƂŎ󂯎��
	Boss(const CVector3D& pos);
	// �f�X�g���N�^
	~Boss();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	void TakeDamage(int damage) override; // TakeDamage ���I�[�o�[���C�h

	// HP���擾(�ǉ�)
	static int Hp();

	// isMove�̎擾
	static bool IsMove();


private:
	// ���
	enum class EState
	{
		Idle,	// �ҋ@���
		Jump,	// �W�����v��
		Attack,	// �U����
		Death,	// ���S
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
	// ��_�����̍X�V����
	void StateDamage();

	//--------------------------------------------------------
	bool m_isInvincible;          // ���G��Ԃ��ǂ���
	float m_invincibleTime;       // ���G���Ԃ̎c��i�b�j
	//--------------------------------------------------------

	EState m_state;
	int m_stateStep;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		Dash,	// �_�b�V��
		Jump,	// �W�����v
		Death,	// ���S
		Attack,	// �U��
		Damage, // ��_��
		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// Boss�h���S���̉摜

	// HP(���L�p)
	static int sHp;
	//
	static bool s_isMove;
};