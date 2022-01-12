#include <maya/MPxSelectionContext.h>
#include <maya/MPxContext.h>
#include <maya/MSceneMessage.h>

class TempPivotContext : public MPxSelectionContext
{
    public:
        TempPivotContext();

        void toolOnSetup(MEvent& event) override;
        void toolOffCleanup() override;
        
        // Callback issued when selection list changes
        static void updateManipulators(void* data);

    private:
        MCallbackId id1 = 0;
};

