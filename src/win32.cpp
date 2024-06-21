#include <windows.h>
#include <winuser.h>

#include <cstdarg>
#include <cstdio>

// TODO: move from global later
static bool running;

// Function for formatted output for the OutputDebugStringA windows function
void OutputDebugFormatA(const char *format, ...) {
    va_list argsPtr;
    va_start(argsPtr, format);
    char msg[255] = {0};
    vsprintf_s(msg, format, argsPtr);
    va_end(argsPtr);
    OutputDebugStringA(msg);
}

static void win32ResizeDIBSection(int width, int height) {
    BITMAPINFO;
    void *
    HBITMAP bitmapHandle = CreateDIBSection(
        deviceContext, const BITMAPINFO *pbmi, DIB_RGB_COLORS, void **ppvBits,
        HANDLE hSection, DWORD offset
    );
}

static void win32UpdateWindow(
    HDC deviceContext, int x, int y, int width, int height
) {
    StretchDIBits(
        deviceContext, x, y, width, height, x, y, width, height,
        const void *lpBits, const BITMAPINFO *lpbmi, DIB_RGB_COLORS, SRCCOPY
    )
}

LRESULT CALLBACK
mainWindowCallback(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (message) {
        case WM_SIZE: {
            RECT clientRect;
            GetClientRect(window, &clientRect);

            int width = clientRect.right - clientRect.left;
            int height = clientRect.bottom - clientRect.top;
            win32ResizeDIBSection(width, height);
        } break;

        case WM_DESTROY: {
            // TODO: Handle this as an error - recreate window?
            running = false;
        } break;

        case WM_CLOSE: {
            // TODO: Handle with message to the user
            running = false;
        } break;

        case WM_ACTIVATEAPP: {
            OutputDebugStringA("WM_ACTIVATEAPP\n");
        } break;

        case WM_PAINT: {
            PAINTSTRUCT paint;
            HDC deviceContext = BeginPaint(window, &paint);
            int x = paint.rcPaint.top;
            int y = paint.rcPaint.left;
            int width = paint.rcPaint.right - paint.rcPaint.left;
            int height = paint.rcPaint.bottom - paint.rcPaint.top;
            win32UpdateWindow(window, x, y, width, height);
            PatBlt(deviceContext, x, y, width, height, WHITENESS);
            EndPaint(window, &paint);

        } break;

        default: {
            // OutputDebugStringA("default\n");
            result = DefWindowProcA(window, message, wParam, lParam);
        }
    }

    return result;
}

int CALLBACK WinMain(
    HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd
) {
    WNDCLASSA windowCls = {};
    // Bit field for propreties we want to have. Mabye remove HREDRAW, VREDRAW,
    // and OWNDC matter.
    windowCls.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    // Pointer to user defined function that defines how the window responds to
    // events.
    windowCls.lpfnWndProc = mainWindowCallback;
    // Instance of our program that gets passed to the window.
    windowCls.hInstance = hInstance;
    // windowcls.hIcon = ;
    // Name for window class. We will pass this name when creating a window to
    // create a window using this window class.
    windowCls.lpszClassName = "HandmadeHeroWindowClass";

    if (RegisterClassA(&windowCls)) {
        HWND windowHandle = CreateWindowEx(
            0, windowCls.lpszClassName, "HandmadeHero",
            WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,
            CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0
        );

        if (windowHandle) {
            MSG message;
            BOOL messageResult = GetMessage(&message, 0, 0, 0);

            running = true;
            while (running) {
                TranslateMessage(&message);
                DispatchMessageA(&message);
                messageResult = GetMessage(&message, 0, 0, 0);
            }
        } else {
            OutputDebugFormatA("Error handling to be implemented 1");
        }
    } else {
        OutputDebugFormatA("Error handling to be implemented 2");
    }

    return (0);
}
