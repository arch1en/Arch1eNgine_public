
class MsvsProjectGen:

    def GetVersion(self, version):
        return f'VisualStudioVersion = {version}'

    def GetMinimumVersion(self, version):
        return f"MinimumVisualStudioVersion = {version}"

    def CreateSubProject(self, GUID, Name, Path):

    def AddSubProjectConfiguration(self, Configuration, Platform):
        return f"""
<ProjectConfiguration Include="{Configuration}|{Platform}">
 <Configuration>{Configuration}</Configuration>
 <Platform>{Platform}</Platform>
</ProjectConfiguration>
"""



