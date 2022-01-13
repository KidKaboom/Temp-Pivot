#include "TempPivotContext.h"
#include "TempPivotManip.h"

#include <maya/MModelMessage.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MObjectArray.h>
#include <maya/MDagPathArray.h>


TempPivotContext::TempPivotContext()
{
    setTitleString("Temp Pivot");
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

bool TempPivotContext::isDependFree(MFnDependencyNode& node)
{

    // Check Translation
    MPlug tPlug = node.findPlug("translate", true);

    if (!tPlug.isChannelBoxFlagSet())
    {
        return false;
    }

    // Check Rotation
    MPlug rPlug = node.findPlug("rotate", true);

    if (!rPlug.isChannelBoxFlagSet())
    {
        return false;
    }

    return true;
}

void TempPivotContext::updateManipulators(void* data)
{
    MStatus status = MStatus::kSuccess;

    // Update Manipulators
    TempPivotContext* ctxPtr = (TempPivotContext*)data;
    ctxPtr->deleteManipulators();

    // Get Active Selected
    MSelectionList list;
    status = MGlobal::getActiveSelectionList(list);
    MItSelectionList iter(list, MFn::kInvalid, &status);

    if (MS::kSuccess == status) 
    {
        MObjectArray objects;

        for (; !iter.isDone(); iter.next()) 
        {
            // Make sure the selection list item is a depend node and has the
            // required plugs before manipulating it.

            MObject dependNode;
            iter.getDependNode(dependNode);

            if (dependNode.isNull() || !dependNode.hasFn(MFn::kDependencyNode))
            {
                MGlobal::displayWarning("Object in selection list is not a depend node.");
                continue;
            }

            MFnDependencyNode dependNodeFn(dependNode);

            //// Check Translation
            //MPlug tPlug = dependNodeFn.findPlug("translate", true, &status);

            //if (!status)
            //{
            //    MGlobal::displayWarning("Object cannot be manipulated: " + dependNodeFn.name());
            //    continue;
            //}

            //// Check Rotation
            //MPlug rPlug = dependNodeFn.findPlug("rotate", true, &status);

            //if (!status) 
            //{
            //    MGlobal::displayWarning("Object cannot be manipulated: " + dependNodeFn.name());
            //    continue;
            //}

            if (!isDependFree(dependNodeFn))
            {
                MGlobal::displayWarning("Object cannot be manipulated: " + dependNodeFn.name());
                continue;
            }

            // Append
            objects.append(dependNode);
        }

        if (objects.length() == 0)
        {
            return;
        }
        else
        {
            // Create manipulator
            MString manipName("tempPivotManip");
            MObject manipObject;

            TempPivotManip* manipulator = (TempPivotManip*)TempPivotManip::newManipulator(manipName, manipObject);

            if (NULL != manipulator)
            {
                // Add the manipulator
                ctxPtr->addManipulator(manipObject);

                for (int x = 0; x < objects.length(); x++)
                {
                    MObject object = objects[x];

                    // Connect the manipulator to the object in the selection list.
                    if (!manipulator->connectToDependNode(object))
                    {
                        MFnDependencyNode dependNodeFn(object);
                        MGlobal::displayWarning("Error connecting manipulator to object: " + dependNodeFn.name());
                    }
                }
            }
        }
        
        
    }
}


