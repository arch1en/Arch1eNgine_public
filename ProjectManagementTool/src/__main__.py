from pathlib import Path
import uuid
import os
import yaml
import importlib

from src import CommonFunctions

from src.ProjectFilesBuilder import MSVSBuilder

ProjectRootDir = Path().absolute().parent.parent


def GetIntermediateFilesDirName():
    return "Build"

builder = MSVSBuilder.MsvsBuilder();

ApplicationProperties = yaml.safe_load(open(os.path.join(ProjectRootDir, "Application.yaml"), "r"))

RootProject = MSVSBuilder.MsvsRootProject()
RootProject.DirectoryOverride = os.path.join(ProjectRootDir)
RootProject.Properties = ApplicationProperties
RootProject.Name = ApplicationProperties['Project']['Name']

Configurations = ApplicationProperties['Configurations']
Platforms = []
for Configuration in Configurations:
    RootProject.Configurations.append(Configuration['Name'])
    for Platform in Configuration['Platforms']:
        if Platform['Name'] not in Platforms:
            Platforms.append(Platform['Name'])
            RootProject.Platforms.append(Platform['Name'])

ApplicationProject = MSVSBuilder.MsvsProject()
ApplicationProject.Name = "Application"
ApplicationProject.DirectoryOverride = CommonFunctions.GetApplicationRootDir()
RootProject.AddChildProject(ApplicationProject)

ModulesProjectFolder = MSVSBuilder.MsvsFolderProject()
ModulesProjectFolder.Name = "Modules"
ApplicationProject.AddChildProject(ModulesProjectFolder)

ForeignsProjectFolder = MSVSBuilder.MsvsFolderProject()
ForeignsProjectFolder.Name = "Foreigns"
ApplicationProject.AddChildProject(ForeignsProjectFolder)

for subdir, dir, files in os.walk(ProjectRootDir):
    for file in files:
        if file == 'Properties.yaml':
            filepath = subdir + os.sep + file

            f = open(filepath, "r")
            Properties = yaml.safe_load(f)

            DependencyType = CommonFunctions.GetDependencyTypeByPath(filepath)

            NewDependencyProject = None

            if DependencyType != 'Application':
                if Properties['LinkageType'].casefold() == "include":
                    NewDependencyProject = MSVSBuilder.MsvsFilterProject()
                else:
                    NewDependencyProject = MSVSBuilder.MsvsSubProject()

                NewDependencyProject.Name = Properties['Name']
                NewDependencyProject.DirectoryOverride = subdir + os.sep

                try:
                    if DependencyType == "Modules":
                        ModulesProjectFolder.AddChildProject(NewDependencyProject)
                    elif DependencyType == "Foreigns":
                        ForeignsProjectFolder.AddChildProject(NewDependencyProject)
                except:
                    raise Exception(f"{filepath} Configuration file is ill-formed.")

builder.SetRootProject(RootProject)
builder.GenerateRequiredFiles()

print(uuid.uuid4())
print(ProjectRootDir)
