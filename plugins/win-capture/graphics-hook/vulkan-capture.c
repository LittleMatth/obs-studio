#include <windows.h>
#include "graphics-hook.h"

#if COMPILE_VULKAN_HOOK

#define VK_USE_PLATFORM_WIN32_KHR

#include <malloc.h>
#include <vulkan/vulkan.h>
#include <vulkan/vk_layer.h>
#undef VK_LAYER_EXPORT
#if defined(WIN32)
#define VK_LAYER_EXPORT __declspec(dllexport)
#else
#define VK_LAYER_EXPORT 
#endif

#include <vulkan/vulkan_win32.h>
#include <../Source/layers/vk_layer_dispatch_table.h>

#define COBJMACROS
#include <dxgi.h>
#include <d3d11.h>


//#define DEBUG_PRINT
//#define DEBUG_PRINT_PROCADDR

const char * VkFormatString(VkFormat format) {
	switch (format) {
	default:
	case VK_FORMAT_UNDEFINED: return "VK_FORMAT_UNDEFINED"; break;
	case VK_FORMAT_R4G4_UNORM_PACK8: return "VK_FORMAT_R4G4_UNORM_PACK8"; break;
	case VK_FORMAT_R4G4B4A4_UNORM_PACK16: return "VK_FORMAT_R4G4B4A4_UNORM_PACK16"; break;
	case VK_FORMAT_B4G4R4A4_UNORM_PACK16: return "VK_FORMAT_B4G4R4A4_UNORM_PACK16"; break;
	case VK_FORMAT_R5G6B5_UNORM_PACK16: return "VK_FORMAT_R5G6B5_UNORM_PACK16"; break;
	case VK_FORMAT_B5G6R5_UNORM_PACK16: return "VK_FORMAT_B5G6R5_UNORM_PACK16"; break;
	case VK_FORMAT_R5G5B5A1_UNORM_PACK16: return "VK_FORMAT_R5G5B5A1_UNORM_PACK16"; break;
	case VK_FORMAT_B5G5R5A1_UNORM_PACK16: return "VK_FORMAT_B5G5R5A1_UNORM_PACK16"; break;
	case VK_FORMAT_A1R5G5B5_UNORM_PACK16: return "VK_FORMAT_A1R5G5B5_UNORM_PACK16"; break;
	case VK_FORMAT_R8_UNORM: return "VK_FORMAT_R8_UNORM"; break;
	case VK_FORMAT_R8_SNORM: return "VK_FORMAT_R8_SNORM"; break;
	case VK_FORMAT_R8_USCALED: return "VK_FORMAT_R8_USCALED"; break;
	case VK_FORMAT_R8_SSCALED: return "VK_FORMAT_R8_SSCALED"; break;
	case VK_FORMAT_R8_UINT: return "VK_FORMAT_R8_UINT"; break;
	case VK_FORMAT_R8_SINT: return "VK_FORMAT_R8_SINT"; break;
	case VK_FORMAT_R8_SRGB: return "VK_FORMAT_R8_SRGB"; break;
	case VK_FORMAT_R8G8_UNORM: return "VK_FORMAT_R8G8_UNORM"; break;
	case VK_FORMAT_R8G8_SNORM: return "VK_FORMAT_R8G8_SNORM"; break;
	case VK_FORMAT_R8G8_USCALED: return "VK_FORMAT_R8G8_USCALED"; break;
	case VK_FORMAT_R8G8_SSCALED: return "VK_FORMAT_R8G8_SSCALED"; break;
	case VK_FORMAT_R8G8_UINT: return "VK_FORMAT_R8G8_UINT"; break;
	case VK_FORMAT_R8G8_SINT: return "VK_FORMAT_R8G8_SINT"; break;
	case VK_FORMAT_R8G8_SRGB: return "VK_FORMAT_R8G8_SRGB"; break;
	case VK_FORMAT_R8G8B8_UNORM: return "VK_FORMAT_R8G8B8_UNORM"; break;
	case VK_FORMAT_R8G8B8_SNORM: return "VK_FORMAT_R8G8B8_SNORM"; break;
	case VK_FORMAT_R8G8B8_USCALED: return "VK_FORMAT_R8G8B8_USCALED"; break;
	case VK_FORMAT_R8G8B8_SSCALED: return "VK_FORMAT_R8G8B8_SSCALED"; break;
	case VK_FORMAT_R8G8B8_UINT: return "VK_FORMAT_R8G8B8_UINT"; break;
	case VK_FORMAT_R8G8B8_SINT: return "VK_FORMAT_R8G8B8_SINT"; break;
	case VK_FORMAT_R8G8B8_SRGB: return "VK_FORMAT_R8G8B8_SRGB"; break;
	case VK_FORMAT_B8G8R8_UNORM: return "VK_FORMAT_B8G8R8_UNORM"; break;
	case VK_FORMAT_B8G8R8_SNORM: return "VK_FORMAT_B8G8R8_SNORM"; break;
	case VK_FORMAT_B8G8R8_USCALED: return "VK_FORMAT_B8G8R8_USCALED"; break;
	case VK_FORMAT_B8G8R8_SSCALED: return "VK_FORMAT_B8G8R8_SSCALED"; break;
	case VK_FORMAT_B8G8R8_UINT: return "VK_FORMAT_B8G8R8_UINT"; break;
	case VK_FORMAT_B8G8R8_SINT: return "VK_FORMAT_B8G8R8_SINT"; break;
	case VK_FORMAT_B8G8R8_SRGB: return "VK_FORMAT_B8G8R8_SRGB"; break;
	case VK_FORMAT_R8G8B8A8_UNORM: return "VK_FORMAT_R8G8B8A8_UNORM"; break;
	case VK_FORMAT_R8G8B8A8_SNORM: return "VK_FORMAT_R8G8B8A8_SNORM"; break;
	case VK_FORMAT_R8G8B8A8_USCALED: return "VK_FORMAT_R8G8B8A8_USCALED"; break;
	case VK_FORMAT_R8G8B8A8_SSCALED: return "VK_FORMAT_R8G8B8A8_SSCALED"; break;
	case VK_FORMAT_R8G8B8A8_UINT: return "VK_FORMAT_R8G8B8A8_UINT"; break;
	case VK_FORMAT_R8G8B8A8_SINT: return "VK_FORMAT_R8G8B8A8_SINT"; break;
	case VK_FORMAT_R8G8B8A8_SRGB: return "VK_FORMAT_R8G8B8A8_SRGB"; break; //dota2
	case VK_FORMAT_B8G8R8A8_UNORM: return "VK_FORMAT_B8G8R8A8_UNORM"; break;
	case VK_FORMAT_B8G8R8A8_SNORM: return "VK_FORMAT_B8G8R8A8_SNORM"; break;
	case VK_FORMAT_B8G8R8A8_USCALED: return "VK_FORMAT_B8G8R8A8_USCALED"; break;
	case VK_FORMAT_B8G8R8A8_SSCALED: return "VK_FORMAT_B8G8R8A8_SSCALED"; break;
	case VK_FORMAT_B8G8R8A8_UINT: return "VK_FORMAT_B8G8R8A8_UINT"; break;
	case VK_FORMAT_B8G8R8A8_SINT: return "VK_FORMAT_B8G8R8A8_SINT"; break;
	case VK_FORMAT_B8G8R8A8_SRGB: return "VK_FORMAT_B8G8R8A8_SRGB"; break;
	case VK_FORMAT_A8B8G8R8_UNORM_PACK32: return "VK_FORMAT_A8B8G8R8_UNORM_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_SNORM_PACK32: return "VK_FORMAT_A8B8G8R8_SNORM_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_USCALED_PACK32: return "VK_FORMAT_A8B8G8R8_USCALED_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: return "VK_FORMAT_A8B8G8R8_SSCALED_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_UINT_PACK32: return "VK_FORMAT_A8B8G8R8_UINT_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_SINT_PACK32: return "VK_FORMAT_A8B8G8R8_SINT_PACK32"; break;
	case VK_FORMAT_A8B8G8R8_SRGB_PACK32: return "VK_FORMAT_A8B8G8R8_SRGB_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_UNORM_PACK32: return "VK_FORMAT_A2R10G10B10_UNORM_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_SNORM_PACK32: return "VK_FORMAT_A2R10G10B10_SNORM_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_USCALED_PACK32: return "VK_FORMAT_A2R10G10B10_USCALED_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: return "VK_FORMAT_A2R10G10B10_SSCALED_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_UINT_PACK32: return "VK_FORMAT_A2R10G10B10_UINT_PACK32"; break;
	case VK_FORMAT_A2R10G10B10_SINT_PACK32: return "VK_FORMAT_A2R10G10B10_SINT_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return "VK_FORMAT_A2B10G10R10_UNORM_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_SNORM_PACK32: return "VK_FORMAT_A2B10G10R10_SNORM_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_USCALED_PACK32: return "VK_FORMAT_A2B10G10R10_USCALED_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: return "VK_FORMAT_A2B10G10R10_SSCALED_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_UINT_PACK32: return "VK_FORMAT_A2B10G10R10_UINT_PACK32"; break;
	case VK_FORMAT_A2B10G10R10_SINT_PACK32: return "VK_FORMAT_A2B10G10R10_SINT_PACK32"; break;
	case VK_FORMAT_R16_UNORM: return "VK_FORMAT_R16_UNORM"; break;
	case VK_FORMAT_R16_SNORM: return "VK_FORMAT_R16_SNORM"; break;
	case VK_FORMAT_R16_USCALED: return "VK_FORMAT_R16_USCALED"; break;
	case VK_FORMAT_R16_SSCALED: return "VK_FORMAT_R16_SSCALED"; break;
	case VK_FORMAT_R16_UINT: return "VK_FORMAT_R16_UINT"; break;
	case VK_FORMAT_R16_SINT: return "VK_FORMAT_R16_SINT"; break;
	case VK_FORMAT_R16_SFLOAT: return "VK_FORMAT_R16_SFLOAT"; break;
	case VK_FORMAT_R16G16_UNORM: return "VK_FORMAT_R16G16_UNORM"; break;
	case VK_FORMAT_R16G16_SNORM: return "VK_FORMAT_R16G16_SNORM"; break;
	case VK_FORMAT_R16G16_USCALED: return "VK_FORMAT_R16G16_USCALED"; break;
	case VK_FORMAT_R16G16_SSCALED: return "VK_FORMAT_R16G16_SSCALED"; break;
	case VK_FORMAT_R16G16_UINT: return "VK_FORMAT_R16G16_UINT"; break;
	case VK_FORMAT_R16G16_SINT: return "VK_FORMAT_R16G16_SINT"; break;
	case VK_FORMAT_R16G16_SFLOAT: return "VK_FORMAT_R16G16_SFLOAT"; break;
	case VK_FORMAT_R16G16B16_UNORM: return "VK_FORMAT_R16G16B16_UNORM"; break;
	case VK_FORMAT_R16G16B16_SNORM: return "VK_FORMAT_R16G16B16_SNORM"; break;
	case VK_FORMAT_R16G16B16_USCALED: return "VK_FORMAT_R16G16B16_USCALED"; break;
	case VK_FORMAT_R16G16B16_SSCALED: return "VK_FORMAT_R16G16B16_SSCALED"; break;
	case VK_FORMAT_R16G16B16_UINT: return "VK_FORMAT_R16G16B16_UINT"; break;
	case VK_FORMAT_R16G16B16_SINT: return "VK_FORMAT_R16G16B16_SINT"; break;
	case VK_FORMAT_R16G16B16_SFLOAT: return "VK_FORMAT_R16G16B16_SFLOAT"; break;
	case VK_FORMAT_R16G16B16A16_UNORM: return "VK_FORMAT_R16G16B16A16_UNORM"; break;
	case VK_FORMAT_R16G16B16A16_SNORM: return "VK_FORMAT_R16G16B16A16_SNORM"; break;
	case VK_FORMAT_R16G16B16A16_USCALED: return "VK_FORMAT_R16G16B16A16_USCALED"; break;
	case VK_FORMAT_R16G16B16A16_SSCALED: return "VK_FORMAT_R16G16B16A16_SSCALED"; break;
	case VK_FORMAT_R16G16B16A16_UINT: return "VK_FORMAT_R16G16B16A16_UINT"; break;
	case VK_FORMAT_R16G16B16A16_SINT: return "VK_FORMAT_R16G16B16A16_SINT"; break;
	case VK_FORMAT_R16G16B16A16_SFLOAT: return "VK_FORMAT_R16G16B16A16_SFLOAT"; break;
	case VK_FORMAT_R32_UINT: return "VK_FORMAT_R32_UINT"; break;
	case VK_FORMAT_R32_SINT: return "VK_FORMAT_R32_SINT"; break;
	case VK_FORMAT_R32_SFLOAT: return "VK_FORMAT_R32_SFLOAT"; break;
	case VK_FORMAT_R32G32_UINT: return "VK_FORMAT_R32G32_UINT"; break;
	case VK_FORMAT_R32G32_SINT: return "VK_FORMAT_R32G32_SINT"; break;
	case VK_FORMAT_R32G32_SFLOAT: return "VK_FORMAT_R32G32_SFLOAT"; break;
	case VK_FORMAT_R32G32B32_UINT: return "VK_FORMAT_R32G32B32_UINT"; break;
	case VK_FORMAT_R32G32B32_SINT: return "VK_FORMAT_R32G32B32_SINT"; break;
	case VK_FORMAT_R32G32B32_SFLOAT: return "VK_FORMAT_R32G32B32_SFLOAT"; break;
	case VK_FORMAT_R32G32B32A32_UINT: return "VK_FORMAT_R32G32B32A32_UINT"; break;
	case VK_FORMAT_R32G32B32A32_SINT: return "VK_FORMAT_R32G32B32A32_SINT"; break;
	case VK_FORMAT_R32G32B32A32_SFLOAT: return "VK_FORMAT_R32G32B32A32_SFLOAT"; break;
	case VK_FORMAT_R64_UINT: return "VK_FORMAT_R64_UINT"; break;
	case VK_FORMAT_R64_SINT: return "VK_FORMAT_R64_SINT"; break;
	case VK_FORMAT_R64_SFLOAT: return "VK_FORMAT_R64_SFLOAT"; break;
	case VK_FORMAT_R64G64_UINT: return "VK_FORMAT_R64G64_UINT"; break;
	case VK_FORMAT_R64G64_SINT: return "VK_FORMAT_R64G64_SINT"; break;
	case VK_FORMAT_R64G64_SFLOAT: return "VK_FORMAT_R64G64_SFLOAT"; break;
	case VK_FORMAT_R64G64B64_UINT: return "VK_FORMAT_R64G64B64_UINT"; break;
	case VK_FORMAT_R64G64B64_SINT: return "VK_FORMAT_R64G64B64_SINT"; break;
	case VK_FORMAT_R64G64B64_SFLOAT: return "VK_FORMAT_R64G64B64_SFLOAT"; break;
	case VK_FORMAT_R64G64B64A64_UINT: return "VK_FORMAT_R64G64B64A64_UINT"; break;
	case VK_FORMAT_R64G64B64A64_SINT: return "VK_FORMAT_R64G64B64A64_SINT"; break;
	case VK_FORMAT_R64G64B64A64_SFLOAT: return "VK_FORMAT_R64G64B64A64_SFLOAT"; break;
	case VK_FORMAT_B10G11R11_UFLOAT_PACK32: return "VK_FORMAT_B10G11R11_UFLOAT_PACK32"; break;
	case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: return "VK_FORMAT_E5B9G9R9_UFLOAT_PACK32"; break;
	case VK_FORMAT_D16_UNORM: return "VK_FORMAT_D16_UNORM"; break;
	case VK_FORMAT_X8_D24_UNORM_PACK32: return "VK_FORMAT_X8_D24_UNORM_PACK32"; break;
	case VK_FORMAT_D32_SFLOAT: return "VK_FORMAT_D32_SFLOAT"; break;
	case VK_FORMAT_S8_UINT: return "VK_FORMAT_S8_UINT"; break;
	case VK_FORMAT_D16_UNORM_S8_UINT: return "VK_FORMAT_D16_UNORM_S8_UINT"; break;
	case VK_FORMAT_D24_UNORM_S8_UINT: return "VK_FORMAT_D24_UNORM_S8_UINT"; break;
	case VK_FORMAT_D32_SFLOAT_S8_UINT: return "VK_FORMAT_D32_SFLOAT_S8_UINT"; break;
	case VK_FORMAT_BC1_RGB_UNORM_BLOCK: return "VK_FORMAT_BC1_RGB_UNORM_BLOCK"; break;
	case VK_FORMAT_BC1_RGB_SRGB_BLOCK: return "VK_FORMAT_BC1_RGB_SRGB_BLOCK"; break;
	case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: return "VK_FORMAT_BC1_RGBA_UNORM_BLOCK"; break;
	case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: return "VK_FORMAT_BC1_RGBA_SRGB_BLOCK"; break;
	case VK_FORMAT_BC2_UNORM_BLOCK: return "VK_FORMAT_BC2_UNORM_BLOCK"; break;
	case VK_FORMAT_BC2_SRGB_BLOCK: return "VK_FORMAT_BC2_SRGB_BLOCK"; break;
	case VK_FORMAT_BC3_UNORM_BLOCK: return "VK_FORMAT_BC3_UNORM_BLOCK"; break;
	case VK_FORMAT_BC3_SRGB_BLOCK: return "VK_FORMAT_BC3_SRGB_BLOCK"; break;
	case VK_FORMAT_BC4_UNORM_BLOCK: return "VK_FORMAT_BC4_UNORM_BLOCK"; break;
	case VK_FORMAT_BC4_SNORM_BLOCK: return "VK_FORMAT_BC4_SNORM_BLOCK"; break;
	case VK_FORMAT_BC5_UNORM_BLOCK: return "VK_FORMAT_BC5_UNORM_BLOCK"; break;
	case VK_FORMAT_BC5_SNORM_BLOCK: return "VK_FORMAT_BC5_SNORM_BLOCK"; break;
	case VK_FORMAT_BC6H_UFLOAT_BLOCK: return "VK_FORMAT_BC6H_UFLOAT_BLOCK"; break;
	case VK_FORMAT_BC6H_SFLOAT_BLOCK: return "VK_FORMAT_BC6H_SFLOAT_BLOCK"; break;
	case VK_FORMAT_BC7_UNORM_BLOCK: return "VK_FORMAT_BC7_UNORM_BLOCK"; break;
	case VK_FORMAT_BC7_SRGB_BLOCK: return "VK_FORMAT_BC7_SRGB_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK"; break;
	case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: return "VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK"; break;
	case VK_FORMAT_EAC_R11_UNORM_BLOCK: return "VK_FORMAT_EAC_R11_UNORM_BLOCK"; break;
	case VK_FORMAT_EAC_R11_SNORM_BLOCK: return "VK_FORMAT_EAC_R11_SNORM_BLOCK"; break;
	case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: return "VK_FORMAT_EAC_R11G11_UNORM_BLOCK"; break;
	case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: return "VK_FORMAT_EAC_R11G11_SNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: return "VK_FORMAT_ASTC_4x4_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: return "VK_FORMAT_ASTC_4x4_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: return "VK_FORMAT_ASTC_5x4_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: return "VK_FORMAT_ASTC_5x4_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_5x5_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_5x5_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_6x5_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_6x5_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_6x6_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_6x6_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x5_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x5_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x6_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x6_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: return "VK_FORMAT_ASTC_8x8_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: return "VK_FORMAT_ASTC_8x8_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x5_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x5_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x6_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x6_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x8_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x8_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: return "VK_FORMAT_ASTC_10x10_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: return "VK_FORMAT_ASTC_10x10_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: return "VK_FORMAT_ASTC_12x10_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: return "VK_FORMAT_ASTC_12x10_SRGB_BLOCK"; break;
	case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: return "VK_FORMAT_ASTC_12x12_UNORM_BLOCK"; break;
	case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: return "VK_FORMAT_ASTC_12x12_SRGB_BLOCK"; break;
	case VK_FORMAT_G8B8G8R8_422_UNORM: return "VK_FORMAT_G8B8G8R8_422_UNORM"; break;
	case VK_FORMAT_B8G8R8G8_422_UNORM: return "VK_FORMAT_B8G8R8G8_422_UNORM"; break;
	case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM"; break;
	case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM: return "VK_FORMAT_G8_B8R8_2PLANE_420_UNORM"; break;
	case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM"; break;
	case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM: return "VK_FORMAT_G8_B8R8_2PLANE_422_UNORM"; break;
	case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM: return "VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM"; break;
	case VK_FORMAT_R10X6_UNORM_PACK16: return "VK_FORMAT_R10X6_UNORM_PACK16"; break;
	case VK_FORMAT_R10X6G10X6_UNORM_2PACK16: return "VK_FORMAT_R10X6G10X6_UNORM_2PACK16"; break;
	case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16: return "VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16"; break;
	case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: return "VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16"; break;
	case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: return "VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16"; break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16"; break;
	case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16"; break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16"; break;
	case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16"; break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16"; break;
	case VK_FORMAT_R12X4_UNORM_PACK16: return "VK_FORMAT_R12X4_UNORM_PACK16"; break;
	case VK_FORMAT_R12X4G12X4_UNORM_2PACK16: return "VK_FORMAT_R12X4G12X4_UNORM_2PACK16"; break;
	case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16: return "VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16"; break;
	case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: return "VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16"; break;
	case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: return "VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16"; break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16"; break;
	case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16"; break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16"; break;
	case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16"; break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: return "VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16"; break;
	case VK_FORMAT_G16B16G16R16_422_UNORM: return "VK_FORMAT_G16B16G16R16_422_UNORM"; break;
	case VK_FORMAT_B16G16R16G16_422_UNORM: return "VK_FORMAT_B16G16R16G16_422_UNORM"; break;
	case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM"; break;
	case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM: return "VK_FORMAT_G16_B16R16_2PLANE_420_UNORM"; break;
	case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM"; break;
	case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM: return "VK_FORMAT_G16_B16R16_2PLANE_422_UNORM"; break;
	case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM: return "VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM"; break;
	case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: return "VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG"; break;
	case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: return "VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG"; break;
	}
	return "VK_FORMAT_UNDEFINED";
}

