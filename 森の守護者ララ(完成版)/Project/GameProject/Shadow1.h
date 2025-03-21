#pragma once
#include "EnemyBase.h"

class Shadow1 : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="type">�X���C���̎��</param>
	/// <param name="pos">�����ʒu</param>
	Shadow1(int type, const CVector3D& pos);
	// �f�X�g���N�^
	~Shadow1();

	// ���܂ꂽ����
	virtual void Death() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

private:
	// ���
	enum class EState
	{
		Idle,	// �ҋ@���
		Action,	// ���܂ꂽ
	};
	void ChangeState(EState state);

	// �ҋ@���̍X�V����
	void StateIdle();
	// ���܂ꂽ���̍X�V����
	void StateAction();

	EState m_state;
	int m_stateStep;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		Idle,	// �ҋ@
		Action,	// ���܂ꂽ

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// �摜
	int m_type;			// �X���C���̎��
};