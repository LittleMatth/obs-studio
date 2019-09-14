#include "graphics-hook-config.h"

#if COMPILE_VULKAN_HOOK

#define VK_USE_PLATFORM_WIN32_KHR

#include <malloc.h>
#include <windows.h>
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

#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#include <stdio.h>
#define DbgOut(x) OutputDebugStringA(x)
#define DbgOut2(x,y,z)				\
	{					\
		char string[256];		\
		sprintf(string, x, y, z);	\
		DbgOut(string);			\
	}					\

#else
#define DbgOut(x)
#define DbgOut2(x,y,z)
#endif

#define MAX_INSTANCE_COUNT 16
#define MAX_SURFACE_PER_INSTANCE 16
#define MAX_DEVICE_COUNT 16
#define MAX_SWAPCHAIN_PER_DEVICE 16
#define MAX_PHYSICALDEVICE_COUNT 16


static BOOL initialized = FALSE;
CRITICAL_SECTION mutex;


// use the loader's dispatch table pointer as a key for dispatch map lookups
#define TOKEY(x) (*(void **)x)

typedef struct swapchainData {
	VkSwapchainKHR swapchain;
	VkSurfaceKHR surface;
	VkImage exportedImages[2];
	VkDeviceMemory exportedImagesMemory[2];
	VkMemoryGetWin32HandleInfoKHR getWin32HandleInfo[2];
	HANDLE handle[2];
}swapchainData;

typedef struct deviceData {
	VkLayerDispatchTable dispatchTable;
	VkPhysicalDevice physicalDevice;
	swapchainData swapchains[MAX_SWAPCHAIN_PER_DEVICE];
	uint32_t queueFamilyIdx;
}deviceData;


swapchainData* GetSwapchainData(deviceData* devData, VkSwapchainKHR swapchain) {
	for (int i = 0; i < MAX_SWAPCHAIN_PER_DEVICE; ++i) {
		if (devData->swapchains[i].swapchain == swapchain) {
			return &devData->swapchains[i];
		}
	}
	DbgOut("GetSwapchainData failed, swapchain not found");
	return NULL;
}

