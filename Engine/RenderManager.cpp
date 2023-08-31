#include "RenderManager.h"

#include "Externals/DirectXTex/d3dx12.h"

#include <filesystem>

#include "Core/Window.h"
#include "Core/Graphics.h"
#include "Core/ImGuiManager.h"
#include "Core/ShaderCompiler.h"
#include "Core/CommandList.h"
#include "Core/Texture.h"
#include "Core/TextureManager.h"
#include "Model.h"
#include "Sprite.h"

namespace {

    struct SceneConstant {
        Matrix4x4 cameraVPMatrix;
        Vector3 cameraPosition;
        float sunLightIntensity;
        Vector4 sunLightColor;
        Vector3 sunLightDirection;
    };

    enum RootParameter {
        kRPModelInstance,      // 定数バッファ
        kRPSceneConstant,     // 定数バッファ
        kRPMaterial,          // 定数バッファ
        kRPTexture,           // テクスチャ
        kRPShadowMaps,        // テクスチャ
        kRPShadowMatrices,    // 構造化バッファ

        kNumRootParameters
    };

}

RenderManager& RenderManager::Get() {
    static RenderManager instance;
    return instance;
}

void RenderManager::Create() {
    graphics_ = &Graphics::Get();
    imguiManager_ = &ImGuiManager::Get();

    CreateRootSignature();
    CreatePipelineStateObjects();
    CreateBuffers();

    auto& window = Window::Get();
    spriteMatrix_ = Matrix4x4::MakeOrthographicProjection(float(window.GetClientWidth()), float(window.GetClientHeight()), 0.0f, 1.0f);
    spriteMatrix_.m[1][1] *= -1.0f;
    spriteMatrix_.m[3][0] = -1.0f;
    spriteMatrix_.m[3][1] = 1.0f;
    defaultTexture_ = TextureManager::Load("Resource/white.png");
}

void RenderManager::AddModelInstance(const ModelInstance& modelInstance) {
    auto model = modelInstance.GetModel();
    RenderObject renderObject{};
    renderObject.vertexBufferView = model->vertexBufferView_;
    renderObject.indexBufferView = model->indexBufferView_;
    renderObject.modelInstance = modelInstance.modelInstanceBuffer_.Get()->GetGPUVirtualAddress();

    auto& materials = model->materials_;
    for (auto& mesh : model->meshes_) {
        renderObject.indexCount = mesh.indexCount;
        renderObject.indexStartLocation = mesh.indexStartLocation;
        renderObject.material = materials[mesh.materialIndex].constantBuffer_.Get()->GetGPUVirtualAddress();
        if (materials[mesh.materialIndex].texture_) {
            renderObject.texture = materials[mesh.materialIndex].texture_->GetSRV().GPU();
        }
        else {
            renderObject.texture = defaultTexture_->GetSRV().GPU();
        }

        if (modelInstance.castShadow_) {
            shadowPassObjects_.emplace_back(renderObject);
        }
        if (modelInstance.receiveShadow_) {
            receiveShadowPassObjects_.emplace_back(renderObject);
        }
        else {
            defaultPassObjects_.emplace_back(renderObject);
        }
    }
}

void RenderManager::AddSprite(const Sprite& sprite) {
    sprite.UpdateBuffers(spriteMatrix_);

    RenderSprite renderSprite{};
    renderSprite.vertexBufferView = sprite.vertexBufferView_;
    if (sprite.texture_) {
        renderSprite.texture = sprite.texture_->GetSRV().GPU();
    }
    else {
        renderSprite.texture = defaultTexture_->GetSRV().GPU();
    }

    spritePassObjects_.emplace_back(renderSprite);
}

