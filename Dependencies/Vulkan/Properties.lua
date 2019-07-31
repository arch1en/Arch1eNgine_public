Properties =
{
    Name = "Vulkan",
	IncludeDirs = { "Include" },
	LinkageType	= "Dynamic",
	UserConfig =
	{
		PathData =
		{
			Windows = {
				Registry = "HKLM:\\SOFTWARE\\LunarG\\VulkanSDK\\VK_SDK_PATHs",
				Defaults = { "D:\\_SDK\\VulkanSDK\\*\\", "X:\\_SDK\\Vulkan\\*\\" }
			}
		}
	},
	PropertyGroups =
    {
        {
            Name = "Debug|Win32",
            LinkFileNames = { "vulkan-1" },
			LibraryDir = "Lib32"
        },
        {
            Name = "Debug|Win64",
            LinkFileNames = { "vulkan-1" },
            LibraryDir = "Lib"
        }
    }
}

