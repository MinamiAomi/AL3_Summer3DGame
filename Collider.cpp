#include "Collider.h"

#include "GameObject.h"

namespace {

    bool SphereToSphere(const Vector3& c1, float r1, const Vector3& c2, float r2) {
        return (c2 - c1).LengthSquare() <= (r1 + r2) * (r1 + r2);
    }

    bool SphereToCapsule(const Vector3& c, float rS, const Vector3& s, const Vector3& e, float rC) {
        Vector3 diff = e - s;
        float segLen = diff.Length();
        Vector3 segDir = diff * (1.0f / segLen);
        float dis = Dot(c - s, segDir);
        dis = std::clamp(dis, 0.0f, segLen);
        Vector3 point = s + dis * segDir;
        return SphereToSphere(c, rS, point, rC);
    }

    bool CapsuleToCapsule(const Vector3& s1, const Vector3& e1, float r1, const Vector3& s2, const Vector3& e2, float r2) {
        s1; e1; r1; s2; e2; r2;
        return false;
    }

}

void Collider::Callback(Collider* other) {
    if (callback_) {
        callback_(*other);
    }
}

bool Collider::CanCollision(Collider* other) const {
    return (this->collisionAttribute_ & other->collisionMask_) && (other->collisionAttribute_ & this->collisionMask_);
}

bool SphereCollider::IsCollision(SphereCollider* other) {
    Vector3 c1 = this->GetLocalOffset() * this->GetGameObject()->GetTransform().worldMatrix;
    Vector3 c2 = other->GetLocalOffset() * other->GetGameObject()->GetTransform().worldMatrix;
    return SphereToSphere(c1, this->GetRadius(), c2, other->GetRadius());
}

bool SphereCollider::IsCollision(CapsuleCollider* other) {
    Vector3 c = this->GetLocalOffset() * this->GetGameObject()->GetTransform().worldMatrix;
    Vector3 s = other->GetLocalStart() * other->GetGameObject()->GetTransform().worldMatrix;
    Vector3 e = other->GetLocalEnd() * other->GetGameObject()->GetTransform().worldMatrix;
    return SphereToCapsule(c, this->GetRadius(), s, e, other->GetRadius());
}

bool CapsuleCollider::IsCollision(SphereCollider* other) {
    Vector3 c = other->GetLocalOffset() * this->GetGameObject()->GetTransform().worldMatrix;
    Vector3 s = this->GetLocalStart() * other->GetGameObject()->GetTransform().worldMatrix;
    Vector3 e = this->GetLocalEnd() * other->GetGameObject()->GetTransform().worldMatrix;
    return SphereToCapsule(c, other->GetRadius(), s, e, this->GetRadius());
}

bool CapsuleCollider::IsCollision(CapsuleCollider* other) {
    other;
    return false;
}
