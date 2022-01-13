#include "TempPivotContext.h"
#include "TempPivotManip.h"

#include <maya/MModelMessage.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MPlug.h>


TempPivotContext::TempPivotContext()
{
    setTitleString("Temp Pivot");
    MGlobal::displayWarning("Missing tool icon.");
    //setImage();
}

void TempPivotContext::toolOnSetup(MEvent&)
{
    setHelpString("Rotate the object using the rotation handles.");
    updateManipulators(this);

    MStatus status;

    id1 = MModelMessage::addCallback(
        MModelMessage::kActiveListModified,
        updateManipulators,
        this, 
        &status
    );
    
    if (!status) {
        MGlobal::displayError("Model addCallback failed.");
    }
}

void TempPivotContext::toolOffCleanup()
{
    MStatus status;
    status = MModelMessage::removeCallback(id1);

    if (!status) {
        MGlobal::displayError("Model remove callback failed");
    }

    MPxContext::toolOffCleanup();
}

void TempPivotContext::updateManipulators(void* data)
{
    MStatus stat = MStatus::kSuccess;

    TempPivotContext* ctxPtr = (TempPivotContext*)data;
    ctxPtr->deleteManipulators();

    // Add the rotate manipulator to each selected object.  This produces 
    // behavior different from the default rotate manipulator behavior.  Here,
    // a distinct rotate manipulator is attached to every object.
    // 
    MSelectionList list;
    stat = MGlobal::getActiveSelectionList(list);

    MItSelectionList iter(list, MFn::kInvalid, &stat);

    if (MS::kSuccess == stat) {
        for (; !iter.isDone(); iter.next()) {
            // Make sure the selection list item is a depend node and has the
            // required plugs before manipulating it.
            //
            MObject dependNode;
            iter.getDependNode(dependNode);
            if (dependNode.isNull() || !dependNode.hasFn(MFn::kDependencyNode))
            {
                MGlobal::displayWarning("Object in selection list is not "
                    "a depend node.");
                continue;
            }
            MFnDependencyNode dependNodeFn(dependNode);
            /* MPlug rPlug = */ dependNodeFn.findPlug("rotate", true, &stat);
            if (!stat) {
                MGlobal::displayWarning("Object cannot be manipulated: " +
                    dependNodeFn.name());
                continue;
            }

            // Add manipulator to the selected object
            //
            MString manipName("tempPivotManip");
            MObject manipObject;

            TempPivotManip* manipulator =
                (TempPivotManip*)TempPivotManip::newManipulator(
                    manipName,
                    manipObject);

            if (NULL != manipulator) 
            {
                // Add the manipulator
                //
                ctxPtr->addManipulator(manipObject);

                // Connect the manipulator to the object in the selection list.
                //
                if (!manipulator->connectToDependNode(dependNode))
                {
                    MGlobal::displayWarning(
                        "Error connecting manipulator to object: " + dependNodeFn.name()
                    );
                }
            }
        }
    }
}
