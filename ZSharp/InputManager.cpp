#include "InputManager.h"

namespace ZSharp {
InputManager::InputManager()
  : mListenerList() {
  mKeyboard.fill(KeyState::Clear);
}

InputManager* InputManager::GetInstance() {
  static InputManager inputManager;
  return &inputManager;
}

void InputManager::Update(std::uint8_t key, InputManager::KeyState state) {
  mKeyboard[key] = state;
}

void InputManager::Process() {
  for (uint8_t i = 0; i < mKeyboard.size(); ++i) {
    switch (mKeyboard[i]) {
      case KeyState::Clear:
        break;
      case KeyState::Down:
        for (IInputListener* listener : mListenerList) {
          listener->OnKeyDown(i);
        }
        break;
      case KeyState::Up:
        for (IInputListener* listener : mListenerList) {
          listener->OnKeyUp(i);
        }
        break;
    }
  }

  mKeyboard.fill(KeyState::Clear);
}

void InputManager::Register(IInputListener* inputListener) {
  mListenerList.insert(inputListener);
}

void InputManager::Unregister(IInputListener* inputListener) {
  mListenerList.erase(inputListener);
}
}
