#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vulkan/vulkan.h>

void get_gpu_model_names(char *gpu_models[], int *gpu_count) {
    VkInstance instance;
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "GPU Info",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "No Engine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_0,
    };

    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &appInfo,
    };

    if (vkCreateInstance(&createInfo, NULL, &instance) != VK_SUCCESS) {
        fprintf(stderr, "Failed to create Vulkan instance.\n");
        *gpu_count = 0;
        return;
    }

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, NULL);

    if (deviceCount == 0) {
        fprintf(stderr, "Failed to find GPUs with Vulkan support.\n");
        *gpu_count = 0;
        vkDestroyInstance(instance, NULL);
        return;
    }

    size_t total_size = deviceCount * VK_MAX_PHYSICAL_DEVICE_NAME_SIZE;
    char *memory_block = malloc(total_size);

    if (!memory_block) {
        fprintf(stderr, "Memory allocation failed.\n");
        *gpu_count = 0;
        vkDestroyInstance(instance, NULL);
        return;
    }

    *gpu_count = deviceCount;
    VkPhysicalDevice devices[deviceCount];
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices);

    for (uint32_t i = 0; i < deviceCount; i++) {
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(devices[i], &deviceProperties);

        gpu_models[i] = memory_block + (i * VK_MAX_PHYSICAL_DEVICE_NAME_SIZE);
        memcpy(gpu_models[i], deviceProperties.deviceName, VK_MAX_PHYSICAL_DEVICE_NAME_SIZE);
        gpu_models[i][VK_MAX_PHYSICAL_DEVICE_NAME_SIZE - 1] = '\0'; // Null-terminate
    }

    vkDestroyInstance(instance, NULL);
}


