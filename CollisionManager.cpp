#include "CollisionManager.h"

#include "Collider.h"

CollisionManager& CollisionManager::Get() {
    static CollisionManager instance;
    return instance;
}

void CollisionManager::CheckCollision() {
    auto iterA = colliders_.begin();
    for (; iterA != colliders_.end(); ++iterA) {
        auto colliderA = *iterA;
        // 非アクティブは判定しない
        if (!colliderA->IsActive()) { continue; }

        auto iterB = iterA;
        ++iterB;
        for (; iterB != colliders_.end(); ++iterB) {
            auto colliderB = *iterB;
            // 非アクティブは判定しない
            if (!colliderB->IsActive()) { continue; }

            if (colliderA->IsCollision(colliderB)) {
                colliderA->Callback(colliderB);
                colliderB->Callback(colliderA);
            }
        }

    }

}
