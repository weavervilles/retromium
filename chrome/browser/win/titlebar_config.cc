// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/win/titlebar_config.h"

<<<<<<< HEAD
#include <Windows.h>
#include "base/command_line.h"
=======
>>>>>>> 4abd918b29516f4a97125e618c490f82492b935b
#include "base/win/windows_version.h"
#include "chrome/browser/themes/theme_service.h"
#include "chrome/browser/themes/theme_service_factory.h"
#include "chrome/browser/ui/views/frame/browser_view.h"
#include "ui/color/win/accent_color_observer.h"
#include "ui/native_theme/native_theme.h"

<<<<<<< HEAD
bool ShouldCustomDrawSystemTitlebar() {
  // Some extra code added here because those with pre-win8 and no DWM will have to fallback on the custom titlebar.
  BOOL result = FALSE;
	
  typedef HRESULT(WINAPI* DwmIsCompositionEnabledFunc)(BOOL* enabled);

  DwmIsCompositionEnabledFunc func_ = nullptr;
	
  HMODULE dwmapi_library_ = LoadLibraryW(L"dwmapi.dll");
  if (dwmapi_library_) {
    func_ = reinterpret_cast<DwmIsCompositionEnabledFunc>(
        GetProcAddress(dwmapi_library_, "DwmIsCompositionEnabled"));
  }
  else
	  return true;
  
  if (func_) {
	  func_(&result);
  }
  else
	  return true;
  
  
  // Cache flag lookup.
  static const bool custom_titlebar_disabled =
      base::CommandLine::InitializedForCurrentProcess() &&
      base::CommandLine::ForCurrentProcess()->HasSwitch(
          switches::kDisableWindows10CustomTitlebar);

  return (!custom_titlebar_disabled &&
         base::win::GetVersion() >= base::win::Version::WIN10) || !result;
=======
// Allows the titlebar to be drawn by the system using the Mica material
// on Windows 11, version 22H2 and above.
BASE_FEATURE(kWindows11MicaTitlebar,
             "Windows11MicaTitlebar",
             base::FEATURE_DISABLED_BY_DEFAULT);

bool ShouldBrowserCustomDrawTitlebar(BrowserView* browser_view) {
  return !ShouldDefaultThemeUseMicaTitlebar() ||
         !ThemeServiceFactory::GetForProfile(browser_view->GetProfile())
              ->UsingSystemTheme() ||
         (!browser_view->browser()->is_type_normal() &&
          !browser_view->browser()->is_type_popup() &&
          !browser_view->browser()->is_type_devtools());
>>>>>>> 4abd918b29516f4a97125e618c490f82492b935b
}

bool ShouldDefaultThemeUseMicaTitlebar() {
  return SystemTitlebarCanUseMicaMaterial() &&
         !ui::AccentColorObserver::Get()->accent_color().has_value() &&
         !ui::NativeTheme::GetInstanceForNativeUi()
              ->UserHasContrastPreference();
}

bool SystemTitlebarCanUseMicaMaterial() {
  return base::win::GetVersion() >= base::win::Version::WIN11_22H2 &&
         base::FeatureList::IsEnabled(kWindows11MicaTitlebar);
}
