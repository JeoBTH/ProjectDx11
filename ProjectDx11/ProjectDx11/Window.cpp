#include "Window.hpp"

LRESULT CALLBACK Window::WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam)
{
	InputHandler* inputHandler = reinterpret_cast<InputHandler*>(GetWindowLongPtr(handle, GWLP_USERDATA));

	switch (msg)
	{
	case WM_KEYDOWN:
		inputHandler->SetKeyState(static_cast<unsigned char>(wparam), true);
		break;
	case WM_KEYUP:
		inputHandler->SetKeyState(static_cast<unsigned char>(wparam), false);
		break;
	case WM_LBUTTONDOWN:
		inputHandler->SetMouseButtonState(VK_LBUTTON, true);
		break;
	case WM_LBUTTONUP:
		inputHandler->SetMouseButtonState(VK_LBUTTON, false);
		break;
	case WM_RBUTTONDOWN:
		inputHandler->SetMouseButtonState(VK_RBUTTON, true);
		break;
	case WM_RBUTTONUP:
		inputHandler->SetMouseButtonState(VK_RBUTTON, false);
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, msg, wparam, lparam);
}

Window::Window(int width, int height)
{
	// Define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = "ProjectDx11";
	RegisterClass(&wc);


	//Create Window
	m_handle = CreateWindow("ProjectDx11", "ProjectDx11", WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE, 100, 100, width, height, nullptr, nullptr, nullptr, nullptr);
}

Window::~Window()
{
}

HWND Window::getHandle()
{
	return m_handle;
}

bool Window::processMessages()
{
	MSG msg = { 0 };


	while (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
		{
			return false;
		}
	}

	return true;
}

void Window::setInputHandler(InputHandler* handler)
{
	inputHandler = handler;
	SetWindowLongPtr(m_handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(inputHandler)); // Store it in the window
}