const char * VkResultString(VkResult result) {
	switch (result) {
	case VK_SUCCESS: return "VK_SUCCESS"; break;
	case VK_NOT_READY: return "VK_NOT_READY"; break;
	case VK_TIMEOUT: return "VK_TIMEOUT"; break;
	case VK_EVENT_SET: return "VK_EVENT_SET"; break;
	case VK_EVENT_RESET: return "VK_EVENT_RESET"; break;
	case VK_INCOMPLETE: return "VK_INCOMPLETE"; break;
	case VK_ERROR_OUT_OF_HOST_MEMORY: return "VK_ERROR_OUT_OF_HOST_MEMORY"; break;
	case VK_ERROR_OUT_OF_DEVICE_MEMORY: return "VK_ERROR_OUT_OF_DEVICE_MEMORY"; break;
	case VK_ERROR_INITIALIZATION_FAILED: return "VK_ERROR_INITIALIZATION_FAILED"; break;
	case VK_ERROR_DEVICE_LOST: return "VK_ERROR_DEVICE_LOST"; break;
	case VK_ERROR_MEMORY_MAP_FAILED: return "VK_ERROR_MEMORY_MAP_FAILED"; break;
	case VK_ERROR_LAYER_NOT_PRESENT: return "VK_ERROR_LAYER_NOT_PRESENT"; break;
	case VK_ERROR_EXTENSION_NOT_PRESENT: return "VK_ERROR_EXTENSION_NOT_PRESENT"; break;
	case VK_ERROR_FEATURE_NOT_PRESENT: return "VK_ERROR_FEATURE_NOT_PRESENT"; break;
	case VK_ERROR_INCOMPATIBLE_DRIVER: return "VK_ERROR_INCOMPATIBLE_DRIVER"; break;
	case VK_ERROR_TOO_MANY_OBJECTS: return "VK_ERROR_TOO_MANY_OBJECTS"; break;
	case VK_ERROR_FORMAT_NOT_SUPPORTED: return "VK_ERROR_FORMAT_NOT_SUPPORTED"; break;
	case VK_ERROR_FRAGMENTED_POOL: return "VK_ERROR_FRAGMENTED_POOL"; break;
	case VK_ERROR_OUT_OF_POOL_MEMORY: return "VK_ERROR_OUT_OF_POOL_MEMORY"; break;
	case VK_ERROR_INVALID_EXTERNAL_HANDLE: return "VK_ERROR_INVALID_EXTERNAL_HANDLE"; break;
	case VK_ERROR_SURFACE_LOST_KHR: return "VK_ERROR_SURFACE_LOST_KHR"; break;
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR: return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR"; break;
	case VK_SUBOPTIMAL_KHR: return "VK_SUBOPTIMAL_KHR"; break;
	case VK_ERROR_OUT_OF_DATE_KHR: return "VK_ERROR_OUT_OF_DATE_KHR"; break;
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR: return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR"; break;
	case VK_ERROR_VALIDATION_FAILED_EXT: return "VK_ERROR_VALIDATION_FAILED_EXT"; break;
	case VK_ERROR_INVALID_SHADER_NV: return "VK_ERROR_INVALID_SHADER_NV"; break;
	case VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT: return "VK_ERROR_INVALID_DRM_FORMAT_MODIFIER_PLANE_LAYOUT_EXT"; break;
	case VK_ERROR_FRAGMENTATION_EXT: return "VK_ERROR_FRAGMENTATION_EXT"; break;
	case VK_ERROR_NOT_PERMITTED_EXT: return "VK_ERROR_NOT_PERMITTED_EXT"; break;
	case VK_ERROR_INVALID_DEVICE_ADDRESS_EXT: return "VK_ERROR_INVALID_DEVICE_ADDRESS_EXT"; break;
	case VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT: return "VK_ERROR_FULL_SCREEN_EXCLUSIVE_MODE_LOST_EXT"; break;
		//case VK_ERROR_OUT_OF_POOL_MEMORY_KHR: return "VK_ERROR_OUT_OF_POOL_MEMORY_KHR"; break;
		//case VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR: return "VK_ERROR_INVALID_EXTERNAL_HANDLE_KHR"; break;
		//case VK_RESULT_BEGIN_RANGE: return "VK_RESULT_BEGIN_RANGE"; break;
		//case VK_RESULT_END_RANGE: return "VK_RESULT_END_RANGE"; break;
	case VK_RESULT_RANGE_SIZE: return "VK_RESULT_RANGE_SIZE"; break;
	case VK_RESULT_MAX_ENUM: return "VK_RESULT_MAX_ENUM"; break;
	default: return "UNKNOWN VK_RESULT"; break;
	}
}

