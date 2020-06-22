#ifndef GEOMETRYSYSTEMUTILITIES_H
#define GEOMETRYSYSTEMUTILITIES_H

#include "GeometrySystem/Mesh.h"

namespace GeometrySystemUtilities 
{
	// Handles single mesh import.
	bool OpenMesh(const char* Path, Mesh& MeshData);
}

#endif