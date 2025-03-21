#pragma once
#include "Task.h"

// �S�ẴI�u�W�F�N�g�̃x�[�X�N���X
class ObjectBase : public Task
{
public:
	// �R���X�g���N�^
	ObjectBase();
	ObjectBase(const CVector3D& pos);
	// �f�X�g���N�^
	virtual ~ObjectBase();

	// ���W���擾
	const CVector3D& GetPos() const;
	// ���W��ݒ�
	void SetPos(const CVector3D& pos);

	/// <summary>
	/// 3�������W����X�N���[�����W�i2D�j�ɕϊ�
	/// </summary>
	/// <param name="grounded">true�Ȃ�΁A�����͍l�������n�ʂ̈ʒu��Ԃ�</param>
	/// <returns>�X�N���[�����W</returns>
	virtual CVector2D CalcScreenPos(bool grounded = false) const;

	// �e�`��
	virtual void RenderShadow();
	virtual void RenderShadow1(); // �A�C�e���֌W

	//�@�I�u�W�F�N�g�̃T�C�Y�����Ƃɓ����蔻��̉����̃T�C�Y��Ԃ�
	virtual float GetChipSizeX();
	//�@�I�u�W�F�N�g�̃T�C�Y�����Ƃɓ����蔻��̉��s���̃T�C�Y��Ԃ�
	virtual float GetChipSizeY();
	//�@�I�u�W�F�N�g�̃T�C�Y�����Ƃɓ����蔻��̍����̃T�C�Y��Ԃ�
	virtual float GetChipSizeZ();

protected:
	CVector3D m_pos;		// �I�u�W�F�N�g�̍��W
	bool m_isGrounded;		// �n�ʂɐڒn���Ă��邩�ǂ���
	CImage* mp_shadowImg;	// �e�̉摜
};