#ifdef DEBUG_PRINT
#include <stdio.h>
#define DbgOut(x) OutputDebugStringA(x)
#define DbgOut1(x,y)				\
	{					\
		char string[256];		\
		sprintf(string, x, y);		\
		DbgOut(string);			\
	}					\

#define DbgOut2(x,y,z)				\
	{					\
		char string[256];		\
		sprintf(string, x, y, z);	\
		DbgOut(string);			\
	}					\

#if defined( DEBUG_PRINT_PROCADDR )
#define DbgOutProcAddr(x, y, z) DbgOut2(x, y, z)
#else
#define DbgOutProcAddr(x, y, z)
#endif
#define DbgOutRes(x,y)						\
	{							\
		char string[256];				\
		sprintf(string, x, VkResultString(y));		\
		DbgOut(string);					\
	}							\

#else
#define DbgOut(x)
#define DbgOut1(x,y)
#define DbgOut2(x,y,z)
#define DbgOutProcAddr(x, y, z)
#define DbgOutRes(x,y)
#endif


#define MAX_INSTANCE_COUNT 16
#define MAX_SURFACE_PER_INSTANCE 16
#define MAX_DEVICE_COUNT 16
#define MAX_SWAPCHAIN_PER_DEVICE 16
#define MAX_PHYSICALDEVICE_COUNT 16
#define MAX_IMAGES_PER_SWAPCHAIN 16

static BOOL initialized = FALSE;
static BOOL hooked = FALSE;
CRITICAL_SECTION mutex;

// use the loader's dispatch table pointer as a key for dispatch map lookups
#define TOKEY(x) (*(void **)x)

typedef struct swapchainData {
	VkSwapchainKHR			swapchain;
	VkExtent2D			imageExtent;
	VkFormat			imageFormat;
	VkSurfaceKHR			surface;
	VkImage				exportedImage;
	VkDeviceMemory			exportedImageMemory;
	HANDLE				handle;
	struct shtex_data *		shtex_info;
	ID3D11Texture2D *		d3d11_tex;
	BOOL				sharedTextureCaptured;
	VkImage				swapchainImages[MAX_IMAGES_PER_SWAPCHAIN];
}swapchainData;

typedef struct deviceData {
	VkLayerDispatchTable		dispatchTable;
	VkPhysicalDevice		physicalDevice;
	VkDevice			device;
	swapchainData			swapchains[MAX_SWAPCHAIN_PER_DEVICE];

	uint32_t			queueFamilyIdx;
	VkCommandPool			cmdPool;
	VkCommandBuffer			cmdBuffer;
	VkQueue				queue;
	VkSemaphore			semaphore;
	VkExternalMemoryPropertiesKHR	externalMemoryProperties;

	ID3D11Device *			d3d11_device;
	ID3D11DeviceContext *		d3d11_context;
	IDXGISwapChain *		dxgi_swap;
	HWND				dummy_hwnd;

}deviceData;

swapchainData* GetSwapchainData(deviceData* devData, VkSwapchainKHR swapchain) {
	for (int i = 0; i < MAX_SWAPCHAIN_PER_DEVICE; ++i) {
		if (devData->swapchains[i].swapchain == swapchain) {
			return &devData->swapchains[i];
		}
	}
	DbgOut("# OBS_Layer # GetSwapchainData failed, swapchain not found\n");
	return NULL;
}

swapchainData* GetNewSwapchainData(deviceData* devData) {
	for (int i = 0; i < MAX_SWAPCHAIN_PER_DEVICE; ++i) {
		if (devData->swapchains[i].surface == NULL && devData->swapchains[i].swapchain == NULL) {
			return &devData->swapchains[i];
		}
	}
	DbgOut("# OBS_Layer # GetNewSwapchainData failed, no more free slot\n");
	return NULL;
}

// devices storage : devices/deviceTable share the same index
// maintain those on the leading deviceCount elements
deviceData deviceTable[MAX_DEVICE_COUNT];
void*  devices[MAX_DEVICE_COUNT];
uint8_t deviceCount;

uint8_t GetDeviceIndex(VkDevice* dev) {
	for (uint8_t i = 0; i < deviceCount; ++i) {
		if (devices[i] == dev) {
			return i;
		}
	}
	return UINT8_MAX;
};

deviceData* GetDeviceData(void* dev) {
	EnterCriticalSection(&mutex);

	deviceData* devData = NULL;
	uint8_t idx = GetDeviceIndex(dev);
	if (idx < deviceCount) {
		devData = &deviceTable[idx];
	}
	else if (deviceCount >= MAX_DEVICE_COUNT - 1) {
		DbgOut("# OBS_Layer # Out of Device Slot\n");
	}
	else {

		deviceData* newDeviceData = &deviceTable[deviceCount];
		devices[deviceCount] = dev;
		deviceCount++;
		devData = newDeviceData;
	}
	LeaveCriticalSection(&mutex);
	return devData;
}

VkLayerDispatchTable* GetDeviceDispatchTable(void* dev) {

	deviceData* devData = GetDeviceData(dev);
	return &devData->dispatchTable;
}

void RemoveDevice(void* dev) {
	EnterCriticalSection(&mutex);
	uint8_t idx = GetDeviceIndex(dev);
	deviceData* devData = (deviceData*)(&devices[idx]);
	
	for (int i = 0; i < MAX_SWAPCHAIN_PER_DEVICE; ++i) {
		swapchainData * swchData = &devData->swapchains[i];

		if(swchData->exportedImage)
			devData->dispatchTable.DestroyImage(devData->device, swchData->exportedImage, NULL);

		if (swchData->exportedImageMemory)
			devData->dispatchTable.FreeMemory(devData->device, swchData->exportedImageMemory, NULL);

		swchData->handle = INVALID_HANDLE_VALUE;
		swchData->swapchain = VK_NULL_HANDLE;
		swchData->surface = NULL;

		if (swchData->d3d11_tex)
			ID3D11Resource_Release(swchData->d3d11_tex);

		swchData->sharedTextureCaptured = 0;
	}

	if (devData->d3d11_context)
		ID3D11DeviceContext_Release(devData->d3d11_context);
	if (devData->d3d11_device)
		ID3D11Device_Release(devData->d3d11_device);
	if (devData->dxgi_swap)
		IDXGISwapChain_Release(devData->dxgi_swap);

	if (idx > 0 && idx < deviceCount - 1) { //not the last, move the last at this new location
		devices[idx] = devices[deviceCount - 1];
		memcpy(&deviceTable[idx], &deviceTable[deviceCount - 1], sizeof(deviceData));
	}

	devices[deviceCount - 1] = NULL;
	memset(&deviceTable[deviceCount - 1], 0, sizeof(deviceData));
	deviceCount--;
	LeaveCriticalSection(&mutex);
}

typedef struct surfaceData {
	VkSurfaceKHR surface;
	HINSTANCE hinstance;
	HWND hwnd;
}surfaceData;

typedef struct instanceData {
	VkLayerInstanceDispatchTable dispatchTable;
	uint32_t physicalDeviceCount;
	VkPhysicalDevice* availablePhysicalDevices[MAX_PHYSICALDEVICE_COUNT];

	surfaceData surfaces[MAX_SURFACE_PER_INSTANCE];
}instanceData;

surfaceData* FindSurfaceData(instanceData* instData, VkSurfaceKHR surface) {
	int firstFreeSlot = MAX_SURFACE_PER_INSTANCE;
	for (int i = 0; i < MAX_SURFACE_PER_INSTANCE; ++i) {
		if (instData->surfaces[i].surface == surface) {
			return &instData->surfaces[i];
		} else if (instData->surfaces[i].surface == NULL && firstFreeSlot == MAX_SWAPCHAIN_PER_DEVICE) {
			firstFreeSlot = i;
		}
	}
	if (firstFreeSlot != MAX_SWAPCHAIN_PER_DEVICE) {
		instData->surfaces[firstFreeSlot].surface = surface;
		return &instData->surfaces[firstFreeSlot];
	}

	DbgOut("# OBS_Layer # FindSurfaceData failed, no more free slot\n");
	return NULL;
}


// instances level disptach table storage : instanceKeys/instanceTable share the same index
// maintain those on the leading instanceCount elements
instanceData instanceTable[MAX_INSTANCE_COUNT];
void* instanceKeys[MAX_INSTANCE_COUNT];
uint8_t instanceCount;

uint8_t GetInstanceIndex(void* inst) {
	for (uint8_t i = 0; i < instanceCount; ++i) {
		if (instanceKeys[i] == inst) {
			return i;
		}
	}
	return UINT8_MAX;
};

instanceData* GetInstanceData(void* inst) {
	EnterCriticalSection(&mutex);

	instanceData* instData = NULL;
	uint8_t idx = GetInstanceIndex(inst);
	if (idx < instanceCount) {
		instData = &instanceTable[idx];
	}
	else if (instanceCount >= MAX_INSTANCE_COUNT - 1) {
		DbgOut("# OBS_Layer # Out of Instance Slot\n");
	}
	else {

		instanceData* newInstanceData = &instanceTable[instanceCount];
		instanceKeys[instanceCount] = inst;
		instanceCount++;
		instData = newInstanceData;
	}
	LeaveCriticalSection(&mutex);
	return instData;
}

VkLayerInstanceDispatchTable* GetInstanceDispatchTable(void* inst) {
	instanceData* instData = GetInstanceData(inst);
	return &instData->dispatchTable;
}

void RemoveInstance(void* inst) {

	EnterCriticalSection(&mutex);
	uint8_t idx = GetInstanceIndex(inst);

	if (idx>0 && idx < instanceCount-1) { //not the last, move the last at this new location
		instanceKeys[idx] = instanceKeys[instanceCount - 1];
		memcpy(&instanceTable[idx], &instanceTable[instanceCount - 1], sizeof(instanceData));
	}

	instanceKeys[instanceCount - 1] = NULL;
	memset(&instanceTable[instanceCount - 1], 0, sizeof(instanceData));
	instanceCount--;
	LeaveCriticalSection(&mutex);
}


#define DUMMY_WINDOW_CLASS_NAME L"graphics_hook_vk_dummy_window"
/* clang-format off */

static const GUID GUID_IDXGIFactory1 =
{ 0x770aae78, 0xf26f, 0x4dba, {0xa8, 0x29, 0x25, 0x3c, 0x83, 0xd1, 0xb3, 0x87} };
static const GUID GUID_IDXGIResource =
{ 0x035f3ab4, 0x482e, 0x4e50, {0xb4, 0x1f, 0x8a, 0x7f, 0x8b, 0xd8, 0x96, 0x0b} };

/* clang-format on */

static inline bool vk_shtex_init_window(deviceData * data)
{
	data->dummy_hwnd = CreateWindowExW(
		0, DUMMY_WINDOW_CLASS_NAME, L"Dummy VK window, ignore",
		WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS, 0, 0, 2, 2, NULL,
		NULL, GetModuleHandle(NULL), NULL);
	if (!data->dummy_hwnd) {
		hlog("vk_shtex_init_window: failed to create window: %d",
			GetLastError());
		return false;
	}

	return true;
}

typedef HRESULT(WINAPI *create_dxgi_factory1_t)(REFIID, void **);

static const D3D_FEATURE_LEVEL feature_levels[] = {
	D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
};

