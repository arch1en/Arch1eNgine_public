import os
import uuid


class BaseBuilder:

    def __init__(self):
        self.RootProject = None

    def SetRootProject(self):
        pass

    def GenerateRequiredFiles(self):
        pass


class BaseProject:
    def __init__(self):
        self.Name = ""
        self.DirectoryOverride = ""
        self.ChildProjects = []
        self.ParentProject = None
        self.Properties = None
        self.GUID = str(uuid.uuid4()).upper()

    def AddChildProject(self, ChildProject):
        assert(ChildProject.ParentProject is None)
        self.ChildProjects.append(ChildProject)
        ChildProject.ParentProject = self

    def RemoveChildProject(self, ChildProject):
        assert (ChildProject.ParentProject is not None)
        self.ChildProjects.remove(ChildProject)
        ChildProject.ParentProject = None

    def GetRootDir(self):
        if self.DirectoryOverride is "":
            assert self.ParentProject is not None, f"Project {self.Name} has no parent."
            return os.path.join(self.ParentProject.GetRootDir(),self.Name, os.sep)
        return self.DirectoryOverride

    def GetProjectDir(self):
        return os.path.join(self.GetRootDir(), "Project")

    def GetSourceDir(self):
        return os.path.join(self.GetRootDir(), "Source")

    def GetBinariesDir(self):
        return os.path.join(self.GetRootDir(), "Binaries")

    def GetIntermediateDir(self):
        return os.path.join(self.GetRootDir(), "Intermediate")

    def GetData(self):
        pass

class RootProject(BaseProject):
    def __init__(self):
        super(RootProject, self).__init__()
        self.Configurations = []
        self.Platforms = []

class Project(BaseProject):
    def __init__(self):
        super(Project, self).__init__()
        self.IncludeFiles = []
        self.LibraryFiles = []
        self.LibraryDependencies = []
        self.Configurations = []
        self.Platforms = []