// LightmapUEr
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "LightMapUErStyle.h"

class FLightMapUErCommands : public TCommands<FLightMapUErCommands>
{
public:

	FLightMapUErCommands()
		: TCommands<FLightMapUErCommands>(TEXT("LightMapUEr"), NSLOCTEXT("Contexts", "LightMapUEr", "LightMapUEr Plugin"), NAME_None, FLightMapUErStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};