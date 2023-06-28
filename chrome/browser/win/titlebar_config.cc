// Copyright 2016 The Chromium Authors
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "chrome/browser/win/titlebar_config.h"

#include <Windows.h>
#include "base/command_line.h"

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
