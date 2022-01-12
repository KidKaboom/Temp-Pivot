#include <maya/MPxContextCommand.h>
#include <maya/MPxContext.h>

#include "TempPivotContext.h"

class TempPivotContextCommand : public MPxContextCommand
{
	public:
		static const MString name() { return "tempPivotContext"; }
		static void* creator() { return new TempPivotContextCommand; }
		//static MSyntax newSyntax();

		TempPivotContextCommand() {};
		MPxContext* makeObj() override { return new TempPivotContext(); }

	//	bool isUndoable() const { return true; }
	//	MStatus doIt(const MArgList& args);
	//	MStatus redoIt();
	//	MStatus undoIt();

	//private:
	//	MStatus parseArgs(const MArgList& args);
};