static inline bool vk_shtex_init_d3d11(deviceData * data)
{
	D3D_FEATURE_LEVEL level_used;
	IDXGIFactory1 *factory;
	IDXGIAdapter *adapter;
	HRESULT hr;

	HMODULE d3d11 = load_system_library("d3d11.dll");
	if (!d3d11) {
		hlog("vk_shtex_init_d3d11: failed to load D3D11.dll: %d",
			GetLastError());
		return false;
	}

	HMODULE dxgi = load_system_library("dxgi.dll");
	if (!dxgi) {
		hlog("vk_shtex_init_d3d11: failed to load DXGI.dll: %d",
			GetLastError());
		return false;
	}

	DXGI_SWAP_CHAIN_DESC desc = { 0 };
	desc.BufferCount = 2;
	desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.BufferDesc.Width = 2;
	desc.BufferDesc.Height = 2;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.Windowed = true;
	desc.OutputWindow = data->dummy_hwnd;

	create_dxgi_factory1_t create_factory =
		(void *)GetProcAddress(dxgi, "CreateDXGIFactory1");
	if (!create_factory) {
		hlog("vk_shtex_init_d3d11: failed to load CreateDXGIFactory1 "
			"procedure: %d",
			GetLastError());
		return false;
	}

	PFN_D3D11_CREATE_DEVICE_AND_SWAP_CHAIN create =
		(void *)GetProcAddress(d3d11, "D3D11CreateDeviceAndSwapChain");
	if (!create) {
		hlog("vk_shtex_init_d3d11: failed to load "
			"D3D11CreateDeviceAndSwapChain procedure: %d",
			GetLastError());
		return false;
	}

	hr = create_factory(&GUID_IDXGIFactory1, (void **)&factory);
	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11: failed to create factory", hr);
		return false;
	}

	hr = IDXGIFactory1_EnumAdapters1(factory, 0,
		(IDXGIAdapter1 **)&adapter);
	IDXGIFactory1_Release(factory);

	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11: failed to create adapter", hr);
		return false;
	}

	hr = create(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, feature_levels,
		sizeof(feature_levels) / sizeof(D3D_FEATURE_LEVEL),
		D3D11_SDK_VERSION, &desc, &data->dxgi_swap,
		&data->d3d11_device, &level_used, &data->d3d11_context);
	IDXGIAdapter_Release(adapter);

	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11: failed to create device", hr);
		return false;
	}

	return true;
}

DXGI_FORMAT GetNeededDXGIFormat(VkFormat vulkanFormat)
{
	//this is not a real format matching !
	//ex: we need to avoid stacking srg
	DXGI_FORMAT converted = DXGI_FORMAT_UNKNOWN;
	switch (vulkanFormat)
	{
	default:
	case VK_FORMAT_UNDEFINED: break;
	case VK_FORMAT_R4G4_UNORM_PACK8: break;
	case VK_FORMAT_R4G4B4A4_UNORM_PACK16: break;
	case VK_FORMAT_B4G4R4A4_UNORM_PACK16: converted = DXGI_FORMAT_B4G4R4A4_UNORM;  break;
	case VK_FORMAT_R5G6B5_UNORM_PACK16: break;
	case VK_FORMAT_B5G6R5_UNORM_PACK16: converted = DXGI_FORMAT_B5G6R5_UNORM; break;
	case VK_FORMAT_R5G5B5A1_UNORM_PACK16: break;
	case VK_FORMAT_B5G5R5A1_UNORM_PACK16: converted = DXGI_FORMAT_B5G5R5A1_UNORM; break;
	case VK_FORMAT_A1R5G5B5_UNORM_PACK16: break;
	case VK_FORMAT_R8_UNORM: converted = DXGI_FORMAT_R8_UNORM; break;
	case VK_FORMAT_R8_SNORM: converted = DXGI_FORMAT_R8_SNORM; break;
	case VK_FORMAT_R8_USCALED: break;
	case VK_FORMAT_R8_SSCALED: break;
	case VK_FORMAT_R8_UINT: converted = DXGI_FORMAT_R8_UINT; break;
	case VK_FORMAT_R8_SINT: converted = DXGI_FORMAT_R8_SINT; break;
	case VK_FORMAT_R8_SRGB: break;
	case VK_FORMAT_R8G8_UNORM: converted = DXGI_FORMAT_R8G8_UNORM; break;
	case VK_FORMAT_R8G8_SNORM: converted = DXGI_FORMAT_R8G8_SNORM; break;
	case VK_FORMAT_R8G8_USCALED: break;
	case VK_FORMAT_R8G8_SSCALED: break;
	case VK_FORMAT_R8G8_UINT: converted = DXGI_FORMAT_R8G8_UINT; break;
	case VK_FORMAT_R8G8_SINT: converted = DXGI_FORMAT_R8G8_UINT; break;
	case VK_FORMAT_R8G8_SRGB: break;
	case VK_FORMAT_R8G8B8_UNORM: break;
	case VK_FORMAT_R8G8B8_SNORM: break;
	case VK_FORMAT_R8G8B8_USCALED: break;
	case VK_FORMAT_R8G8B8_SSCALED: break;
	case VK_FORMAT_R8G8B8_UINT: break;
	case VK_FORMAT_R8G8B8_SINT: break;
	case VK_FORMAT_R8G8B8_SRGB: break;
	case VK_FORMAT_B8G8R8_UNORM: break;
	case VK_FORMAT_B8G8R8_SNORM: break;
	case VK_FORMAT_B8G8R8_USCALED: break;
	case VK_FORMAT_B8G8R8_SSCALED: break;
	case VK_FORMAT_B8G8R8_UINT: break;
	case VK_FORMAT_B8G8R8_SINT: break;
	case VK_FORMAT_B8G8R8_SRGB: break;
	case VK_FORMAT_R8G8B8A8_UNORM: converted = DXGI_FORMAT_R8G8B8A8_UNORM; break;
	case VK_FORMAT_R8G8B8A8_SNORM: converted = DXGI_FORMAT_R8G8B8A8_SNORM; break;
	case VK_FORMAT_R8G8B8A8_USCALED: break;
	case VK_FORMAT_R8G8B8A8_SSCALED: break;
	case VK_FORMAT_R8G8B8A8_UINT: converted = DXGI_FORMAT_R8G8B8A8_UINT; break;
	case VK_FORMAT_R8G8B8A8_SINT: converted = DXGI_FORMAT_R8G8B8A8_SINT; break;
	case VK_FORMAT_R8G8B8A8_SRGB: converted = DXGI_FORMAT_R8G8B8A8_UNORM; break; //dota2
	case VK_FORMAT_B8G8R8A8_UNORM: break;
	case VK_FORMAT_B8G8R8A8_SNORM: break;
	case VK_FORMAT_B8G8R8A8_USCALED: break;
	case VK_FORMAT_B8G8R8A8_SSCALED: break;
	case VK_FORMAT_B8G8R8A8_UINT: break;
	case VK_FORMAT_B8G8R8A8_SINT: break;
	case VK_FORMAT_B8G8R8A8_SRGB: converted = DXGI_FORMAT_B8G8R8A8_UNORM; break;
	case VK_FORMAT_A8B8G8R8_UNORM_PACK32: break;
	case VK_FORMAT_A8B8G8R8_SNORM_PACK32: break;
	case VK_FORMAT_A8B8G8R8_USCALED_PACK32: break;
	case VK_FORMAT_A8B8G8R8_SSCALED_PACK32: break;
	case VK_FORMAT_A8B8G8R8_UINT_PACK32: break;
	case VK_FORMAT_A8B8G8R8_SINT_PACK32: break;
	case VK_FORMAT_A8B8G8R8_SRGB_PACK32: break;
	case VK_FORMAT_A2R10G10B10_UNORM_PACK32: converted = DXGI_FORMAT_R10G10B10A2_UNORM; break;
	case VK_FORMAT_A2R10G10B10_SNORM_PACK32: break;
	case VK_FORMAT_A2R10G10B10_USCALED_PACK32: break;
	case VK_FORMAT_A2R10G10B10_SSCALED_PACK32: break;
	case VK_FORMAT_A2R10G10B10_UINT_PACK32: converted = DXGI_FORMAT_R10G10B10A2_UINT; break;
	case VK_FORMAT_A2R10G10B10_SINT_PACK32: break;
	case VK_FORMAT_A2B10G10R10_UNORM_PACK32: converted = DXGI_FORMAT_R10G10B10A2_UNORM; break; //no man sky
	case VK_FORMAT_A2B10G10R10_SNORM_PACK32: break;
	case VK_FORMAT_A2B10G10R10_USCALED_PACK32: break;
	case VK_FORMAT_A2B10G10R10_SSCALED_PACK32: break;
	case VK_FORMAT_A2B10G10R10_UINT_PACK32: break;
	case VK_FORMAT_A2B10G10R10_SINT_PACK32: break;
	case VK_FORMAT_R16_UNORM: converted = DXGI_FORMAT_R16_UNORM; break;
	case VK_FORMAT_R16_SNORM: converted = DXGI_FORMAT_R16_SNORM; break;
	case VK_FORMAT_R16_USCALED: break;
	case VK_FORMAT_R16_SSCALED: break;
	case VK_FORMAT_R16_UINT: converted = DXGI_FORMAT_R16_UINT; break;
	case VK_FORMAT_R16_SINT: converted = DXGI_FORMAT_R16_SINT; break;
	case VK_FORMAT_R16_SFLOAT: converted = DXGI_FORMAT_R16_FLOAT; break;
	case VK_FORMAT_R16G16_UNORM: converted = DXGI_FORMAT_R16G16_UNORM; break;
	case VK_FORMAT_R16G16_SNORM: converted = DXGI_FORMAT_R16G16_SNORM; break;
	case VK_FORMAT_R16G16_USCALED: break;
	case VK_FORMAT_R16G16_SSCALED: break;
	case VK_FORMAT_R16G16_UINT: converted = DXGI_FORMAT_R16G16_UINT; break;
	case VK_FORMAT_R16G16_SINT: converted = DXGI_FORMAT_R16G16_SINT; break;
	case VK_FORMAT_R16G16_SFLOAT: converted = DXGI_FORMAT_R16G16_FLOAT; break;
	case VK_FORMAT_R16G16B16_UNORM: break;
	case VK_FORMAT_R16G16B16_SNORM: break;
	case VK_FORMAT_R16G16B16_USCALED: break;
	case VK_FORMAT_R16G16B16_SSCALED: break;
	case VK_FORMAT_R16G16B16_UINT: break;
	case VK_FORMAT_R16G16B16_SINT: break;
	case VK_FORMAT_R16G16B16_SFLOAT: break;
	case VK_FORMAT_R16G16B16A16_UNORM: converted = DXGI_FORMAT_R16G16B16A16_UNORM; break;
	case VK_FORMAT_R16G16B16A16_SNORM: converted = DXGI_FORMAT_R16G16B16A16_SNORM; break;
	case VK_FORMAT_R16G16B16A16_USCALED: break;
	case VK_FORMAT_R16G16B16A16_SSCALED: break;
	case VK_FORMAT_R16G16B16A16_UINT: converted = DXGI_FORMAT_R16G16B16A16_UINT; break;
	case VK_FORMAT_R16G16B16A16_SINT: converted = DXGI_FORMAT_R16G16B16A16_SINT; break;
	case VK_FORMAT_R16G16B16A16_SFLOAT: converted = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
	case VK_FORMAT_R32_UINT: converted = DXGI_FORMAT_R32_UINT; break;
	case VK_FORMAT_R32_SINT: converted = DXGI_FORMAT_R32_SINT; break;
	case VK_FORMAT_R32_SFLOAT: converted = DXGI_FORMAT_R32_FLOAT; break;
	case VK_FORMAT_R32G32_UINT: converted = DXGI_FORMAT_R32G32_UINT; break;
	case VK_FORMAT_R32G32_SINT: converted = DXGI_FORMAT_R32G32_SINT; break;
	case VK_FORMAT_R32G32_SFLOAT: converted = DXGI_FORMAT_R32G32_FLOAT; break;
	case VK_FORMAT_R32G32B32_UINT: converted = DXGI_FORMAT_R32G32B32_UINT; break;
	case VK_FORMAT_R32G32B32_SINT: converted = DXGI_FORMAT_R32G32B32_SINT; break;
	case VK_FORMAT_R32G32B32_SFLOAT: converted = DXGI_FORMAT_R32G32B32_FLOAT; break;
	case VK_FORMAT_R32G32B32A32_UINT: converted = DXGI_FORMAT_R32G32B32A32_UINT; break;
	case VK_FORMAT_R32G32B32A32_SINT: converted = DXGI_FORMAT_R32G32B32A32_SINT; break;
	case VK_FORMAT_R32G32B32A32_SFLOAT: converted = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
	case VK_FORMAT_R64_UINT: break;
	case VK_FORMAT_R64_SINT: break;
	case VK_FORMAT_R64_SFLOAT: break;
	case VK_FORMAT_R64G64_UINT: break;
	case VK_FORMAT_R64G64_SINT: break;
	case VK_FORMAT_R64G64_SFLOAT: break;
	case VK_FORMAT_R64G64B64_UINT: break;
	case VK_FORMAT_R64G64B64_SINT: break;
	case VK_FORMAT_R64G64B64_SFLOAT: break;
	case VK_FORMAT_R64G64B64A64_UINT: break;
	case VK_FORMAT_R64G64B64A64_SINT: break;
	case VK_FORMAT_R64G64B64A64_SFLOAT: break;
	case VK_FORMAT_B10G11R11_UFLOAT_PACK32: break;
	case VK_FORMAT_E5B9G9R9_UFLOAT_PACK32: break;
	case VK_FORMAT_D16_UNORM: break;
	case VK_FORMAT_X8_D24_UNORM_PACK32: break;
	case VK_FORMAT_D32_SFLOAT: break;
	case VK_FORMAT_S8_UINT: break;
	case VK_FORMAT_D16_UNORM_S8_UINT: break;
	case VK_FORMAT_D24_UNORM_S8_UINT: break;
	case VK_FORMAT_D32_SFLOAT_S8_UINT: break;
	case VK_FORMAT_BC1_RGB_UNORM_BLOCK: break;
	case VK_FORMAT_BC1_RGB_SRGB_BLOCK: break;
	case VK_FORMAT_BC1_RGBA_UNORM_BLOCK: break;
	case VK_FORMAT_BC1_RGBA_SRGB_BLOCK: break;
	case VK_FORMAT_BC2_UNORM_BLOCK: break;
	case VK_FORMAT_BC2_SRGB_BLOCK: break;
	case VK_FORMAT_BC3_UNORM_BLOCK: break;
	case VK_FORMAT_BC3_SRGB_BLOCK: break;
	case VK_FORMAT_BC4_UNORM_BLOCK: break;
	case VK_FORMAT_BC4_SNORM_BLOCK: break;
	case VK_FORMAT_BC5_UNORM_BLOCK: break;
	case VK_FORMAT_BC5_SNORM_BLOCK: break;
	case VK_FORMAT_BC6H_UFLOAT_BLOCK: break;
	case VK_FORMAT_BC6H_SFLOAT_BLOCK: break;
	case VK_FORMAT_BC7_UNORM_BLOCK: break;
	case VK_FORMAT_BC7_SRGB_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8_UNORM_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8_SRGB_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8A1_UNORM_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8A1_SRGB_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8A8_UNORM_BLOCK: break;
	case VK_FORMAT_ETC2_R8G8B8A8_SRGB_BLOCK: break;
	case VK_FORMAT_EAC_R11_UNORM_BLOCK: break;
	case VK_FORMAT_EAC_R11_SNORM_BLOCK: break;
	case VK_FORMAT_EAC_R11G11_UNORM_BLOCK: break;
	case VK_FORMAT_EAC_R11G11_SNORM_BLOCK: break;
	case VK_FORMAT_ASTC_4x4_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_4x4_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_5x4_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_5x4_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_5x5_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_5x5_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_6x5_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_6x5_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_6x6_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_6x6_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_8x5_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_8x5_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_8x6_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_8x6_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_8x8_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_8x8_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_10x5_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_10x5_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_10x6_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_10x6_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_10x8_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_10x8_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_10x10_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_10x10_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_12x10_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_12x10_SRGB_BLOCK: break;
	case VK_FORMAT_ASTC_12x12_UNORM_BLOCK: break;
	case VK_FORMAT_ASTC_12x12_SRGB_BLOCK: break;
	case VK_FORMAT_G8B8G8R8_422_UNORM: break;
	case VK_FORMAT_B8G8R8G8_422_UNORM: break;
	case VK_FORMAT_G8_B8_R8_3PLANE_420_UNORM: break;
	case VK_FORMAT_G8_B8R8_2PLANE_420_UNORM: break;
	case VK_FORMAT_G8_B8_R8_3PLANE_422_UNORM: break;
	case VK_FORMAT_G8_B8R8_2PLANE_422_UNORM: break;
	case VK_FORMAT_G8_B8_R8_3PLANE_444_UNORM: break;
	case VK_FORMAT_R10X6_UNORM_PACK16: break;
	case VK_FORMAT_R10X6G10X6_UNORM_2PACK16: break;
	case VK_FORMAT_R10X6G10X6B10X6A10X6_UNORM_4PACK16: break;
	case VK_FORMAT_G10X6B10X6G10X6R10X6_422_UNORM_4PACK16: break;
	case VK_FORMAT_B10X6G10X6R10X6G10X6_422_UNORM_4PACK16: break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_420_UNORM_3PACK16: break;
	case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_420_UNORM_3PACK16: break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_422_UNORM_3PACK16: break;
	case VK_FORMAT_G10X6_B10X6R10X6_2PLANE_422_UNORM_3PACK16: break;
	case VK_FORMAT_G10X6_B10X6_R10X6_3PLANE_444_UNORM_3PACK16: break;
	case VK_FORMAT_R12X4_UNORM_PACK16: break;
	case VK_FORMAT_R12X4G12X4_UNORM_2PACK16: break;
	case VK_FORMAT_R12X4G12X4B12X4A12X4_UNORM_4PACK16: break;
	case VK_FORMAT_G12X4B12X4G12X4R12X4_422_UNORM_4PACK16: break;
	case VK_FORMAT_B12X4G12X4R12X4G12X4_422_UNORM_4PACK16: break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_420_UNORM_3PACK16: break;
	case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_420_UNORM_3PACK16: break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_422_UNORM_3PACK16: break;
	case VK_FORMAT_G12X4_B12X4R12X4_2PLANE_422_UNORM_3PACK16: break;
	case VK_FORMAT_G12X4_B12X4_R12X4_3PLANE_444_UNORM_3PACK16: break;
	case VK_FORMAT_G16B16G16R16_422_UNORM: break;
	case VK_FORMAT_B16G16R16G16_422_UNORM: break;
	case VK_FORMAT_G16_B16_R16_3PLANE_420_UNORM: break;
	case VK_FORMAT_G16_B16R16_2PLANE_420_UNORM: break;
	case VK_FORMAT_G16_B16_R16_3PLANE_422_UNORM: break;
	case VK_FORMAT_G16_B16R16_2PLANE_422_UNORM: break;
	case VK_FORMAT_G16_B16_R16_3PLANE_444_UNORM: break;
	case VK_FORMAT_PVRTC1_2BPP_UNORM_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC1_4BPP_UNORM_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC2_2BPP_UNORM_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC2_4BPP_UNORM_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC1_2BPP_SRGB_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC1_4BPP_SRGB_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC2_2BPP_SRGB_BLOCK_IMG: break;
	case VK_FORMAT_PVRTC2_4BPP_SRGB_BLOCK_IMG: break;
	}
	if (converted == DXGI_FORMAT_UNKNOWN)
	{
		hlog("vk_shtex_init_d3d11_tex: unknown swapchain format, defaulting to B8G8R8A8_UNORM");
		converted = DXGI_FORMAT_B8G8R8A8_UNORM;
	}
	return converted;

}

