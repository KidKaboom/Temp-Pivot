#include <maya/MPxManipContainer.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MManipData.h>


class TempPivotManip : public MPxManipContainer
{
public:
    static const MTypeId id() { return 0x00139d00; };
    static const MString name() { return "tempPivotManip"; }
    static void* creator() { return new TempPivotManip(); }
    static MStatus initialize() { return MPxManipContainer::initialize(); }

    TempPivotManip(){}
    ~TempPivotManip() override {}

    MStatus createChildren() override;
    MStatus connectToDependNode(const MObject& node) override;
    
    // Callback function
    MManipData rotationChangedCallback(unsigned index);

private:
    MDagPath fRotateManip;
    MDagPath fStateManip;
    unsigned rotatePlugIndex;
};
