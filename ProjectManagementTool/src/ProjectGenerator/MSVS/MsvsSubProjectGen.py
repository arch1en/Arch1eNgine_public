
class ItemDefinitionGroup:

    def __init__(self):
        #ClCompile
        self.WarningLevel = 3
        self.FunctionLevelLinking = True
        self.IntrinsicFunctions = True
        self.SDLCheck = True
        self.PreprocessorDefinitions = []
        self.ConformanceMode = True
        self.LanguageStandard = 'stdcpp17'
        self.AdditionalIncludeDirectories = []

        #Link
        self.SubSystem = 'Console'
        self.EnableCOMDATFolding = True
        self.OptimizeReferences = True
        self.GenerateDebugInformation = True
        self.AdditionalLibraryDirectories = []
        self.AdditionalDependencies = []

class GlobalPropertyGroup:

    def __init__(self):
        self.ProjectVersion = 16.0
        self.ProjectGuid = '{CDBA3BF1-1687-46B9-B0C1-5A537DF9AC05}'
        self.Keyword = 'Win32Proj'
        self.RootNamespace = ''
        self.WindowsTargetPlatformVersion = 10.0

class PropertyGroupConditionalLabeled:
    def __init__(self):
        self.ConfigurationType = 'Application'
        self.UseDebugLibraries = True
        self.PlatformToolset = 'v142'
        self.WholeProgramOptimization = False
        self.CharacterSet = 'Unicode'

class PropertyGroupConditional:
    def __init__(self):


def GenerateMsvsSubProjectFile(ConfigurationPlatformPairs, CompilationFiles, HeaderFiles, OtherFiles, GlobalPropertyGroup, ItemDefinitionGroup, PropertyGroup):
    Node = "<?xml version=\"1.0\" encoding=\"utf-8\"?>"
    Node += "<Project DefaultTargets=\"Build\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">"

    Node += AddProjectConfigurations(Configurations, Platforms)
    Node += AddCompilationFiles(CompilationFiles)
    Node += AddHeaderFiles(HeaderFiles)
    if len(OtherFiles):
        Node += AddOtherFiles(OtherFiles)
    Node += AddGlobalPropertyGroup(GlobalPropertyGroup)
    Node += "<Import Project=\"$(VCTargetsPath)\Microsoft.Cpp.Default.props\" />"
    Node += "<ImportGroup Label=\"ExtensionSettings\">"
    Node += "</ImportGroup>"
    Node += "<ImportGroup Label=\"Shared\">"
    Node += "</ImportGroup>"

    for cp in ConfigurationPlatformPairs:
            Node += f"  <ImportGroup Label=\"PropertySheets\" Condition=\"\'$(Configuration)|$(Platform)'=='{cp}\'\">"
            Node += "<Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
            Node += "</ImportGroup>"

    Node += "<PropertyGroup Label=\"UserMacros\" />"

    for cp in ConfigurationPlatformPairs:
            Node += "<PropertyGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'Debug|Win32\'\">"
            Node += " <LinkIncremental>true</LinkIncremental>"
            Node += " <EnableClangTidyCodeAnalysis>true</EnableClangTidyCodeAnalysis>"
            Node += "</PropertyGroup>"

    Node += "</Project>"
    return Node

def AddProjectConfigurations(Configurations, Platforms):
    Node = "<ItemGroup Label=\"ProjectConfigurations\">"

    for c in Configurations:
        for p in Platforms:
            Node += f" <ProjectConfiguration Include=\"{c}|{p}\">"
            Node += f" <Configuration>{c}</Configuration>"
            Node += f"<Platform>{p}</Platform>"
            Node += "</ProjectConfiguration>"

    Node += "</ItemGroup>"
    return Node

# Add cpp files.
def AddCompilationFiles(Paths):
        Node = "<ItemGroup>\n"
        for i in Paths:
            Node += f"<ClCompile Include =\"{i}\" />\n"
        Node += "</ItemGroup>"
        return Node

def AddHeaderFiles(self, paths):
        Node = "<ItemGroup>\n"
        for i in paths:
            Node += f"<ClInclude Include=\"{i}\" />\n"
        Node += "</ItemGroup>"
        return Node

def AddOtherFiles(self, paths):
        Node = "<ItemGroup>\n"
        for i in paths:
            Node += f"<ClInclude Include=\"{i}\" />\n"
        Node += "</ItemGroup>"
        return Node

