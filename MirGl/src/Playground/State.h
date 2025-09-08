#pragma once

namespace Mir{
    enum class ActiveWindow { 
        MAIN_VIEWPORT,
        SECOND_VIEWPORT 
    };

    enum class AppState{ 
        NONE, 
        DRAG_DROP_STARTED, 
        DRAG_DROP_ENDED,
        MOUSE_LOCKED 
    };
}