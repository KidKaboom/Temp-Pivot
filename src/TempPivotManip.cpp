#include "TempPivotManip.h"

#include <maya/MFnRotateManip.h>
#include <maya/MFnStateManip.h>
#include <maya/MFnFreePointTriadManip.h>
#include <maya/MFnToggleManip.h>
#include <maya/MGlobal.h>
#include <maya/MEulerRotation.h>
#include <maya/MPlug.h>
#include <maya/MVector.h>

MTypeId TempPivotManip::id(0x00139d00);

// This function is a utility that can be used to extract vector values from
// plugs.
//
MVector vectorPlugValue(const MPlug& plug) {
    if (plug.numChildren() == 3)
    {
        double x, y, z;
        MPlug rx = plug.child(0);
        MPlug ry = plug.child(1);
        MPlug rz = plug.child(2);
        rx.getValue(x);
        ry.getValue(y);
        rz.getValue(z);
        MVector result(x, y, z);
        return result;
    }
    else {
        MGlobal::displayError("Expected 3 children for plug " + MString(plug.name()));
        MVector result(0, 0, 0);
        return result;
    }
}

MStatus TempPivotManip::createChildren()
{
    MStatus status = MStatus::kSuccess;
    // Add the rotation manip
    //
    fRotateManip = addRotateManip("RotateManip", "rotation");
    // Add the state manip.  The state manip is used to cycle through the 
    // rotate manipulator modes to demonstrate how they work.
    //
    fStateManip = addStateManip("StateManip", "state");
    // The state manip permits 4 states.  These correspond to:
    // 0 - Rotate manip in objectSpace mode
    // 1 - Rotate manip in worldSpace mode
    // 2 - Rotate manip in gimbal mode
    // 3 - Rotate manip in objectSpace mode with snapping on
    //
    // Note that while the objectSpace and gimbal modes will operator similar 
    // to the built-in Maya rotate manipulator, the worldSpace mode will 
    // produce unusual rotations because the plugin does not convert worldSpace
    // rotations to object space.
    //
    MFnStateManip stateManip(fStateManip);
    stateManip.setMaxStates(4);
    stateManip.setInitialState(0);

    //
    fTranslateManip = addFreePointTriadManip("TranslateManip", "translate");

    // Toggle Manip
    fToggleManip = addToggleManip("ToggleManip", "toggle");

    return status;
}

MStatus TempPivotManip::connectToDependNode(const MObject& node)
{
    MStatus status;
    // Find the rotate and rotatePivot plugs on the node.  These plugs will 
    // be attached either directly or indirectly to the manip values on the
    // rotate manip.
    //
    MFnDependencyNode nodeFn(node);
    MPlug rPlug = nodeFn.findPlug("rotate", true, &status);
    if (!status)
    {
        MGlobal::displayError("Could not find rotate plug on node");
        return status;
    }
    MPlug rcPlug = nodeFn.findPlug("rotatePivot", true, &status);
    if (!status)
    {
        MGlobal::displayError("Could not find rotatePivot plug on node");
        return status;
    }
    // If the translate pivot exists, it will be used to move the state manip
    // to a convenient location.
    //
    MPlug tPlug = nodeFn.findPlug("translate", true, &status);

    // FreePointTriadManip
    //
    MFnFreePointTriadManip freePointTriadManipFn(fTranslateManip);

    if (MStatus::kFailure != status) {
        //freePointTriadManipFn.connectToPointPlug(tPlug);
    }

    // To avoid having the object jump back to the default rotation when the
    // manipulator is first used, extract the existing rotation from the node
    // and set it as the initial rotation on the manipulator.
    //
    MEulerRotation existingRotation(vectorPlugValue(rPlug));
    MVector existingTranslation(vectorPlugValue(tPlug));
    // 
    // The following code configures default settings for the rotate 
    // manipulator.
    //
    MFnRotateManip rotateManip(fRotateManip);
    rotateManip.setInitialRotation(existingRotation);

    // Set Default Rotation Mode
    rotateManip.setRotateMode(MFnRotateManip::kObjectSpace);
    
    
    rotateManip.displayWithNode(node);
    // Add a callback function to be called when the rotation value changes
    //
    rotatePlugIndex = addManipToPlugConversionCallback(rPlug,
        (manipToPlugConversionCallback)
        &TempPivotManip::rotationChangedCallback);
    // Create a direct (1-1) connection to the rotation center plug
    //
    rotateManip.connectToRotationCenterPlug(rcPlug);
    // Place the state manip at a distance of 2.0 units away from the object
    // along the X-axis.
    //
    MFnStateManip stateManip(fStateManip);
    stateManip.setTranslation(existingTranslation + MVector(2, 0, 0),
        MSpace::kTransform);
    // add the rotate XYZ plugs to the In-View Editor
    //
    MPlug rxPlug = rPlug.child(0);
    addPlugToInViewEditor(rxPlug);
    MPlug ryPlug = rPlug.child(1);
    addPlugToInViewEditor(ryPlug);
    MPlug rzPlug = rPlug.child(2);
    addPlugToInViewEditor(rzPlug);
    finishAddingManips();
    MPxManipContainer::connectToDependNode(node);
    return status;
}

