#include "TaskManager.h"
#include "Player.h"
#include "Slime.h"
#include "Mosubi.h"
#include "Rock.h"
#include "Potion.h"
#include "Mana.h"
#include "Ground.h"                   // 3/18�ύX�i����j
#include "BGTree1.h"                   // 3/18�ύX�i����j
#include "BGTree2.h"                   // 3/18�ύX�i����j
#include "BGTree3.h"                   // 3/18�ύX�i����j
#include "BGFog.h"                   // 3/18�ύX�i����j
#include "Boss.h"

//UI�֌W
#include "UI.h"
#include <chrono>
#include "HPBar.h"
//

// �e�֌W
#include "Shadow1.h"
#include"Shadow2.h"

#include <cstdlib> // rand() �� srand() �̂���
#include <ctime>   // time() �̂���

#include <iostream>
#include <thread>


//--------------------------------------------
//�O���[�o���ϐ��̈�
//--------------------------------------------
Player* g_player = nullptr;			 // �v���C���[�̃|�C���^
Slime* g_slime = nullptr;			 // �L���[�S���̃|�C���^
Mosubi* g_mosubi = nullptr;			 // ���X�r�̃|�C���^
Rock* g_rock = nullptr;				 // ��̃|�C���^
Potion* g_potion = nullptr;          // �|�[�V�����̃|�C���^
Mana* g_mana = nullptr;              // �}�i�̃|�C���^
Boss* g_boss = nullptr;              // �{�X�̃|�C���^			// 3/18�ύX�i����j
Ground* g_ground = nullptr;// �n�ʉ摜�̃|�C���^				// 3/18�ύX�i����j
BGTree1* g_bgTree1 = nullptr;// �w�i1�̃|�C���^					// 3/18�ύX�i����j
BGTree2* g_bgTree2 = nullptr;// �w�i2�̃|�C���^					// 3/18�ύX�i����j
BGTree3* g_bgTree3 = nullptr;// �w�i3�̃|�C���^					// 3/18�ύX�i����j
BGFog* g_bgFog = nullptr;// �w�i4�̃|�C���^       �@            // 3/18�ύX�i����j
Shadow1* g_shadow1 = nullptr; // �v���C���[�̉e�̃|�C���^
Shadow2* g_shadow2 = nullptr; // �v���C���[�ȊO�̂��ׂẲe�̃|�C���^

// �Q�[�����Z�b�g�ɕK�v
//std::vector<Slime*> g_slimes;  // �����̃L���E�S����ێ�����
//std::vector<Mosubi*> g_mosubis;  // �����̃��X�r��ێ�����
//std::vector<Rock*> g_rocks;  // �����̊��ێ�����

std::vector<EnemyBase*> g_enemybases; // �S�Ă̏�Q����ێ�
std::vector<Shadow2*> g_shadow2s; // �S�Ẳe(�v���C���[�ȊO)��ێ�

// �ǉ�
std::vector<Ground*> g_grounds; // �S�Ă̏�Q����ێ�
// ���̃\�[�X�t�@�C���Œ�`����Ă���ϐ����g��
extern float testScrollX;
std::vector<BGTree1*> g_bgTree1s; // �S�Ă̏�Q����ێ�
std::vector<BGTree2*> g_bgTree2s; // �S�Ă̏�Q����ێ�
std::vector<BGTree3*> g_bgTree3s; // �S�Ă̏�Q����ێ�
std::vector<BGFog*> g_bgFogs; // �S�Ă̏�Q����ێ�
//

//int generatedCount = 0; // ��Q�������̃J�E���g
float elapsedTime1;

// �o�ߎ��Ԃ�ǐՂ��邽�߂̕ϐ�
std::chrono::steady_clock::time_point lastGenerationTime;
float generationInterval = 3.0f; // ��Q�������̊Ԋu�i�b�P�ʁj
//int generatedCount = 0;


CImage* g_field = nullptr;	// �t�B�[���h�摜�̃|�C���^

// UI�֌W
UI* g_ui = nullptr;      // UI �N���X�̃|�C���^
std::chrono::steady_clock::time_point g_startTime; // �Q�[���J�n����
//


// BGM�֌W(�ǉ�)
// �O���[�o���ϐ��Ƃ���BGM���Ǘ�����ϐ���ǉ�
CSoundBase* g_bgm = nullptr;
bool isBGM = true; // BGM�̔���
//

