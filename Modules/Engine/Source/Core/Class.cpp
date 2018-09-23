////////////////////////////////////////
//
//  @project    : Arch1eN Engine
//  @author     : Artur Ostrowski
//  @usage      : Base type for all classes. Required for delegates and modules.
//  @version    : 1.0.0
//
////////////////////////////////////////
//#include "stdafx.h"
#include "Class.h"

Class::Class()
	: ClassID{ staticClassID++ }
{

}

Class::~Class()
{}
