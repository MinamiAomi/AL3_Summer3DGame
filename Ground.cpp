#include "Ground.h"

#include "GameScene.h"

void Ground::Initialize() {
    modelInstance_.SetModel(gameScene.GetModel("Resource/ground/ground.obj"));
    modelInstance_.SetWorldMatrix(Matrix4x4::identity);
    modelInstance_.SetCastShadow(true);
    modelInstance_.SetReceiveShadow(true);

    sunLight_.direction = Vector3(0.0f, -1.0f, 0.0f).Normalized();
    sunLight_.color = Vector4::one;
    sunLight_.intensity = 1.0f;
    RenderManager::Get().SetSunLight(sunLight_);
}

void Ground::Draw() {
    RenderManager::Get().AddModelInstance(modelInstance_);
}