MManipData TempPivotManip::rotationChangedCallback(unsigned index) {
    static MEulerRotation cache;
    MObject obj = MObject::kNullObj;
    // If we entered the callback with an invalid index, print an error and
    // return.  Since we registered the callback only for one plug, all 
    // invocations of the callback should be for that plug.
    //
    if (index != rotatePlugIndex)
    {
        MGlobal::displayError("Invalid index in rotation changed callback!");
        // For invalid indices, return vector of 0's
        MFnNumericData numericData;
        obj = numericData.create(MFnNumericData::k3Double);
        numericData.setData(0.0, 0.0, 0.0);
        return obj;
    }
    // Assign function sets to the manipulators
    //
    MFnStateManip stateManip(fStateManip);
    MFnRotateManip rotateManip(fRotateManip);
    // Adjust settings on the rotate manip based on the state of the state 
    // manip.
    //
    int mode = stateManip.state();
    if (mode != 3)
    {
        rotateManip.setRotateMode((MFnRotateManip::RotateMode)stateManip.state());
        rotateManip.setSnapMode(false);
    }
    else {
        // State 3 enables snapping for an object space manip.  In this case,
        // we snap every 15.0 degrees.
        //
        rotateManip.setRotateMode(MFnRotateManip::kObjectSpace);
        rotateManip.setSnapMode(true);
        rotateManip.setSnapIncrement(15.0);
    }
    // The following code creates a data object to be returned in the 
    // MManipData.  In this case, the plug to be computed must be a 3-component
    // vector, so create data as MFnNumericData::k3Double
    //
    MFnNumericData numericData;
    obj = numericData.create(MFnNumericData::k3Double);
    // Retrieve the value for the rotation from the manipulator and return it
    // directly without modification.  If the manipulator should eg. slow down
    // rotation, this method would need to do some math with the value before
    // returning it.
    //
    MEulerRotation manipRotation;
    if (!getConverterManipValue(rotateManip.rotationIndex(), manipRotation))
    {
        MGlobal::displayError("Error retrieving manip value");
        numericData.setData(0.0, 0.0, 0.0);
    }
    else {
        numericData.setData(manipRotation.x, manipRotation.y, manipRotation.z);
    }
    return MManipData(obj);
}

MStatus TempPivotManip::doPress()
{
    return MS::kUnknownParameter;
}

MStatus TempPivotManip::doDrag()
{
    return MS::kUnknownParameter;
}

MStatus TempPivotManip::doRelease()
{
    // Custom release handling
    updateManipLocations();
    // Let Maya do it's work too
    return MS::kUnknownParameter;
}

void TempPivotManip::updateManipLocations()
{
}
