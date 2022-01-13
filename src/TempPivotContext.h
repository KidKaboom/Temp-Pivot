#ifndef TEMPPIVOTCONTEXT_HEADER
#define TEMPPIVOTCONTEXT_HEADER

#include "TempPivotToolCmd.h"

#include <maya/MPxSelectionContext.h>
#include <maya/MPxContext.h>
#include <maya/MSceneMessage.h>
#include <maya/MVector.h>
#include <maya/MString.h>
#include <maya/MToolsInfo.h>
#include <maya/MPlug.h>
#include <maya/MFnDependencyNode.h>

class TempPivotContext : public MPxSelectionContext
{
    public:
        TempPivotContext();

        void toolOnSetup(MEvent& event) override;
        void toolOffCleanup() override;
        
        // Callback issued when selection list changes
        static void updateManipulators(void* data);
        static bool isDependFree(MFnDependencyNode &node);

        MStatus doPress(MEvent& event);
        MStatus doDrag(MEvent& event);
        MStatus doRelease(MEvent& event);

        void setType(MString type) { mType = type; MToolsInfo::setDirtyFlag(*this); }
        void setIsLast(bool value) { mIsLast = value; MToolsInfo::setDirtyFlag(*this); }
        void setAlignType(MString type) { mAlignType = type; MToolsInfo::setDirtyFlag(*this); }
        void setPosition(MPoint position) { mPosition = position; MToolsInfo::setDirtyFlag(*this); }

    private:
        MCallbackId mId = 0;

        MVector mRotation;
        MString mType;
        MPoint mPosition;
        bool mIsLast;
        MString mAlignType;

        TempPivotToolCmd* mCmd;

};

#endif