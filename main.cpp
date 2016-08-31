#include "animation.h"
#include "coord_axis.h"
#include "sphere.h"
#include "satellite.h"
#include "planet.h"
#include "background.h"
#include <CommCtrl.h>

#pragma comment(lib, "ComCtl32.lib")

TCHAR czClassName[] = L"myClass";
TCHAR czFormName[] = L"";
HWND hwndA;

int run_animation(void)
{
	anim* planets = new anim(); //Получение экземпляра объекта класса anim
	anim** ref = planets->get_ref();
	*ref = planets;

	*planets << new planet( //Добавление планеты, аргументами принимает радиус, массу и координаты
		20, 
		6e+23, 
		vec{0, 0, 0});	
	*planets << new satellite( //Добавление спутника, аргументами принимает радиус, начальное положение, скорость и массу
		1.5, 
		vec{0, 2e7, 0},
		vec{15e3, 5900, 0},
		100); 
//	*planets << new axis(); //Добавление осей координат
	*planets << new background();
	Sleep(2000); //Ожидание освобождения процессора для плавного запуска
	planets->run(); //Запуск анимации

	return 0;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		switch ((LOWORD(wParam)))
		{
		case 1001:
			run_animation();
			break;
		case 1002:
			run_animation();
			break;
		case 1003:
			run_animation();
			break;
		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
		}
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
}

ATOM myRegistryClass(HINSTANCE hInst, int cmdMode)
{
	WNDCLASS cw;

	cw.style = CS_VREDRAW | CS_HREDRAW;
	cw.cbClsExtra = 0;
	cw.cbWndExtra = 0;
	cw.hbrBackground = (HBRUSH)(COLOR_WINDOW + 2);
	cw.hCursor = LoadCursor(NULL, IDC_ARROW);
	cw.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	cw.hInstance = hInst;
	cw.lpfnWndProc = WndProc;
	cw.lpszClassName = czClassName;
	cw.lpszMenuName = NULL;

	RegisterClass(&cw);

	return 0;
}


BOOL InitInstance(HINSTANCE hInst, int Mode)
{
	hwndA = CreateWindow(
		czClassName,
		czFormName,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInst,
		NULL);

	ShowWindow(hwndA, Mode);
	UpdateWindow(hwndA);

	return TRUE;
}


/*Главная функция*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR IpszCmdParam, int cmdShow)
{
	MSG msg;

	myRegistryClass(hInst, cmdShow);
	InitInstance(hInst, cmdShow);

	InitCommonControls();

	/*Создание строки состояния*/
	HWND hStatusWindow = CreateStatusWindow(WS_CHILD | WS_VISIBLE, L"", hwndA, 5000);

	HMENU hMainMenu = CreateMenu();
	HMENU hPopMenuFile = CreatePopupMenu();

	AppendMenu(hMainMenu, MF_STRING | MF_POPUP, (UINT)hPopMenuFile, L"Законы");

	AppendMenu(hPopMenuFile, MF_STRING, 1001, L"Первый закон Кеплера");
	AppendMenu(hPopMenuFile, MF_STRING, 1002, L"Второй закон Кеплера");
	AppendMenu(hPopMenuFile, MF_STRING, 1003, L"Третий закон Кеплера");

	SetMenu(hwndA, hMainMenu);
	SetMenu(hwndA, hPopMenuFile);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}