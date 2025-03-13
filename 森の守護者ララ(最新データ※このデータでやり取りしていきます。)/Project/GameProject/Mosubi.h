#pragma once
#include "EnemyBase.h"

class Mosubi : public EnemyBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="type">�X���C���̎��</param>
	/// <param name="pos">�����ʒu</param>
	Mosubi(int type, const CVector3D& pos);
	// �f�X�g���N�^
	~Mosubi();

	// ������������
	virtual void Death() override;

	// �X�V����
	void Update() override;
	// �`�揈��
	void Render() override;

	//�@�L�����̃T�C�Y�����Ƃɓ����蔻��̃T�C�Y��Ԃ�
	float GetChipSizeX()override; //�@����
	float GetChipSizeY()override; //�@����
	float GetChipSizeZ()override; //�@���s��

private:
	// ���
	enum class EState
	{
		Idle,	// �ҋ@���
		Action,	// ��������
	};
	void ChangeState(EState state);

	// �ҋ@���̍X�V����
	void StateIdle();
	// �����������̍X�V����
	void StateAction();

	EState m_state;
	int m_stateStep;

	// �A�j���[�V�����̎��
	enum class EAnimType
	{
		Idle,	// �ҋ@
		Action,	// ��������

		Num,
	};
	static TexAnimData ANIM_DATA[];
	CImage* mp_image;	// �摜
	int m_type;			// ���X�r�̎��
};