void RenderManager::Render() {
    if (camera_ && sunLight_) {
        UpdateSceneConstant();
        UpdateShadowMatrices();
    }

    auto& commandList = graphics_->GetCommandList();

    commandList.SetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, graphics_->GetDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV));

    commandList.TransitionBarrier(shadowBuffer_, D3D12_RESOURCE_STATE_DEPTH_WRITE);
    commandList.CommitResourceBarrier();
    commandList.SetRenderTargets(0, nullptr, shadowBuffer_.GetDepthStencilView().CPU());
    commandList.ClearDepth(shadowBuffer_);
    commandList.SetViewportAndScissor(0, 0, kShadowMapWidth, kShadowMapHeight);

    commandList.SetGraphicsRootSignature(rootSignature_);
    commandList.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    commandList.SetPipelineState(shadowPSO_);
    commandList.SetGraphicsRootSRV(kRPShadowMatrices, shadowMatricesBuffer_.Get()->GetGPUVirtualAddress());

    // 影を描画

    for (const auto& object : shadowPassObjects_) {
        commandList.SetGraphicsRootCBV(kRPModelInstance, object.modelInstance);
        commandList.SetVertexBuffer(object.vertexBufferView);
        commandList.SetIndexBuffer(object.indexBufferView);
        commandList.DrawIndexed(object.indexCount, object.indexStartLocation);
    }

    commandList.TransitionBarrier(shadowBuffer_, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);

    auto& colorBuffer = graphics_->GetCurrentSwapChainColorBuffer();
    auto& depthBuffer = graphics_->GetSwapChainDepthBuffer();
    commandList.TransitionBarrier(colorBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
    commandList.CommitResourceBarrier();

    commandList.SetRenderTarget(colorBuffer.GetRenderTargetView().CPU(), depthBuffer.GetDepthStencilView().CPU());
    commandList.ClearColor(colorBuffer);
    commandList.ClearDepth(depthBuffer);
    commandList.SetViewportAndScissor(0, 0, colorBuffer.GetWidth(), colorBuffer.GetHeight());

    // 影を受けない描画
    commandList.SetPipelineState(defaultPSO_);
    commandList.SetGraphicsRootCBV(kRPSceneConstant, sceneConstantBuffer_.Get()->GetGPUVirtualAddress());
    commandList.SetGraphicsRootDescriptorTable(kRPShadowMaps, shadowBuffer_.GetShaderResourceView());

    for (const auto& object : defaultPassObjects_) {
        commandList.SetGraphicsRootCBV(kRPModelInstance, object.modelInstance);
        commandList.SetGraphicsRootCBV(kRPMaterial, object.material);
        commandList.SetGraphicsRootDescriptorTable(kRPTexture, object.texture);
        commandList.SetVertexBuffer(object.vertexBufferView);
        commandList.SetIndexBuffer(object.indexBufferView);
        commandList.DrawIndexed(object.indexCount, object.indexStartLocation);
    }

    // 影を受ける描画
    commandList.SetPipelineState(receiveShadowPSO_);
    commandList.SetGraphicsRootDescriptorTable(kRPShadowMaps, shadowBuffer_.GetShaderResourceView());

    for (const auto& object : receiveShadowPassObjects_) {
        commandList.SetGraphicsRootCBV(kRPModelInstance, object.modelInstance);
        commandList.SetGraphicsRootCBV(kRPMaterial, object.material);
        commandList.SetGraphicsRootDescriptorTable(kRPTexture, object.texture);
        commandList.SetVertexBuffer(object.vertexBufferView);
        commandList.SetIndexBuffer(object.indexBufferView);
        commandList.DrawIndexed(object.indexCount, object.indexStartLocation);
    }

    commandList.SetPipelineState(spritePSO_);
    for (auto& sprite : spritePassObjects_) {
        commandList.SetGraphicsRootDescriptorTable(kRPTexture, sprite.texture);
        commandList.SetVertexBuffer(sprite.vertexBufferView);
        commandList.Draw(6);
    }
   
    imguiManager_->Render();
    commandList.TransitionBarrier(colorBuffer, D3D12_RESOURCE_STATE_PRESENT);
    commandList.CommitResourceBarrier();

    graphics_->Render();

    shadowPassObjects_.clear();
    defaultPassObjects_.clear();
    receiveShadowPassObjects_.clear();
    spritePassObjects_.clear();
}

