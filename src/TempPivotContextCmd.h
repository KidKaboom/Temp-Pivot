#include "TempPivotContext.h"

#include <maya/MPxContextCommand.h>
#include <maya/MPxContext.h>


class TempPivotContextCmd : public MPxContextCommand
{
	public:
		static const MString name() { return "tempPivotCtx"; }
		static void* creator() { return new TempPivotContextCmd; }
		//static MSyntax newSyntax();

		TempPivotContextCmd() {}
		MPxContext* makeObj();

		MStatus appendSyntax();
	
	private:
		TempPivotContext* ctx;
};
