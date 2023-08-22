#pragma once

#include "Engine/RenderManager.h"
#include "Math/Camera.h"
#include "Math/LightStructs.h"

class GameScene {
public:
    GameScene();
    void Initialize();
    void Update();
    void Draw();

private:
    RenderManager& renderManager_;


};