void RenderManager::CreateRootSignature() {

    CD3DX12_DESCRIPTOR_RANGE textureRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
    CD3DX12_DESCRIPTOR_RANGE shadowMapRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 1);

    CD3DX12_ROOT_PARAMETER rootParameters[kNumRootParameters]{};
    rootParameters[kRPModelInstance].InitAsConstantBufferView(0);
    rootParameters[kRPSceneConstant].InitAsConstantBufferView(1);
    rootParameters[kRPMaterial].InitAsConstantBufferView(2);
    rootParameters[kRPTexture].InitAsDescriptorTable(1, &textureRange);
    rootParameters[kRPShadowMaps].InitAsDescriptorTable(1, &shadowMapRange);
    rootParameters[kRPShadowMatrices].InitAsShaderResourceView(2);

    D3D12_STATIC_SAMPLER_DESC samplers[2]{};
    samplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
    samplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
    samplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
    samplers[0].MinLOD = 0.0f;
    samplers[0].MaxLOD = D3D12_FLOAT32_MAX;
    samplers[0].ShaderRegister = 0;
    samplers[0].RegisterSpace = 0;
    samplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    samplers[1].Filter = D3D12_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    samplers[1].AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplers[1].AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplers[1].AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
    samplers[1].ComparisonFunc = D3D12_COMPARISON_FUNC_GREATER;
    samplers[1].MinLOD = 0.0f;
    samplers[1].MaxLOD = D3D12_FLOAT32_MAX;
    samplers[1].MaxAnisotropy = 1;
    samplers[1].ShaderRegister = 1;
    samplers[1].RegisterSpace = 0;
    samplers[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;

    D3D12_ROOT_SIGNATURE_DESC desc{};
    desc.NumParameters = _countof(rootParameters);
    desc.pParameters = rootParameters;
    desc.NumStaticSamplers = _countof(samplers);
    desc.pStaticSamplers = samplers;
    desc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

    rootSignature_.Create(desc);
}

