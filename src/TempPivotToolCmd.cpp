#include "TempPivotToolCmd.h"
#include "TempPivotFlags.h"

#include <maya/MDGModifier.h>
#include <maya/MArgDatabase.h>
#include <maya/MString.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>
#include <maya/MDagPath.h>
#include <maya/MFnTransform.h>


MSyntax TempPivotToolCmd::newSyntax()
{
	MSyntax syntax;

	// Add Flags
	syntax.addFlag(kTypeFlag, kTypeFlagLong, MSyntax::kString);
	syntax.addFlag(kPositionFlag, kPositionFlagLong, MSyntax::kDouble, MSyntax::kDouble, MSyntax::kDouble);
	syntax.addFlag(kLastFlag, kLastFlagLong, MSyntax::kBoolean);
	syntax.addFlag(kAlignFlag, kAlignFlagLong, MSyntax::kString);

	// No query or edit
	syntax.enableEdit(false);
	syntax.enableQuery(false);

	return syntax;
}

MStatus TempPivotToolCmd::parseArgs(const MArgList& args)
{
	MStatus status = MS::kSuccess;

	MArgParser argParser(syntax(), args);
	MArgDatabase argData(syntax(), args, &status);

	// Type
	if (argData.isFlagSet(kTypeFlag))
	{
		MString tmp;
		status = argData.getFlagArgument(kTypeFlag, 0, tmp);

		if (!status)
		{
			displayError("Type flag invalid.");
			return status;
		}

		mType = tmp;
	}
	else 
	{
		mType = kTypeCenter;
	}

	// Position
	if (mType == kTypeCustom && argData.isFlagSet(kPositionFlag))
	{
		status = argData.getFlagArgument(kPositionFlag, 0, mPosition.x);

		if (!status)
		{
			displayError("Position X flag invalid.");
			return status;
		}

		status = argData.getFlagArgument(kPositionFlag, 1, mPosition.y);

		if (!status)
		{
			displayError("Position Y flag invalid.");
			return status;
		}

		status = argData.getFlagArgument(kPositionFlag, 2, mPosition.z);

		if (!status)
		{
			displayError("Position Z flag invalid.");
			return status;
		}
	}

	// Last
	if (argData.isFlagSet(kLastFlag))
	{
		bool tmp;
		status = argData.getFlagArgument(kLastFlag, 0, tmp);

		if (!status)
		{
			displayError("Last flag invalid.");
			return status;
		}

		mIsLast = tmp;
	}
	else
	{
		mIsLast = true;
	}

	// Align
	if (argData.isFlagSet(kAlignFlag))
	{
		MString tmp;
		status = argData.getFlagArgument(kAlignFlag, 0, tmp);

		if (!status)
		{
			displayError("Align flag invalid.");
			return status;
		}

		mAlignType = tmp;
	}
	else
	{
		mAlignType = kAlignWorld;
	}

	return status;
}

MStatus TempPivotToolCmd::doIt(const MArgList& args)
{
	MStatus status = MS::kSuccess;

	// Parse Args
	status = parseArgs(args);

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
	command.addArg(commandString());
	command.addArg(MString(kTypeFlag));
	command.addArg(mType);
	command.addArg(MString(kPositionFlag));
	command.addArg(mPosition);
	command.addArg(MString(kLastFlag));
	command.addArg(mIsLast);
	command.addArg(MString(kAlignFlag));
	command.addArg(mAlignType);
	return MPxToolCommand::doFinalize(command);
}


