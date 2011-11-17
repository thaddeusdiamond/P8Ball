/**
 * @file
 * @author		Microsoft Corporation
 * @author		Thaddeus Diamond
 * @version		0.1
 *
 * @section DESCRIPTION
 * This is the main implementation file from where our application is launched
 */

#include "pch.h"
#include "ViewProvider.h"

[Platform::MTAThread]
int main(array<Platform::String^>^)
{
    auto viewProvider = ref new ViewProviderFactory();
    Windows::ApplicationModel::Core::CoreApplication::Run(viewProvider);
	return EXIT_FAILURE;
}
