from xml.etree import ElementTree as ET
from pathlib import Path
import uuid

from src.ProjectFilesBuilder.BaseBuilder import BaseBuilder

FolderTypeGUID =
ProjectTypeGUID =

class MsvsBuilder(BaseBuilder):
    def __init__(self):
        super(MsvsBuilder, self).__init__()
        self.SolutionName = ""

    def SetRootProject(self, RootProject):
        self.RootProject = RootProject

    def AppendSubProject(self):
        pass

    def GenerateRequiredFiles(self):
        assert(self.RootProject.Name is not "")
        assert(self.RootProject.Directory is not "")

        # *.sln

        SlnFile = open(str(Path(self.RootProject.Directory) / self.RootProject.Name) + ".sln")

        SlnBody = """
        Microsoft Visual Studio Solution File, Format Version 12.00
        VisualStudioVersion = """ + VisualStudioVersion + """
        MinimumVisualStudioVersion = 10.0.40219.1
        """


        for Project in self.SubProjects:
            assert (Project.Name is not "")
            assert (Project.Directory is not "")

            # *.vcxproj

            # *.vcxproj.filters

            # *.vcxproj.user
            doc = ET.Element('Project', {"ToolsVersion": "Current",
                                         "xmlns": "http://schemas.microsoft.com/developer/msbuild/2003"})
            node = ET.SubElement(doc, 'PropertyGroup')
            et = ET.ElementTree(doc)

            f = open(str(Path(Project.Directory) / Project.Name) + ".vcxproj.user", "wb")

            et.write(f, encoding="utf-8", xml_declaration=True)

# aka. Solution
class MsvsRootProject(BaseBuilder.RootProject):
    def __init__(self):
        super(MsvsRootProject, self).__init__()
        self.GUID = uuid.uuid4()
        self.TypeGUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
# aka. Project
class MsvsSubProject(BaseBuilder.SubProject):
    def __init__(self):
        super(MsvsRootProject, self).__init__()
        self.GUID = uuid.uuid4()

class MsvsFolderProject(BaseBuilder.Project):
    def __init__(self):
        super(MsvsFolderProject, self).__init__()
        self.TypeGUID = "2150E333-8FDC-42A3-9474-1A3956D46DE8"