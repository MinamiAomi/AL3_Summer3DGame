#include "SystemManager.h"

#include "Core/TextureManager.h"
#include "Engine/RenderManager.h"
#include "Engine/Input.h"
#include "GameScene.h"

void SystemManager::Initialize() {
    titleSprite_.SetTexture(TextureManager::Load("Resource/title.png"));
    titleSprite_.SetTranslate({ 640.0f, 300.0f });
    titleSprite_.SetAnchor({ 0.5f,0.5f });
    titleSprite_.SetSize({ 640.0f,180.0f });
    titleSprite_.SetTexcoordSize({ 640.0f,180.0f });
    pushSprite_.SetTranslate({ 640.0f, 500.0f });
    pushSprite_.SetTexture(TextureManager::Load("Resource/push.png"));
    pushSprite_.SetAnchor({ 0.5f,0.5f });
    pushSprite_.SetSize({ 272.0f,64.0f });
    pushSprite_.SetTexcoordSize({ 272.0f,64.0f });
    clearSprite_.SetTranslate({ 640.0f, 300.0f });
    clearSprite_.SetTexture(TextureManager::Load("Resource/clear.png"));
    clearSprite_.SetAnchor({ 0.5f,0.5f });
    clearSprite_.SetSize({ 640.0f,180.0f });
    clearSprite_.SetTexcoordSize({ 640.0f,180.0f });
}

void SystemManager::Update() {
    auto& input = Input::Get();
    auto& gamepad = input.GetXInputState();
    auto& preGamepad = input.GetPreXInputState();

    switch (state_)
    {
    case SystemManager::kTitle:
        if (gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preGamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
            state_ = kInGame;
        }
        return;
    case SystemManager::kInGame:

        return;
    case SystemManager::kClear:
        if (gamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A && !(preGamepad.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
            gameScene.ReloadScene();
            return;
        }
        return;
    }
}

void SystemManager::Draw() {
    auto& renderManager = RenderManager::Get();
    switch (state_)
    {
    case SystemManager::kTitle:
        renderManager.AddSprite(titleSprite_);
        renderManager.AddSprite(pushSprite_);
        break;
    case SystemManager::kClear:
        renderManager.AddSprite(clearSprite_);
        break;
    }
}
