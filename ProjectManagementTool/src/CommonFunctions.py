
import os
from pathlib import Path

def GetProjectBuilderIndexByName(Properties, Name):
    for Index in range(len(Properties['ProjectBuilders'])):
        if Properties['ProjectBuilders'][Index]['Name'] == Name:
            return Index


def GetProjectBuilderPropertiesByName(ApplicationProperties, Name):
    for BuilderProperties in ApplicationProperties['ProjectBuilders']:
        if BuilderProperties['Name'] == Name:
            return BuilderProperties

    return None

def GetProjectBuilderPlatformPropertiesByName(ProjectBuilderProperties, Name):
    for PlatformProperties in ProjectBuilderProperties['Platforms']:
        if PlatformProperties['Name'] == Name:
            return PlatformProperties

    return None

def GetDependencyTypeByPath(DependencyPropertyFilePath):
    DependencyType = DependencyPropertyFilePath.split(os.sep)
    Type = DependencyType[-3]
    if Type == "Modules" or Type == "Foreign":
        return Type
    else:
        Type = DependencyType[-2]
        if Type != "Application":
            raise RuntimeError("Could not retrieve dependency type by path.")
        return Type


def GetApplicationRootDir():
    return Path(__file__).parent.parent.parent.absolute() / "Application"


