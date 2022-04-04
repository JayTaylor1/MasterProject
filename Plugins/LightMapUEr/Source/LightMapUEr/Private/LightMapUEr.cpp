// LightmapUEr
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.


#include "LightMapUEr.h"

#include "LightMapUErStyle.h"
#include "LightMapUErCommands.h"

#include "LevelEditor.h"

#include "SLightMapUErWidget.h"

static const FName LightMapUErTabName("AutoLightmapUEr");

#define LOCTEXT_NAMESPACE "FLightMapUErModule"

void FLightMapUErModule::StartupModule()
{
	FLightMapUErStyle::Initialize();
	FLightMapUErStyle::ReloadTextures();

	FLightMapUErCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FLightMapUErCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FLightMapUErModule::PluginButtonClicked),
		FCanExecuteAction());
		
	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	
	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FLightMapUErModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}
	
	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FLightMapUErModule::AddToolbarExtension));
		
		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}
	

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(LightMapUErTabName, FOnSpawnTab::CreateRaw(this, &FLightMapUErModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FLightMapUErTabTitle", "AutoLightmapUEr"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FLightMapUErModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FLightMapUErStyle::Shutdown();

	FLightMapUErCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(LightMapUErTabName);
}

TSharedRef<SDockTab> FLightMapUErModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{

		return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
			.ShouldAutosize(true)
			[
				SNew(SLightMapUErWidget)
			];
}

void FLightMapUErModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(LightMapUErTabName);
}

void FLightMapUErModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FLightMapUErCommands::Get().OpenPluginWindow);
}

void FLightMapUErModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FLightMapUErCommands::Get().OpenPluginWindow);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FLightMapUErModule, LightMapUEr)