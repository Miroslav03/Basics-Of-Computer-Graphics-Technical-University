#define UNICODE
#define _UNICODE

#include <windows.h>
#include <gdiplus.h>
#include <cmath>
#include <algorithm>  // For std::min

#pragma comment (lib, "Gdiplus.lib")

using namespace Gdiplus;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
void DrawCircleWithLines(HDC hdc, int centerX, int centerY, int radius);

// Rainbow colors
Color rainbowColors[7] = {
    Color(255, 0, 0),      // Red
    Color(255, 165, 0),    // Orange
    Color(255, 255, 0),    // Yellow
    Color(0, 255, 0),      // Green
    Color(0, 0, 255),      // Blue
    Color(75, 0, 130),     // Indigo
    Color(238, 130, 238)   // Violet
};

// Entry point
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow) {
    // Initialize GDI+
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    // Register the window class
    const wchar_t CLASS_NAME[] = L"RainbowWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    // Create the window
    HWND hwnd = CreateWindowEx(0, CLASS_NAME, L"Rainbow Lines in Circle", WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

    if (hwnd == NULL) {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Main message loop
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Shutdown GDI+
    GdiplusShutdown(gdiplusToken);
    return 0;
}

// Window procedure to handle messages
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Get the center of the window
            RECT rect;
            GetClientRect(hwnd, &rect);
            int centerX = (rect.right - rect.left) / 2;
            int centerY = (rect.bottom - rect.top) / 2;
            int radius = std::min(centerX, centerY) - 20;

            // Draw circle and lines
            DrawCircleWithLines(hdc, centerX, centerY, radius);

            EndPaint(hwnd, &ps);
            break;
        }
        case WM_LBUTTONDOWN: {
            // Force a repaint when the mouse is clicked
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_DESTROY: {
            PostQuitMessage(0);
            return 0;
        }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

// Function to draw the circle and 7 lines
void DrawCircleWithLines(HDC hdc, int centerX, int centerY, int radius) {
    Graphics graphics(hdc);

    // Draw the circle
    Pen circlePen(Color(0, 0, 0), 3);
    graphics.DrawEllipse(&circlePen, centerX - radius, centerY - radius, radius * 2, radius * 2);

    // Draw 7 lines inside the circle
    for (int i = 0; i < 7; i++) {
        double angle = (2 * M_PI / 7) * i;  // Angle for each line
        int x = centerX + static_cast<int>(radius * cos(angle));
        int y = centerY + static_cast<int>(radius * sin(angle));

        Pen linePen(rainbowColors[i], 3);
        graphics.DrawLine(&linePen, centerX, centerY, x, y);
    }
}
