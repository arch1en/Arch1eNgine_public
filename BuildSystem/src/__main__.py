from pathlib import Path
import uuid

from src.ProjectFilesBuilder import MSVSBuilder

ProjectRootDir = Path().absolute().parent.parent


def GetIntermediateFilesDirName():
    return "Build"


builder = MSVSBuilder.MsvsBuilder();
RootProject = MSVSBuilder.MsvsRootProject()

RootProject.Directory = ProjectRootDir
RootProject.Name = "Arch1eNgine"

builder.SetRootProject(RootProject)

builder.GenerateRequiredFiles()

print(uuid.uuid4())
print(ProjectRootDir)
