#include <maya/MPxToolCommand.h>
#include <maya/MStatus.h>
#include <maya/MVector.h>
#include <maya/MSyntax.h>
#include <maya/MArgList.h>

class TempPivotCmd : public MPxToolCommand
{
public:
    static void* creator() { return new TempPivotCmd; }
    static MSyntax newSyntax();

    TempPivotCmd() { setCommandString("tempPivotCmd"); }
    ~TempPivotCmd() override {}

    MStatus doIt(const MArgList& args) override;
    MStatus redoIt() override;
    MStatus undoIt() override;
    bool isUndoable() const override { return true; }
    MStatus finalize() override;
    void setVector(double x, double y, double z) { delta.x = x; delta.y = y; delta.z = z; }

    MStatus parseArgs(const MArgList &args);

private:
    MVector delta;  // the delta vectors
};