static inline bool vk_shtex_init_d3d11_tex(deviceData * dev_data, swapchainData * swpch_data)
{
	IDXGIResource *dxgi_res;
	HRESULT hr;

	D3D11_TEXTURE2D_DESC desc = { 0 };
	desc.Width = swpch_data->imageExtent.width;
	desc.Height = swpch_data->imageExtent.height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;

	hlog("OBS requesting %s texture format", VkFormatString(swpch_data->imageFormat) );

	desc.Format = GetNeededDXGIFormat(swpch_data->imageFormat);// DXGI_FORMAT_B8G8R8A8_UNORM;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.MiscFlags = D3D11_RESOURCE_MISC_SHARED;
	desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;

	hr = ID3D11Device_CreateTexture2D(dev_data->d3d11_device, &desc, NULL,
		&swpch_data->d3d11_tex);
	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11_tex: failed to create texture",
			hr);
		return false;
	}

	hr = ID3D11Device_QueryInterface(swpch_data->d3d11_tex, &GUID_IDXGIResource,
		(void **)&dxgi_res);
	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11_tex: failed to get IDXGIResource",
			hr);
		return false;
	}

	hr = IDXGIResource_GetSharedHandle(dxgi_res, &swpch_data->handle);
	IDXGIResource_Release(dxgi_res);

	if (FAILED(hr)) {
		hlog_hr("vk_shtex_init_d3d11_tex: failed to get shared handle",
			hr);
		return false;
	}

	return true;
}

static inline bool vk_shtex_init_vulkan_tex(deviceData * devData, swapchainData * swpchData) {

	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	VkExternalMemoryImageCreateInfoKHR externalMemoryImageInfo;
	externalMemoryImageInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO_KHR;
	externalMemoryImageInfo.pNext = NULL;
	externalMemoryImageInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT;

	VkImageCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.pNext = &externalMemoryImageInfo;
	createInfo.flags = 0;			// VkImageCreateFlags       
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.format = swpchData->imageFormat; //VK_FORMAT_B8G8R8A8_UNORM;
	createInfo.extent.width = swpchData->imageExtent.width;
	createInfo.extent.height = swpchData->imageExtent.height;
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;			// VkImageTiling            
	createInfo.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT; //pCreateInfo->imageUsage;			// VkImageUsageFlags        
	createInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;// pCreateInfo->imageSharingMode;			// VkSharingMode            
	createInfo.queueFamilyIndexCount = 0;// pCreateInfo->queueFamilyIndexCount;	// uint32_t                 
	createInfo.pQueueFamilyIndices = 0;// pCreateInfo->pQueueFamilyIndices;		// const uint32_t*          
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	VkResult res;
	res = dispatchTable->CreateImage(devData->device, &createInfo, NULL, &swpchData->exportedImage);
	DbgOutRes("# OBS_Layer # CreateImage %s\n", res);

	VkExportMemoryAllocateInfo expMemAllocInfo;
	expMemAllocInfo.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO;
	expMemAllocInfo.pNext = NULL;
	expMemAllocInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT;

	VkMemoryRequirements memRequirements;

	if (devData->externalMemoryProperties.externalMemoryFeatures & VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_KHR)
	{
		VkMemoryDedicatedRequirementsKHR dedicatedRequirements;
		memset(&dedicatedRequirements, 0, sizeof(dedicatedRequirements));
		dedicatedRequirements.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_REQUIREMENTS_KHR;
		dedicatedRequirements.pNext = NULL;

		VkMemoryRequirements2KHR memoryRequirements2;
		memset(&memoryRequirements2, 0, sizeof(VkMemoryRequirements2KHR));
		memoryRequirements2.sType = VK_STRUCTURE_TYPE_MEMORY_REQUIREMENTS_2_KHR;
		memoryRequirements2.pNext = &dedicatedRequirements;

		VkImageMemoryRequirementsInfo2KHR imageRequirementsInfo;
		memset(&imageRequirementsInfo, 0, sizeof(VkImageMemoryRequirementsInfo2KHR));
		imageRequirementsInfo.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_REQUIREMENTS_INFO_2_KHR;
		imageRequirementsInfo.pNext = NULL;
		imageRequirementsInfo.image = swpchData->exportedImage;

		dispatchTable->GetImageMemoryRequirements2KHR(devData->device, &imageRequirementsInfo, &memoryRequirements2);
		memRequirements = memoryRequirements2.memoryRequirements;
	}
	else
	{
		dispatchTable->GetImageMemoryRequirements(devData->device, swpchData->exportedImage, &memRequirements);
	}



	uint32_t memoryTypeIndex = 0;

	VkLayerInstanceDispatchTable* instDisp = GetInstanceDispatchTable(TOKEY(devData->physicalDevice));


	VkPhysicalDeviceMemoryProperties memProperties;
	instDisp->GetPhysicalDeviceMemoryProperties(devData->physicalDevice, &memProperties);

	for (; memoryTypeIndex < memProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((memRequirements.memoryTypeBits & (1 << memoryTypeIndex)) && (memProperties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			break;
		}
	}

	VkImportMemoryWin32HandleInfoKHR importMemoryInfo;
	importMemoryInfo.sType = VK_STRUCTURE_TYPE_IMPORT_MEMORY_WIN32_HANDLE_INFO_KHR;
	importMemoryInfo.pNext = NULL;
	importMemoryInfo.name = NULL;
	importMemoryInfo.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT;
	importMemoryInfo.handle = swpchData->handle;


	VkMemoryAllocateInfo memAllocInfo;
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = &importMemoryInfo;
	memAllocInfo.allocationSize = memRequirements.size;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;

	VkMemoryDedicatedAllocateInfoKHR dedicatedAllocationInfo;
	dedicatedAllocationInfo.sType = VK_STRUCTURE_TYPE_MEMORY_DEDICATED_ALLOCATE_INFO;
	dedicatedAllocationInfo.pNext = NULL;
	dedicatedAllocationInfo.buffer = VK_NULL_HANDLE;

	if (devData->externalMemoryProperties.externalMemoryFeatures & VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_KHR)
	{
		dedicatedAllocationInfo.image = swpchData->exportedImage;
		importMemoryInfo.pNext = &dedicatedAllocationInfo;
	}


	res = dispatchTable->AllocateMemory(devData->device, &memAllocInfo, NULL, &swpchData->exportedImageMemory);
	DbgOutRes("# OBS_Layer # AllocateMemory %s\n", res);

	if (VK_SUCCESS != res) {
		hlog("vk_shtex_init_vulkan_tex: failed to AllocateMemory : %s", VkResultString(res));
		dispatchTable->DestroyImage(devData->device, swpchData->exportedImage, NULL);
		swpchData->exportedImage = NULL;
		return false;
	}


	if (!(devData->externalMemoryProperties.externalMemoryFeatures & VK_EXTERNAL_MEMORY_FEATURE_DEDICATED_ONLY_BIT_KHR)) {
		res = dispatchTable->BindImageMemory(devData->device, swpchData->exportedImage, swpchData->exportedImageMemory, 0);
		DbgOutRes("# OBS_Layer # BindImageMemory %s\n", res);
	}
	else {
		VkBindImageMemoryInfoKHR bindImageMemoryInfo;
		memset(&bindImageMemoryInfo, 0, sizeof(VkBindImageMemoryInfoKHR));
		bindImageMemoryInfo.sType = VK_STRUCTURE_TYPE_BIND_IMAGE_MEMORY_INFO;
		bindImageMemoryInfo.image = swpchData->exportedImage;
		bindImageMemoryInfo.memory = swpchData->exportedImageMemory;
		bindImageMemoryInfo.memoryOffset = 0;
		res = dispatchTable->BindImageMemory2KHR(devData->device, 1, &bindImageMemoryInfo);
		DbgOutRes("# OBS_Layer # BindImageMemory2KHR %s\n", res);
	}
	if (VK_SUCCESS != res) {
		dispatchTable->DestroyImage(devData->device, swpchData->exportedImage, NULL);
		return false;
	}
	return true;
}


BOOL Init_VulkanLayer() {
	if (!initialized) {
		InitializeCriticalSection(&mutex);

		instanceCount = 0;
		memset(&instanceTable, 0, sizeof(instanceData) * MAX_INSTANCE_COUNT);
		memset(&instanceKeys, 0, sizeof(void*) * MAX_INSTANCE_COUNT);

		deviceCount = 0;
		memset(&deviceTable, 0, sizeof(deviceData) * MAX_DEVICE_COUNT);
		memset(&devices, 0, sizeof(void*) * MAX_DEVICE_COUNT);

		initialized = TRUE;
	}
	return TRUE;
}

BOOL Shutdown_VulkanLayer() {
	if (initialized) {
		DeleteCriticalSection(&mutex);
		initialized = FALSE;
	}
	return TRUE;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL OBS_CreateInstance(
	const VkInstanceCreateInfo*                 pCreateInfo,
	const VkAllocationCallbacks*                pAllocator,
	VkInstance*                                 pInstance)
{
	VkLayerInstanceCreateInfo *layerCreateInfo = (VkLayerInstanceCreateInfo *)pCreateInfo->pNext;

	// step through the chain of pNext until we get to the link info
	while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_INSTANCE_CREATE_INFO ||
		layerCreateInfo->function != VK_LAYER_LINK_INFO))
	{
		layerCreateInfo = (VkLayerInstanceCreateInfo *)layerCreateInfo->pNext;
	}

	if (layerCreateInfo == NULL)
	{
		// No loader instance create info
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkGetInstanceProcAddr gpa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
	// move chain on for next layer
	layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

	PFN_vkCreateInstance createFunc = (PFN_vkCreateInstance)gpa(VK_NULL_HANDLE, "vkCreateInstance");

	BOOL VK_KHR_external_memory_capabilities_enabled = FALSE;
	BOOL VK_KHR_get_physical_device_properties2_enabled = FALSE;
	for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
		VK_KHR_external_memory_capabilities_enabled |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME));
		VK_KHR_get_physical_device_properties2_enabled |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME));
	}

	if (!VK_KHR_external_memory_capabilities_enabled || !VK_KHR_get_physical_device_properties2_enabled) {
		uint32_t extCount = pCreateInfo->enabledExtensionCount;
		uint32_t newExtCount = pCreateInfo->enabledExtensionCount;

		newExtCount += VK_KHR_external_memory_capabilities_enabled ? 0 : 1;
		newExtCount += VK_KHR_get_physical_device_properties2_enabled ? 0 : 1;



		const char** extNames = (const char**)alloca(sizeof(const char*) * newExtCount);
		for (uint32_t i = 0; i < extCount; ++i) {
			extNames[i] = (const char*)(pCreateInfo->ppEnabledExtensionNames[i]);
		}
		if (!VK_KHR_external_memory_capabilities_enabled) {
			extNames[extCount++] = VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME;
		}
		if (!VK_KHR_get_physical_device_properties2_enabled) {
			extNames[extCount++] = VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME;
		}
		VkInstanceCreateInfo* createInfo = (VkInstanceCreateInfo*)(pCreateInfo); //remove createInfo constness
		createInfo->enabledExtensionCount = extCount;
		createInfo->ppEnabledExtensionNames = extNames;
	}

	VkResult res = createFunc(pCreateInfo, pAllocator, pInstance);
	DbgOutRes("# OBS_Layer # CreateInstance %s\n", res);
	
	VkLayerInstanceDispatchTable* dispatchTable = GetInstanceDispatchTable(TOKEY(*pInstance));

	// fetch our own dispatch table for the functions we need, into the next layer
	dispatchTable->GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)gpa(*pInstance, "vkGetInstanceProcAddr");
	dispatchTable->DestroyInstance = (PFN_vkDestroyInstance)gpa(*pInstance, "vkDestroyInstance");
	dispatchTable->EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)gpa(*pInstance, "vkEnumerateDeviceExtensionProperties");
	dispatchTable->EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)gpa(*pInstance, "vkEnumeratePhysicalDevices");
	dispatchTable->CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)gpa(*pInstance, "vkCreateWin32SurfaceKHR");

	dispatchTable->GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
	dispatchTable->GetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)gpa(*pInstance, "vkGetPhysicalDeviceMemoryProperties");
	dispatchTable->GetPhysicalDeviceImageFormatProperties2KHR = (PFN_vkGetPhysicalDeviceImageFormatProperties2KHR)gpa(*pInstance, "vkGetPhysicalDeviceImageFormatProperties2KHR");

	return res;
}

