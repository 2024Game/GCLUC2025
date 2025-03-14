#include "TaskManager.h"
#include "Player.h"
#include "Slime.h"
#include "Mosubi.h"
#include "Rock.h"

//UI�֌W
#include "UI.h"
#include <chrono>
//

//--------------------------------------------
//�O���[�o���ϐ��̈�
//--------------------------------------------
Player* g_player = nullptr;	// �v���C���[�̃|�C���^
Slime* g_slime = nullptr;	// �L���[�S���̃|�C���^
Mosubi* g_mosubi = nullptr; // ���X�r�̃|�C���^
Rock* g_rock = nullptr;     // ��̃|�C���^

CImage* g_field = nullptr;	// �t�B�[���h�摜�̃|�C���^

// UI�֌W
UI* g_ui = nullptr;      // UI �N���X�̃|�C���^
std::chrono::steady_clock::time_point g_startTime; // �Q�[���J�n����
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


// ��Q���̃��X�g
std::vector<Rock*> g_rocks;
std::vector<Slime*> g_slimes;
std::vector<Mosubi*> g_mosubis;


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
			isStart = true; // �o�ߎ���(�^�C��)�𓮂���
			mState = EState::EPLAY; // �Q�[���v���C��Ԃɂ���
		}
		break;
	case EState::EPLAY: // �Q�[���v���C��Ԃ�������A
		isGame = true; // �Q�[��(MainLoop())�𓮂���
		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��A
		{
			isStart = false; // �o�ߎ���(�^�C��)���~�߂�
			mState = EState::ECLEAR; // �Q�[���X�^�[�g��Ԃɂ���
		}
		break;
	case EState::ECLEAR: // �Q�[���N���A��Ԃ�������A
		isGame = false; // �Q�[��(MainLoop())���~�߂�
		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��A
		{

			// �Q�[���̃C���X�^���X�폜(�ǉ�)
			TaskManager::ClearInstance(); // �����̃C���X�^���X���폜

			// �Q�[���̃C���X�^���X�쐬(�ǉ�)
			TaskManager::Instance(); // �V�����C���X�^���X���쐬

			// **�Q�[���̏�����**
			// �����Ńv���C���[��G�L�������č쐬(Init()���ĂԂƃG���[�ɂȂ�)
			// �t�B�[���h�̉摜�𐶐�
			g_field = CImage::CreateImage("background.png");

			// �v���C���[�𐶐�
			g_player = new Player(
				CVector3D(SCREEN_WIDTH * 0.0f, 0.0f, 0.0f));

			// **��Q�����ꊇ����**
			struct ObstacleData {
				float x, y, z;
			};

			// **�X���C���̔z�u**
			std::vector<ObstacleData> slimePositions = {
				{SCREEN_WIDTH * 0.40f, 0.0f, 0.0f},
				{SCREEN_WIDTH * 0.60f, 50.0f, 0.0f}
			};
			for (auto& pos : slimePositions) {
				g_slimes.push_back(new Slime(0, CVector3D(pos.x, pos.y, pos.z)));
			}

			// **���X�r�̔z�u**
			std::vector<ObstacleData> mosubiPositions = {
				{SCREEN_WIDTH * 0.80f, 160.0f, 0.0f},
				{SCREEN_WIDTH * 0.80f, 400.0f, 0.0f}
			};
			for (auto& pos : mosubiPositions) {
				g_mosubis.push_back(new Mosubi(0, CVector3D(pos.x, pos.y, pos.z)));
			}

			// **��̔z�u**
			std::vector<ObstacleData> rockPositions = {
				{SCREEN_WIDTH * 0.40f, 100.0f, 150.0f},
				{SCREEN_WIDTH * 0.40f, 100.0f, -150.0f}
			};
			for (auto& pos : rockPositions) {
				g_rocks.push_back(new Rock(0, CVector3D(pos.x, pos.y, pos.z)));
			}

			// UI�֌W
			// UI �𐶐�
			g_ui = new UI();
			g_ui->InitHp("HPUI.png", Player::Hp(), 100, 100); // HP�A�C�R���摜���Z�b�g
			g_ui->InitMana("mana.png", 100, 100); // �}�i�A�C�R���摜���Z�b�g

			// �S�Ẵ^�X�N���X�V
			TaskManager::Instance()->Update();


			//isStart = false; // �o�ߎ���(�^�C��)���~�߂�
			mState = EState::ESTART; // �Q�[���X�^�[�g��Ԃɂ���
		}
		break;
	case EState::EOVER: // �Q�[���I�[�o�[��Ԃ�������A
		isGame = false; // �Q�[��(MainLoop())���~�߂�
		if (PUSH(CInput::eButton10)) // Enter�L�[�������ꂽ��A
		{
			isStart = false; // �o�ߎ���(�^�C��)���~�߂�
			mState = EState::ESTART; // �Q�[���X�^�[�g��Ԃɂ���
		}
		break;
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
		g_field->Draw();

		// �e�̕`��
		g_player->RenderShadow();
		g_slime->RenderShadow();
		g_mosubi->RenderShadow();
		g_rock->RenderShadow();

		// �S�Ẵ^�X�N��`��
		TaskManager::Instance()->Render();

		// UI�֌W
		// UI �̕`��
		g_ui->Render();

		// HP�����L
		g_ui->SetHP(Player::Hp());

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

	// �v���C���[�𐶐�
	g_player = new Player(
		CVector3D(SCREEN_WIDTH * 0.0f, 0.0f, 0.0f));

	///*
	// ��Q���𐶐�
	// �L���[�S��
	g_slime = new Slime(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 0.0f, 0.0f));

	// ���X�r
	g_mosubi = new Mosubi(0,
		CVector3D(SCREEN_WIDTH * 0.80f, 160.0f, 0.0f));
	//g_mosubi = new Mosubi(0,
		//CVector3D(SCREEN_WIDTH * 0.80f, 400.0f, 0.0f));

	// ��
	g_rock = new Rock(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 100.0f, 150.0f));
	g_rock = new Rock(0,
		CVector3D(SCREEN_WIDTH * 0.40f, 100.0f, -150.0f));//*/

	/*// **��Q�����ꊇ����**
	struct ObstacleData {
		float x, y, z;
	};

	// **�X���C���̔z�u**
	std::vector<ObstacleData> slimePositions = {
		{SCREEN_WIDTH * 0.40f, 0.0f, 0.0f},
		{SCREEN_WIDTH * 0.60f, 50.0f, 0.0f}
	};
	for (auto& pos : slimePositions) {
		g_slimes.push_back(new Slime(0, CVector3D(pos.x, pos.y, pos.z)));
	}

	// **���X�r�̔z�u**
	std::vector<ObstacleData> mosubiPositions = {
		{SCREEN_WIDTH * 0.80f, 160.0f, 0.0f},
		{SCREEN_WIDTH * 0.80f, 400.0f, 0.0f}
	};
	for (auto& pos : mosubiPositions) {
		g_mosubis.push_back(new Mosubi(0, CVector3D(pos.x, pos.y, pos.z)));
	}

	// **��̔z�u**
	std::vector<ObstacleData> rockPositions = {
		{SCREEN_WIDTH * 0.40f, 100.0f, 150.0f},
		{SCREEN_WIDTH * 0.40f, 100.0f, -150.0f}
	};
	for (auto& pos : rockPositions) {
		g_rocks.push_back(new Rock(0, CVector3D(pos.x, pos.y, pos.z)));
	}*/

	// UI�֌W
	// UI �𐶐�
	g_ui = new UI();
	g_ui->InitHp("HPUI.png", 3, 100, 100); // HP�A�C�R���摜���Z�b�g
	g_ui->InitMana("mana.png", 100, 100); // �}�i�A�C�R���摜���Z�b�g
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
		sprintf_s(title, "Sample3");
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