// �V�[���֌W
enum class EState
{
	ESTART,  //�Q�[���J�n
	EPLAY,   //�Q�[����
	ECLEAR,  //�Q�[���N���A
	EOVER,   //�Q�[���I�[�o�[
};

EState mState;

bool isGame; // �Q�[���̔���
bool isStart; // �o�ߎ��Ԃ̔���
bool isDeath = false; // ���S�̔���(��)
bool isClear = false; // �N���A���Ԃ̔���
float deathCount = 0.0f; // ���S�J�E���g(��)
float g_clearTime = 0.0f; // �Q�[���N���A���̌o�ߎ���
bool isFinish = false; // �N���A�̔���(��)
float finishCount = 0.0f; // �N���A�J�E���g(��)
float ax;
int sN = 0;


// �p�^�[���̊֐�
void Pataan1()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan2()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan3()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan4()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan5()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	//g_enemybases.push_back(new Potion(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan6()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 300.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan7()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan8()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan9()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan10()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -230.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan11()
{
	ax = 2880.0f;
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax - 70.0f) + (1920 * sN), -200.0f, -200.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D((ax - 70.0f) + (1920 * sN), 160.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan12()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan13()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan14()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_enemybases.push_back(new Potion(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan15()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan16()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan17()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Potion(0, CVector3D((ax -100.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan18()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 100.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 250.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan19()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Rock(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 250.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D(ax + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Mosubi(0, CVector3D(ax + (1920 * sN), 160.0f, -80.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}
void Pataan20()
{
	ax = 2880.0f;
	g_enemybases.push_back(new Potion(0, CVector3D((ax - 70.0f) + (1920 * sN), 0.0f, -230.0f)));
	g_enemybases.push_back(new Rock(0, CVector3D(ax + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 300.0f) + (1920 * sN), -200.0f, -50.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 300.0f) + (1920 * sN), 0.0f, -80.0f)));
	g_shadow2s.push_back(new Shadow2(0, CVector3D((ax + 70.0f) + (1920 * sN), -200.0f, 100.0f))); // �e�̐���
	g_enemybases.push_back(new Slime(0, CVector3D((ax + 70.0f) + (1920 * sN), 0.0f, 70.0f)));
	g_enemybases.push_back(new Mana(0, CVector3D((ax + 400.0f) + (1920 * sN), 0.0f, 70.0f)));
}

void GenerateObject()
{
	// �����_���̎����x�����ݒ�
	static bool seedSet = false;
	if (!seedSet) 
	{
		srand(static_cast<unsigned int>(time(0))); // �����_���̎��ݒ�
		seedSet = true;
	}

	// 0�`7�̃����_���ȃp�^�[����I��
	int randomPattern = rand() % 20;
	//int randomPattern = 5;

	// �����_���ȃp�^�[���ɉ����Ċ֐����Ăяo��
	switch (randomPattern) 
	{
	case 0: Pataan1(); break;
	case 1: Pataan2(); break;
	case 2: Pataan3(); break;
	case 3: Pataan4(); break;
	case 4: Pataan5(); break;
	case 5: Pataan6(); break;
	case 6: Pataan7(); break;
	case 7: Pataan8(); break;
	case 8: Pataan9(); break;
	case 9: Pataan10(); break;
	case 10: Pataan11(); break;
	case 11: Pataan12(); break;
	case 12: Pataan13(); break;
	case 13: Pataan14(); break;
	case 14: Pataan15(); break;
	case 15: Pataan16(); break;
	case 16: Pataan17(); break;
	case 17: Pataan18(); break;
	case 18: Pataan19(); break;
	case 19: Pataan20(); break;
	}

	sN++;

	//printf("Integer: %d\n", sN);
}


// �Q�[���ăX�^�[�g(���Z�b�g)�p(Main����ɌĂ�(Init()��Relese()���Ă�)�ƃG���[�ɂȂ邽��)
void ResetGame()
{
	// �v���C���[�̍Đ���
	if (g_player)
	{
		g_player->Kill();
	}
	g_player = new Player(CVector3D(SCREEN_WIDTH * 0.0f, 0.0f, -80.0f));

	// �v���C���[�̉e�̍Đ���
	if (g_shadow1)
	{
		g_shadow1->Kill();
	}
	g_shadow1 = new Shadow1(0, CVector3D(0.0f, -300.0f, -140.0f));;

	// ��Q���̍폜
	// ��������Ă���S�Ă̏�Q�����폜
	for (auto& enemybase : g_enemybases) 
	{
		enemybase->Kill();  // �����̃L���E�S�����폜
	}
	g_enemybases.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g

	// �L���E�S��/���X�r�̉e�̍폜
	for (auto& shadow2 : g_shadow2s)
	{
		shadow2->Kill();
	}
	g_shadow2s.clear();

	// ��������ǉ�
	// �{�X�̍Đ���
	if (g_boss)
	{
		g_boss->Kill();
	}
	g_boss = new Boss(CVector3D(100.0f, 400.0f, -335.0f));
	//

	// �w�i�̍Đ���
	///*// ��������Ă���S�Ă̔w�i���폜
	for (auto& ground : g_grounds)
	{
		//delete ground;  // `delete` ���g���Ċm���Ƀ��������
		ground->Kill();  // �����̃L���E�S�����폜
	}
	g_grounds.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g
	testScrollX = 0.0f;  // �X�N���[���ʒu�����Z�b�g
	g_grounds.push_back(new Ground(0, CVector3D(0.0f, 0.0f, -320.0f)));
	g_grounds.push_back(new Ground(0, CVector3D(1920.0f, 0.0f, -320.0f)));
	g_grounds.push_back(new Ground(0, CVector3D(3840.0f, 0.0f, -320.0f)));

	for (auto& bgTree1 : g_bgTree1s)
	{
		//delete ground;  // `delete` ���g���Ċm���Ƀ��������
		bgTree1->Kill();  // �����̃L���E�S�����폜
	}
	g_bgTree1s.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g
	testScrollX = 0.0f;
	g_bgTree1 = new BGTree1(0, CVector3D(0.0f, 0.0f, -330.0f));                   // 3/18�ύX�i����j
	g_bgTree1 = new BGTree1(0, CVector3D(1920.0f, 0.0f, -330.0f));                // 3/18�ύX�i����j
	g_bgTree1 = new BGTree1(0, CVector3D(3840.0f, 0.0f, -330.0f));                // 3/18�ύX�i����j

	for (auto& bgTree2 : g_bgTree2s)
	{
		//delete ground;  // `delete` ���g���Ċm���Ƀ��������
		bgTree2->Kill();  // �����̃L���E�S�����폜
	}
	g_bgTree2s.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g
	testScrollX = 0.0f;
	g_bgTree2 = new BGTree2(0, CVector3D(0.0f, 0.0f, -340.0f));                   // 3/17�ύX�i����j
	g_bgTree2 = new BGTree2(0, CVector3D(1920.0f, 0.0f, -340.0f));                // 3/17�ύX�i����j
	g_bgTree2 = new BGTree2(0, CVector3D(3840.0f, 0.0f, -340.0f));                // 3/17�ύX�i����j

	for (auto& bgTree3 : g_bgTree3s)
	{
		//delete ground;  // `delete` ���g���Ċm���Ƀ��������
		bgTree3->Kill();  // �����̃L���E�S�����폜
	}
	g_bgTree3s.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g
	testScrollX = 0.0f;
	g_bgTree3 = new BGTree3(0, CVector3D(0.0f, 0.0f, -350.0f));                   // 3/17�ύX�i����j
	g_bgTree3 = new BGTree3(0, CVector3D(1920.0f, 0.0f, -350.0f));                // 3/17�ύX�i����j
	g_bgTree3 = new BGTree3(0, CVector3D(3840.0f, 0.0f, -350.0f));                // 3/17�ύX�i����j

	for (auto& bgFog : g_bgFogs)
	{
		//delete ground;  // `delete` ���g���Ċm���Ƀ��������
		bgFog->Kill();  // �����̃L���E�S�����폜
	}
	g_bgFogs.clear();  // �x�N�^�[���N���A���āA�|�C���^�����Z�b�g
	testScrollX = 0.0f;
	g_bgFog = new BGFog(0, CVector3D(0.0f, 0.0f, -360.0f));                   // 3/17�ύX�i����j
	g_bgFog = new BGFog(0, CVector3D(1920.0f, 0.0f, -360.0f));                // 3/17�ύX�i����j
	g_bgFog = new BGFog(0, CVector3D(3840.0f, 0.0f, -360.0f));                // 3/17�ύX�i����j
	//*/

	// UI�̍Đ���
	delete g_ui;
	g_ui = new UI();
	g_ui->InitHp("HPUI.png", 3, 100, 100);  // HP�A�C�R���摜���Z�b�g
	g_ui->InitMana("mana.png", 100, 100);  // �}�i�A�C�R���摜���Z�b�g
	g_ui->InitTimeBack("timeback.png", 700, 120); // �^�C���̌��̔摜���Z�b�g
	g_ui->InitBossHPBar("boss_hp_back.png", "boss_hp_front.png", 800, 50); // HP�o�[���Z�b�g
}

void MainLoop()
{
	//--------------------------------------------------------------
	//�Q�[�����̓����͂����ɏ���
	//�Q�[�����͂��̊֐�_��1�b�Ԃ�60��Ăяo���Ă���
	//--------------------------------------------------------------

	// �V�[���֌W
	switch (mState)
	{
	case EState::ESTART: // �Q�[���X�^�[�g��Ԃ�������A
		isGame = false; // �Q�[��(MainLoop())���~�߂�
		g_ui->Start("title.png", 1920, 1080); // �^�C�g����ʂ̕`��

		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��A
		{
			// BGM�֌W(�ǉ�)
			// �^�C�g��BGM���~
			SOUND("title_bgm")->Stop();
			// BGM�̃t���O��true��
			isBGM = true;
			//

			isStart = true; // �o�ߎ���(�^�C��)�𓮂���
			mState = EState::EPLAY; // �Q�[���v���C��Ԃɂ���
		}
		break;
	case EState::EPLAY: // �Q�[���v���C��Ԃ�������A
		isGame = true; // �Q�[��(MainLoop())�𓮂���

		// BGM�֌W(�ǉ�)
		// isBGM�̔���
		if (isBGM)
		{
			// �Q�[��BGM���Đ�
			SOUND("game_bgm")->Play(true);
			isBGM = false;
		}
		//
		break;
	case EState::ECLEAR: // �Q�[���N���A��Ԃ�������A
		isGame = false; // �Q�[��(MainLoop())���~�߂�

		g_ui->Clear("gameclear.png", 1920, 1080); // �N���A��ʂ̕`��

		// BGM�֌W(�ǉ�)
		// isBGM�̔���
		if (isBGM)
		{
			// �N���ABGM���Đ�
			SOUND("clear_bgm")->Play(false);
			isBGM = false;
		}
		//

		// �o�ߎ��Ԃ�ۑ��i���߂ăN���A��ԂɂȂ����Ƃ��̂݁j
		if (isClear)
		{
			// ���݂̎��Ԃ��擾
			auto now = std::chrono::steady_clock::now();

			// �Q�[���J�n���Ԃ���̌o�ߎ��Ԃ��v�Z�i�b�P�ʁj
			g_clearTime = std::chrono::duration<float>(now - g_startTime).count();
			isClear = false; // �Čv����h��
		}

		// UI�ɃN���A���Ԃ�\��
		g_ui->SetClearTime(g_clearTime);
		g_ui->Render1();

		/*// Enter�L�[�������ƃQ�[���I��
		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��
		{
			// **�N���[���A�b�v����**
			TaskManager::ClearInstance(); // �Q�[���̃C���X�^���X���폜

			// **�v���O���������S�ɏI��**
			PostQuitMessage(0); // Windows�A�v���̐������I�����@
			// exit(0); // ����� exit(0) �ł�OK
		}*/

		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��A
		{
			// **�Q�[���̏�����
			// �Q�[���̃C���X�^���X�폜
			//TaskManager::ClearInstance();

			// �V�����C���X�^���X�쐬
			//TaskManager::Instance();

			// �Q�[���̏�����
			ResetGame();

			// BGM�֌W(�ǉ�)
			// �N���ABGM���~
			SOUND("clear_bgm")->Stop();
			// BGM�̃t���O��true��
			isBGM = true;
			//

			// �Q�[���̏�Ԃ�������
			deathCount = 0.0f;
			isDeath = false;
			isStart = true;

			sN = 0;

			// �K�v�ȃ^�X�N���X�V
			//TaskManager::Instance()->Update();

			mState = EState::EPLAY; // �Q�[���X�^�[�g��Ԃɂ���
		}
		break;
	case EState::EOVER: // �Q�[���I�[�o�[��Ԃ�������A
		isGame = false; // �Q�[��(MainLoop())���~�߂�

		g_ui->Over("gameover.png", 1920, 1080); // �N���A��ʂ̕`��

		// BGM�֌W(�ǉ�)
		// isBGM�̔���
		if (isBGM)
		{
			// �I�[�o�[BGM���Đ�
			SOUND("over_bgm")->Play(false);
			isBGM = false;
		}
		//

		// Enter�L�[�������ƃQ�[���I��
		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��
		{
			// �Q�[���̏�����
			ResetGame();

			// BGM�֌W(�ǉ�)
			// �N���ABGM���~
			SOUND("over_bgm")->Stop();
			// BGM�̃t���O��true��
			isBGM = true;
			//

			// �Q�[���̏�Ԃ�������
			deathCount = 0.0f;
			isDeath = false;
			isStart = true;

			sN = 0;

			mState = EState::EPLAY; // �Q�[���X�^�[�g��Ԃɂ���

			/*
			// **�N���[���A�b�v����**
			TaskManager::ClearInstance(); // �Q�[���̃C���X�^���X���폜

			// **�v���O���������S�ɏI��**
			PostQuitMessage(0); // Windows�A�v���̐������I�����@
			// exit(0); // ����� exit(0) �ł�OK
			*/
		}

		break;
	}

	// ���S����(��)
	if (isDeath)
	{
		// BGM�֌W(�ǉ�)
			// �Q�[��BGM���~
		SOUND("game_bgm")->Stop();
		// BGM�̃t���O��true��
		isBGM = true;
		//
		//

		mState = EState::EOVER;
		isDeath = false;
	}

	// �N���A����(��)
	if (isFinish)
	{
		// BGM�֌W(�ǉ�)
		// �Q�[��BGM���~
		SOUND("game_bgm")->Stop();
		// BGM�̃t���O��true��
		isBGM = true;
		//
		//

		mState = EState::ECLEAR;
		isFinish = false;
	}

	// �Q�[����(EState::EPLAY)�Ȃ�A�ȉ������s
	if (isGame)
	{
		// �Q�[�����Ȃ�o�ߎ��Ԃ��ŏ���0�ɂ���
		if (isStart)
		{
			// �Q�[���J�n���Ԃ��L�^
			g_startTime = std::chrono::steady_clock::now();
			//
			isStart = false;
		}


		// ���݂̎��Ԃ��擾
		auto now1 = std::chrono::steady_clock::now();
		std::chrono::duration<float> elapsedTime1 = now1 - lastGenerationTime; // �o�ߎ��Ԃ��v�Z

		// ��莞�Ԃ��Ƃɏ�Q���𐶐�
		if (elapsedTime1.count() >= generationInterval)
		{
			// ��Q���𐶐�
			GenerateObject();

			// �Ō�̐������Ԃ��X�V
			lastGenerationTime = now1;
		}

		// UI�֌W
		// �o�ߎ��Ԃ��v�Z
		auto now = std::chrono::steady_clock::now();
		float elapsedTime = std::chrono::duration<float>(now - g_startTime).count();

		// UI �Ɍo�ߎ��Ԃ��Z�b�g
		g_ui->SetElapsedTime(elapsedTime);

		// �S�Ẵ^�X�N���X�V
		TaskManager::Instance()->Update();

		//�@�S�Ă̓����蔻��̃`�F�b�N
		TaskManager::Instance()->CheckCollisions();

		// �t�B�[���h�摜��`��
		//g_field->Draw();

		// �e�̕`��
		//g_player->RenderShadow();
		/* �ȉ��ɏC��
		g_slime->RenderShadow();
		g_mosubi->RenderShadow();*/
		
		// �e�͈�U�폜
		//for (auto& slime : g_slimes) {
		//	slime->RenderShadow();
		//}
		//for (auto& mosubi : g_mosubis) {
		//	mosubi->RenderShadow();
		//}
		//for (auto& enemybase : g_enemybases) {
				//enemybase->RenderShadow();
			//}

		// �����͌ォ��C��
		//g_potion->RenderShadow1();
		//g_mana->RenderShadow1();
		//

		// �S�Ẵ^�X�N��`��
		TaskManager::Instance()->Render();

		// UI�֌W
		// UI �̕`��
		g_ui->Render();

		// �{�XHP�o�[�̕\��
		g_ui->RenderBossHP();

		// HP�����L
		g_ui->SetHP(Player::Hp());

		// �{�X�p
		// HP�����L
		g_ui->SetBossHP(Boss::Hp());  // �{�X��HP��ݒ�

		// ���S����(��)
		if (Player::Hp() <= 0)
		{
			deathCount++;
			if (deathCount >= 110.0f)
			{
				isDeath = true;
				deathCount = 0.0f;
			}
		}

		// �N���A����(��)
		if (Boss::Hp() <= 0)
		{
			finishCount++;
			if (finishCount >= 60.0f)
			{
				isFinish = true;
				finishCount = 0.0f;

				isClear = true; // �Čv�Z��h��
			}
		}

		// �f�o�b�O�����̕`��
		DebugPrint::Render();
	}
}
void Init()
{
	CFPS::SetFPS(60);
	//�t���[�����䏉����
	CFPS::Init();
	//�{�^���̐ݒ�
	CInput::Init();
	CInput::SetButton(0, CInput::eButton1, 'Z');
	CInput::SetButton(0, CInput::eButton2, 'X');
	CInput::SetButton(0, CInput::eButton3, 'C');
	CInput::SetButton(0, CInput::eButton4, 'V');
	CInput::SetButton(0, CInput::eButton5, VK_SPACE);
	CInput::SetButton(0, CInput::eButton10, VK_RETURN);
	CInput::SetButton(0, CInput::eUp, VK_UP);
	CInput::SetButton(0, CInput::eDown, VK_DOWN);
	CInput::SetButton(0, CInput::eLeft, VK_LEFT);
	CInput::SetButton(0, CInput::eRight, VK_RIGHT);
	CInput::SetButton(0, CInput::eMouseL, VK_LBUTTON);
	CInput::SetButton(0, CInput::eMouseR, VK_RBUTTON);
	CInput::SetButton(0, CInput::eMouseC, VK_MBUTTON);
	//	CInput::SetMouseInside(true);
	//	CInput::ShowCursor(false);
	CInput::Update();
	CInput::Update();

	SetCurrentDirectory("data");
	CSound::GetInstance();

	//-----------------------------------------------------
	//�������̖��߂�����
	//�Q�[���N�����Ɉ�x�����Ă΂��
	//-----------------------------------------------------

	// �V�[���֌W(�ǉ�)
	// �ŏ��̓^�C�g����ʂ�\��
	mState = EState::ESTART;

	// �t�B�[���h�̉摜�𐶐�
	g_field = CImage::CreateImage("background.png");

	// �v���C���[/�{�X/��Q��/�w�i/UI�̏�����
	ResetGame();


	// �|�[�V�����ƃ}�i�̕\��
	//g_potion = new Potion(0,
		//CVector3D(SCREEN_WIDTH * 2.00f, 0.0f, -80.0f));
	//g_mana = new Mana(0,
		//CVector3D(SCREEN_WIDTH * 2.40f, 0.0f, -80.0f));

	// BGM�֌W(�ǉ�)
	// �eBGM�̓ǂݍ���
	// �^�C�g��BGM
	SOUND("title_bgm")->Load("title.wav", 1, false); // �ύX
	// �Q�[��BGM
	SOUND("game_bgm")->Load("game.wav", 1, false); // �ύX
	// �N���ABGM
	SOUND("clear_bgm")->Load("clear.wav", 1, false); // �ύX
	// �I�[�o�[BGM
	SOUND("over_bgm")->Load("over.wav", 1, false); // �ύX

	// ���ʂ̒����i0.0f �` 1.0f �͈̔͂Ŏw��j
	// �^�C�g��BGM
	SOUND("title_bgm")->Volume(0.01f);  // 01%�̉���
	// �Q�[��BGM
	SOUND("game_bgm")->Volume(0.01f);  // 01%�̉���
	// �N���ABGM
	SOUND("clear_bgm")->Volume(0.01f);  // 01%�̉���
	// �I�[�o�[BGM
	SOUND("over_bgm")->Volume(0.01f);  // 01%�̉���

	// �^�C�g��BGM�̍Đ�
	SOUND("title_bgm")->Play(true);
	//
}


void Release()
{
	DebugPrint::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
	TaskManager::ClearInstance();

	// UI�֌W(�ǉ�)
	// UI �����
	delete g_ui;
	g_ui = nullptr;
	//
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//��ʉ𑜓x�ϓ�
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//��ʉ𑜓x�Œ�
	//CCamera::GetCamera()->SetSize(SCREEN_WIDTH, SCREEN_HEIGHT);

	CCamera::GetCamera()->Viewport(0, 0, w, h);
	CCamera::GetCurrent()->Perspective(DtoR(60.0), (float)w / (float)h, 1.0, 1000.0);

	glfwGetWindowPos(window, &GL::window_x, &GL::window_y);
	GL::UpdateWindowRect(GL::window_x, GL::window_y, w, h);
	CInput::UpdateClipCursor(true);

}
static void WheelCallback(GLFWwindow* _window, double _offsetx, double _offsety)
{
	CInput::AddMouseWheel((int)_offsety);
}
static void PosCallback(GLFWwindow* _window, int x, int y)
{
	GL::window_x = x;
	GL::window_y = y;
	GL::UpdateWindosRect(x, y, GL::window_width, GL::window_height);
	CInput::UpdateClipCursor(true);
}
static void FocusCallback(GLFWwindow* _window, int f)
{
	CInput::UpdateClipCursor(f);
	GL::focus = f;
}

static void error_callback(int error, const char* description)
{
	printf("Error: %s\n", description);
}

//�t���X�N���[��?�E�C���h�E���[�h�̐؂�ւ�
//Alt+Enter�Ő؂�ւ���
void CheckFullScreen()
{
	static int key_enter = 0;
	int key_enter_buf = key_enter;
	if (key_enter_buf ^ (key_enter = glfwGetKey(GL::window, GLFW_KEY_ENTER)) && key_enter && (glfwGetKey(GL::window, GLFW_KEY_LEFT_ALT) || glfwGetKey(GL::window, GLFW_KEY_RIGHT_ALT)))
	{
		GL::ChangeFullScreen(!GL::full_screen);
	}
}

int __main(int* argcp, char** argv)
{
	// ���������[�N���o
	//	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF);
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


	//OpenGL4.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) return -1;
	//	glutInit(argcp, argv);

	GL::window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple", nullptr, nullptr);
	glfwGetWindowSize(GL::window, &GL::window_width, &GL::window_height);
	glfwGetWindowPos(GL::window, &GL::window_x, &GL::window_y);

	glfwSetFramebufferSizeCallback(GL::window, ResizeCallback);
	glfwSetScrollCallback(GL::window, WheelCallback);
	glfwSetWindowFocusCallback(GL::window, FocusCallback);
	glfwSetWindowPosCallback(GL::window, PosCallback);
	if (!GL::window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(GL::window);
	glfwSwapInterval(1);
	ResizeCallback(GL::window, SCREEN_WIDTH, SCREEN_HEIGHT);

	GLenum err = glewInit();
	if (err == GLEW_OK)
	{
		printf("%s\n", glewGetString(GLEW_VERSION));
	}
	else
	{
		printf("%s\n", glewGetErrorString(err));
		getchar();
		return -1;
	}
	HDC glDc = wglGetCurrentDC();
	GL::hWnd = WindowFromDC(glDc);
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glEnable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);//�u�����h�̗L����
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//�Œ�V�F�[�_�[�p
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	Init();
	while (!glfwWindowShouldClose(GL::window))
	{
		CheckFullScreen();


		CInput::Update();
		//�e�o�b�t�@�[���N���A
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		MainLoop();


		glfwSwapBuffers(GL::window);


		CFPS::Wait();

		char title[32];
		sprintf_s(title, "Lara, Guardian of the Forest");
		glfwSetWindowTitle(GL::window, title);

		glfwPollEvents();
		if (glfwGetKey(GL::window, GLFW_KEY_ESCAPE))
		{
			glfwSetWindowShouldClose(GL::window, GL_TRUE);
		}

	}

	glfwTerminate();

	Release();
	return 0;
}

INT WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR argv, INT argc)
{
	return __main(&__argc, __argv);
}

int main(int argc, char** argv)
{
	return __main(&argc, argv);
}
