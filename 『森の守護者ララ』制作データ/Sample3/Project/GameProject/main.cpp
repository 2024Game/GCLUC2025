#include "TaskManager.h"
#include "Player.h"
#include "Slime.h"

//UI関係(追加)
#include "UI.h"
#include <chrono>
//

//--------------------------------------------
//グローバル変数領域
//--------------------------------------------
Player* g_player = nullptr;	// プレイヤーのポインタ
Slime* g_slime = nullptr;	// スライムのポインタ

CImage* g_field = nullptr;	// フィールド画像のポインタ

// UI関係(追加)
UI* g_ui = nullptr;      // UI クラスのポインタ
std::chrono::steady_clock::time_point g_startTime; // ゲーム開始時間
//

void MainLoop()
{
	//--------------------------------------------------------------
	//ゲーム中の動きはここに書く
	//ゲーム中はこの関数_を1秒間に60回呼び出している
	//--------------------------------------------------------------

	// UI関係(追加)
	// 経過時間を計算
	auto now = std::chrono::steady_clock::now();
	float elapsedTime = std::chrono::duration<float>(now - g_startTime).count();

	// UI に経過時間をセット
	g_ui->SetElapsedTime(elapsedTime);
	//

	// 全てのタスクを更新
	TaskManager::Instance()->Update();

	// フィールド画像を描画
	g_field->Draw();
	
	// 全てのタスクを描画
	TaskManager::Instance()->Render();

	// UI関係(追加)
	// UI の描画
	g_ui->Render();
	//

	// デバッグ文字の描画
	DebugPrint::Render();
}
void Init()
{
	CFPS::SetFPS(60);
	//フレーム制御初期化
	CFPS::Init();
	//ボタンの設定
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
	//初期化の命令を書く
	//ゲーム起動時に一度だけ呼ばれる
	//-----------------------------------------------------

	// フィールドの画像を生成
	g_field = CImage::CreateImage("background.png");

	// プレイヤーを生成
	g_player = new Player(
		CVector3D(SCREEN_WIDTH * 0.5f, 0.0f, 0.0f));

	// スライムを生成
	g_slime = new Slime(0,
		CVector3D(SCREEN_WIDTH * 0.75f,0.0f, 0.0f));

	// UI関係(追加)
	// UI を生成
	g_ui = new UI();
	g_ui->InitHp("HPUI.png", 3, 100, 100); // HPアイコン画像をセット
	g_ui->InitMana("mana.png", 100, 100); // マナアイコン画像をセット

	// ゲーム開始時間を記録
	g_startTime = std::chrono::steady_clock::now();
	//
}


void Release()
{
	DebugPrint::ClearInstance();
	CLoadThread::ClearInstance();
	CSound::ClearInstance();
	CResourceManager::ClearInstance();
	TaskManager::ClearInstance();

	// UI関係(追加)
	// UI を解放
	delete g_ui;
	g_ui = nullptr;
	//
}

static void ResizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);

	//画面解像度変動
	CCamera::GetCamera()->SetSize((float)w, (float)h);
	//画面解像度固定
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

//フルスクリーン?ウインドウモードの切り替え
//Alt+Enterで切り替える
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
	// メモリリーク検出
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
	glEnable(GL_BLEND);//ブレンドの有効化
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//固定シェーダー用
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_ALPHA_TEST);

	Init();
	while (!glfwWindowShouldClose(GL::window))
	{
		CheckFullScreen();


		CInput::Update();
		//各バッファーをクリア
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