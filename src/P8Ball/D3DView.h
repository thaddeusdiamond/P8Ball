/**
 * @file
 * @author		Microsoft Corporation
 * @author		Thaddeus Diamond
 * @version		Unknown
 *
 * @section DESCRIPTION
 * This is the standard D3D viewer header that comes with a new Windows 8 immersive application
 */
#pragma once

#include "P8Ball.h"

ref class D3DView
{
public:
    D3DView(Windows::UI::Core::CoreWindow^ window, Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
    ~D3DView();

    void Run();

    void OnWindowSizeChanged(Windows::UI::Core::CoreWindow^ sender, Windows::UI::Core::WindowSizeChangedEventArgs^ args);
    void OnLogicalDpiChanged(Platform::Object^ sender);

private:

    // Basics
    Windows::UI::Core::CoreWindow^ m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^ m_view;

    // Custom DirectX renderer
    P8Ball^ m_renderer;
};