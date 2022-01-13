#include <maya/MPxManipContainer.h>
#include <maya/MGlobal.h>
#include <maya/MDagPath.h>
#include <maya/MManipData.h>


class TempPivotManip : public MPxManipContainer
{
public:
    static MTypeId  id;
    static const MString name() { return "tempPivotManip"; }
    static void* creator() { return new TempPivotManip(); }
    static MStatus initialize() { return MPxManipContainer::initialize(); }

    TempPivotManip(){}
    ~TempPivotManip() override {}

    MStatus createChildren() override;
    MStatus connectToDependNode(const MObject& node) override;

    MStatus doPress() override;
    MStatus doDrag() override;
    MStatus doRelease() override;

    void updateManipLocations();
    
    // Callback function
    MManipData rotationChangedCallback(unsigned index);

private:
    MDagPath fRotateManip;
    MDagPath fTranslateManip;
    MDagPath fStateManip;
    MDagPath fToggleManip;
    unsigned rotatePlugIndex;
};
