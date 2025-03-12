#pragma once

#ifndef UI_H
#define UI_H

/*
* UI
* ���[�U�[�C���^�[�t�F�[�X�N���X
*/

class UI
{
public:
    // �R���X�g���N�^
    UI();

    //�f�X�g���N�^
    ~UI();

    // HP�A�C�R���̐ݒ�
    void InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight);

    // �}�i�A�C�R���̐ݒ�
    void InitMana(const char* manaTexture, int iconWidth, int iconHeight);

    // �o�ߎ��Ԃ�ݒ肷��֐���ǉ�
    void SetElapsedTime(float time);

    // UI�̕\��
    void Render();

    // HP��ݒ�
    void SetHP(int hp); 

    // �}�i��ݒ�
    void SetMana(int mana); 

private:
    float m_elapsedTime; // �o�ߎ��Ԃ�ۑ�

    std::vector<CImage> m_hpIcons; // HP�A�C�R���p
    int m_maxHP; // �ő�HP
    int m_currentHP; // ���݂�HP

    std::vector<CImage> m_manaIcons; // �}�i�A�C�R���p
    int m_maxMana; // �ő�}�i
    int m_currentMana; // ���݂̃}�i
};

#endif
