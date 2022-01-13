#ifndef TEMPPIVOTTOOLCMD_HEADER
#define TEMPPIVOTTOOLCMD_HEADER

#include <vector>

#include <maya/MPxToolCommand.h>
#include <maya/MStatus.h>
#include <maya/MVector.h>
#include <maya/MPoint.h>
#include <maya/MSyntax.h>
#include <maya/MArgList.h>
#include <maya/MMatrix.h>
#include <maya/MMatrixArray.h>


class TempPivotToolCmd : public MPxToolCommand
{
public:
    static MString name() { return "tempPivot"; }

    static void* creator() { return new TempPivotToolCmd; }
    static MSyntax newSyntax();

    TempPivotToolCmd() { setCommandString(TempPivotToolCmd::name()); }
    ~TempPivotToolCmd() override {}

    MStatus doIt(const MArgList& args) override;
    MStatus redoIt() override;
    MStatus undoIt() override;
    bool isUndoable() const override { return true; }
    MStatus finalize() override;

private:
    MMatrix parentMatrix;
    MMatrixArray childrenMatrix;
};

#endif