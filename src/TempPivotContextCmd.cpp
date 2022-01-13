#include "TempPivotContextCmd.h"

MPxContext* TempPivotContextCmd::makeObj()
{ 
	return new TempPivotContext(); 
}

MStatus TempPivotContextCmd::appendSyntax()
{
	MSyntax mySyntax = syntax();

	return MS::kSuccess;
}
