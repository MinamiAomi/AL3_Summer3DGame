#include "Ground.h"

#include "GameScene.h"

void Ground::Initialize() {
    modelInstance_.SetModel(gameScene.GetModel("Resource/ground/ground.obj"));
    modelInstance_.SetWorldMatrix(Matrix4x4::identity);
    modelInstance_.SetCastShadow(true);
    modelInstance_.SetReceiveShadow(true);
    skydomeInstance_.SetModel(gameScene.GetModel("Resource/skydome/skydome.obj"));
    skydomeInstance_.SetWorldMatrix(Matrix4x4::MakeTranslation({0.0f, -50.0f, 0.0f}));
    skydomeInstance_.SetCastShadow(false);
    skydomeInstance_.SetReceiveShadow(false);

    sunLight_.direction = Vector3(0.0f, -1.0f, 0.0f).Normalized();
    sunLight_.color = Vector4::one;
    sunLight_.intensity = 1.0f;
    RenderManager::Get().SetSunLight(sunLight_);
}

void Ground::Draw() {
    RenderManager::Get().AddModelInstance(modelInstance_);
    RenderManager::Get().AddModelInstance(skydomeInstance_);
}
