// LightmapUEr
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.


// This code is held together with kitten whiskers and toothpicks.
// There's nothing to see or learn here.
// Seriously.
// Turn back.

#pragma once

#include "CoreMinimal.h"
#include "LevelEditor.h"
#include "Editor.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Widgets/Input/SNumericEntryBox.h"
#include "Widgets/Layout/SSeparator.h"
#include "Misc/FileHelper.h"
#include "Misc/ScopedSlowTask.h"
#include "Misc/MessageDialog.h"
#include "HAL/PlatformFilemanager.h"

DECLARE_DELEGATE(DoUpdateDelegate);

struct BoundsInfo
{
public:
	float size;
	int resolution;

	BoundsInfo()
	{
		size = 0;
		resolution = 0;
	}

	BoundsInfo(float s, int r)
	{
		size = s;
		resolution = r;
	}
};

struct BoundsPreset
{
	public:
		FString presetName;
		TArray<TSharedPtr<BoundsInfo>> currentBounds;

	BoundsPreset()
	{
		presetName = "noname";
		currentBounds.Empty();
	}
};

class SLightMapUErWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SLightMapUErWidget) {}

	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	TArray<TSharedPtr<BoundsInfo>> workBounds;

	TArray<BoundsPreset> boundsInfo;
	TSharedPtr<SListView<TSharedPtr<BoundsInfo>>> boundsInfoListWidget;
	TSharedRef<ITableRow> MakeListViewWidget(TSharedPtr<BoundsInfo> Item, const TSharedRef<STableViewBase>& OwnerTable);

	TSharedPtr<SComboBox<TSharedPtr<FString>>> boundsComboBox;
	TArray<TSharedPtr<FString>> boundsNames;
	TSharedPtr<FString> initialSelected;
	FString currentSelected;
	int currentBounds = 0;

	bool trawlAllActors = false;
	bool ignoreActors = false;
	bool drillDown = false;
	bool fullSize = false;
	bool surface = false;

	float scaleFactor = 1000.f;

	void FillBounds();
	void SortBoundsList();

	FReply HandleGoButton();
	FReply HandleClearButton();
	FReply AddLineButton();
	FReply RemoveLineButton();

	FReply RenamePresetButton();
	FReply AddPresetButton();
	FReply RemovePresetButton();


	TOptional<float> GetScaleValue() const
	{
		return scaleFactor;
	}

	void SetScaleValue(float inRate, ETextCommit::Type TextType)
	{
		scaleFactor = inRate;
	}

	FText GetExtentTooltipText() const;
	FText GetExtentHeader() const;

	void LoadConfig()
	{
		boundsInfo.Empty();
		workBounds.Empty();
		FString configName = FPaths::EngineConfigDir() + "AutoLightmapUErPlugin.cfg";
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*(configName)))
		{
			TArray<FString> configLines;
			FString UTF16test;
			FFileHelper::LoadFileToString(UTF16test, *configName);
			UTF16test = UTF16test.Replace(TEXT("\r"), TEXT(""));
			UTF16test.ParseIntoArray(configLines, TEXT("\n"), true);
			UTF16test.Empty();
			for (int i = 0; i < configLines.Num(); i++)
			{
				TArray<FString> brokenUp;
				configLines[i].ParseIntoArray(brokenUp, TEXT(","), true);
				if (brokenUp.Num() == 2)
				{
					if (brokenUp[0] == "lightmapAllActors")
						trawlAllActors = (brokenUp[1] == "1");
					else
						if (brokenUp[0] == "scaleFactor")
							scaleFactor = FCString::Atof(*brokenUp[1]);
						else
							if (brokenUp[0] == "ignoreLightmapActors")
								ignoreActors = (brokenUp[1] == "1");
							else
								if (brokenUp[0] == "drillDown")
									drillDown = (brokenUp[1] == "1");
								else
									if (brokenUp[0] == "fullSize")
										fullSize = (brokenUp[1] == "1");
									else
										if (brokenUp[0] == "surface")
											surface = (brokenUp[1] == "1");
				}
				else
				{
					BoundsPreset newPreset;
					newPreset.presetName = brokenUp[0];
					if (brokenUp.Num() % 2 == 1)
					{
						for (int j = 1; j < brokenUp.Num(); j += 2)
							newPreset.currentBounds.Add(MakeShareable(new BoundsInfo(FCString::Atof(*brokenUp[j]), FCString::Atoi(*brokenUp[j + 1]))));
						boundsInfo.Add(newPreset);
					}
					else
						UE_LOG(LogTemp, Error, TEXT("Error reading config file."));
				}
			}
		}
		else
			FillBounds();
		if (boundsInfo.Num() == 0) // ran into errors reading the file?
			FillBounds();

		boundsNames.Empty();
		currentBounds = 0;
		currentSelected = boundsInfo[currentBounds].presetName;
		for (int i = 0; i < boundsInfo.Num(); i++)
			boundsNames.Add(MakeShareable(new FString(boundsInfo[i].presetName)));
		initialSelected = boundsNames[currentBounds];
	}

	void SaveConfig()
	{
		FString configName = FPaths::EngineConfigDir() + "AutoLightmapUErPlugin.cfg";
		FString SaveText = "";
		SaveText += "lightmapAllActors," + FString::FromInt((int32)trawlAllActors) + "\r\n";
		SaveText += "ignoreLightmapActors," + FString::FromInt((int32)ignoreActors) + "\r\n";
		SaveText += "scaleFactor," + FString::SanitizeFloat(scaleFactor) + "\r\n";
		SaveText += "drillDown," + FString::FromInt((int32)drillDown) + "\r\n";
		SaveText += "fullSize," + FString::FromInt((int32)fullSize) + "\r\n";
		SaveText += "surface," + FString::FromInt((int32)surface) + "\r\n";
		for (int i = 0; i < boundsInfo.Num(); i++)
		{
			SaveText += boundsInfo[i].presetName;
			for (int j = 0; j < boundsInfo[i].currentBounds.Num(); j++)
			{
				SaveText += "," + FString::SanitizeFloat(boundsInfo[i].currentBounds[j]->size);
				SaveText += "," + FString::FromInt(boundsInfo[i].currentBounds[j]->resolution);
			}
			SaveText += "\r\n";
		}
		FFileHelper::SaveStringToFile(SaveText, *configName);
	}


	bool CanRemoveLine() const
	{
		return (boundsInfoListWidget->GetNumItemsSelected() > 0);
	}


	ECheckBoxState GetDoAllActors() const
	{
		return trawlAllActors ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SetDoAllActors(ECheckBoxState NewState)
	{
		trawlAllActors = (NewState == ECheckBoxState::Checked);
		SaveConfig();
	}


	ECheckBoxState GetIgnoreActors() const
	{
		return ignoreActors ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SetIgnoreActors(ECheckBoxState NewState)
	{
		ignoreActors = (NewState == ECheckBoxState::Checked);
		SaveConfig();
	}


	ECheckBoxState GetDrill() const
	{
		return drillDown ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SetDrill(ECheckBoxState NewState)
	{
		drillDown = (NewState == ECheckBoxState::Checked);
		SaveConfig();
	}


	ECheckBoxState GetFullSize() const
	{
		return fullSize ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SetFullSize(ECheckBoxState NewState)
	{
		fullSize = (NewState == ECheckBoxState::Checked);
		SaveConfig();
	}

	ECheckBoxState GetSurface() const
	{
		return surface ? ECheckBoxState::Checked : ECheckBoxState::Unchecked;
	}

	void SetSurface(ECheckBoxState NewState)
	{
		surface = (NewState == ECheckBoxState::Checked);
		if (surface)
		{
			FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("WARNING!\r\nThe default presets (Conservative & Greedy) are *not* designed\r\nfor this setting. You will need to create your own, from scratch.\r\nThis setting also automatically turns on \"all actors\".\r\n\r\nTo help you create your own preset,\r\nthis will write the actor surfaces to your output log."));
			trawlAllActors = true;
		}
		SaveConfig();
	}


	void HandleComboSelectionChanged(TSharedPtr<FString> inSelection, ESelectInfo::Type SelectInfo)
	{
		if (inSelection.IsValid())
		{
			currentSelected = *inSelection;
			currentBounds = boundsNames.IndexOfByKey(inSelection); 
			FillWorkBounds();
			boundsInfoListWidget->RequestListRefresh();
		}
	}

	TSharedRef<SWidget> HandleComboGenerateWidget(TSharedPtr<FString> inItem)
	{
		return SNew(STextBlock)
			.Text(FText::FromString(*inItem));
	}

	FText HandleComboText() const
	{
		return FText::FromString(currentSelected);
	}


	void FillCurrentBounds() // copy data from the temporary array back to the current preset
	{
		if ((currentBounds > -1) && (currentBounds<boundsInfo.Num()))
		{
			boundsInfo[currentBounds].currentBounds.Empty();
			for (int i = 0; i < workBounds.Num(); i++)
				boundsInfo[currentBounds].currentBounds.Add(workBounds[i]);
		}
	}

	void FillWorkBounds()  // copy data from preset to temporary array we're using for the listview
	{
		workBounds.Empty();
		if ((currentBounds>-1)&&(currentBounds<boundsInfo.Num()))
		for (int i = 0; i < boundsInfo[currentBounds].currentBounds.Num(); i++)
			workBounds.Add(boundsInfo[currentBounds].currentBounds[i]);
	}


	float returnLargestValue(float a, float b, float c) // probably need to revisit this after more people have used it? I don't know.
	{
		float largest = FMath::Max(a, FMath::Max(b, c));
		if (largest == a)
		{
			if ((b!=0)&& (a / b) > scaleFactor)
				largest = largest / b;
			else
				if ((c!=0) && (a / c) > scaleFactor)
					largest = largest / c;
		}
		else
			if (largest == b)
			{
				if ((a!=0) && (b / a) > scaleFactor)
					largest = largest / a;
				else
					if ((c!=0) && (b / c) > scaleFactor)
						largest = largest / c;
			}
			else
				if (largest == c)
				{
					if ((a != 0) && (c / a) > scaleFactor)
						largest = largest / a;
					else
						if ((b != 0) && (c / b) > scaleFactor)
							largest = largest / b;
				}
		return largest;
	}

	bool doNotEditSourceMeshes = false;

	void EditStaticMeshMinimum(UStaticMesh* USM, int wantedRes)
	{
		bool modified = false;
		if (doNotEditSourceMeshes) 
			return;

		if (USM)
		{
			TArray<FStaticMeshSourceModel>& sourceModels = USM->GetSourceModels();
			for (int i = 0; i < sourceModels.Num(); i++)
			{
				if (sourceModels[i].BuildSettings.MinLightmapResolution > wantedRes)
				{
					UE_LOG(LogTemp, Warning, TEXT("AutoLightmapUEr: Editing %s from %i to %i for LOD %i"), *USM->GetPathName(), sourceModels[i].BuildSettings.MinLightmapResolution, wantedRes, i);
					
					sourceModels[i].BuildSettings.MinLightmapResolution = wantedRes;
					modified = true;
				}
			}
			if (modified)
			{
				USM->Build();
				USM->MarkPackageDirty();
			}
		}
	}

	void FindBounds(FString nameToSearchFor, int &boundsToUse)
	{
		nameToSearchFor = FPaths::GetBaseFilename(nameToSearchFor); // find child, see if we have a preset with that name
		int indexOf = boundsInfo.IndexOfByPredicate([&](BoundsPreset bp) { return (bp.presetName == nameToSearchFor); });
		if (indexOf > -1)
			boundsToUse = indexOf;
	}
};