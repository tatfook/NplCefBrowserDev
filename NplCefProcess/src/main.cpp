#include <windows.h>
#include "PluginAPI.h"
#include "include/cef_app.h"
#include "include/cef_sandbox_win.h"

int main(int argc, char* argv[]) {

	// Enable High-DPI support on Windows 7 or newer.
	//CefEnableHighDPISupport();

	void* sandbox_info = NULL;

#if defined(CEF_USE_SANDBOX)
	// Manage the life span of the sandbox information object. This is necessary
	// for sandbox support on Windows. See cef_sandbox_win.h for complete details.
	CefScopedSandboxInfo scoped_sandbox;
	sandbox_info = scoped_sandbox.sandbox_info();
#endif
	// Provide CEF with command-line arguments.
	CefMainArgs main_args;

	// CEF applications have multiple sub-processes (render, plugin, GPU, etc)
	// that share the same executable. This function checks the command-line and,
	// if this is a sub-process, executes the appropriate logic.
	int exit_code = CefExecuteProcess(main_args, NULL, sandbox_info);
	return exit_code;
}