VK_LAYER_EXPORT VkResult VKAPI_CALL OBS_DestroyInstance(VkInstance instance,
	const VkAllocationCallbacks* pAllocator) {
	VkLayerInstanceDispatchTable* dispatchTable = GetInstanceDispatchTable(TOKEY(instance));
	dispatchTable->DestroyInstance(instance, pAllocator);
	RemoveInstance(instance);
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_EnumerateInstanceLayerProperties(uint32_t* pPropertyCount, VkLayerProperties* pProperties) {
	if (pPropertyCount) *pPropertyCount = 1;

	if (pProperties)
	{
		strcpy(pProperties->layerName, "VK_LAYER_OBS_HOOK");
		strcpy(pProperties->description, "Open Broadcaster Software hook");
		pProperties->implementationVersion = 1;
		pProperties->specVersion = VK_API_VERSION_1_1;
	}
	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_EnumerateInstanceExtensionProperties(const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_OBS_HOOK"))
		return VK_ERROR_LAYER_NOT_PRESENT;

	if (pPropertyCount) *pPropertyCount = 1;

	if (pProperties) {
		strcpy(pProperties[0].extensionName, VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
		pProperties[0].specVersion = VK_API_VERSION_1_1;
	}

	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_EnumerateDeviceExtensionProperties(VkPhysicalDevice physicalDevice, const char* pLayerName, uint32_t* pPropertyCount, VkExtensionProperties* pProperties) {
	// pass through any queries that aren't to us
	if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_OBS_HOOK"))
	{
		if (physicalDevice == VK_NULL_HANDLE)
			return VK_SUCCESS;

		VkLayerInstanceDispatchTable* disp = GetInstanceDispatchTable(TOKEY(physicalDevice));
		return disp->EnumerateDeviceExtensionProperties(physicalDevice, pLayerName, pPropertyCount, pProperties);
	}

	if (pPropertyCount) *pPropertyCount = 2;

	if (pProperties)
	{
		strcpy(pProperties[0].extensionName, VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME);
		pProperties[0].specVersion = VK_API_VERSION_1_1;
		strcpy(pProperties[1].extensionName, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME);
		pProperties[1].specVersion = VK_API_VERSION_1_1;
	}

	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices){
	instanceData* instData = GetInstanceData(TOKEY(instance));
	VkLayerInstanceDispatchTable* dispatchTable = &instData->dispatchTable;

	VkResult res = dispatchTable->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);

	if (res == VK_SUCCESS) {

		uint32_t PhyDevCount = *pPhysicalDeviceCount;
		if (PhyDevCount >= MAX_PHYSICALDEVICE_COUNT) {
			PhyDevCount = MAX_PHYSICALDEVICE_COUNT;
			DbgOut2("# OBS_Layer # Out of physical device storage for instance %p, clamping to %d\n", instance, PhyDevCount);
		}
		instData->physicalDeviceCount = PhyDevCount;

		if (pPhysicalDevices != NULL) {
			for (uint32_t i = 0; i < PhyDevCount; ++i) {
				instData->availablePhysicalDevices[i] = (VkPhysicalDevice*)pPhysicalDevices[i];
			}
		}
	}
	return res;
}


BOOL isSharedTextureSupported(VkLayerInstanceDispatchTable* instdisp, VkPhysicalDevice physicalDevice, VkFormat imageFormat, VkImageUsageFlags imageUsage, VkExternalMemoryPropertiesKHR* pExternalMemoryProperties)
{
	VkPhysicalDeviceImageFormatInfo2KHR        imageFormatInfo;
	VkPhysicalDeviceExternalImageFormatInfoKHR externalImageFormat;

	externalImageFormat.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTERNAL_IMAGE_FORMAT_INFO_KHR;
	externalImageFormat.pNext = NULL;
	//externalImageFormat.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT_KHR;
	externalImageFormat.handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_KMT_BIT_KHR;

	imageFormatInfo.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_IMAGE_FORMAT_INFO_2_KHR;
	imageFormatInfo.pNext = &externalImageFormat;
	imageFormatInfo.format = imageFormat;
	imageFormatInfo.type = VK_IMAGE_TYPE_2D;
	imageFormatInfo.tiling = VK_IMAGE_TILING_OPTIMAL;
	imageFormatInfo.flags = 0;
	imageFormatInfo.usage = imageUsage;

	VkExternalImageFormatPropertiesKHR externalImageFormatProperties;
	memset(&externalImageFormatProperties, 0, sizeof(VkExternalImageFormatPropertiesKHR));
	externalImageFormatProperties.sType = VK_STRUCTURE_TYPE_EXTERNAL_IMAGE_FORMAT_PROPERTIES_KHR;
	externalImageFormatProperties.pNext = NULL;

	VkImageFormatProperties2KHR imageFormatProperties;
	memset(&imageFormatProperties, 0, sizeof(VkImageFormatProperties2KHR));
	imageFormatProperties.sType = VK_STRUCTURE_TYPE_IMAGE_FORMAT_PROPERTIES_2_KHR;
	imageFormatProperties.pNext = &externalImageFormatProperties;

	VkResult result = instdisp->GetPhysicalDeviceImageFormatProperties2KHR(physicalDevice, &imageFormatInfo, &imageFormatProperties);

	*pExternalMemoryProperties = externalImageFormatProperties.externalMemoryProperties;

	return ((VK_SUCCESS == result) && (pExternalMemoryProperties->externalMemoryFeatures & VK_EXTERNAL_MEMORY_FEATURE_IMPORTABLE_BIT_KHR));
}


VKAPI_ATTR VkResult VKAPI_CALL OBS_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {

	VkDeviceCreateInfo* createInfo = (VkDeviceCreateInfo*)(pCreateInfo); //remove createInfo constness
	VkLayerInstanceDispatchTable* instdisp = GetInstanceDispatchTable(TOKEY(physicalDevice));

// ensure needed device extension are available and enabled
#pragma region(needed device extention)
	BOOL VK_KHR_external_memory_win32_available = FALSE;
	BOOL VK_KHR_external_memory_available = FALSE;
	BOOL VK_KHR_dedicated_allocation_available = FALSE;
	BOOL VK_KHR_get_memory_requirements2_available = FALSE;
	BOOL VK_KHR_bind_memory2_available = FALSE;
	{
		uint32_t extCount = 0;
		instdisp->EnumerateDeviceExtensionProperties(physicalDevice, NULL, &extCount, NULL);

		VkExtensionProperties* props = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties) * extCount);
		instdisp->EnumerateDeviceExtensionProperties(physicalDevice, NULL, &extCount, props);
		for (uint32_t e = 0; e < extCount; e++)
		{
			VK_KHR_external_memory_win32_available |= (0 == strcmp(props[e].extensionName, VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME));
			VK_KHR_external_memory_available |= (0 == strcmp(props[e].extensionName, VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME));
			VK_KHR_dedicated_allocation_available |= (0 == strcmp(props[e].extensionName, VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME));
			VK_KHR_get_memory_requirements2_available |= (0 == strcmp(props[e].extensionName, VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME));
			VK_KHR_bind_memory2_available |= (0 == strcmp(props[e].extensionName, VK_KHR_BIND_MEMORY_2_EXTENSION_NAME));
		}
	}

	if (	VK_KHR_external_memory_win32_available
		&& VK_KHR_external_memory_available
		&& VK_KHR_dedicated_allocation_available
		&& VK_KHR_get_memory_requirements2_available
		&& VK_KHR_bind_memory2_available ) {
			// add the exentions we need if not already there
		BOOL found_VK_KHR_external_memory_win32 = FALSE;
		BOOL found_VK_KHR_external_memory = FALSE;
		BOOL found_VK_KHR_dedicated_allocation = FALSE;
		BOOL found_VK_KHR_get_memory_requirements2 = FALSE;
		BOOL found_VK_KHR_bind_memory2 = FALSE;

		for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
			found_VK_KHR_external_memory_win32 |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME));
			found_VK_KHR_external_memory |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME));
			found_VK_KHR_dedicated_allocation |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME));
			found_VK_KHR_get_memory_requirements2 |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME));
			found_VK_KHR_bind_memory2 |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_BIND_MEMORY_2_EXTENSION_NAME));
		}

		uint32_t extIndex = pCreateInfo->enabledExtensionCount;
		createInfo->enabledExtensionCount += found_VK_KHR_external_memory_win32 ? 0 : 1;
		createInfo->enabledExtensionCount += found_VK_KHR_external_memory ? 0 : 1;
		createInfo->enabledExtensionCount += found_VK_KHR_dedicated_allocation ? 0 : 1;
		createInfo->enabledExtensionCount += found_VK_KHR_get_memory_requirements2 ? 0 : 1;
		createInfo->enabledExtensionCount += found_VK_KHR_bind_memory2 ? 0 : 1;

		const char** extNames = (const char**)alloca(sizeof(const char*) * pCreateInfo->enabledExtensionCount);
		for (uint32_t i = 0; i < extIndex; ++i) {
			extNames[i] = (const char*)(pCreateInfo->ppEnabledExtensionNames[i]);
		}

		if (!found_VK_KHR_external_memory_win32) {
			extNames[extIndex++] = (const char*)&VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME;
		}
		if (!found_VK_KHR_external_memory) {
			extNames[extIndex++] = (const char*)&VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME;
		}
		if (!found_VK_KHR_dedicated_allocation) {
			extNames[extIndex++] = (const char*)&VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME;
		}
		if (!found_VK_KHR_get_memory_requirements2) {
			extNames[extIndex++] = (const char*)&VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME;
		}
		if (!found_VK_KHR_bind_memory2) {
			extNames[extIndex++] = (const char*)&VK_KHR_BIND_MEMORY_2_EXTENSION_NAME;
		}


		createInfo->ppEnabledExtensionNames = extNames;
	}
	else {
		DbgOut("# OBS_Layer # Needed device extensions are not available\n");
	}
