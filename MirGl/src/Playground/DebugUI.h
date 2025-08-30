#pragma once

namespace Mir {

    struct DebugData;
    class Object;
    class DebugUI {
    private:
        DebugData* data;  // Pointer to DebugData

    public:
        DebugUI(DebugData* debugData = nullptr);  // Constructor to initialize DebugData pointer
        void setData(DebugData* debugData) { data = debugData; } 
        void render();
        void renderCameraInfo();
        void renderScreenToWorld();
        void renderMouseInfo();
        void renderTimingInfo();
        void trashPile();
        void objects();
        void object(Object& object);
        

    };

}