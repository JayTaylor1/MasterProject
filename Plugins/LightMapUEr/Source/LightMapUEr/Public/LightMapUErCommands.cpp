// LightmapUEr
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.


#include "LightMapUErCommands.h"

#define LOCTEXT_NAMESPACE "FLightMapUErModule"

void FLightMapUErCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "AutoLightmapUEr", "Open AutoLightmapUEr window", EUserInterfaceActionType::Button, FInputGesture());
}

#undef LOCTEXT_NAMESPACE