#pragma endregion

	// retrieve a usable queue, in order to issue our copy command
	uint32_t qFamilyIdx = 0;
#pragma region(usablequeue)
	// find or create a usable queue
	uint32_t queueFamilyPropertyCount = 0;
	VkQueueFamilyProperties pQueueFamilyProperties[16];

	instdisp->GetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, NULL);
	// only support 16 queue family
	queueFamilyPropertyCount = (queueFamilyPropertyCount > 16) ? 16 : queueFamilyPropertyCount;

	instdisp->GetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertyCount, pQueueFamilyProperties);

	// find a queue that supports all capabilities, and if one doesn't exist, add it.
	BOOL found = FALSE;

	// we need graphics, and if there is a graphics queue there must be a graphics & compute queue.
	VkQueueFlags search = (VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);

	// for queue priorities, if we need it
	float one = 1.0f;

	// if we need to change the requested queues, it will point to this
	VkDeviceQueueCreateInfo *modQueues = NULL;

	for (uint32_t i = 0; i < pCreateInfo->queueCreateInfoCount; i++) {
		uint32_t idx = pCreateInfo->pQueueCreateInfos[i].queueFamilyIndex;
		// this requested queue is one we can use too
		if ((idx < queueFamilyPropertyCount) && (pQueueFamilyProperties[idx].queueFlags & search) == search && pCreateInfo->pQueueCreateInfos[i].queueCount > 0)
		{
			qFamilyIdx = idx;
			found = TRUE;
			break;
		}
	}
	// if we didn't find it, search for which queue family we should add a request for
	if (!found)
	{
		for (uint32_t i = 0; i < queueFamilyPropertyCount; i++)
		{
			if ((pQueueFamilyProperties[i].queueFlags & search) == search)
			{
				qFamilyIdx = i;
				found = TRUE;
				break;
			}
		}

		if (!found)
		{
			return VK_ERROR_INITIALIZATION_FAILED;
		}

		// we found the queue family, add it
		modQueues = (VkDeviceQueueCreateInfo*)alloca( sizeof(VkDeviceQueueCreateInfo)*(pCreateInfo->queueCreateInfoCount + 1) );
		memcpy(modQueues, pCreateInfo->pQueueCreateInfos, sizeof(VkDeviceQueueCreateInfo)*pCreateInfo->queueCreateInfoCount);

		modQueues[pCreateInfo->queueCreateInfoCount].queueFamilyIndex = qFamilyIdx;
		modQueues[pCreateInfo->queueCreateInfoCount].queueCount = 1;
		modQueues[pCreateInfo->queueCreateInfoCount].pQueuePriorities = &one;
		modQueues[pCreateInfo->queueCreateInfoCount].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		modQueues[pCreateInfo->queueCreateInfoCount].pNext = NULL;
		modQueues[pCreateInfo->queueCreateInfoCount].flags = 0;

		createInfo->pQueueCreateInfos = modQueues;
		createInfo->queueCreateInfoCount++;
	}
#pragma endregion



	VkLayerDeviceCreateInfo *layerCreateInfo = (VkLayerDeviceCreateInfo *)pCreateInfo->pNext;

	// step through the chain of pNext until we get to the link info
	while (layerCreateInfo && (layerCreateInfo->sType != VK_STRUCTURE_TYPE_LOADER_DEVICE_CREATE_INFO ||
		layerCreateInfo->function != VK_LAYER_LINK_INFO))
	{
		layerCreateInfo = (VkLayerDeviceCreateInfo *)layerCreateInfo->pNext;
	}

	if (layerCreateInfo == NULL)
	{
		// No loader instance create info
		return VK_ERROR_INITIALIZATION_FAILED;
	}

	PFN_vkGetInstanceProcAddr gipa = layerCreateInfo->u.pLayerInfo->pfnNextGetInstanceProcAddr;
	PFN_vkGetDeviceProcAddr gdpa = layerCreateInfo->u.pLayerInfo->pfnNextGetDeviceProcAddr;
	// move chain on for next layer
	layerCreateInfo->u.pLayerInfo = layerCreateInfo->u.pLayerInfo->pNext;

	PFN_vkCreateDevice createFunc = (PFN_vkCreateDevice)gipa(VK_NULL_HANDLE, "vkCreateDevice");

	/*VkResult ret = */createFunc(physicalDevice, pCreateInfo, pAllocator, pDevice);

	// store the table by key
	deviceData* devData = GetDeviceData(TOKEY(*pDevice));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	// store the queueFamilyIdx needed for graphics command
	devData->queueFamilyIdx = qFamilyIdx;

	// store the physicalDevice on which device is created
	devData->physicalDevice = physicalDevice;

	// store the device
	devData->device = *pDevice;

	// feed our dispatch table for the functions we need (function pointer into the next layer)
	dispatchTable->GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)gdpa(*pDevice, "vkGetDeviceProcAddr");
	dispatchTable->DestroyDevice = (PFN_vkDestroyDevice)gdpa(*pDevice, "vkDestroyDevice");

	dispatchTable->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gdpa(*pDevice, "vkCreateSwapchainKHR");
	dispatchTable->DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)gdpa(*pDevice, "vkDestroySwapchainKHR");
	dispatchTable->QueuePresentKHR = (PFN_vkQueuePresentKHR)gdpa(*pDevice, "vkQueuePresentKHR");

	dispatchTable->AllocateMemory = (PFN_vkAllocateMemory)gdpa(*pDevice, "vkAllocateMemory");
	dispatchTable->FreeMemory = (PFN_vkFreeMemory)gdpa(*pDevice, "vkFreeMemory");
	dispatchTable->BindImageMemory = (PFN_vkBindImageMemory)gdpa(*pDevice, "vkBindImageMemory");
	dispatchTable->BindImageMemory2KHR = (PFN_vkBindImageMemory2KHR)gdpa(*pDevice, "vkBindImageMemory2KHR");

	dispatchTable->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gdpa(*pDevice, "vkGetSwapchainImagesKHR");

	dispatchTable->CreateImage = (PFN_vkCreateImage)gdpa(*pDevice, "vkCreateImage");
	dispatchTable->DestroyImage = (PFN_vkDestroyImage)gdpa(*pDevice, "vkDestroyImage");
	dispatchTable->GetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)gdpa(*pDevice, "vkGetImageMemoryRequirements");
	dispatchTable->GetImageMemoryRequirements2KHR = (PFN_vkGetImageMemoryRequirements2KHR)gdpa(*pDevice, "vkGetImageMemoryRequirements2KHR");

	dispatchTable->BeginCommandBuffer = (PFN_vkBeginCommandBuffer)gdpa(*pDevice, "vkBeginCommandBuffer");
	dispatchTable->EndCommandBuffer = (PFN_vkEndCommandBuffer)gdpa(*pDevice, "vkEndCommandBuffer");

	dispatchTable->CmdCopyImage = (PFN_vkCmdCopyImage)gdpa(*pDevice, "vkCmdCopyImage");
	//dispatchTable->CmdBlitImage = (PFN_vkCmdBlitImage)gdpa(*pDevice, "vkCmdBlitImage"); //might help handling formats

	dispatchTable->CmdPipelineBarrier = (PFN_vkCmdPipelineBarrier)gdpa(*pDevice, "vkCmdPipelineBarrier");
	dispatchTable->GetDeviceQueue = (PFN_vkGetDeviceQueue)gdpa(*pDevice, "vkGetDeviceQueue");
	dispatchTable->QueueSubmit = (PFN_vkQueueSubmit)gdpa(*pDevice, "vkQueueSubmit");

	dispatchTable->QueueWaitIdle = (PFN_vkQueueWaitIdle)gdpa(*pDevice, "vkQueueWaitIdle");
	dispatchTable->DeviceWaitIdle = (PFN_vkDeviceWaitIdle)gdpa(*pDevice, "vkDeviceWaitIdle");

	dispatchTable->CreateCommandPool = (PFN_vkCreateCommandPool)gdpa(*pDevice, "vkCreateCommandPool");
	dispatchTable->AllocateCommandBuffers = (PFN_vkAllocateCommandBuffers)gdpa(*pDevice, "vkAllocateCommandBuffers");
	
	// retrieve the queue
	dispatchTable->GetDeviceQueue(*pDevice, qFamilyIdx, 0, &devData->queue);

	if (devData->cmdPool == VK_NULL_HANDLE)
	{
		VkCommandPoolCreateInfo poolInfo;
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.pNext = NULL;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = qFamilyIdx;

		VkResult res = dispatchTable->CreateCommandPool(*pDevice, &poolInfo, NULL, &devData->cmdPool);
		DbgOutRes("# OBS_Layer # CreateCommandPool %s\n", res);

		VkCommandBufferAllocateInfo cmdInfo;
		cmdInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdInfo.pNext = NULL;
		cmdInfo.commandPool = devData->cmdPool;
		cmdInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdInfo.commandBufferCount = 1;

		res = dispatchTable->AllocateCommandBuffers(*pDevice, &cmdInfo, &devData->cmdBuffer);
		DbgOutRes("# OBS_Layer # AllocateCommandBuffers %s\n", res);
	}

	VkFormat          imageFormat = VK_FORMAT_R8G8B8A8_UNORM;
	VkImageUsageFlags imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;


	if (!isSharedTextureSupported(instdisp, physicalDevice, imageFormat, imageUsage, &devData->externalMemoryProperties)) {
		hlog(" Vulkan CreateDevice : texture sharing is not supported\n");
	}

	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL OBS_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
	pAllocator; //unused
	RemoveDevice(&device);
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
	deviceData* devData = GetDeviceData(TOKEY(device));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	swapchainData* swchData = GetNewSwapchainData(devData);

	swchData->surface = pCreateInfo->surface;
	swchData->imageExtent = pCreateInfo->imageExtent;
	swchData->imageFormat = pCreateInfo->imageFormat;
	

	VkResult res = dispatchTable->CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);

	uint32_t pSwapchainImageCount = 0;
	res = dispatchTable->GetSwapchainImagesKHR(devData->device, *pSwapchain, &pSwapchainImageCount, NULL);
	DbgOutRes("# OBS_Layer # GetSwapchainImagesKHR %s\n", res);

	if (pSwapchainImageCount > 0) {
		pSwapchainImageCount = (pSwapchainImageCount < MAX_IMAGES_PER_SWAPCHAIN) ? pSwapchainImageCount : MAX_IMAGES_PER_SWAPCHAIN;
		res = dispatchTable->GetSwapchainImagesKHR(devData->device, *pSwapchain, &pSwapchainImageCount, swchData->swapchainImages);
		DbgOutRes("# OBS_Layer # GetSwapchainImagesKHR %s\n", res);
	}

	swchData->swapchain = *pSwapchain;

	return res;
}