def AddItemDefinitionGroup(self, Configuration, Platform, ItemDefinitionGroupData):
        Node = f"<ItemDefinitionGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\'\">"
        Node += "   <ClCompile>"
        Node += "       <PrecompiledHeader>"
        Node += "       </PrecompiledHeader>"
        Node += f"      <WarningLevel>Level{ItemDefinitionGroupData.WarningLevel}</WarningLevel>"
        Node += f"      <FunctionLevelLinking>{str(ItemDefinitionGroupData.FunctionLevelLinking)}</FunctionLevelLinking>"
        Node += f"      <IntrinsicFunctions>{str(ItemDefinitionGroupData.IntrinsicFunctions)}</IntrinsicFunctions>"
        Node += f"      <SDLCheck>{str(ItemDefinitionGroupData.SDLCheck)}</SDLCheck>"
        Node += f"      <PreprocessorDefinitions>{';'.join(x for x in ItemDefinitionGroupData.PreprocessorDefinitions)}</PreprocessorDefinitions>"
        Node += f"      <PreprocessorDefinitions>{ItemDefinitionGroupData.ConformanceMode}</PreprocessorDefinitions>"
        Node += f"      <LanguageStandard>{ItemDefinitionGroupData.LanguageStandard}</LanguageStandard>"
        Node += f"      <AdditionalIncludeDirectories>{';'.join(x for x in ItemDefinitionGroupData.AdditionalIncludeDirectories)}</AdditionalIncludeDirectories>"
        Node += "   </ClCompile>"
        Node += "   <Link>"
        Node += f"       <SubSystem>{ItemDefinitionGroupData.Console}</SubSystem>"
        Node += f"       <EnableCOMDATFolding>{ItemDefinitionGroupData.EnableCOMDATFolding}</EnableCOMDATFolding>"
        Node += f"       <OptimizeReferences>{ItemDefinitionGroupData.OptimizeReferences}</OptimizeReferences>"
        Node += f"       <GenerateDebugInformation>{ItemDefinitionGroupData.GenerateDebugInformation}</GenerateDebugInformation>"
        Node += f"       <AdditionalIncludeDirectories>{';'.join(x for x in ItemDefinitionGroupData.AdditionalIncludeDirectories)}</AdditionalIncludeDirectories>"
        Node += f"       <AdditionalLibraryDirectories>{';'.join(x for x in ItemDefinitionGroupData.AdditionalLibraryDirectories)}</AdditionalLibraryDirectories>"
        Node += f"       <AdditionalDependencies>{';'.join(x for x in ItemDefinitionGroupData.AdditionalLibraryDirectories)}</AdditionalDependencies>"
        Node += "   </Link>"
        Node += "</ItemDefinitionGroup>"

        return Node

def AddGlobalPropertyGroup(self, globalPropertyGroup):
        Node = f"<PropertyGroup Label=\"Globals\">"
        Node += "   <PrecompiledHeader>"
        Node += "   </PrecompiledHeader>"
        Node += f"  <VCProjectVersion>{globalPropertyGroup.VCProjectVersion}</VCProjectVersion>"
        Node += f"  <ProjectGuid>{globalPropertyGroup.ProjectGuid}</ProjectGuid>"
        Node += f"  <Keyword>{globalPropertyGroup.Keyword}</Keyword>"
        Node += f"  <RootNamespace>{globalPropertyGroup.RootNamespace}</RootNamespace>"
        Node += f"  <WindowsTargetPlatformVersion>{globalPropertyGroup.WindowsTargetPlatformVersion}</WindowsTargetPlatformVersion>"
        Node += "</PropertyGroup>"

        return Node

def AddPropertyGroup(self, Configuration, Platform, propertyGroup):
        Node = f"<PropertyGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\'\">"
        Node += f"  <ConfigurationType>{propertyGroup.ConfigurationType}</ConfigurationType>"
        Node += f"  <UseDebugLibraries>{propertyGroup.UseDebugLibraries}</UseDebugLibraries>"
        Node += f"  <PlatformToolset>{propertyGroup.PlatformToolset}</PlatformToolset>"
        Node += f"  <CharacterSet>{propertyGroup.CharacterSet}</CharacterSet>"
        Node += "</PropertyGroup>"

        return Node

def AddImportGroup(self, Configuration, Platform):
        Node = f"<ImportGroup Condition=\"\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\'\">"
        Node += f"  <ConfigurationType>{propertyGroup.ConfigurationType}</ConfigurationType>"
        Node += f"  <UseDebugLibraries>{propertyGroup.UseDebugLibraries}</UseDebugLibraries>"
        Node += f"  <PlatformToolset>{propertyGroup.PlatformToolset}</PlatformToolset>"
        Node += f"  <CharacterSet>{propertyGroup.CharacterSet}</CharacterSet>"
        Node += "</ImportGroup>"

        return Node

def AddImportGroup(self, Configuration, Platform):
        Node =   f"<ImportGroup Label=\"PropertySheets\" Condition=\"\'$(Configuration)|$(Platform)\'==\'{Configuration}|{Platform}\'">
        Node +=  f"<Import Project=\"$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props\" Condition=\"exists('$(UserRootDir)\Microsoft.Cpp.$(Platform).user.props')\" Label=\"LocalAppDataPlatform\" />"
        Node +=  "</ImportGroup>"
