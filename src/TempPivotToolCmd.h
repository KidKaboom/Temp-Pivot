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
    //void setVector(double x, double y, double z) { delta.x = x; delta.y = y; delta.z = z; }

    void setPosition(MPoint position) { mPosition = position; }

private:
    MStatus parseArgs(const MArgList& args);
    MVector mRotation;
    MString mType;
    MPoint mPosition;
    bool mIsLast;
    MString mAlignType;

    MMatrix parentMatrix;
    MMatrixArray childrenMatrix;
};