void RenderManager::CreatePipelineStateObjects() {
    const std::string shaderPath = "Engine/Shader/";

    // シャドウ用パイプライン
    {
        auto vs = ShaderCompiler::Compile(shaderPath + "ShadowVS.hlsl", L"vs_6_0");
        auto gs = ShaderCompiler::Compile(shaderPath + "ShadowGS.hlsl", L"gs_6_0");

        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = rootSignature_.GetRootSignature().Get();
        desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
        desc.GS = CD3DX12_SHADER_BYTECODE(gs->GetBufferPointer(), gs->GetBufferSize());
        desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_FRONT;
        desc.RasterizerState = rasterizerDesc;

        D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        desc.DepthStencilState = depthStencilDesc;

        D3D12_INPUT_ELEMENT_DESC inputElements[]{
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
        };

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
        inputLayoutDesc.NumElements = _countof(inputElements);
        inputLayoutDesc.pInputElementDescs = inputElements;
        desc.InputLayout = inputLayoutDesc;

        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        desc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
        desc.SampleDesc = DXGI_SAMPLE_DESC{ .Count = 1, .Quality = 0 };

        shadowPSO_.Create(desc);
    }
    // メイン描画用パイプライン2種
    {
        auto vs = ShaderCompiler::Compile(shaderPath + "DefaultVS.hlsl", L"vs_6_0");
        auto ps = ShaderCompiler::Compile(shaderPath + "DefaultPS.hlsl", L"ps_6_0");

        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = rootSignature_.GetRootSignature().Get();
        desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
        desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());
        desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

        D3D12_BLEND_DESC blendDesc{};
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        desc.BlendState = blendDesc;

        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
        desc.RasterizerState = rasterizerDesc;

        D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
        desc.DepthStencilState = depthStencilDesc;

        D3D12_INPUT_ELEMENT_DESC inputElements[]{
            { "POSITION",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "NORMAL",     0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
        inputLayoutDesc.NumElements = _countof(inputElements);
        inputLayoutDesc.pInputElementDescs = inputElements;
        desc.InputLayout = inputLayoutDesc;

        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc = DXGI_SAMPLE_DESC{ .Count = 1, .Quality = 0 };
        defaultPSO_.Create(desc);

        vs = ShaderCompiler::Compile(shaderPath + "ReceiveShadowVS.hlsl", L"vs_6_0");
        ps = ShaderCompiler::Compile(shaderPath + "ReceiveShadowPS.hlsl", L"ps_6_0");

        desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
        desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());
        receiveShadowPSO_.Create(desc);
    }
    {
        auto vs = ShaderCompiler::Compile(shaderPath + "SpriteVS.hlsl", L"vs_6_0");
        auto ps = ShaderCompiler::Compile(shaderPath + "SpritePS.hlsl", L"ps_6_0");

        D3D12_GRAPHICS_PIPELINE_STATE_DESC desc{};
        desc.pRootSignature = rootSignature_.GetRootSignature().Get();
        desc.VS = CD3DX12_SHADER_BYTECODE(vs->GetBufferPointer(), vs->GetBufferSize());
        desc.PS = CD3DX12_SHADER_BYTECODE(ps->GetBufferPointer(), ps->GetBufferSize());
        desc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

        D3D12_BLEND_DESC blendDesc{};
        auto& renderTarget = blendDesc.RenderTarget[0];
        renderTarget.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
        renderTarget.BlendOpAlpha = D3D12_BLEND_OP_ADD;
        renderTarget.SrcBlendAlpha = D3D12_BLEND_ONE;
        renderTarget.DestBlendAlpha = D3D12_BLEND_ZERO;
        renderTarget.BlendEnable = true;
        renderTarget.BlendOp = D3D12_BLEND_OP_ADD;
        renderTarget.SrcBlend = D3D12_BLEND_SRC_ALPHA;
        renderTarget.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
        desc.BlendState = blendDesc;

        D3D12_RASTERIZER_DESC rasterizerDesc{};
        rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
        rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
        desc.RasterizerState = rasterizerDesc;

        D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
        depthStencilDesc.DepthEnable = TRUE;
        depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
        depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
        desc.DepthStencilState = depthStencilDesc;

        D3D12_INPUT_ELEMENT_DESC inputElements[]{
            { "POSITION",   0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "TEXCOORD",   0, DXGI_FORMAT_R32G32_FLOAT,    0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR",     0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
        };

        D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
        inputLayoutDesc.NumElements = _countof(inputElements);
        inputLayoutDesc.pInputElementDescs = inputElements;
        desc.InputLayout = inputLayoutDesc;

        desc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

        desc.NumRenderTargets = 1;
        desc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
        desc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
        desc.SampleDesc = DXGI_SAMPLE_DESC{ .Count = 1, .Quality = 0 };
        spritePSO_.Create(desc);
    }
}


void RenderManager::CreateBuffers() {
    shadowBuffer_.Create(kShadowMapWidth, kShadowMapHeight, kNumShadowMaps, DXGI_FORMAT_D32_FLOAT);
    shadowBuffer_.CreateDepthStencilView();
    auto srvFormat = DXGI_FORMAT_R32_FLOAT;
    shadowBuffer_.CreateShaderResourceView(&srvFormat);

    sceneConstantBuffer_.Create(sizeof(SceneConstant));
    shadowMatricesBuffer_.Create(sizeof(Matrix4x4) * kNumShadowMaps);
}

void RenderManager::UpdateSceneConstant() {
    SceneConstant sceneConstant{};
    sceneConstant.cameraVPMatrix = camera_->GetViewProjectionMatrix();
    sceneConstant.cameraPosition = camera_->GetPosition();
    sceneConstant.sunLightColor = sunLight_->color;
    sceneConstant.sunLightDirection = sunLight_->direction.Normalized();
    sceneConstant.sunLightIntensity = sunLight_->intensity;
    sceneConstantBuffer_.Copy(sceneConstant);
}

void RenderManager::UpdateShadowMatrices() {

    // シャドウマップのエリアを計算する
    float splitAreaTable[kNumShadowMaps + 1]{};
    {
        const float lambda = 0.1f;
        float f = camera_->GetFarClip();
        float n = camera_->GetNearClip();
        for (uint32_t i = 0; i < kNumShadowMaps + 1; ++i) {
            float im = float(i) / kNumShadowMaps;
            float clog = n * std::pow(f / n, im);
            float cuni = n + (f - n) * im;
            splitAreaTable[i] = Math::Lerp(lambda, clog, cuni);
        }
    }

    // 基本となるライト行列
    Matrix4x4 lightViewMatrix;
    Matrix4x4 lightProjectionMatrix;
    Matrix4x4 lightViewProjectionMatrix;
    {
        float epsilon = 0.0001f;
        float distance = 50.0f;
        float nearClip = 1.0f;
        float farClip = 100.0f;
        Vector3 dir = sunLight_->direction.Normalized();
        if (1.0f - std::abs(Dot(dir, Vector3::right)) < epsilon) {
            lightViewMatrix = Matrix4x4::MakeAffineInverse(Matrix4x4::MakeLookRotation(dir, Vector3::up), -dir * distance + camera_->GetPosition());
        }
        else {
            lightViewMatrix = Matrix4x4::MakeAffineInverse(Matrix4x4::MakeLookRotation(dir, Vector3::right), -dir * distance + camera_->GetPosition());
        }
        lightProjectionMatrix = Matrix4x4::MakeOrthographicProjection(float(kShadowMapWidth), float(kShadowMapHeight), nearClip, farClip);

        lightViewProjectionMatrix = lightViewMatrix * lightProjectionMatrix;
    }

    Matrix4x4 shadowMatrices[kNumShadowMaps];
    {
        for (uint32_t i = 0; i < kNumShadowMaps; ++i) {
            // 分割した透視投影行列
            Matrix4x4 splitAreaProjectionMatrix = camera_->GetProjectionMatrix();
            splitAreaProjectionMatrix.m[2][2] = splitAreaTable[i + 1] / (splitAreaTable[i + 1] - splitAreaTable[i]);
            splitAreaProjectionMatrix.m[3][2] = splitAreaProjectionMatrix.m[2][2] * -splitAreaTable[i];

            // 分割した視錐台
            Frustum splitAreaFrustum(camera_->GetViewMatrix() * splitAreaProjectionMatrix);

            // 視錐台を収めるAABBを求める
            Math::AABB cropAABB(lightViewProjectionMatrix.ApplyTransformWDivide(splitAreaFrustum.GetCorner(Frustum::NearLowerLeft)));
            for (int j = 1; j < Frustum::NumCorners; ++j) {
                cropAABB.Merge(lightViewProjectionMatrix.ApplyTransformWDivide(splitAreaFrustum.GetCorner(static_cast<Frustum::CornerID>(j))));
            }

            // クロップ行列を求める
            Vector3 scale, offset;
            scale.x = 2.0f / (cropAABB.max.x - cropAABB.min.x);
            scale.y = 2.0f / (cropAABB.max.y - cropAABB.min.y);
            scale.z = 1.0f;
            offset.x = -0.5f * (cropAABB.max.x + cropAABB.min.x) * scale.x;
            offset.y = -0.5f * (cropAABB.max.y + cropAABB.min.y) * scale.y;
            offset.z = 0.0f;
            Matrix4x4 cropMatrix{
                scale.x,    0.0f,       0.0f,       0.0f,
                0.0f,       scale.y,    0.0f,       0.0f,
                0.0f,       0.0f,       scale.z,    0.0f,
                offset.x,   offset.y,   offset.z,   1.0f,
            };

            shadowMatrices[i] = lightViewProjectionMatrix * cropMatrix;
        }
    }

    shadowMatricesBuffer_.Copy(shadowMatrices, sizeof(shadowMatrices));
}
