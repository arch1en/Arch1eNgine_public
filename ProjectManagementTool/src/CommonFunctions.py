
from pathlib import Path

def GetProjectRootDir():
    return Path(__file__).parent.parent.absolute()