/**
 * @file
 * @author		Microsoft Corporation
 * @version		Unknown
 *
 * @section DESCRIPTION
 * This is the standard D3D view provider header that comes with a new Windows 8 immersive application
 */

#pragma once

enum class ActivationEntryPoint
{
    Unknown,
    P8Ball
};

ref class ViewProvider : public Windows::ApplicationModel::Infrastructure::IViewProvider
{
public:
    ViewProvider();

    void Initialize(Windows::UI::Core::CoreWindow^ window, Windows::ApplicationModel::Core::CoreApplicationView^ applicationView);
    void Load(Platform::String^ entryPoint);
    void Run();
    void Uninitialize();

private:
    ActivationEntryPoint m_activationEntryPoint;
    Windows::UI::Core::CoreWindow^ m_window;
    Windows::ApplicationModel::Core::CoreApplicationView^ m_applicationView;
};

ref class ViewProviderFactory : Windows::ApplicationModel::Infrastructure::IViewProviderFactory 
{
public:
    ViewProviderFactory() {}
    Windows::ApplicationModel::Infrastructure::IViewProvider^ CreateViewProvider()
    {
        return ref new ViewProvider();
    }
};
