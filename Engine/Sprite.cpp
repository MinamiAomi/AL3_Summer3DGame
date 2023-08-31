#include "Sprite.h"

#include "RenderManager.h"
#include "Core/Texture.h"

namespace {
    const uint32_t kNumVertices = 6;

    struct Vertex {
        Vector4 position;
        Vector2 texcoord;
        Vector4 color;
    };
}

Sprite::Sprite() {

    vertexBuffer_.Create(sizeof(Vertex) * kNumVertices);

    vertexBufferView_.BufferLocation = vertexBuffer_.Get()->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = UINT(vertexBuffer_.GetBufferSize());
    vertexBufferView_.StrideInBytes = sizeof(Vertex);
}

void Sprite::UpdateBuffers(const Matrix4x4& matrix) const {

    Vector4 positions[4] = {
        { 0.0f, 0.0f, 0.0f, 1.0f },
        { 1.0f, 0.0f, 0.0f, 1.0f },
        { 0.0f, 1.0f, 0.0f, 1.0f },
        { 1.0f, 1.0f, 0.0f, 1.0f },
    };
    Matrix4x4 world = Matrix4x4::MakeRotationZ(rotate_);
    world.SetTranslate(Vector3(translate_, 0.0f));
    world = world * matrix;

    for (auto& position : positions) {
        position.x -= anchor_.x;
        position.y -= anchor_.y;
        position.x *= size_.x;
        position.y *= size_.y;
        position = position * world;
    }


    Vector2 texcoords[4] = {
        { 0.0f, 0.0f },
        { 1.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 1.0f },
    };

    float width = 1.0f;
    float height = 1.0f;

    if (texture_) {
        auto& desc = texture_->GetDesc();
        width = 1.0f / float(desc.Width);
        height = 1.0f / float(desc.Height);
    }

    Vector2 scale = Vector2::Scale(texcoordSize_, { width, height });
    Vector2 offset = Vector2::Scale(texcoordOffset_, { width, height });
    for (auto& texcoord : texcoords) {
        texcoord = Vector2::Scale(texcoord, scale) + offset;
    }

    Vertex vertices[kNumVertices] = {
        { positions[0], texcoords[0], color_ },
        { positions[1], texcoords[1], color_ },
        { positions[2], texcoords[2], color_ },
        { positions[2], texcoords[2], color_ },
        { positions[1], texcoords[1], color_ },
        { positions[3], texcoords[3], color_ }
    };
    vertexBuffer_.Copy(vertices, sizeof(Vertex) * kNumVertices);
}

