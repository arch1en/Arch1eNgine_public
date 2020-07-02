from pathlib import Path
import uuid
import os
import yaml
import importlib

from src.ProjectFilesBuilder import MSVSBuilder

ProjectRootDir = Path().absolute().parent.parent


def GetIntermediateFilesDirName():
    return "Build"

builder = MSVSBuilder.MsvsBuilder();

ApplicationProperties = yaml.safe_load(open(os.path.join(ProjectRootDir, "Application.yaml"), "r"))

RootProject = MSVSBuilder.MsvsRootProject()
RootProject.DirectoryOverride = os.path.join(ProjectRootDir)
RootProject.Properties = ApplicationProperties
RootProject.Name = "Arch1eNgine"

for Configuration in ApplicationProperties['Configurations']:
    RootProject.Configurations.append(Configuration['Name'])

for Platform in ApplicationProperties['Platforms']:
    RootProject.Platforms.append(Platform['Name'])

ApplicationsProjectFolder = MSVSBuilder.MsvsFolderProject()
ApplicationsProjectFolder.Name = "Applications"
RootProject.AddChildProject(ApplicationsProjectFolder)

ModulesProjectFolder = MSVSBuilder.MsvsFolderProject()
ModulesProjectFolder.Name = "Modules"
RootProject.AddChildProject(ModulesProjectFolder)

ForeignsProjectFolder = MSVSBuilder.MsvsFolderProject()
ForeignsProjectFolder.Name = "Foreigns"
RootProject.AddChildProject(ForeignsProjectFolder)

for subdir, dir, files in os.walk(ProjectRootDir):
    for file in files:
        if file == 'Properties.yaml':
            filepath = subdir + os.sep + file

            f = open(filepath, "r")
            Properties = yaml.safe_load(f)

            NewDependencyProject = MSVSBuilder.MsvsSubProject()
            NewDependencyProject.Name = Properties['Name']
            NewDependencyProject.DirectoryOverride = subdir + os.sep

            try:
                if Properties['DependencyType'] == "Module":
                    ModulesProjectFolder.AddChildProject(NewDependencyProject)
                elif Properties['DependencyType'] == "Foreign":
                    ForeignsProjectFolder.AddChildProject(NewDependencyProject)
                elif Properties['DependencyType'] == "Application":
                    ApplicationsProjectFolder.AddChildProject(NewDependencyProject)
            except:
                raise Exception(f"{filepath} Configuration file is ill-formed.")

builder.SetRootProject(RootProject)
builder.GenerateRequiredFiles()

print(uuid.uuid4())
print(ProjectRootDir)
