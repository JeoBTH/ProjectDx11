#pragma once
#include <unordered_map>
#include <string>

using namespace std;

class InputHandler
{
private:
    std::unordered_map<unsigned char, bool> currentKeyState;
    std::unordered_map<unsigned char, bool> previousKeyState;

    std::unordered_map<int, bool> currentMouseState;
    std::unordered_map<int, bool> previousMouseState;


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
};
