#include "lipch.h"
#include "ConvertD3D11.h"

#include <iostream>

namespace li
{
	D3D11_USAGE ConvertD3D11::BufferUsage(li::BufferUsage usage)
	{
		switch (usage)
		{
		case BufferUsage::StaticDraw:
			return D3D11_USAGE_IMMUTABLE;
		case BufferUsage::DynamicDraw:
			return D3D11_USAGE_DYNAMIC;
		default:
			LI_CORE_ERROR("Unknown buffer usage {}.", usage);
			return D3D11_USAGE_DEFAULT;
		}
	}

	D3D11_TEXTURE_ADDRESS_MODE ConvertD3D11::WrapType(li::WrapType type)
	{
		switch (type)
		{
		case WrapType::Repeat:
			return D3D11_TEXTURE_ADDRESS_WRAP;
		case WrapType::MirroredRepeat:
			return D3D11_TEXTURE_ADDRESS_MIRROR;
		case WrapType::ClampToEdge:
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		case WrapType::ClampToBorder:
			return D3D11_TEXTURE_ADDRESS_BORDER;
		default:
			LI_CORE_ERROR("Unknown wrap type {}.", type);
			return D3D11_TEXTURE_ADDRESS_CLAMP;
		}
	}
	DXGI_FORMAT ConvertD3D11::TextureFormat(int num_channels)
	{
		switch (num_channels)
		{
		case 1:
			return DXGI_FORMAT_R8_UNORM;
		case 2:
			return DXGI_FORMAT_R8G8_UNORM;
		case 4:
			return DXGI_FORMAT_R8G8B8A8_UNORM;
			break;
		default:
			LI_CORE_ERROR("{} channels not supported!", num_channels);
			return DXGI_FORMAT_UNKNOWN;
		}
	}
}
