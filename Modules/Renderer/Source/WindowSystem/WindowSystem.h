#ifndef WINDOWSYSTEM_H
#define WINDOWSYSTEM_H

#include <vector>
#include <memory>

#include "Implementations/Window_Impl.h"
#include "WindowSystemUtilities.h"
#include "RenderingSystem/RenderingSystemUtilities.h"

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
