#include "InputHandler.hpp"

InputHandler::InputHandler()
{

}

InputHandler::~InputHandler()
{

}

void InputHandler::update()
{

    // Update keyboard input (this should be handled by Windows messages in Window.cpp)
    // Here we're assuming keyStates are updated externally via WM_KEYDOWN, WM_KEYUP

    previousKeyState = currentKeyState;
    previousMouseState = currentMouseState;

}

bool InputHandler::IsKeyPressed(unsigned char key) const
{
    return currentKeyState.at(key) && !previousKeyState.at(key);
}

bool InputHandler::IsKeyReleased(unsigned char key) const
{
    return !currentKeyState.at(key) && previousKeyState.at(key);
}

bool InputHandler::IsKeyDown(unsigned char key) const
{
    return currentKeyState.at(key);
}

bool InputHandler::IsMouseButtonPressed(int button) const
{
    return currentMouseState.at(button) && !previousMouseState.at(button);
}

bool InputHandler::IsMouseButtonReleased(int button) const
{
    return !currentMouseState.at(button) && previousMouseState.at(button);
}

bool InputHandler::IsMouseButtonDown(int button) const
{
    return currentMouseState.at(button);
}

void InputHandler::SetKeyState(unsigned char key, bool isPressed)
{
    currentKeyState[key] = isPressed;
}

void InputHandler::SetMouseButtonState(int button, bool isPressed)
{
    currentMouseState[button] = isPressed;
}

