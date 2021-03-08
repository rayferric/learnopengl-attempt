#ifndef EYESPARK_WINDOW_H
#define EYESPARK_WINDOW_H

#include "../node/camera/Camera.h"
#include "../renderer/Renderer.h"
#include "../util/Error.h"
#include "../util/String.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002

typedef HGLRC(APIENTRY *PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int *);

class Window {
private:
    static HINSTANCE hInstance;
    static HWND hWnd;
    static HDC hDc;
    static HGLRC hRc;
    static HICON hIcon;
    static LONG style;
    static LONG styleEx;
    static WINDOWPLACEMENT placement;
    static bool open;
    static bool focus;
    static bool fullscreen;
    static bool resizeRequested;

    static LRESULT CALLBACK wndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
        switch(msg) {
        case WM_CREATE:
            open = true;
            break;
        case WM_DESTROY:
            open = false;
            break;
        case WM_SETFOCUS:
			focus = true;
			break;
		case WM_KILLFOCUS:
			focus = false;
			break;
        case WM_SIZE:
            resizeRequested = true;
            break;
        default:
            return DefWindowProc(hWnd, msg, wParam, lParam);
        }
        return 0;
    }
public:
    static Renderer *renderer;
    static Camera *camera;

    static void process() {
        MSG msg;
        while(PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if(resizeRequested) {
            resizeRequested = false;
            if(renderer == nullptr || camera == nullptr)return;
            int w, h;
            getClientSize(&w, &h);
            renderer->resize(w, h);
            camera->aspect = (float)w/h;
            camera->updateProjection();
        }
    }
    static void init(String title, int width, int height) {
        //Register window class
        hInstance = GetModuleHandle(nullptr);
        WNDCLASSEX wndClassEx;
        ZeroMemory(&wndClassEx, sizeof(WNDCLASSEX));
        wndClassEx.cbSize = sizeof(WNDCLASSEX);
        wndClassEx.style = CS_OWNDC;
        wndClassEx.lpfnWndProc = wndProc;
        wndClassEx.hInstance = hInstance;
        wndClassEx.hCursor = LoadCursor(nullptr, IDC_ARROW);
        wndClassEx.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
        wndClassEx.lpszClassName = "EYESPARK_WINDOW_WNDCLASS";
        if(!RegisterClassEx(&wndClassEx))Error::error(Error::WINDOW_CREATION_ERROR, 0, nullptr);
        //Convert client size to window size
        RECT rect;
        ZeroMemory(&rect, sizeof(RECT));
        rect.right = width;
        rect.bottom = height;
        AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
        width = rect.right - rect.left;
        height = rect.bottom - rect.top;
        //Create the window
        hWnd = CreateWindow(wndClassEx.lpszClassName,
                title.data(),
                WS_OVERLAPPEDWINDOW,
                (GetSystemMetrics(SM_CXSCREEN) - width) / 2,
				(GetSystemMetrics(SM_CYSCREEN) - height) / 2,
                width, height,
                nullptr, nullptr,
                hInstance, nullptr);
        if(hWnd == nullptr)Error::error(Error::WINDOW_CREATION_ERROR, 0, nullptr);
        //Create device context
        PIXELFORMATDESCRIPTOR pfd;
        ZeroMemory(&pfd, sizeof(PIXELFORMATDESCRIPTOR));
        pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
        pfd.nVersion = 1;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 24;
        int attribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
                0
		};

        hDc = GetDC(hWnd);
        int pixelFormat = ChoosePixelFormat(hDc, &pfd);
        if(!pixelFormat)Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);
        if(!SetPixelFormat(hDc, pixelFormat, &pfd))Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);
        hRc = wglCreateContext(hDc);
        if(!hRc)Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);
        wglMakeCurrent(hDc, hRc);

		PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
		if(wglCreateContextAttribsARB == nullptr) {
            wglMakeCurrent(hDc, nullptr);
            wglDeleteContext(hRc);
            Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);
		}
		HGLRC newHRc = wglCreateContextAttribsARB(hDc, 0, attribs);
		if(!newHRc) {
		    wglMakeCurrent(hDc, nullptr);
            wglDeleteContext(hRc);
            Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);
		}
		wglMakeCurrent(hDc, nullptr);
		wglDeleteContext(hRc);
		hRc = newHRc;
		if(!wglMakeCurrent(hDc, hRc))Error::error(Error::CONTEXT_CREATION_ERROR, 0, nullptr);

        style = GetWindowLong(hWnd, GWL_STYLE);
        styleEx = GetWindowLong(hWnd, GWL_EXSTYLE);
        GetWindowPlacement(hWnd, &placement);

        ShowWindow(hWnd, SW_SHOW);

        if(!gladLoadGL())Error::error(Error::EXTENSIONS_LOADING_ERROR, 0, nullptr);
    }
    static bool isOpen() {
        return open;
    }
    static void terminate() {
        wglDeleteContext(hRc);
        DestroyWindow(hWnd);
        UnregisterClass("EYESPARK_WINDOW_WNDCLASS", hInstance);
    }
    static void swapBuffers() {
        SwapBuffers(hDc);
    }
    static void getClientSize(int *width, int *height) {
        RECT rect;
        GetClientRect(hWnd, &rect);
        *width = rect.right;
        *height = rect.bottom;
    }
    static void setTitle(String title) {
        SetWindowText(hWnd, title.data());
    }
    static void setIcon(String name) {
        if(hIcon != nullptr)DestroyIcon(hIcon);
        hIcon = LoadIcon(hInstance, name.data());
        SendMessage(hWnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
        SendMessage(hWnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    }
    static void setFullscreen(bool fullscreen) {
        if(Window::fullscreen == fullscreen)return;
        Window::fullscreen = fullscreen;
        if(fullscreen) {
            GetWindowPlacement(hWnd, &placement);
            SetWindowLong(hWnd, GWL_STYLE, (style & ~WS_BORDER & ~WS_DLGFRAME & ~WS_THICKFRAME) | WS_POPUP);
            SetWindowLong(hWnd, GWL_EXSTYLE, (styleEx & ~WS_EX_WINDOWEDGE) | WS_EX_TOPMOST);
            ShowWindow(hWnd, SW_SHOWMAXIMIZED);
        } else {
            SetWindowLong(hWnd, GWL_STYLE, style);
            SetWindowLong(hWnd, GWL_EXSTYLE, styleEx);
            ShowWindow(hWnd, SW_SHOW);
            SetWindowPlacement(hWnd, &placement);
        }
    }
    static bool isFullscreen() {
        return fullscreen;
    }
    static bool hasFocus() {
        return focus;
    }
    static void getCursorPos(int *x, int *y) {
        POINT point;
        GetCursorPos(&point);
        ScreenToClient(hWnd, &point);
        *x = point.x;
        *y = point.y;
    }
    static void setCursorPos(int x, int y) {
        POINT point;
        point.x = x;
        point.y = y;
        ClientToScreen(hWnd, &point);
        SetCursorPos(point.x, point.y);
    }
    static void setCursorVisible(bool visible) {
        ShowCursor(visible);
    }
};

HINSTANCE Window::hInstance;
HWND Window::hWnd;
HDC Window::hDc;
HGLRC Window::hRc;
HICON Window::hIcon = nullptr;
LONG Window::style;
LONG Window::styleEx;
WINDOWPLACEMENT Window::placement;
bool Window::open = false;
bool Window::focus = true;
bool Window::fullscreen = false;
bool Window::resizeRequested = true;
Renderer *Window::renderer = nullptr;
Camera *Window::camera = nullptr;

#endif // EYESPARK_WINDOW_H
