from xml.etree import ElementTree as ET
import os
import uuid
from pathlib import Path

from xml.dom import minidom

from src.ProjectFilesBuilder import BaseBuilder
from src import CommonFunctions


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
        return (Project.GetProjectDir().replace(self.GetRootProject().GetRootDir() + os.sep,
                                                "")) + os.sep + Project.Name + ".vcxproj"

    def GenerateRequiredFiles(self):
        assert (self.RootProject.Name != "")
        assert (self.RootProject.GetRootDir() != "")

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
        assert Project.Name != ""
        assert Project.GetRootDir() != ""
        assert Project.TypeGUID is not None

        ProjectBuilderProperties = CommonFunctions.GetProjectBuilderPropertiesByName(self.RootProject.Properties, 'MSVS')

        if not isinstance(Project, MsvsRootProject):
            self.SlnBody += 'Project("{' + Project.TypeGUID + '}") = "' + Project.Name + '", "' + str(
                Project.Name if isinstance(Project, MsvsFolderProject) else self.GetSlnRelativeVcxprojPath(
                    Project)) + '", "{' + str(Project.GUID) + '}"\rEndProject\r'

            # NestedProjects
            for ChildProject in Project.ChildProjects:
                self.SlnGlobalSectionNestedProjects.append(
                    '\t{' + str(ChildProject.GUID) + '} = {' + str(Project.GUID) + '}\r')

        # if isinstance(Project, BaseBuilder.RootProject):
        # ProjectConfigurationPlatforms

        # *.vcxproj
        if not isinstance(Project, MsvsFolderProject):

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:

                    PlatformProperties = CommonFunctions.GetProjectBuilderPlatformPropertiesByName(ProjectBuilderProperties, Platform)

                    ConfigurationPlatformPair = f"{Configuration}|{Platform}"
                    ConfigurationPlatformMappedPair = f'{Configuration}|{PlatformProperties["MappingName"]}'

                    self.SlnGlobalSectionProjectConfigurationPlatforms.append('\t{' + str(
                        Project.GUID) + '}.' + ConfigurationPlatformPair + '.ActiveCfg = ' + ConfigurationPlatformMappedPair + '\r')
                    self.SlnGlobalSectionProjectConfigurationPlatforms.append('\t{' + str(
                        Project.GUID) + '}.' + ConfigurationPlatformPair + '.Build.0 = ' + ConfigurationPlatformMappedPair + '\r')

            # Header
            VcxprojProject = ET.Element('Project', {'DefaultTargets': "Build",
                                                    'xmlns': 'http://schemas.microsoft.com/developer/msbuild/2003'})
            # ProjectConfigurations
            VcxprojItemGroupProjectConfigurations = ET.SubElement(VcxprojProject, 'ItemGroup',
                                                                  {'Label': "ProjectConfigurations"})

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:
                    PlatformProperties = CommonFunctions.GetProjectBuilderPlatformPropertiesByName(ProjectBuilderProperties, Platform)

                    ConfigurationPlatformPair = f"{Configuration}|{Platform}"
                    VcxprojProjectConfiguration = ET.SubElement(VcxprojItemGroupProjectConfigurations,
                                                                'ProjectConfiguration',
                                                                {'Include': ConfigurationPlatformPair})
                    ET.SubElement(VcxprojProjectConfiguration, 'Configuration').text = Configuration
                    ET.SubElement(VcxprojProjectConfiguration, 'Platform').text = PlatformProperties['MappingName']

            # Headers include.

            VcxprojItemGroupHeaders = ET.SubElement(VcxprojProject, 'ItemGroup')
            VcxprojItemGroupNones = ET.SubElement(VcxprojProject, 'ItemGroup')
            VcxprojItemGroupCompiles = ET.SubElement(VcxprojProject, 'ItemGroup')

            for path, dirs, files in os.walk(Project.GetSourceDir()):
                for file in files:
                    if file.endswith('h') or file.endswith('hpp'):
                        ET.SubElement(VcxprojItemGroupHeaders, 'ClInclude', {'Include': os.path.join(path, file)})
                    elif file.endswith('inl'):
                        ET.SubElement(VcxprojItemGroupNones, 'None', {'Include': os.path.join(path, file)})
                    elif file.endswith('cpp') or file.endswith('c'):
                        ET.SubElement(VcxprojItemGroupCompiles, 'ClCompile', {'Include': os.path.join(path, file)})

            # PropertyGroup Globals
            VcxprojPropertyGroupLabelGlobal = ET.SubElement(VcxprojProject, 'PropertyGroup', {'Label': 'Globals'})

            ProjectBuilderProperties = CommonFunctions.GetProjectBuilderPropertiesByName(self.RootProject.Properties, 'MSVS')

            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'VCProjectVersion').text = str(ProjectBuilderProperties['DefaultProjectVersion'])
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'ProjectGuid').text = '{' + Project.GUID + '}'
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'Keyword').text = 'Win32Proj'
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'RootNamespace').text = Project.Name
            ET.SubElement(VcxprojPropertyGroupLabelGlobal, 'WindowsTargetPlatformVersion').text = str(ProjectBuilderProperties['WindowsTargetPlatformVersion'])

            ET.SubElement(VcxprojProject, 'Import',
                                         {'Project': '$(VCTargetsPath)\Microsoft.Cpp.Default.props'})

            # PropertyGroups Conditional Label

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:

                    PlatformProperties = CommonFunctions.GetProjectBuilderPlatformPropertiesByName(
                        ProjectBuilderProperties, Platform)

                    RootMsvsProjectBuilder = self.RootProject.Properties['ProjectBuilders'][
                        CommonFunctions.GetProjectBuilderIndexByName(self.RootProject.Properties, "MSVS")]

                    PropertyGroup = ET.SubElement(VcxprojProject, 'PropertyGroup', {
                        'Condition': f"\'$(Configuration)|$(Platform)\'==\'{Configuration}|{PlatformProperties['MappingName']}\'", 'Label': 'Configuration'})

                    ConfigurationType = ET.SubElement(PropertyGroup, 'ConfigurationType')
                    ConfigurationType.text = "Application"

                    UseDebugLibraries = ET.SubElement(PropertyGroup, 'UseDebugLibraries')
                    UseDebugLibraries.text = 'true' if self.RootProject.GetConfigurationPlatformProperties(Configuration, Platform)[1]["DebugLevel"] > 0 else 'false'

                    PlatformToolset = ET.SubElement(PropertyGroup, 'PlatformToolset')
                    PlatformToolset.text = RootMsvsProjectBuilder['PlatformToolset']

                    WholeProgramOptimization = ET.SubElement(PropertyGroup, 'WholeProgramOptimization')
                    WholeProgramOptimization.text = 'true' if self.RootProject.GetConfigurationPlatformProperties(Configuration, Platform)[1]["WholeProgramOptimization"] > 0 else 'false'

                    CharacterSet = ET.SubElement(PropertyGroup, 'CharacterSet')
                    CharacterSet.text = RootMsvsProjectBuilder['CharacterSet']

            ET.SubElement(VcxprojProject, 'Import', {'Project' : '$(VCTargetsPath)\Microsoft.Cpp.props'})
            ET.SubElement(VcxprojProject, 'ImportGroup', {'Label': 'ExtensionSettings'})
            ET.SubElement(VcxprojProject, 'ImportGroup', {'Label': 'Shared'})

            # ImportGroup Label Condition

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:
                    ImportGroup = ET.SubElement(VcxprojProject, 'ImportGroup', {'Label': 'PropertySheets', 'Condition': f'\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\''})
                    ET.SubElement(ImportGroup, 'Import', {'Project': '$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props', 'Condition':'exists(\'$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\')', 'Label': 'LocalAppDataPlatform'})

            ET.SubElement(VcxprojProject, 'PropertyGroup', {'Label': 'UserMacros'})

            # PropertyGroups Conditional

            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:
                    PropertyGroup = ET.SubElement(VcxprojProject, 'PropertyGroup', {'Condition': f'\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\''})
                    LinkIncremental = ET.SubElement(PropertyGroup, 'LinkIncremental')
                    LinkIncremental.text = str(self.RootProject.GetConfigurationPlatformProperties(Configuration, Platform)[1]['LinkIncremental'])
                    OutputDir = ET.SubElement(PropertyGroup, 'OutDir')
                    OutputDir.text = str(Path(Project.GetBinariesDir()) / Configuration / Platform)
                    IntermediateDir = ET.SubElement(PropertyGroup, 'IntDir')
                    IntermediateDir.text = str(Path(Project.GetIntermediateDir()) / Configuration / Platform)

            # Item Definition Groups

            # Todo, check this for errors.
            for Configuration in self.RootProject.Configurations:
                for Platform in self.RootProject.Platforms:

                    ConfigurationProperties, PlatformProperties = self.RootProject.GetConfigurationPlatformProperties(Configuration, Platform)

                    ItemDefinitionGroup = ET.SubElement(VcxprojProject, 'ItemDefinitionGroup', {'Condition': f'\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\''})
                    ClCompile = ET.SubElement(ItemDefinitionGroup, 'ClCompile')

                    WarningLevel = ET.SubElement(ClCompile, 'WarningLevel')
                    WarningLevel.text = 'Level' + str(PlatformProperties['WarningLevel']).casefold()

                    FunctionLevelLinking = ET.SubElement(ClCompile, 'FunctionLevelLinking')
                    FunctionLevelLinking.text = 'true'

                    IntrinsicFunctions = ET.SubElement(ClCompile, 'IntrinsicFunctions')
                    IntrinsicFunctions.text = 'true'

                    SDLCheck = ET.SubElement(ClCompile, 'WarningLevel')
                    SDLCheck.text = str(PlatformProperties['SDLCheck']).casefold()

                    PreprocessorDefinitions = ET.SubElement(ClCompile, 'PreprocessorDefinitions')
                    PreprocessorDefinitions.text = ';'.join([x for x in PlatformProperties['PreprocessorDefinitions'] ]) + ';%(PreprocessorDefinitions)'

                    ConformanceMode = ET.SubElement(ClCompile, 'ConformanceMode')
                    ConformanceMode.text = str(PlatformProperties['ConformanceMode']).casefold()

                    LanguageStandard = ET.SubElement(ClCompile, 'LanguageStandard')
                    LanguageStandard.text = str(RootMsvsProjectBuilder['LanguageStandard']).casefold()

                    Link = ET.SubElement(ItemDefinitionGroup, 'Link')

                    SubSystem = ET.SubElement(Link, 'SubSystem')
                    SubSystem.text = "Console"

                    EnableCOMDATFolding = ET.SubElement(Link, 'EnableCOMDATFolding')
                    EnableCOMDATFolding.text = "true"

                    OptimizeReferences = ET.SubElement(Link, 'OptimizeReferences')
                    OptimizeReferences.text = "true"

                    GenerateDebugInformation = ET.SubElement(Link, 'GenerateDebugInformation')
                    GenerateDebugInformation.text = "true"

            ET.SubElement(VcxprojProject, 'Import', {'Project': '$(VCTargetsPath)\Microsoft.Cpp.targets'})
            ET.SubElement(VcxprojProject, 'ImportGroup', {'Label': 'ExtensionTargets'})

            # Saving.
            VcxprojElementTree = ET.ElementTree(VcxprojProject)
            VcxprojElementTreeRawString = ET.tostring(VcxprojProject, 'utf-8')
            VcxprojElementTreeReparsed = minidom.parseString(VcxprojElementTreeRawString)

            VcxprojPrettyXml = VcxprojElementTreeReparsed.toprettyxml(indent='\t')
            VcxprojFilePath = os.path.join(Project.GetProjectDir(), Project.Name) + ".vcxproj"
            Path(Project.GetProjectDir()).mkdir(parents=True,
                                                exist_ok=True)  # Make sure to create a directory tree for the files.

            with open(VcxprojFilePath, "w+") as f:
                f.write(VcxprojPrettyXml)
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
                        VcxprojFiltersClInclude = ET.SubElement(VcxprojFiltersClIncludes, 'ClInclude',
                                                                {'Include': os.path.join(path, file)})
                        FilterPath = path
                        FilterPath = FilterPath.replace(Project.GetSourceDir(), '')
                        if len(FilterPath) > 0:
                            VcxprojFiltersClIncludeFilter = ET.SubElement(VcxprojFiltersClInclude, 'Filter')
                            VcxprojFiltersClIncludeFilter.text = FilterPath
                    elif file.endswith('cpp') or file.endswith('c'):
                        ET.SubElement(VcxprojFiltersClCompiles, 'ClCompile', {'Include': os.path.join(path, file)})

            # Saving
            VcxprojFiltersElementTreeRawString = ET.tostring(VcxprojFilters, 'utf-8')
            VcxprojFiltersElementTreeReparsed = minidom.parseString(VcxprojFiltersElementTreeRawString)
            Path(Project.GetProjectDir()).mkdir(parents=True,
                                                exist_ok=True)  # Make sure to create a directory tree for the files.
            VcxprojFiltersPrettyXml = VcxprojFiltersElementTreeReparsed.toprettyxml(indent='\t')

            VcxprojFiltersFilePath = os.path.join(Project.GetProjectDir(), Project.Name) + ".vcxproj.filters"
            with open(VcxprojFiltersFilePath, "w+") as f:
                f.write(VcxprojFiltersPrettyXml)
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
                           self.SlnGlobalSectionSolutionConfigurationPlatforms, self.SlnGlobalSectionSolutionProperties,
                           self.SlnGlobalSectionNestedProjects,
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

    def GetConfigurationPlatformProperties(self, ConfigurationName, PlatformName):
        for Configuration in self.Properties['Configurations']:
            if Configuration["Name"] == ConfigurationName:
                for Platform in Configuration["Platforms"]:
                    if Platform["Name"] == PlatformName:
                        return Configuration, Platform

        return None, None



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
