
#include <Windows.h>

#include "Core/Window.h"
#include "Core/Graphics.h"
#include "Core/ImGuiManager.h"
#include "Core/ShaderCompiler.h"

#include "Engine/RenderManager.h"

#include "GameScene.h"

int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

    auto& window = Window::Get();
    window.Create("Title", 1280, 720);
    auto& graphics = Graphics::Get();
    graphics.Create();
    auto& imguiManager = ImGuiManager::Get();
    imguiManager.Create();
    ShaderCompiler::Get().Create();
    auto& renderManager = RenderManager::Get();
    renderManager.Create();

    GameScene gameScene;
    gameScene.Initialize();

    while (window.ProcessMessage()) {
        imguiManager.NewFrame();

        gameScene.Update();

        gameScene.Draw();

        renderManager.Render();
    }

    imguiManager.Destroy();
    window.Destroy();

    return 0;
}