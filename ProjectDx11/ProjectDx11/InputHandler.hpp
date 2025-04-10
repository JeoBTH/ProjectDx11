#pragma once
#include <unordered_map>
#include <string>
#include <Windows.h>


using namespace std;

class InputHandler
{
private:
    static const int KEY_COUNT = 256; // static const - a single constant value, shared across all instances of the class. It does not occupy memory per instance. Used to define the array size.
    bool currentKeyState[KEY_COUNT];
    bool previousKeyState[KEY_COUNT];

    static const int MOUSE_BUTTON_COUNT = 5;
    bool currentMouseState[MOUSE_BUTTON_COUNT];
    bool previousMouseState[MOUSE_BUTTON_COUNT];

    POINT lastMousePos = { 0, 0 };
    float deltaX = 0.0f;
    float deltaY = 0.0f;

public:
	InputHandler(string name);
	~InputHandler();
	void update();

    string name;

    bool IsKeyPressed(unsigned char key) const;
    bool IsKeyReleased(unsigned char key) const;
    bool IsKeyDown(unsigned char key) const;

    bool IsMouseButtonPressed(int button) const;
    bool IsMouseButtonReleased(int button) const;
    bool IsMouseButtonDown(int button) const;

    void SetKeyState(unsigned char key, bool isPressed);
    void SetMouseButtonState(int button, bool isPressed);

    void updateMouseDelta();
    float GetMouseDeltaX() const { return deltaX; }
    float GetMouseDeltaY() const { return deltaY; }
};
