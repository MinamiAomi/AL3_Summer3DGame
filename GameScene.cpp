#include "GameScene.h"

#include "CollisionManager.h"
#include "Ground.h"
#include "FollowCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "SystemManager.h"

GameScene::GameScene() : renderManager_(RenderManager::Get()) {

}

GameScene::~GameScene() {
    CollisionManager::Get().Clear();
}

void GameScene::Initialize() {
    LoadResource();
    CreateGameObjects();

    for (auto& gameObject : gameObjects_) {
        gameObject->Initialize();
    }
    reload_ = false;
}

void GameScene::LoadResource() {
    LoadModel("Resource/player/arm.obj");
    LoadModel("Resource/player/head.obj");
    LoadModel("Resource/player/body.obj");
    LoadModel("Resource/player/bullet.obj");
    LoadModel("Resource/ground/ground.obj");
    LoadModel("Resource/cannon/cannon.obj");
    LoadModel("Resource/enemy/enemy.obj");
    LoadModel("Resource/enemy/apple.obj");
}

void GameScene::CreateGameObjects() {
    AddGameObject<Ground>("Ground");
    AddGameObject<Player>("Player");
    AddGameObject<Enemy>("Enemy");
    AddGameObject<FollowCamera>("FollowCamera");
    AddGameObject<SystemManager>("System");

}

void GameScene::LoadModel(const std::string& path) {
    auto model = models_.emplace(path, std::make_shared<Model>());
    model.first->second->CreateFromOBJ(path);
}

void GameScene::Update() {
    for (auto& gameObject : gameObjects_) {
        if (gameObject->GetState() == GameObject::kActive) {
            gameObject->Update();
        }
    }

    CollisionManager::Get().CheckCollision();
}

void GameScene::Draw() {
    for (auto& gameObject : gameObjects_) {
        if (gameObject->GetState() == GameObject::kActive) {
            gameObject->Draw();
        }
    }
}

void GameScene::EndFrame() {
    gameObjects_.remove_if([](const auto& gameObject) {
        return gameObject->GetState() == GameObject::kDead;
        });

    if (reload_) {
        gameObjects_.clear();
        CreateGameObjects();

        for (auto& gameObject : gameObjects_) {
            gameObject->Initialize();
        }
        reload_ = false;
    }
}

GameObject* GameScene::FindGameObject(const std::string& name) {
    auto iter = std::find_if(gameObjects_.begin(), gameObjects_.end(), [&](const auto& gameObject) {
        return gameObject->GetName() == name;
        });
    if (iter != gameObjects_.end()) {
        return iter->get();
    }
    return nullptr;
}

void GameScene::ReloadScene() {
    reload_ = true;
}

