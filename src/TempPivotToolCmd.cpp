#include "TempPivotToolCmd.h"
#include "macros.h"

#include <maya/MDGModifier.h>
#include <maya/MArgDatabase.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>



MStatus TempPivotToolCmd::doIt(const MArgList& args)
{
	MStatus status = MS::kSuccess;

	// Parse Args
	//status = parseArgs(args);

	// Do It
	if (status == MS::kSuccess)
	{
		status = redoIt();
	}

	return status;
}

MStatus TempPivotToolCmd::redoIt()
{
	MStatus status = MS::kSuccess;

	// Create a selection list iterator
	//
	MSelectionList slist;
	MGlobal::getActiveSelectionList(slist);
	MItSelectionList iter(slist, MFn::kInvalid, &status);

	if (MS::kSuccess == status) {
		MDagPath    mdagPath;       // Item dag path
		MObject     mComponent;     // Current component
		MSpace::Space spc = MSpace::kWorld;

		// Translate all selected objects
		//
		for (; !iter.isDone(); iter.next())
		{
			// Get path and possibly a component
			//
			iter.getDagPath(mdagPath, mComponent);
			MFnTransform transFn(mdagPath, &status);

			/*if (MS::kSuccess == status) {
				status = transFn.translateBy(vector, spc);
				CHECKRESULT(stat, "Error doing translate on transform");
				continue;
			}*/
		}
	}
	else {
		cerr << "Error creating selection list iterator" << endl;
	}

	return status;
}

MStatus TempPivotToolCmd::undoIt()
{
	return MS::kSuccess;
}

MStatus TempPivotToolCmd::finalize()
{
	MArgList command;
	/*command.addArg(commandString());
	command.addArg(MString(kTypeFlag));
	command.addArg(mType);
	command.addArg(MString(kPositionFlag));
	command.addArg(mPosition);
	command.addArg(MString(kLastFlag));
	command.addArg(mIsLast);
	command.addArg(MString(kAlignFlag));
	command.addArg(mAlignType);*/
	return MPxToolCommand::doFinalize(command);
}


