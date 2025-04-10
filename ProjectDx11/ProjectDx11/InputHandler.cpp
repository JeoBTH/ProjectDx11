#include "InputHandler.hpp"

InputHandler::InputHandler(string name)
{
    this->name = name;
    // Initialize common keys, like WASD, to false (not pressed)
    currentKeyState['W'] = false;
    currentKeyState['A'] = false;
    currentKeyState['S'] = false;
    currentKeyState['D'] = false;

    // Initialize previous state in the same way if needed
    previousKeyState['W'] = false;
    previousKeyState['A'] = false;
    previousKeyState['S'] = false;
    previousKeyState['D'] = false;

    // Initialize mouse state
    currentMouseState[VK_LBUTTON] = false;
    currentMouseState[VK_RBUTTON] = false;
    previousMouseState[VK_LBUTTON] = false;
    previousMouseState[VK_RBUTTON] = false;
}

InputHandler::~InputHandler()
{

}

void InputHandler::update()
{
    previousKeyState = currentKeyState;
    previousMouseState = currentMouseState;
}

bool InputHandler::IsKeyPressed(unsigned char key) const
{
    auto currentIt = currentKeyState.find(key);
    auto previousIt = previousKeyState.find(key);

    if (currentIt == currentKeyState.end() || previousIt == previousKeyState.end()) 
    {
        return false; // Return false if the key is not found (it's not initialized)
    }

    return currentIt->second && !previousIt->second;
}

bool InputHandler::IsKeyReleased(unsigned char key) const
{
    auto currentIt = currentKeyState.find(key);
    auto previousIt = previousKeyState.find(key);

    if (currentIt == currentKeyState.end() || previousIt == previousKeyState.end()) {
        return false; // Return false if the key is not found (it's not initialized)
    }

    return !currentIt->second && previousIt->second;
}

bool InputHandler::IsKeyDown(unsigned char key) const
{
    auto currentIt = currentKeyState.find(key);

    if (currentIt == currentKeyState.end()) {
        return false; // Return false if the key is not found (it's not initialized)
    }

    return currentIt->second;
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
    if (currentKeyState.find(key) == currentKeyState.end()) 
    {
        currentKeyState[key] = false; // Initialize key to false if not found
    }
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