swapchainData* FindSwapchainData(deviceData* devData, VkSurfaceKHR surface) {
	int firstFreeSlot = MAX_SWAPCHAIN_PER_DEVICE;
	for (int i = 0; i < MAX_SWAPCHAIN_PER_DEVICE; ++i) {
		if (devData->swapchains[i].surface == surface) {
			return &devData->swapchains[i];
		} else if (devData->swapchains[i].surface == NULL && firstFreeSlot == MAX_SWAPCHAIN_PER_DEVICE) {
			firstFreeSlot = i;
		}
	}

	if (firstFreeSlot != MAX_SWAPCHAIN_PER_DEVICE) {
		devData->swapchains[firstFreeSlot].surface = surface;
		return &devData->swapchains[firstFreeSlot];
	}

	DbgOut("FindSwapchainData failed, no more free slot");
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
		DbgOut("Out of Device Slot");
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

	DbgOut("FindSurfaceData failed, no more free slot");
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
		DbgOut("Out of Instance Slot");
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

	VkResult ret = createFunc(pCreateInfo, pAllocator, pInstance);

	if (ret != VK_SUCCESS) {
		DbgOut("\nCreateInstance forwarding failed");
	}
	
	VkLayerInstanceDispatchTable* dispatchTable = GetInstanceDispatchTable(TOKEY(*pInstance));

	// fetch our own dispatch table for the functions we need, into the next layer
	dispatchTable->GetInstanceProcAddr = (PFN_vkGetInstanceProcAddr)gpa(*pInstance, "vkGetInstanceProcAddr");
	dispatchTable->DestroyInstance = (PFN_vkDestroyInstance)gpa(*pInstance, "vkDestroyInstance");
	dispatchTable->EnumerateDeviceExtensionProperties = (PFN_vkEnumerateDeviceExtensionProperties)gpa(*pInstance, "vkEnumerateDeviceExtensionProperties");
	dispatchTable->EnumeratePhysicalDevices = (PFN_vkEnumeratePhysicalDevices)gpa(*pInstance, "vkEnumeratePhysicalDevices");
	dispatchTable->CreateWin32SurfaceKHR = (PFN_vkCreateWin32SurfaceKHR)gpa(*pInstance, "vkCreateWin32SurfaceKHR");

	dispatchTable->GetPhysicalDeviceQueueFamilyProperties = (PFN_vkGetPhysicalDeviceQueueFamilyProperties)gpa(*pInstance, "vkGetPhysicalDeviceQueueFamilyProperties");
	dispatchTable->GetPhysicalDeviceMemoryProperties = (PFN_vkGetPhysicalDeviceMemoryProperties)gpa(*pInstance, "vkGetPhysicalDeviceMemoryProperties");

	return VK_SUCCESS;
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
	pProperties; //unused
	if (pLayerName == NULL || strcmp(pLayerName, "VK_LAYER_OBS_HOOK"))
		return VK_ERROR_LAYER_NOT_PRESENT;

	// don't expose any extensions
	if (pPropertyCount) *pPropertyCount = 0;
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

	// expose external memory extension --> VK_KHR_external_memory
	// even if pLayerName is never called with "VK_LAYER_OBS_HOOK"
	// --> https://github.com/KhronosGroup/Vulkan-Loader/issues/133
	if (pPropertyCount) *pPropertyCount = 2;

	if (pProperties)
	{
		strcpy(pProperties[0].extensionName, VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME);
		pProperties[0].specVersion = VK_API_VERSION_1_1;
		strcpy(pProperties[1].extensionName, VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME);
		pProperties[1].specVersion = VK_API_VERSION_1_1;
	}
		

	return VK_SUCCESS;
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_EnumeratePhysicalDevices(VkInstance instance, uint32_t* pPhysicalDeviceCount, VkPhysicalDevice* pPhysicalDevices){
	DbgOut("\n##### EnumeratePhysicalDevices #####");

	instanceData* instData = GetInstanceData(TOKEY(instance));
	VkLayerInstanceDispatchTable* dispatchTable = &instData->dispatchTable;

	VkResult res = dispatchTable->EnumeratePhysicalDevices(instance, pPhysicalDeviceCount, pPhysicalDevices);

	if (res == VK_SUCCESS) {

		uint32_t PhyDevCount = *pPhysicalDeviceCount;
		if (PhyDevCount >= MAX_PHYSICALDEVICE_COUNT) {
			PhyDevCount = MAX_PHYSICALDEVICE_COUNT;
			DbgOut2("\nOut of physical device storage for instance %p, clamping to %d", instance, PhyDevCount);
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

VKAPI_ATTR VkResult VKAPI_CALL OBS_CreateDevice(VkPhysicalDevice physicalDevice, const VkDeviceCreateInfo* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDevice* pDevice) {

	VkDeviceCreateInfo* createInfo = (VkDeviceCreateInfo*)(pCreateInfo); //remove createInfo constness
	VkLayerInstanceDispatchTable* instdisp = GetInstanceDispatchTable(TOKEY(physicalDevice));

// ensure needed device extension are available and enabled
#pragma region(needed device extention)
	BOOL VK_KHR_external_memory_win32_available = FALSE;
	//BOOL VK_KHR_external_memory_capabilities_available = FALSE;
	{
		uint32_t extCount = 0;
		instdisp->EnumerateDeviceExtensionProperties(physicalDevice, NULL, &extCount, NULL);

		VkExtensionProperties* props = (VkExtensionProperties*)alloca(sizeof(VkExtensionProperties) * extCount);
		instdisp->EnumerateDeviceExtensionProperties(physicalDevice, NULL, &extCount, props);
		for (uint32_t e = 0; e < extCount; e++)
		{
			VK_KHR_external_memory_win32_available |= (0 == strcmp(props[e].extensionName, VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME));
			//VK_KHR_external_memory_capabilities_available |= (0 == strcmp(props[e].extensionName, VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME));
		}
	}

	//if (VK_KHR_external_memory_win32_available && VK_KHR_external_memory_capabilities_available) {
	if (VK_KHR_external_memory_win32_available) {
			// add the exentions we need if not already there
		BOOL found_VK_KHR_external_memory_win32 = FALSE;
		//BOOL found_VK_KHR_external_memory_capabilities = FALSE;

		for (uint32_t i = 0; i < pCreateInfo->enabledExtensionCount; ++i) {
			found_VK_KHR_external_memory_win32 |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME));
			//found_VK_KHR_external_memory_capabilities |= (0 == strcmp(pCreateInfo->ppEnabledExtensionNames[i], VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME));
		}

		uint32_t extIndex = pCreateInfo->enabledExtensionCount;
		createInfo->enabledExtensionCount += found_VK_KHR_external_memory_win32 ? 0 : 1;
		//createInfo->enabledExtensionCount += found_VK_KHR_external_memory_capabilities ? 0 : 1;

		const char** extNames = (const char**)alloca(sizeof(const char*) * pCreateInfo->enabledExtensionCount);
		for (uint32_t i = 0; i < extIndex; ++i) {
			extNames[i] = (const char*)(pCreateInfo->ppEnabledExtensionNames[i]);
		}

		if (!found_VK_KHR_external_memory_win32) {
			extNames[extIndex++] = (const char*)&VK_KHR_EXTERNAL_MEMORY_WIN32_EXTENSION_NAME;
		}
		//if (!found_VK_KHR_external_memory_capabilities) {
		//	extNames[extIndex++] = (const char*)&VK_KHR_EXTERNAL_MEMORY_CAPABILITIES_EXTENSION_NAME;
		//}
		createInfo->ppEnabledExtensionNames = extNames;
	}
	else {
		DbgOut("\nNeeded device extensions are not available");
	}
#pragma endregion

	// retrieve a usable queue, in order to issue our copy command
	uint32_t qFamilyIdx = 0;
#pragma region(usablequeue)
	// find or create a usable queue
	uint32_t queueFamilyPropertyCount;
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

	// feed our dispatch table for the functions we need (function pointer into the next layer)
	dispatchTable->GetDeviceProcAddr = (PFN_vkGetDeviceProcAddr)gdpa(*pDevice, "vkGetDeviceProcAddr");
	dispatchTable->DestroyDevice = (PFN_vkDestroyDevice)gdpa(*pDevice, "vkDestroyDevice");

	dispatchTable->CreateSwapchainKHR = (PFN_vkCreateSwapchainKHR)gdpa(*pDevice, "vkCreateSwapchainKHR");
	dispatchTable->DestroySwapchainKHR = (PFN_vkDestroySwapchainKHR)gdpa(*pDevice, "vkDestroySwapchainKHR");
	dispatchTable->QueuePresentKHR = (PFN_vkQueuePresentKHR)gdpa(*pDevice, "vkQueuePresentKHR");

	dispatchTable->AllocateMemory = (PFN_vkAllocateMemory)gdpa(*pDevice, "vkAllocateMemory");
	dispatchTable->FreeMemory = (PFN_vkFreeMemory)gdpa(*pDevice, "vkFreeMemory");
	dispatchTable->MapMemory = (PFN_vkMapMemory)gdpa(*pDevice, "vkMapMemory");
	dispatchTable->BindImageMemory = (PFN_vkBindImageMemory)gdpa(*pDevice, "vkBindImageMemory");

	dispatchTable->GetSwapchainImagesKHR = (PFN_vkGetSwapchainImagesKHR)gdpa(*pDevice, "vkGetSwapchainImagesKHR");

	dispatchTable->GetMemoryWin32HandleKHR = (PFN_vkGetMemoryWin32HandleKHR)gdpa(*pDevice, "vkGetMemoryWin32HandleKHR");
	dispatchTable->CreateImage = (PFN_vkCreateImage)gdpa(*pDevice, "vkCreateImage");
	dispatchTable->GetImageMemoryRequirements = (PFN_vkGetImageMemoryRequirements)gdpa(*pDevice, "vkGetImageMemoryRequirements");

	dispatchTable->BeginCommandBuffer = (PFN_vkBeginCommandBuffer)gdpa(*pDevice, "vkBeginCommandBuffer");
	dispatchTable->EndCommandBuffer = (PFN_vkEndCommandBuffer)gdpa(*pDevice, "vkEndCommandBuffer");
	dispatchTable->CmdCopyImage = (PFN_vkCmdCopyImage)gdpa(*pDevice, "vkCmdCopyImage");

	return VK_SUCCESS;
}

VKAPI_ATTR void VKAPI_CALL OBS_DestroyDevice(VkDevice device, const VkAllocationCallbacks* pAllocator) {
	pAllocator; //unused
	RemoveDevice(&device);
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_CreateSwapchainKHR(VkDevice device, const VkSwapchainCreateInfoKHR* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkSwapchainKHR* pSwapchain) {
	DbgOut("\n##### CreateSwapchainKHR #####");
	deviceData* devData = GetDeviceData(TOKEY(device));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	swapchainData* swchData = FindSwapchainData(devData, pCreateInfo->surface);




	VkExternalMemoryImageCreateInfo extMemImageCreateInfo;
	extMemImageCreateInfo.sType = VK_STRUCTURE_TYPE_EXTERNAL_MEMORY_IMAGE_CREATE_INFO;
	extMemImageCreateInfo.pNext = NULL;
	extMemImageCreateInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT;

	VkImageCreateInfo createInfo;
	createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	createInfo.pNext = &extMemImageCreateInfo;
	createInfo.flags = 0;			// VkImageCreateFlags       
	createInfo.imageType = VK_IMAGE_TYPE_2D;
	createInfo.format = pCreateInfo->imageFormat;
	createInfo.extent.width = pCreateInfo->imageExtent.width;
	createInfo.extent.height = pCreateInfo->imageExtent.height;
	createInfo.extent.depth = 1;
	createInfo.mipLevels = 1;
	createInfo.arrayLayers = 1;
	createInfo.samples = 1;			// VkSampleCountFlagBits    
	createInfo.tiling = VK_IMAGE_TILING_OPTIMAL;			// VkImageTiling            
	createInfo.usage = pCreateInfo->imageUsage;			// VkImageUsageFlags        
	createInfo.sharingMode = pCreateInfo->imageSharingMode;			// VkSharingMode            
	createInfo.queueFamilyIndexCount = pCreateInfo->queueFamilyIndexCount;	// uint32_t                 
	createInfo.pQueueFamilyIndices = pCreateInfo->pQueueFamilyIndices;		// const uint32_t*          
	createInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

	dispatchTable->CreateImage(device, &createInfo, NULL, &swchData->exportedImages[0]);
	dispatchTable->CreateImage(device, &createInfo, NULL, &swchData->exportedImages[1]);

	VkExportMemoryAllocateInfo expMemAllocInfo;
	expMemAllocInfo.sType = VK_STRUCTURE_TYPE_EXPORT_MEMORY_ALLOCATE_INFO;
	expMemAllocInfo.pNext = NULL;
	expMemAllocInfo.handleTypes = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT;

	VkMemoryRequirements memRequirements;
	dispatchTable->GetImageMemoryRequirements(device, swchData->exportedImages[0], &memRequirements);
	dispatchTable->GetImageMemoryRequirements(device, swchData->exportedImages[1], &memRequirements);

	uint32_t memoryTypeIndex = 0;

	VkLayerInstanceDispatchTable* instDisp = GetInstanceDispatchTable(TOKEY(devData->physicalDevice));


	VkPhysicalDeviceMemoryProperties memProperties;
	instDisp->GetPhysicalDeviceMemoryProperties(devData->physicalDevice, &memProperties);

	for (; memoryTypeIndex < memProperties.memoryTypeCount; memoryTypeIndex++) {
		if ((memRequirements.memoryTypeBits & (1 << memoryTypeIndex)) && (memProperties.memoryTypes[memoryTypeIndex].propertyFlags & VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) == VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT) {
			break;
		}
	}

	VkMemoryAllocateInfo memAllocInfo;
	memAllocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memAllocInfo.pNext = &expMemAllocInfo;
	memAllocInfo.allocationSize = memRequirements.size;
	memAllocInfo.memoryTypeIndex = memoryTypeIndex;

	dispatchTable->AllocateMemory(device, &memAllocInfo, NULL, &swchData->exportedImagesMemory[0]);
	dispatchTable->AllocateMemory(device, &memAllocInfo, NULL, &swchData->exportedImagesMemory[1]);

	dispatchTable->BindImageMemory(device, swchData->exportedImages[0], swchData->exportedImagesMemory[0], 0);
	dispatchTable->BindImageMemory(device, swchData->exportedImages[1], swchData->exportedImagesMemory[1], 0);

	swchData->getWin32HandleInfo[0].sType = VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR;
	swchData->getWin32HandleInfo[0].pNext = NULL;
	swchData->getWin32HandleInfo[0].memory = swchData->exportedImagesMemory[0];
	swchData->getWin32HandleInfo[0].handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT;

	swchData->getWin32HandleInfo[1].sType = VK_STRUCTURE_TYPE_MEMORY_GET_WIN32_HANDLE_INFO_KHR;
	swchData->getWin32HandleInfo[1].pNext = NULL;
	swchData->getWin32HandleInfo[1].memory = swchData->exportedImagesMemory[1];
	swchData->getWin32HandleInfo[1].handleType = VK_EXTERNAL_MEMORY_HANDLE_TYPE_D3D11_TEXTURE_BIT;

	dispatchTable->GetMemoryWin32HandleKHR(device, &swchData->getWin32HandleInfo[0], &swchData->handle[0]);
	dispatchTable->GetMemoryWin32HandleKHR(device, &swchData->getWin32HandleInfo[1], &swchData->handle[1]);

	VkResult res = dispatchTable->CreateSwapchainKHR(device, pCreateInfo, pAllocator, pSwapchain);

	swchData->swapchain = *pSwapchain;

	return res;
}

VKAPI_ATTR void VKAPI_CALL OBS_DestroySwapchainKHR(VkDevice device, VkSwapchainKHR swapchain, const VkAllocationCallbacks* pAllocator) {
	deviceData* devData = GetDeviceData(TOKEY(device));
	VkLayerDispatchTable* dispatchTable = &devData->dispatchTable;

	swapchainData* swchData = GetSwapchainData(devData, swapchain);

	dispatchTable->FreeMemory(device, swchData->exportedImagesMemory[0], NULL);
	dispatchTable->FreeMemory(device, swchData->exportedImagesMemory[1], NULL);

	dispatchTable->DestroyImage(device, swchData->exportedImages[0], NULL);
	dispatchTable->DestroyImage(device, swchData->exportedImages[1], NULL);

	swchData->handle[0] = INVALID_HANDLE_VALUE;
	swchData->handle[1] = INVALID_HANDLE_VALUE;

	swchData->swapchain = VK_NULL_HANDLE;

	dispatchTable->DestroySwapchainKHR(device, swapchain, pAllocator);
}

VKAPI_ATTR VkResult VKAPI_CALL OBS_QueuePresentKHR(VkQueue queue, const VkPresentInfoKHR* pPresentInfo)
{
	VkLayerDispatchTable* dispatchTable = GetDeviceDispatchTable(TOKEY(queue));
	DbgOut("\n##### QueuePresentKHR #####");
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
	DbgOut("\n##### CreateWin32SurfaceKHR #####");

	VkResult res = instData->dispatchTable.CreateWin32SurfaceKHR(instance, pCreateInfo, pAllocator, pSurface);

	surfaceData* surfData = FindSurfaceData(instData, *pSurface);

	surfData->hinstance = pCreateInfo->hinstance;
	surfData->hwnd = pCreateInfo->hwnd;

	return res;
}

#define GETPROCADDR(func) if(!strcmp(funcName, "vk" #func)) return (PFN_vkVoidFunction)&OBS_##func;

VK_LAYER_EXPORT VKAPI_ATTR PFN_vkVoidFunction VKAPI_CALL OBS_GetDeviceProcAddr(VkDevice dev, const char *funcName) {
	DbgOut2("\n##### vkGetDeviceProcAddr [%s] called on device %p #####", funcName, dev);
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
	DbgOut2("\n##### vkGetInstanceProcAddr [%s] called on instance %p #####", funcName, instance);
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

VK_LAYER_EXPORT VKAPI_ATTR VkResult VKAPI_CALL OBS_NegotiateLoaderLayerInterfaceVersion(VkNegotiateLayerInterface *pVersionStruct) {

	pVersionStruct->sType = LAYER_NEGOTIATE_INTERFACE_STRUCT;
	pVersionStruct->pNext = NULL;
	pVersionStruct->loaderLayerInterfaceVersion = 2;
	pVersionStruct->pfnGetInstanceProcAddr = OBS_GetInstanceProcAddr;
	pVersionStruct->pfnGetDeviceProcAddr = OBS_GetDeviceProcAddr;
	pVersionStruct->pfnGetPhysicalDeviceProcAddr = NULL;

	return VK_SUCCESS;
}


#include "graphics-hook.h"
bool hook_vulkan(void) {


	static bool hookingVulkan = true;
	while (hookingVulkan) {
		DbgOut("hooking vulkan");
		Sleep(1000);
	}


	return true;
}



#endif
