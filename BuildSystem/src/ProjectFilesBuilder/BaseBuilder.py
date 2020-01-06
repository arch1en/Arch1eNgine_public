
class BaseBuilder:

    def __init__(self):
        self.RootProject = None
        self.SubProjects = []

    def SetRootProject(self):
        pass

    def AppendSubProject(self, SubProject):
        self.SubProjects.append(SubProject)

    def GenerateRequiredFiles(self):
        pass

class Project:
    def __init__(self):
        self.Name = ""

class RootProject(Project):
    def __init__(self):
        super(RootProject, self).__init__()
        self.Directory = ""

class SubProject(Project):
    def __init__(self):
        super(SubProject, self).__init__()
        self.Directory = ""
        self.IncludeFiles = []
        self.LibraryFiles = []
        self.LibraryDependencies = []
        self.Configurations = []
        self.Platforms = []