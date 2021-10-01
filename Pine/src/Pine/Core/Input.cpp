#include "pinepch.h"
#include "Input.h"

#include "Application.h"

namespace Pine {
    
    bool Pine::Input::IsKeyPressed(Key key)
    {
        return Application::Get()->m_Window->IsKeyPressed(key);
    }

    bool Input::isMouseButtonPressed(MouseButton button)
    {
        return Application::Get()->m_Window->IsMouseButtonPressed(button);
    }

}
