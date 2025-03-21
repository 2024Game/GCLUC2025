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

    // �V�[��
    // �X�^�[�g���
    void Start(const char* titleTexture, int gWidth, int gHeight);

    // �Q�[���N���A���
    void Clear(const char* clearTexture, int gWidth, int gHeight);

    // �Q�[���I�[�o�[
    void Over(const char* overTexture, int gWidth, int gHeight);

    // HP�A�C�R���̐ݒ�
    void InitHp(const char* hpTexture, int maxHP, int iconWidth, int iconHeight);

    // �}�i�A�C�R���̐ݒ�
    void InitMana(const char* manaTexture, int iconWidth, int iconHeight);

    // �^�C���̌��̔̐ݒ�
    void InitTimeBack(const char* timeBackTexture, int iconWidth, int iconHeight);

    // �o�ߎ��Ԃ�ݒ肷��֐�
    void SetElapsedTime(float time);

    // �N���A���Ԃ�ݒ肷��֐�
    void SetClearTime(float time);

    // UI�̕\��
    void Render();

    // �N���A���Ԃ̕\��
    void Render1();

    // HP��ݒ�
    void SetHP(int hp); 

    // �}�i��ݒ�
    void SetMana(int mana);
    
    // �{�X�֌W
    void InitBossHPBar(const char* hpBarBackTexture, const char* hpBarFrontTexture, int barWidth, int barHeight);

    void SetBossHP(int hp);

    void RenderBossHP();

private:
    // �V�[��(�ǉ�)
    std::vector<CImage> m_titleG; // �^�C�g����ʗp
    std::vector<CImage> m_clearG; // �N���A��ʗp
    std::vector<CImage> m_overG;  // �I�[�o�[��ʗp

    float m_elapsedTime; // �o�ߎ��Ԃ�ۑ�
    float m_clearTime = 0.0f; // �N���A���Ԃ�ۑ�

    std::vector<CImage> m_hpIcons; // HP�A�C�R���p
    int m_maxHP; // �ő�HP
    int m_currentHP; // ���݂�HP

    std::vector<CImage> m_manaIcons; // �}�i�A�C�R���p
    int m_maxMana; // �ő�}�i
    int m_currentMana; // ���݂̃}�i

    std::vector<CImage> m_timebackIcon; // �^�C���̌��̔p

    // �{�X�֌W
    std::vector<CImage> m_bossHPBarBack; // �w�i�o�[�p
    std::vector<CImage> m_bossHPBarFront; // �O�ʃo�[�p
    int m_bossHPBarWidth; // �{�XHP�o�[�̍ő啝
    int m_bossHPBarHeight; // �{�XHP�o�[�̍���
    int m_maxBossHP; // �ő�HP
    int m_currentBossHP; // ���݂�HP
};

#endif
