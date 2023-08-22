#pragma once

#include <cstdint>

#include "Core/GPUResource.h"
#include "Core/ShadowBuffer.h"
#include "Core/PipelineState.h"
#include "Core/RootSignature.h"
#include "Math/MathUtils.h"
#include "Math/Camera.h"
#include "Math/LightStructs.h"
#include "Math/Frustum.h"

class Graphics;
class ImGuiManager;
class Texture;
class ModelInstance;

class RenderManager {
public:
    static const uint32_t kNumShadowMaps = 3;
    static const uint32_t kShadowMapWidth = 2048;
    static const uint32_t kShadowMapHeight = 2048;

    static RenderManager& Get();

    void Create();
    void Destroy();

    void AddModelInstance(const ModelInstance& modelInstance);
    void Render();

    void SetCamera(const Camera& camera) { camera_ = &camera; }
    void SetSunLight(const DirectionalLight& sunLight) { sunLight_ = &sunLight; }

private:
    RenderManager() = default;
    RenderManager(const RenderManager&) = delete;
    RenderManager& operator=(const RenderManager&) = delete;

    void CreateRootSignature();
    void CreatePipelineStateObjects();
    void CreateBuffers();
    void UpdateSceneConstant();
    void UpdateShadowMatrices();

    Graphics* graphics_;
    ImGuiManager* imguiManager_;

    RootSignature rootSignature_;
    PipelineState shadowPSO_;
    PipelineState defaultPSO_;
    PipelineState receiveShadowPSO_;

    ShadowBuffer shadowBuffer_;

    const Camera* camera_;
    const DirectionalLight* sunLight_;
    UploadBuffer sceneConstantBuffer_;
    UploadBuffer shadowMatricesBuffer_;

    std::shared_ptr<Texture> defaultTexture_;

    struct RenderObject {
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
        D3D12_INDEX_BUFFER_VIEW indexBufferView;
        D3D12_GPU_VIRTUAL_ADDRESS modelInstance;
        D3D12_GPU_VIRTUAL_ADDRESS material;
        D3D12_GPU_DESCRIPTOR_HANDLE texture;
        uint32_t indexCount;
        uint32_t indexStartLocation;
    };
    std::vector<RenderObject> shadowPassObjects_;
    std::vector<RenderObject> defaultPassObjects_;
    std::vector<RenderObject> receiveShadowPassObjects_;
};