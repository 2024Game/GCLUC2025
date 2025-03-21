#pragma once

class HPBar {
private:
    float maxHP;
    float currentHP;
    float x, y;   // �ʒu
    float width, height; // �T�C�Y
public:
    // �f�t�H���g�R���X�g���N�^
    HPBar() : maxHP(1.0f), currentHP(1.0f), x(0), y(0), width(100), height(10) {}

    HPBar(float max, float xPos, float yPos, float w, float h);

    void SetHP(float hp);
    void Draw();
};
