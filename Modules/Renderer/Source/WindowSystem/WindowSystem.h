#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <vector>
#include <memory>

#include "WindowSystemUtilities.h"
#include "Implementations/Window_Impl.h"

class WindowSystem
{
public:

	void CreateNewWindow(WindowProperties aProperties);
	I::Window_Impl* GetMainWindow();
	bool IsMainWindowAvailable();
	void DestroyAllWindows();

private:

	std::vector<std::unique_ptr<I::Window_Impl>> Windows;
};


#endif
