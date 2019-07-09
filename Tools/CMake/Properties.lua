Properties = 
{
    Name = "CMake",
	-- UserConfig is required if dependency properties suppose to be set up by the user.
	UserConfig	=
	{
		-- This helps with the default path to the dependency (a CMake executable for example)
		PathData =
		{
			Win32 = 
			{
				Registry = "HKLM:\\SOFTWARE\\Kitware\\CMake\\InstallDir",
				Defaults = { "D:\\_Tools\\CMake", "E:\\_Tools\\CMake" },
				Executable = "bin\\cmake.exe"
			}
		},
		-- This table will be put to the user config as is.
		AdditionalData =
		{
			DefaultEnvironment = "Visual Studio 16 2019"
		}
	}
}