VKAPI_ATTR void VKAPI_CALL OBS_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) {
	deviceData* devData = GetDeviceData(TOKEY(device));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	swapchainData* swchData = GetSwapchainData(devData, swapchain);
	if (swchData) {
		if (swchData->exportedImage)
			dispatchTable->DestroyImage(device, swchData->exportedImage, NULL);

		if (swchData->exportedImageMemory)
			dispatchTable->FreeMemory(device, swchData->exportedImageMemory, NULL);

		swchData->handle = INVALID_HANDLE_VALUE;
		swchData->swapchain = VK_NULL_HANDLE;
		swchData->surface = NULL;

		if (swchData->d3d11_tex)
			ID3D11Resource_Release(swchData->d3d11_tex);

		swchData->sharedTextureCaptured = 0;
	}
	dispatchTable->DestroySwapchainKHR(device, swapchain, pAllocator);
}


VKAPI_ATTR VkResult VKAPI_CALL OBS_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
	VkResult res = VK_SUCCESS;
	deviceData* devData = GetDeviceData(TOKEY(queue));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;
	DbgOut2("# OBS_Layer # QueuePresentKHR called on devicekey %p, swapchaincount %d\n", dispatchTable, pPresentInfo->swapchainCount);

	for (uint32_t i = 0; i < pPresentInfo->swapchainCount; ++i) {


		swapchainData* swpchData = GetSwapchainData(devData, pPresentInfo->pSwapchains[i]);
		if (hooked) {

			HWND window = NULL;
			for (int inst = 0; inst < instanceCount; ++inst) {
				surfaceData* surfData = FindSurfaceData(&instanceTable[inst], swpchData->surface);
				if (surfData != NULL && surfData->surface == swpchData->surface) {
					window = surfData->hwnd;
				}
			}
			if (window != NULL && !swpchData->sharedTextureCaptured) {
				if (vk_shtex_init_window(devData)) {
					if (vk_shtex_init_d3d11(devData)) {
						if (vk_shtex_init_d3d11_tex(devData, swpchData)) {
							if (vk_shtex_init_vulkan_tex(devData, swpchData)) {
								swpchData->sharedTextureCaptured = capture_init_shtex(&swpchData->shtex_info, window, swpchData->imageExtent.width, swpchData->imageExtent.height, swpchData->imageExtent.width, swpchData->imageExtent.height
									, (uint32_t)swpchData->imageFormat, FALSE, (uintptr_t)swpchData->handle);
							}
						}
					}
				}
			}
		}

		if (swpchData->sharedTextureCaptured) {
			VkCommandBufferBeginInfo beginInfo;
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.pNext = NULL;
			beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			beginInfo.pInheritanceInfo = NULL;

			// do image copy
			res = dispatchTable->BeginCommandBuffer(devData->cmdBuffer, &beginInfo);
			DbgOutRes("# OBS_Layer # BeginCommandBuffer %s\n", res);

			VkImage currentBackBuffer = swpchData->swapchainImages[pPresentInfo->pImageIndices[i]];

			// transition currentBackBuffer to transfer source state
			VkImageMemoryBarrier presentMemoryBarrier;
			presentMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			presentMemoryBarrier.pNext = NULL;
			presentMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
			presentMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;// VK_ACCESS_TRANSFER_READ_BIT;
			presentMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			presentMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			presentMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			presentMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //devData->queueFamilyIdx;
			presentMemoryBarrier.image = currentBackBuffer;
			presentMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			presentMemoryBarrier.subresourceRange.baseMipLevel = 0;
			presentMemoryBarrier.subresourceRange.levelCount = 1;
			presentMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			presentMemoryBarrier.subresourceRange.layerCount = 1;

			// transition exportedTexture to transfer dest state
			VkImageMemoryBarrier destMemoryBarrier;
			destMemoryBarrier.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			destMemoryBarrier.pNext = NULL;
			destMemoryBarrier.srcAccessMask = 0;
			destMemoryBarrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;// VK_ACCESS_TRANSFER_READ_BIT;
			destMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			destMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			destMemoryBarrier.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			destMemoryBarrier.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED; //devData->queueFamilyIdx;
			destMemoryBarrier.image = swpchData->exportedImage;
			destMemoryBarrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			destMemoryBarrier.subresourceRange.baseMipLevel = 0;
			destMemoryBarrier.subresourceRange.levelCount = 1;
			destMemoryBarrier.subresourceRange.baseArrayLayer = 0;
			destMemoryBarrier.subresourceRange.layerCount = 1;

			VkPipelineStageFlags srcStages = VK_PIPELINE_STAGE_TRANSFER_BIT;
			VkPipelineStageFlags dstStages = VK_PIPELINE_STAGE_TRANSFER_BIT;

			dispatchTable->CmdPipelineBarrier(devData->cmdBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1, &presentMemoryBarrier);

			dispatchTable->CmdPipelineBarrier(devData->cmdBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1, &destMemoryBarrier);

			// copy currentBackBuffer's content to our interop image

			VkImageCopy cpy;
			cpy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			cpy.srcSubresource.mipLevel = 0;
			cpy.srcSubresource.baseArrayLayer = 0;
			cpy.srcSubresource.layerCount = 1;
			cpy.srcOffset.x = 0;
			cpy.srcOffset.y = 0;
			cpy.srcOffset.z = 0;
			cpy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			cpy.dstSubresource.mipLevel = 0;
			cpy.dstSubresource.baseArrayLayer = 0;
			cpy.dstSubresource.layerCount = 1;
			cpy.dstOffset.x = 0;
			cpy.dstOffset.y = 0;
			cpy.dstOffset.z = 0;
			cpy.extent.width = swpchData->imageExtent.width;
			cpy.extent.height = swpchData->imageExtent.height;
			cpy.extent.depth = 1;
			dispatchTable->CmdCopyImage(devData->cmdBuffer, currentBackBuffer, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, swpchData->exportedImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &cpy);

			// Restore the swap chain image layout to what it was before.
			// This may not be strictly needed, but it is generally good to restore
			// things to original state.
			presentMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
			presentMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
			presentMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			presentMemoryBarrier.dstAccessMask = 0;
			dispatchTable->CmdPipelineBarrier(devData->cmdBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1, &presentMemoryBarrier);

			destMemoryBarrier.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
			destMemoryBarrier.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			destMemoryBarrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
			destMemoryBarrier.dstAccessMask = 0;
			dispatchTable->CmdPipelineBarrier(devData->cmdBuffer, srcStages, dstStages, 0, 0, NULL, 0, NULL, 1, &destMemoryBarrier);

			dispatchTable->EndCommandBuffer(devData->cmdBuffer);

			VkSubmitInfo submit_info;
			submit_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			submit_info.pNext = NULL;
			submit_info.waitSemaphoreCount = 0;
			submit_info.pWaitSemaphores = NULL;
			submit_info.pWaitDstStageMask = NULL;
			submit_info.commandBufferCount = 1;
			submit_info.pCommandBuffers = &devData->cmdBuffer;
			submit_info.signalSemaphoreCount = 0;
			submit_info.pSignalSemaphores = NULL;

			VkFence nullFence = { VK_NULL_HANDLE };

			res = dispatchTable->QueueSubmit(devData->queue, 1, &submit_info, nullFence);
			DbgOutRes("# OBS_Layer # QueueSubmit %s\n", res);
		}
	}
	
	return dispatchTable->QueuePresentKHR(queue, pPresentInfo);
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_GetSwapchainImagesKHR(VkDevice device, VkSwapchainKHR swapchain, uint32_t* pSwapchainImageCount, VkImage* pSwapchainImages) {
	VkLayerDispatchTable* dispatchTable = GetDeviceDispatchTable(TOKEY(device));
	VkResult res = dispatchTable->GetSwapchainImagesKHR(device, swapchain, pSwapchainImageCount, pSwapchainImages);
	return res;
}


VKAPI_ATTR VkResult VKAPI_CALL OBS_CreateWin32SurfaceKHR(VkInstance instance, const VkWin32SurfaceCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSurfaceKHR* pSurface)
{
	instanceData* instData = GetInstanceData(TOKEY(instance));
	DbgOut("# OBS_Layer # CreateWin32SurfaceKHR\n");

	VkResult res = instData->dispatchTable.CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);
	if (NULL != pSurface && VK_NULL_HANDLE != *pSurface) {
		surfaceData* surfData = FindSurfaceData(instData, *pSurface);

		surfData->hinstance = pCreateInfo->hinstance;
		surfData->hwnd = pCreateInfo->hwnd;
	}
	return res;
}

#define GETPROCADDR(func) if(!strcmp(funcName, "vk" #func)) return (PFN_vkVoidFunction)&OBS_##func;

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL OBS_GetDeviceProcAddr(VkDevice dev, const char *funcName) {
	DbgOutProcAddr("# OBS_Layer # vkGetDeviceProcAddr [%s] called on device %p\n", funcName, dev);
	GETPROCADDR(GetDeviceProcAddr);
	GETPROCADDR(EnumerateDeviceExtensionProperties);
	GETPROCADDR(CreateDevice);
	GETPROCADDR(DestroyDevice);
	GETPROCADDR(CreateSwapchainKHR);
	GETPROCADDR(DestroySwapchainKHR);
	GETPROCADDR(QueuePresentKHR);
	GETPROCADDR(GetSwapchainImagesKHR);
	
	VkLayerDispatchTable* dispatchTable = GetDeviceDispatchTable(TOKEY(dev));
	if (dispatchTable->GetDeviceProcAddr == NULL)
		return NULL;
	return dispatchTable->GetDeviceProcAddr(dev, funcName);
}


VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL OBS_GetInstanceProcAddr(VkInstance instance, const char *funcName) {
	DbgOutProcAddr("# OBS_Layer # vkGetInstanceProcAddr [%s] called on instance %p\n", funcName, instance);
	// instance chain functions we intercept
	GETPROCADDR(GetInstanceProcAddr);
	GETPROCADDR(EnumerateInstanceLayerProperties);
	GETPROCADDR(EnumerateInstanceExtensionProperties);
	GETPROCADDR(EnumeratePhysicalDevices);
	GETPROCADDR(CreateInstance);
	GETPROCADDR(DestroyInstance);
	GETPROCADDR(CreateWin32SurfaceKHR);

	// device chain functions we intercept
	GETPROCADDR(GetDeviceProcAddr);
	GETPROCADDR(EnumerateDeviceExtensionProperties);
	GETPROCADDR(CreateDevice);
	GETPROCADDR(DestroyDevice);

	VkLayerInstanceDispatchTable* dispatchTable = GetInstanceDispatchTable(TOKEY(instance));
	if (dispatchTable->GetInstanceProcAddr == NULL)
		return NULL;
	return dispatchTable->GetInstanceProcAddr(instance, funcName);
}

VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL OBS_GetPhysicalDeviceProcAddr(VkInstance instance, const char *funcName) {

	VkLayerInstanceDispatchTable *instdt = GetInstanceDispatchTable(TOKEY(instance));
	if (instdt->GetPhysicalDeviceProcAddr == NULL)
	{
		return NULL;
	}

	return instdt->GetPhysicalDeviceProcAddr(instance, funcName);
}


VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL OBS_layerGetPhysicalDeviceProcAddr(VkInstance instance, const char *funcName) {
	return OBS_GetPhysicalDeviceProcAddr(instance, funcName);
}


static uint32_t loader_layer_if_version = CURRENT_LOADER_LAYER_INTERFACE_VERSION;

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL OBS_NegotiateLoaderLayerInterfaceVersion(VkNegotiateLayerInterface *pVersionStruct) {

	if (pVersionStruct->loaderLayerInterfaceVersion >= 2) {
		pVersionStruct->sType = LAYER_NEGOTIATE_INTERFACE_STRUCT;
		pVersionStruct->pNext = NULL;
		pVersionStruct->pfnGetInstanceProcAddr = OBS_GetInstanceProcAddr;
		pVersionStruct->pfnGetDeviceProcAddr = OBS_GetDeviceProcAddr;
		pVersionStruct->pfnGetPhysicalDeviceProcAddr = OBS_layerGetPhysicalDeviceProcAddr;
	}

	if (pVersionStruct->loaderLayerInterfaceVersion < CURRENT_LOADER_LAYER_INTERFACE_VERSION) {
		loader_layer_if_version = pVersionStruct->loaderLayerInterfaceVersion;
	}
	else if (pVersionStruct->loaderLayerInterfaceVersion > CURRENT_LOADER_LAYER_INTERFACE_VERSION) {
		pVersionStruct->loaderLayerInterfaceVersion = CURRENT_LOADER_LAYER_INTERFACE_VERSION;
	}


	return VK_SUCCESS;
}


#include "graphics-hook.h"

static bool vk_register_window(void)
{
	WNDCLASSW wc = { 0 };
	wc.style = CS_OWNDC;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpfnWndProc = DefWindowProc;
	wc.lpszClassName = DUMMY_WINDOW_CLASS_NAME;

	if (!RegisterClassW(&wc)) {
		hlog("vk_register_window: failed to register window class: %d",
			GetLastError());
		return false;
	}

	return true;
}


bool hook_vulkan(void) {
	if (instanceCount > 0) {
		if (!vk_register_window()) {
			return true;
		}
		hlog("Hooked Vulkan");
		hooked = TRUE;
	} else {
		hooked = FALSE;
	}
	return hooked;
}



#endif
