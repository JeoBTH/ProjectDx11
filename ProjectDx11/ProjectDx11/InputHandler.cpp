#include "InputHandler.hpp"

InputHandler::InputHandler(string name)
{
    this->name = name;

    for (int i = 0; i < KEY_COUNT; ++i)
    {
        currentKeyState[i] = false;
        previousKeyState[i] = false;
    }

    for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
    {
        currentMouseState[i] = false;
        previousMouseState[i] = false;
    }
}

InputHandler::~InputHandler()
{

}

void InputHandler::update()
{
    for (int i = 0; i < KEY_COUNT; ++i)
    {
        previousKeyState[i] = currentKeyState[i];
    }

    for (int i = 0; i < MOUSE_BUTTON_COUNT; ++i)
    {
        previousMouseState[i] = currentMouseState[i];
    }

    updateMouseDelta();
}

bool InputHandler::IsKeyPressed(unsigned char key) const
{
    return currentKeyState[key] && !previousKeyState[key];
}

bool InputHandler::IsKeyReleased(unsigned char key) const
{
    return !currentKeyState[key] && previousKeyState[key];
}

bool InputHandler::IsKeyDown(unsigned char key) const
{
    return currentKeyState[key];
}

bool InputHandler::IsMouseButtonPressed(int button) const
{
    return currentMouseState[button] && !previousMouseState[button];
}

bool InputHandler::IsMouseButtonReleased(int button) const
{
    return !currentMouseState[button] && previousMouseState[button];
}

bool InputHandler::IsMouseButtonDown(int button) const
{
    return currentMouseState[button];
}

void InputHandler::SetKeyState(unsigned char key, bool isPressed)
{
    currentKeyState[key] = isPressed;
}

void InputHandler::SetMouseButtonState(int button, bool isPressed)
{
    currentMouseState[button] = isPressed;
}

void InputHandler::updateMouseDelta()
{
    POINT currentPos;
    GetCursorPos(&currentPos);
    ScreenToClient(GetForegroundWindow(), &currentPos);

    deltaX = static_cast<float>(currentPos.x - lastMousePos.x);
    deltaY = static_cast<float>(currentPos.y - lastMousePos.y);

    lastMousePos = currentPos;
}
