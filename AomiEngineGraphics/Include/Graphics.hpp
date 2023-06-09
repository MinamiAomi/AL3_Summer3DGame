#pragma once

#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>

namespace AomiEngine {
	
	template<class T>
	using ComPtr = Microsoft::WRL::ComPtr<T>;

	class Graphics {
	public:

		ID3D12Device5* GetDevice() const { return device_.Get(); }
		IDXGIFactory6* GetDXGIFactory() const { return dxgiFactory_.Get(); }

	private:
		ComPtr<ID3D12Device5> device_;
		ComPtr<IDXGIFactory6> dxgiFactory_;
	
		

	};

} // namespace AomiEngine