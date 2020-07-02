from xml.etree import ElementTree as ET
import os
import uuid
from pathlib import Path

from xml.dom import minidom

from src.ProjectFilesBuilder import BaseBuilder


class MsvsBuilder(BaseBuilder.BaseBuilder):
    def __init__(self):
        super(MsvsBuilder, self).__init__()
        self.SlnBody = ""
        self.SlnGlobalSectionSolutionConfigurationPlatforms = []
        self.SlnGlobalSectionNestedProjects = []
        self.SlnGlobalSectionProjectConfigurationPlatforms = []
        self.SlnGlobalSectionSolutionProperties = []
        self.SlnGlobalSectionExtensibilityGlobals = []

    def SetRootProject(self, RootProject):
        self.RootProject = RootProject

    def GetRootProject(self):
        return self.RootProject

    def AppendSubProject(self):
        pass

    def GetSlnRelativeVcxprojPath(self, Project):
        return (Project.GetProjectDir().replace(self.GetRootProject().GetRootDir() + os.sep, "")) + os.sep + Project.Name + ".vcxproj"

    def GenerateRequiredFiles(self):
        assert (self.RootProject.Name is not "")
        assert (self.RootProject.GetRootDir() is not "")

        # *.sln

        self.SlnBody = """
Microsoft Visual Studio Solution File, Format Version 12.00
VisualStudioVersion = """ + str(self.RootProject.VisualStudioVersion) + """
MinimumVisualStudioVersion = 10.0.40219.1
        """

        self.SlnGlobalSectionSolutionConfigurationPlatforms = [
            "GlobalSection(SolutionConfigurationPlatforms) = preSolution\r"]
        self.SlnGlobalSectionNestedProjects = ["GlobalSection(NestedProjects) = preSolution\r"]
        self.SlnGlobalSectionProjectConfigurationPlatforms = [
            "GlobalSection(ProjectConfigurationPlatforms) = postSolution\r"]
        self.SlnGlobalSectionSolutionProperties = [
            "GlobalSection(SolutionProperties) = preSolution\r\tHideSolutionNode = FALSE\r"]
        self.SlnGlobalSectionExtensibilityGlobals = [
            "GlobalSection(ExtensibilityGlobals) = postSolution\r\tSolutionGuid = {" + str(
                self.RootProject.GUID) + "}\r"]

        for Configuration in self.RootProject.Configurations:
            for Platform in self.RootProject.Platforms:
                ConfigurationPlatformPair = f"{Configuration}|{Platform}"
                self.SlnGlobalSectionSolutionConfigurationPlatforms.append(
                    f"\t{ConfigurationPlatformPair} = {ConfigurationPlatformPair}\r\t")

        SlnProjectData = []
        self.RecursiveProcessProjectChain(self.RootProject)

        self.SlnGlobalSectionNestedProjects.append("EndGlobalSection\r")
        self.SlnGlobalSectionProjectConfigurationPlatforms.append("EndGlobalSection\r")
        self.SlnGlobalSectionSolutionConfigurationPlatforms.append("EndGlobalSection\r")
        self.SlnGlobalSectionSolutionProperties.append("EndGlobalSection\r")
        self.SlnGlobalSectionExtensibilityGlobals.append("EndGlobalSection\r")

        self.SetupSolutionGlobalSections()

        with open(os.path.join(self.RootProject.GetRootDir(), self.RootProject.Name) + ".sln", "w+") as f:
            f.write(self.SlnBody)

        # ~*.sln

    def RecursiveProcessProjectChain(self, Project):
        assert Project.Name is not ""
        assert Project.GetRootDir() is not ""
        assert Project.TypeGUID is not None

        if not isinstance(Project, MsvsRootProject):
            self.SlnBody += 'Project("{' + Project.TypeGUID + '}") = "' + Project.Name + '", "' + str(
                Project.Name if isinstance(Project, MsvsFolderProject) else self.GetSlnRelativeVcxprojPath(Project)) + '", "{' + str(Project.GUID) + '}"\rEndProject\r'

            # NestedProjects
            for ChildProject in Project.ChildProjects:
                self.SlnGlobalSectionNestedProjects.append(
                    '\t{' + str(ChildProject.GUID) + '} = {' + str(Project.GUID) + '}\r')

        #if isinstance(Project, BaseBuilder.RootProject):
            # ProjectConfigurationPlatforms

        # *.vcxproj
        if not isinstance(Project, MsvsFolderProject):

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:
                    ConfigurationPlatformPair = f"{Configuration}|{Platform}"
                    self.SlnGlobalSectionProjectConfigurationPlatforms.append('\t{' + str(
                        Project.GUID) + '}.' + ConfigurationPlatformPair + '.ActiveCfg = ' + ConfigurationPlatformPair + '\r')
                    self.SlnGlobalSectionProjectConfigurationPlatforms.append('\t{' + str(
                        Project.GUID) + '}.' + ConfigurationPlatformPair + '.Build.0 = ' + ConfigurationPlatformPair + '\r')

            # Header
            VcxprojProject = ET.Element('Project', {'DefaultTargets': "Build",
                                                    'xmlns': 'http://schemas.microsoft.com/developer/msbuild/2003'})
            # ProjectConfigurations
            VcxprojItemGroupProjectConfigurations = ET.SubElement(VcxprojProject, 'ItemGroup',
                                                                  {'Label': "ProjectConfigurations"})

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:
                    ConfigurationPlatformPair = f"{Configuration}|{Platform}"
                    VcxprojProjectConfiguration = ET.SubElement(VcxprojItemGroupProjectConfigurations,
                                                                'ProjectConfiguration',
                                                                {'Include': ConfigurationPlatformPair})
                    ET.SubElement(VcxprojProjectConfiguration, 'Configuration').text = Configuration
                    ET.SubElement(VcxprojProjectConfiguration, 'Platform').text = Platform

            # Headers include.

            VcxprojItemGroupHeaders = ET.SubElement(VcxprojProject, 'ItemGroup')
            VcxprojItemGroupNones = ET.SubElement(VcxprojProject, 'ItemGroup')
            VcxprojItemGroupCompiles = ET.SubElement(VcxprojProject, 'ItemGroup')

            for path, dirs, files in os.walk(Project.GetSourceDir()):
                for file in files:
                    if file.endswith('h') or file.endswith('hpp'):
                        ET.SubElement(VcxprojItemGroupHeaders, 'ClInclude', {'Include': os.path.join(path, file)})
                    elif file.endswith('inl'):
                        ET.SubElement(VcxprojItemGroupNones, 'None', {'Include':os.path.join(path, file)})
                    elif file.endswith('cpp') or file.endswith('c'):
                        ET.SubElement(VcxprojItemGroupCompiles, 'ClCompile', {'Include': os.path.join(path, file)})

            VcxprojPropertyGroupLabelGlobal = ET.SubElement(VcxprojProject, 'PropertyGroup', {'Label':'Globals'})
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'VCProjectVersion').text = str(self.RootProject.Properties['MSVS']['DefaultProjectVersion'])
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'ProjectGuid').text = '{'+Project.GUID+'}'
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'Keyword').text = 'Win32Proj'
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'RootNamespace').text = Project.Name
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'WindowsTargetPlatformVersion').text = str(self.RootProject.Properties['MSVS']['DefaultWindowsSdkVersion'])

            # Saving.
            VcxprojElementTree = ET.ElementTree(VcxprojProject)
            VcxprojElementTreeRawString = ET.tostring(VcxprojProject, 'utf-8')
            VcxprojElementTreeReparsed = minidom.parseString(VcxprojElementTreeRawString)

            VcxprojPrettyXml = VcxprojElementTreeReparsed.toprettyxml(indent='\t')
            VcxprojFilePath = os.path.join(Project.GetProjectDir(), Project.Name) + ".vcxproj"
            Path(Project.GetProjectDir()).mkdir(parents=True, exist_ok=True) # Make sure to create a directory tree for the files.
            VcxprojFile = open(VcxprojFilePath, "w+")
            VcxprojFile.write(VcxprojPrettyXml)
            VcxprojFile.close()
        # ~*.vcxproj

        # *.vcxproj.filters
        if not isinstance(Project, MsvsFolderProject):
            VcxprojFilters = ET.Element('Project', {"ToolsVersion": "Current",
                                         "xmlns": "http://schemas.microsoft.com/developer/msbuild/2003"})

            VcxprojFiltersClCompiles = ET.SubElement(VcxprojFilters, 'ItemGroup')
            VcxprojFiltersClIncludes = ET.SubElement(VcxprojFilters, 'ItemGroup')

            for path, dirs, files in os.walk(Project.GetSourceDir()):
                for file in files:
                    if file.endswith('h') or file.endswith('hpp'):
                        VcxprojFiltersClInclude = ET.SubElement(VcxprojFiltersClIncludes, 'ClInclude', {'Include': os.path.join(path, file)})
                        FilterPath = path
                        FilterPath = FilterPath.replace(Project.GetSourceDir(), '')
                        if len(FilterPath) > 0:
                            VcxprojFiltersClIncludeFilter = ET.SubElement(VcxprojFiltersClInclude, 'Filter')
                            VcxprojFiltersClIncludeFilter.text = FilterPath
                    elif file.endswith('cpp') or file.endswith('c'):
                        ET.SubElement(VcxprojFiltersClCompiles, 'ClCompile', {'Include': os.path.join(path, file)})

            VcxprojFiltersFilePath = os.path.join(Project.GetProjectDir(), Project.Name) + ".vcxproj.filters"
            with open(VcxprojFiltersFilePath,"wb") as f:
                et = ET.ElementTree(VcxprojFilters)
                et.write(f, encoding="utf-8", xml_declaration=True)
        # ~*.vcxproj.filters

        # *.vcxproj.user
        if not isinstance(Project, MsvsFolderProject):
            doc = ET.Element('Project', {"ToolsVersion": "Current",
                                         "xmlns": "http://schemas.microsoft.com/developer/msbuild/2003"})
            node = ET.SubElement(doc, 'PropertyGroup')

            et = ET.ElementTree(doc)
            Path(Project.GetProjectDir()).mkdir(parents=True, exist_ok=True)
            VcxprojUserFilePath = os.path.join(Project.GetProjectDir(), Project.Name)
            f = open(VcxprojUserFilePath + ".vcxproj.user", "wb")
            et.write(f, encoding="utf-8", xml_declaration=True)
            f.close()
        # ~*.vcxproj.user

        for ChildProject in Project.ChildProjects:
            self.RecursiveProcessProjectChain(ChildProject)

    def SetupSolutionGlobalSections(self):
        SectionsInOrder = [self.SlnGlobalSectionProjectConfigurationPlatforms,
                           self.SlnGlobalSectionSolutionConfigurationPlatforms, self.SlnGlobalSectionSolutionProperties, self.SlnGlobalSectionNestedProjects,
                           self.SlnGlobalSectionExtensibilityGlobals]
        self.SlnBody += "Global"
        for SlnSection in SectionsInOrder:
            for Data in SlnSection:
                self.SlnBody += Data
        self.SlnBody += "EndGlobal"


# aka. Solution
class MsvsRootProject(BaseBuilder.RootProject):
    def __init__(self):
        super(MsvsRootProject, self).__init__()
        self.GUID = str(uuid.uuid4()).upper()
        self.TypeGUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
        self.VisualStudioVersion = 0


# aka. Project
class MsvsSubProject(BaseBuilder.SubProject):
    def __init__(self):
        super(MsvsSubProject, self).__init__()
        self.TypeGUID = "8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942"
        self.GUID = str(uuid.uuid4()).upper()


class MsvsFolderProject(BaseBuilder.Project):
    def __init__(self):
        super(MsvsFolderProject, self).__init__()
        self.GUID = str(uuid.uuid4()).upper()
        self.TypeGUID = "2150E333-8FDC-42A3-9474-1A3956D46DE8"

    def GetData(self):
        return
