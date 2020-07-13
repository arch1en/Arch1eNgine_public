
import os
from pathlib import Path

def GetProjectBuilderIndexByName(Properties, Name):
    for Index in range(len(Properties['ProjectBuilders'])):
        if Properties['ProjectBuilders'][Index]['Name'] == Name:
            return Index


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


