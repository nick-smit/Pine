#include "pinepch.h"
#include "Input.h"

#include "Application.h"

namespace Pine {
    
    bool Pine::Input::IsKeyPressed(Key key)
    {
        return Application::Get()->m_Window->IsKeyPressed(key);
    }

    bool Input::IsMouseButtonPressed(MouseButton button)
    {
        return Application::Get()->m_Window->IsMouseButtonPressed(button);
    }

    float Input::GetMouseX()
    {
        return Application::Get()->m_Window->GetMouseX();
    }
    
    float Input::GetMouseY()
    {
        return Application::Get()->m_Window->GetMouseY();
    }

}
