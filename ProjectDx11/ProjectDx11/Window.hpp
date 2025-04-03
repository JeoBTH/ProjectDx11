#pragma once
#include <Windows.h>
#include "InputHandler.hpp"

class Window
{
private:
	int width = 0;
	int height = 0;
	HWND m_handle;
	InputHandler* inputHandler = nullptr; // Pointer to InputHandler from Game

public:
	Window(int width = 800, int height = 600);
	~Window();

	HWND getHandle();
	bool processMessages();

	InputHandler* getInputHandler() const { return inputHandler; } // Add this getter
	void setInputHandler(InputHandler* handler) { inputHandler = handler; }

	static LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam);

};
