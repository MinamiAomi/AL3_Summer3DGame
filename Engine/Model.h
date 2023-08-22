#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

#include "Core/GPUResource.h"
#include "Math/MathUtils.h"
#include "Material.h"

class Model {
    friend class RenderManager;
public:

    void CreateFromOBJ(const std::filesystem::path& path);

private:
    struct Mesh {
        uint32_t indexCount;
        uint32_t indexStartLocation;
        uint32_t materialIndex;
    };

    void LoadOBJFile(const std::filesystem::path& path);
    void LoadMTLFile(const std::filesystem::path& path);

    GPUBuffer vertexBuffer_;
    GPUBuffer indexBuffer_;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

    std::vector<Mesh> meshes_;
    std::vector<Material> materials_;
};

class ModelInstance {
    friend class RenderManager;
public:
    ModelInstance();

    void SetModel(const std::shared_ptr<const Model>& model) { model_ = model; }
    void SetWorldMatrix(const Matrix4x4& matrix) { modelInstanceBuffer_.Copy(matrix); }
    void SetCastShadow(bool castShadow) { castShadow_ = castShadow; }
    void SetReceiveShadow(bool receiveShadow) { receiveShadow_ = receiveShadow; }

    std::shared_ptr<const Model> GetModel() const { return model_; }

private:
    ModelInstance(const ModelInstance&) = delete;
    ModelInstance& operator=(const ModelInstance&) = delete;

    std::shared_ptr<const Model> model_;
    UploadBuffer modelInstanceBuffer_;
    bool castShadow_;
    bool receiveShadow_;
};