#include "HPBar.h"
//#include <algorithm> // �����Ɉړ�
//#include <GL/gl.h>   // OpenGL�p (�K�v�Ȃ�ǉ�)

HPBar::HPBar(float max, float xPos, float yPos, float w, float h)
    : maxHP(max), currentHP(max), x(xPos), y(yPos), width(w), height(h) {
}

void HPBar::SetHP(float hp) {
    //currentHP = std::max(0.0f, std::min(hp, maxHP)); // std:: ������
    currentHP = (hp < 0.0f) ? 0.0f : (hp > maxHP ? maxHP : hp);
}

void HPBar::Draw() {
    float hpRatio = currentHP / maxHP;

    // �w�i�o�[�i���F�j
    glColor3f(1.0f, 0.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    // HP�̕����i�ԐF�j
    glColor3f(0.0f, 1.0f, 0.0f);
    glBegin(GL_QUADS);
    glVertex2f(x, y);
    glVertex2f(x + width * hpRatio, y);
    glVertex2f(x + width * hpRatio, y + height);
    glVertex2f(x, y + height);
    glEnd();
}
