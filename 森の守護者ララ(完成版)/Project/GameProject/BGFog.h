#pragma once
#include "ObjectBase.h"

class BGFog : public ObjectBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="type">�X���C���̎��</param>
	/// <param name="pos">�����ʒu</param>
	BGFog(int type, const CVector3D& pos);
	// �f�X�g���N�^
	~BGFog();

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	CVector2D CalcScreenPos(bool grounded = false) const override;

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

	void ReSetPos();

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

	int loopN4;  // �� �����o�ϐ��ɕύX
};