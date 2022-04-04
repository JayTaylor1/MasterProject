// LightMapUEr
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.


// This code is held together with kitten whiskers and toothpicks.
// There's nothing to see or learn here.
// Seriously.
// Turn back.

#pragma once
#include "SLightMapUErWidget.h"
#include "SlateOptMacros.h"
#include "Modules/ModuleManager.h"

#include "STurfInputBox.h"
#include "Runtime/Launch/Resources/Version.h"
#include "DesktopPlatformModule.h"
#include "AssetRegistryModule.h"

#include "RawMesh.h"
#include "StaticMeshResources.h"
#include "MeshUtilities.h"
#include "Runtime/Engine/Classes/Engine/StaticMeshActor.h"
#include "Runtime/Engine/Classes/Components/InstancedStaticMeshComponent.h"
#include "Runtime/Slate/Public/SlateFwd.h"

#define LOCTEXT_NAMESPACE "SLightMapUErWidget"


void SLightMapUErWidget::Construct(const FArguments& InArgs)
{
	LoadConfig();
	ChildSlot
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		.Padding(3.0f, 1.0f)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		.WidthOverride(300)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SBorder)
			.Padding(FMargin(3.0f, 2.0f))
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(200)
		.HAlign(HAlign_Left)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f)
		.HAlign(HAlign_Center)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::HandleGoButton)
		.Text(LOCTEXT("doButton", "Apply"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(3.f, 0.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("allactorsDescription", "All "))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Center)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SLightMapUErWidget::GetDoAllActors)
		.OnCheckStateChanged(this, &SLightMapUErWidget::SetDoAllActors)
		.ToolTipText(LOCTEXT("allactorsCheckboxTooltip", "If checked, search for *all* actors that contain a StaticMeshComponent, not just AStaticMeshActors."))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(3.f, 0.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("ignoreactorsDescription", "Ignore "))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Center)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SLightMapUErWidget::GetIgnoreActors)
		.OnCheckStateChanged(this, &SLightMapUErWidget::SetIgnoreActors)
		.ToolTipText(LOCTEXT("ignoreactorsCheckboxTooltip", "If checked, ignores actors that already have a custom lightmap resolution set.\r\nIf you've already used ALM, use \"Clear tags\" first before running ALM again, please."))
		]
		]
	+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.Padding(10.0f, 0.0f)
		.AutoWidth()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("scaleTest", "Scale limit: "))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SNumericEntryBox<float>)
			.MinValue(1.0f)
		.AllowSpin(false)
		.MinDesiredValueWidth(50)
		.Value(this, &SLightMapUErWidget::GetScaleValue)
		.OnValueCommitted(this, &SLightMapUErWidget::SetScaleValue)
		.ToolTipText(LOCTEXT("scaleTestTooltip", "If a side is X times larger than one of the other sides, chances are it's a long, *really* thin mesh...\r\nand it doesn't really need the highest lightmap."))
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(FMargin(9.f, 0.f, 5.f, 0.f))
		.HAlign(HAlign_Center)
		[
			SNew(SBox)
			.WidthOverride(70)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::HandleClearButton)
		.Text(LOCTEXT("clearButton", "Clear tags"))
		.ToolTipText(LOCTEXT("clearIgnore", "Housekeeping:\r\nRemoves all AutoLightmapUEr tags from all actors in the level.\r\nUse when you're done tweaking your lightmaps."))
		]
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(75)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.Padding(3.f, 0.f)
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(47)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("deepDescription", "Go deep "))
		]
		]
	+ SHorizontalBox::Slot()
		.VAlign(VAlign_Center)
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(20)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SLightMapUErWidget::GetDrill)
		.OnCheckStateChanged(this, &SLightMapUErWidget::SetDrill)
		.ToolTipText(LOCTEXT("deepCheckboxTooltip", "If checked, will also modify some of the actual static meshes build values.\r\nThis modification should be benign, but some people have asked not to make it default, so..."))
		]
		]
		]
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		[
			SNew(SBox)
			.WidthOverride(86)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(3.f, 0.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("fullDescription", "Full extents"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SLightMapUErWidget::GetFullSize)
		.OnCheckStateChanged(this, &SLightMapUErWidget::SetFullSize)
		.ToolTipText(LOCTEXT("fullCheckboxTooltip", "Use full size instead of half size while calculating extents"))
		]
	
		]
		]
	+SHorizontalBox::Slot()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		.Padding(3.f, 0.f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("Surface", "Surface"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.VAlign(VAlign_Center)
		[
			SNew(SCheckBox)
			.IsChecked(this, &SLightMapUErWidget::GetSurface)
		.OnCheckStateChanged(this, &SLightMapUErWidget::SetSurface)
		.ToolTipText(LOCTEXT("surfaceCheckboxTooltip", "Use surface area instead of largest volume extent calculation.\r\nWarning: This is a *lot* slower, and none of the presets work with it."))
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.Padding(10.f, 0.0f)
		.AutoWidth()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("preset", "Preset: "))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Fill)
		[
			SAssignNew(boundsComboBox, SComboBox<TSharedPtr<FString>>)
			.OptionsSource(&boundsNames)
		.InitiallySelectedItem(initialSelected)
		.OnSelectionChanged(this, &SLightMapUErWidget::HandleComboSelectionChanged)
		.OnGenerateWidget(this, &SLightMapUErWidget::HandleComboGenerateWidget)
		[
			SNew(STextBlock)
			.Text(this, &SLightMapUErWidget::HandleComboText)
		]
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.Padding(10.f, 0.f)
		.HAlign(HAlign_Right)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::RenamePresetButton)
		.Text(LOCTEXT("renamePresetButton", "Rename"))
		.ToolTipText(LOCTEXT("renameBoundsTooltip", "Rename preset"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::AddPresetButton)
		.Text(LOCTEXT("addPresetButton", "Add"))
		.ToolTipText(LOCTEXT("addBoundsTooltip", "Add a new preset"))
		]
	+ SHorizontalBox::Slot()
		.AutoWidth()
		.HAlign(HAlign_Fill)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::RemovePresetButton)
		.Text(LOCTEXT("removePresetButton", "Remove"))
		.ToolTipText(LOCTEXT("removeBoundsTooltip", "Remove current preset"))
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SSeparator)
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.HAlign(HAlign_Left)
		.VAlign(VAlign_Center)
		.Padding(10.f, 0.0f)
		[
			SNew(STextBlock)
			.Text(LOCTEXT("items", "Items: "))
		]
	+ SHorizontalBox::Slot()
		.Padding(10.f, 0.f)
		.HAlign(HAlign_Right)
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.MaxWidth(40)
		.HAlign(HAlign_Left)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::AddLineButton)
		.Text(LOCTEXT("addLineButton", "+"))
		.ToolTipText(LOCTEXT("addLineTooltip", "Add a new item to the list, defaulting to twice the highest values."))
		]
	+ SHorizontalBox::Slot()
		.HAlign(HAlign_Right)
		.MaxWidth(40)
		[
			SNew(SButton)
			.OnClicked(this, &SLightMapUErWidget::RemoveLineButton)
		.IsEnabled(this, &SLightMapUErWidget::CanRemoveLine)
		.Text(LOCTEXT("removeLineButton", "-"))
		.ToolTipText(LOCTEXT("removeLineTooltip", "Remove currenty selected item(s) from the list."))
		]
		]
		]
		]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		.MaxHeight(330)
		[
			SNew(SBox)
			[
				SAssignNew(boundsInfoListWidget, SListView<TSharedPtr<BoundsInfo>>)
				.ItemHeight(30)
		.ListItemsSource(&workBounds) // this only happens *once* on generation... hence the workaround with the temporary array
		.SelectionMode(ESelectionMode::Multi)
		.OnGenerateRow(this, &SLightMapUErWidget::MakeListViewWidget)
		.HeaderRow(
			SNew(SHeaderRow)
			+ SHeaderRow::Column("Extent").DefaultLabel(this, &SLightMapUErWidget::GetExtentHeader).DefaultTooltip(this, &SLightMapUErWidget::GetExtentTooltipText).FillWidth(5).HAlignCell(HAlign_Center).HAlignHeader(HAlign_Center).FixedWidth(150)
			+ SHeaderRow::Column("resolution").DefaultLabel(LOCTEXT("header_resolution", "Lightmap res to use")).FillWidth(5).HAlignCell(HAlign_Center).HAlignHeader(HAlign_Center).FixedWidth(150)
			)
			]
		]
	+ SVerticalBox::Slot()
		.AutoHeight()
		[
			SNew(SHorizontalBox)
			+ SHorizontalBox::Slot()
		.Padding(5.f, 1.f)
		.HAlign(HAlign_Right)
		[
			SNew(SBox)
			.HeightOverride(28)
		[
			SNew(STextBlock)
			.Justification(ETextJustify::Right)
		.Text(LOCTEXT("version_copyright", "AutoLightmapUEr v1.3b 20180804-1432\r\n(c) 2016-2018 Turfster/NT Entertainment"))
		.ColorAndOpacity(FSlateColor(FLinearColor::Gray))
		]
		]
		]
		];
	FillWorkBounds();
}

TSharedRef<ITableRow> SLightMapUErWidget::MakeListViewWidget(TSharedPtr<BoundsInfo> Item, const TSharedRef<STableViewBase>& OwnerTable)
{
	DoUpdateDelegate testDelegate;
	testDelegate.BindSP(this, &SLightMapUErWidget::SortBoundsList);

	class SLightMapComboWidget : public SMultiColumnTableRow<TSharedPtr<BoundsInfo>>
	{
	public:
		SLATE_BEGIN_ARGS(SLightMapComboWidget) {}
		SLATE_END_ARGS()

			DoUpdateDelegate myDelegate;
		TSharedPtr<BoundsInfo> rowItem;

		void Construct(const FArguments& InArgs, const TSharedRef<STableViewBase>& InOwnerTable, TSharedPtr<BoundsInfo> InItem, DoUpdateDelegate inDelegate)
		{
			rowItem = InItem;
			myDelegate = inDelegate;
			SMultiColumnTableRow<TSharedPtr<BoundsInfo>>::Construct(FSuperRowType::FArguments(), InOwnerTable);
		}

		TOptional<float> GetSize() const
		{
			return rowItem->size;
		}

		void SetSize(float inRate, ETextCommit::Type TextTypee) const
		{
			rowItem->size = inRate;
			myDelegate.Execute();
		}

		TOptional<int32> GetRes() const
		{
			return rowItem->resolution;
		}

		void SetRes(int32 inRate, ETextCommit::Type TextType) const
		{
			if (TextType == ETextCommit::OnEnter || TextType == ETextCommit::OnCleared || TextType == ETextCommit::OnUserMovedFocus)
			{
				rowItem->resolution = inRate;
				myDelegate.Execute();
			}
		}

		TSharedRef<SWidget> GenerateWidgetForColumn(const FName& ColumnName)
		{
			if (ColumnName == "Extent")
			{
				return SNew(SNumericEntryBox<float>)
					.MinValue(1.0f)
					.AllowSpin(false)
					.MinDesiredValueWidth(100)
					.Value(this, &SLightMapComboWidget::GetSize)
					.OnValueCommitted(this, &SLightMapComboWidget::SetSize);
			}
			if (ColumnName == "resolution")
			{
				return
					SNew(SNumericEntryBox<int>)
					.MinValue(1.0f)
					.AllowSpin(false)
					.OnValueCommitted(this, &SLightMapComboWidget::SetRes)
					.MinDesiredValueWidth(100)
					.LabelVAlign(EVerticalAlignment::VAlign_Center)
					.Value(this, &SLightMapComboWidget::GetRes);

			}
			return SNew(STextBlock)
				.Text(FText::FromString("What"));

		}
	};

	return SNew(SLightMapComboWidget, OwnerTable, Item, testDelegate);
}

FText SLightMapUErWidget::GetExtentTooltipText() const
{
	if (surface)
		return LOCTEXT("surfaceExtents", "Total surface area of the mesh, as calculated in unreal units.");
	if (fullSize)
		return LOCTEXT("fullExtents", "Extent value is the size of the longest side.\r\nIf your longest side is 100 unreal units, the value checked will be 100.");
	else
		return LOCTEXT("halfExtents", "Extent value is *half* the size of the longest side.\r\nIf your longest side is 100 unreal units, the value checked will be 50.");
}

FText SLightMapUErWidget::GetExtentHeader() const
{
	if (surface)
		return LOCTEXT("header_surface", "Total surface area");
	if (fullSize)
		return LOCTEXT("header_sizeFull", "Largest extent (full size)");
	else
		return LOCTEXT("header_size", "Largest extent (half-size)");
}

FReply SLightMapUErWidget::AddLineButton()
{
	if (boundsInfo.Num() > 0 && currentBounds > -1 && currentBounds < boundsInfo.Num())
	{
		if (boundsInfo[currentBounds].currentBounds.Num() > 0)
		{
			boundsInfo[currentBounds].currentBounds.Add(
				MakeShareable(new BoundsInfo(boundsInfo[currentBounds].currentBounds[boundsInfo[currentBounds].currentBounds.Num() - 1]->size * 2,
					boundsInfo[currentBounds].currentBounds[boundsInfo[currentBounds].currentBounds.Num() - 1]->resolution * 2)));
		}
		else
			boundsInfo[currentBounds].currentBounds.Add(MakeShareable(new BoundsInfo(4.f, 4)));
	}

	FillWorkBounds();
	boundsInfoListWidget->RequestListRefresh();
	SaveConfig();
	return FReply::Handled();
}

FReply SLightMapUErWidget::RemoveLineButton()
{
	if (boundsInfo.Num() > 0 && currentBounds > -1 && currentBounds < boundsInfo.Num() && boundsInfo[currentBounds].currentBounds.Num()>0)
		if (boundsInfoListWidget->GetNumItemsSelected() > 0)
		{
			TArray<TSharedPtr<BoundsInfo>> selections = boundsInfoListWidget->GetSelectedItems();
			for (int i = 0; i < selections.Num(); i++)
				boundsInfo[currentBounds].currentBounds.Remove(selections[i]);
		}
	FillWorkBounds();
	boundsInfoListWidget->RequestListRefresh();
	SaveConfig();
	return FReply::Handled();
}

FReply SLightMapUErWidget::AddPresetButton()
{
	BoundsPreset newPreset;
	FString newName = "new preset";

	TSharedPtr<STurfInputBox> inputBox = SNew(STurfInputBox)
		.Title(FText::FromString("Enter new preset name"))
		.Input(FText::FromString(newName));

	if (EAppReturnType::Ok == inputBox->ShowModal())
		newName = inputBox->GetInputBoxText();
	newPreset.presetName = newName;
	newPreset.currentBounds.Add(MakeShareable(new BoundsInfo(4.f, 4)));
	boundsInfo.Add(newPreset);
	boundsNames.Add(MakeShareable(new FString(newPreset.presetName)));
	boundsComboBox->RefreshOptions();
	currentSelected = *newName;
	currentBounds = boundsInfo.Num() - 1;
	FillWorkBounds();
	boundsComboBox->SetSelectedItem(boundsNames[currentBounds]);
	boundsInfoListWidget->RequestListRefresh();
	SaveConfig();
	return FReply::Handled();
}

FReply SLightMapUErWidget::RemovePresetButton()
{
	if (FMessageDialog::Open(EAppMsgType::YesNo, FText::FromString("Are you sure you want to delete the current preset?")) == EAppReturnType::No)
		return FReply::Handled();
	if ((currentBounds > -1) && (currentBounds < boundsNames.Num()))
		boundsNames.RemoveAt(currentBounds);
	if ((currentBounds > -1) && (currentBounds < boundsInfo.Num())) // this should match, but...
		boundsInfo.RemoveAt(currentBounds);
	if (boundsInfo.Num() == 0) // always make sure there's at least ONE preset
	{
		UE_LOG(LogTemp, Warning, TEXT("No more presets. Recreating default presets."));
		FillBounds();
	}
	currentBounds = 0;
	boundsComboBox->RefreshOptions();
	initialSelected = boundsNames[currentBounds];
	currentSelected = *(initialSelected);
	FillWorkBounds();
	boundsComboBox->SetSelectedItem(initialSelected);
	boundsInfoListWidget->RequestListRefresh();
	SaveConfig();
	return FReply::Handled();
}

FReply SLightMapUErWidget::RenamePresetButton()
{
	TSharedPtr<STurfInputBox> inputBox = SNew(STurfInputBox)
		.Title(FText::FromString("Enter new name for current preset"))
		.Input(FText::FromString(currentSelected));

	if (EAppReturnType::Ok == inputBox->ShowModal())
	{
		FString newName = inputBox->GetInputBoxText();
		if (newName != "")
		{
			boundsInfo[currentBounds].presetName = newName;
			boundsNames[currentBounds] = MakeShareable(new FString(newName));
			boundsComboBox->RefreshOptions();
			initialSelected = boundsNames[currentBounds];
			currentSelected = newName;
			SaveConfig();
		}
	}

	return FReply::Handled();
}

FReply SLightMapUErWidget::HandleClearButton()
{
	for (TObjectIterator<AActor> Itr; Itr; ++Itr) // clean up our stuff so it doesn't stick around during production
	{
		if (Itr->Tags.Contains("ignoreALM"))
			Itr->Tags.Remove("ignoreALM");
		if (Itr->Tags.Contains("adjustedALM"))
			Itr->Tags.Remove("adjustedALM");
	}
	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Done."));
	return FReply::Handled();
}

FReply SLightMapUErWidget::HandleGoButton()
{
	SaveConfig();
	UStaticMesh* surfaceMesh = NULL;
	FVector scales = FVector(1.f, 1.f, 1.f);

	if (trawlAllActors)
	{

		TArray<FVector> triangle;
		triangle.Add(FVector::ZeroVector);
		triangle.Add(FVector::ZeroVector);
		triangle.Add(FVector::ZeroVector);

		TArray<UActorComponent*> testUSMs;

		for (TObjectIterator<AActor> Itr; Itr; ++Itr)
		{
			if (ignoreActors && Itr->Tags.Contains("ignoreALM"))
				continue;

			testUSMs.Empty();
			TSet<UActorComponent*> setUSMs = Itr->GetComponents(); // rewritten this entire thing for 1.3, since apparently "import an FBX as a lot of staticmeshcomponents in a single blueprint/actor" is a thing?
			for (UActorComponent* hopeful : setUSMs)
			{
				FString testClass = hopeful->GetClass()->GetName();  // what's quicker? A string compare or a full uclass compare?
				if ((testClass == "StaticMeshComponent") ||
					(testClass == "InstancedStaticMeshComponent") ||
					(testClass == "HierarchicalInstancedStaticMeshComponent") || // Eh, getting the ::StaticClass() to work for these is too tricky, string it is!
					(testClass == "SplineMeshComponent"))
					testUSMs.Add(hopeful);
			}

			if (testUSMs.Num()>0)
			{
				for (int USMCounter = 0; USMCounter < testUSMs.Num(); USMCounter++)
				{
					UStaticMeshComponent* testUSM = (UStaticMeshComponent*)testUSMs[USMCounter];
					if (!testUSM)
					{
						UE_LOG(LogTemp, Error, TEXT("Component could not be used as a Static Mesh Component for %s"), *Itr->GetName());
						continue;
					}

					if (testUSM->bOverrideLightMapRes && ignoreActors && !Itr->Tags.Contains("adjustedALM"))
					{
						Itr->Tags.AddUnique("ignoreALM");
						continue;
					}
					// pretty sure we don't need this any more, after the changeup higher up for the FBX BP importing thing?
				/*	FString BPtest = Itr->GetName();
					if (BPtest.Contains("_C_"))
					{
						if (BPtest.EndsWith("_C_0"))
						{
							UE_LOG(LogTemp, Warning, TEXT("Allowing hopefully root blueprint component %s"), *BPtest);
						}
						else
						{
							UE_LOG(LogTemp, Warning, TEXT("Skipping probable blueprint component %s"), *BPtest);
							continue;
						}
					}*/

					FVector origin = FVector::ZeroVector;
					FVector boxextents = FVector::ZeroVector;

				
#if ENGINE_MINOR_VERSION <14
					surfaceMesh = testUSM->StaticMesh;
#else
					surfaceMesh = testUSM->GetStaticMesh();
#endif

					scales = Itr->GetActorScale3D();

					if (surfaceMesh)		// rewrote this too for 1.3. no longer just grabbing the actor bounds
					{
						boxextents = surfaceMesh->GetBounds().BoxExtent;
						boxextents *= testUSM->GetRelativeScale3D();
					}
					else
						Itr->GetActorBounds(false, origin, boxextents);  // this should probably never happen? But hey, gotta have something

					UInstancedStaticMeshComponent* testISMC = (UInstancedStaticMeshComponent*)Itr->GetComponentByClass(UInstancedStaticMeshComponent::StaticClass());
					if (testISMC)  // (Hierarchical) Instanced Static Meshes return the size of *all* the instances *together* if you just get the basic static mesh component cast bounds
					{
						UE_LOG(LogTemp, Warning, TEXT("Instanced Mesh. Only supporting *one* of these per actor."));
#if ENGINE_MINOR_VERSION <14
						boxextents = testISMC->StaticMesh->GetBounds().BoxExtent;
#else
						boxextents = testISMC->GetStaticMesh()->GetBounds().BoxExtent;
#endif
						scales = FVector(1.f, 1.f, 1.f);
						if (surface)
							UE_LOG(LogTemp, Warning, TEXT("Instanced Mesh scales are returned as 'size of the entire volume of all the instances', so reset to 1,1,1"));
					}

					float testVolume = returnLargestValue(boxextents.X, boxextents.Y, boxextents.Z);
				//	UE_LOG(LogTemp, Log, TEXT("Value %f for %s"), testVolume, *testUSMs[USMCounter]->GetName());
					if (testVolume == 0)
						continue;

					if (fullSize)
						testVolume *= 2;

					if (surface)
					{
						if (surfaceMesh != NULL)
						{
							TArray<FStaticMeshSourceModel>& sourceModels = surfaceMesh->GetSourceModels();
							if (sourceModels.Num() > 0)
							{
								FRawMesh rawMesh;
								sourceModels[0].RawMeshBulkData->LoadRawMesh(rawMesh);
								if (rawMesh.IsValid())
								{
									int faceCount = rawMesh.WedgeIndices.Num() / 3;
									testVolume = 0;
									for (int faceIdx = 0; faceIdx < faceCount; faceIdx++)
									{
										triangle[0] = rawMesh.GetWedgePosition(faceIdx * 3) * scales;
										triangle[1] = rawMesh.GetWedgePosition(faceIdx * 3 + 1) * scales;
										triangle[2] = rawMesh.GetWedgePosition(faceIdx * 3 + 2) * scales;

										testVolume += (FVector::CrossProduct(triangle[1] - triangle[0], triangle[2] - triangle[0])).Size() / 2;
									}
									UE_LOG(LogTemp, Log, TEXT("AutoLightMapUEr: Actor %s area calculated to: %f"), *Itr->GetName(), testVolume);
								}
								else
									UE_LOG(LogTemp, Warning, TEXT("Actor %s StaticMesh source data error?!"), *Itr->GetName());
							}
							else
								UE_LOG(LogTemp, Warning, TEXT("Actor %s StaticMesh has no models?!"), *Itr->GetName());
						}
						else
							UE_LOG(LogTemp, Warning, TEXT("Actor %s StaticMesh is empty?!"), *Itr->GetName());
					}

					int boundsToUse = currentBounds;
					FString currentPath = Itr->GetFolderPath().ToString();
					FindBounds(currentPath, boundsToUse);

					float lightmapToUse = 64;
					for (int i = 0; i < boundsInfo[boundsToUse].currentBounds.Num(); i++)
					{
						if (boundsInfo[boundsToUse].currentBounds[i]->size >= testVolume)
						{
							float startOffset = 0;
							float endOffset = boundsInfo[boundsToUse].currentBounds[i]->size - testVolume;
							if (i > 0)
								startOffset = testVolume - boundsInfo[boundsToUse].currentBounds[i - 1]->size;
							if (endOffset < startOffset) // We're closer to the other side
								lightmapToUse = boundsInfo[boundsToUse].currentBounds[i]->resolution;
							break;
						}
						lightmapToUse = boundsInfo[boundsToUse].currentBounds[i]->resolution;
					}

					testUSM->OverriddenLightMapRes = lightmapToUse;
					testUSM->bOverrideLightMapRes = true;
					testUSM->InvalidateLightingCache();
					if (drillDown)
#if ENGINE_MINOR_VERSION <14
						EditStaticMeshMinimum(testUSM->StaticMesh, lightmapToUse);
#else
						EditStaticMeshMinimum(testUSM->GetStaticMesh(), lightmapToUse);
#endif
				}
				Itr->Tags.AddUnique("adjustedALM");
			}

		}
	}
	else
		for (TObjectIterator<AStaticMeshActor> Itr; Itr; ++Itr)
		{
			if (ignoreActors && Itr->Tags.Contains("ignoreALM"))
				continue;
			FVector origin = FVector::ZeroVector;
			FVector boxextents = FVector::ZeroVector;
			Itr->GetActorBounds(false, origin, boxextents);  // note that we're getting axis aligned bounding boxes here for speed. Shouldn't matter too much

			float testVolume = returnLargestValue(boxextents.X, boxextents.Y, boxextents.Z);
			if (fullSize)
				testVolume *= 2;

			int boundsToUse = currentBounds;
			FString currentPath = Itr->GetFolderPath().ToString();
			FindBounds(currentPath, boundsToUse);


			float lightmapToUse = 64;
			for (int i = 0; i < boundsInfo[boundsToUse].currentBounds.Num(); i++)
			{
				if (boundsInfo[boundsToUse].currentBounds[i]->size >= testVolume)
				{
					float startOffset = 0;
					float endOffset = boundsInfo[boundsToUse].currentBounds[i]->size - testVolume;
					if (i > 0)
						startOffset = testVolume - boundsInfo[boundsToUse].currentBounds[i - 1]->size;
					if (endOffset < startOffset) // We're closer to the other side
						lightmapToUse = boundsInfo[boundsToUse].currentBounds[i]->resolution;
					break;
				}
				lightmapToUse = boundsInfo[boundsToUse].currentBounds[i]->resolution;
			}
			if (Itr->GetStaticMeshComponent()) // this should always be the case, but better check
			{
				if (Itr->GetStaticMeshComponent()->bOverrideLightMapRes && ignoreActors && !Itr->Tags.Contains("adjustedALM"))
				{
					Itr->Tags.AddUnique("ignoreALM");
				}
				else
				{
					Itr->GetStaticMeshComponent()->OverriddenLightMapRes = lightmapToUse;
					Itr->GetStaticMeshComponent()->bOverrideLightMapRes = true;
					Itr->GetStaticMeshComponent()->InvalidateLightingCache();
					Itr->Tags.AddUnique("adjustedALM");
					if (drillDown)
#if ENGINE_MINOR_VERSION < 14
						EditStaticMeshMinimum(Itr->GetStaticMeshComponent()->StaticMesh, lightmapToUse);
#else
						EditStaticMeshMinimum(Itr->GetStaticMeshComponent()->GetStaticMesh(), lightmapToUse);
#endif

				}
			}
		}
	
	FViewport* currentViewport = GEditor->GetActiveViewport();
	currentViewport->Invalidate();

	FMessageDialog::Open(EAppMsgType::Ok, FText::FromString("Done. Don't forget to rebuild lighting."));
	return FReply::Handled();
}

void SLightMapUErWidget::SortBoundsList()
{
	workBounds.Sort([&](TSharedPtr<BoundsInfo> a, TSharedPtr<BoundsInfo> b) {if (a->size == b->size) return(a->resolution < b->resolution); else return (a->size < b->size); });
	FillCurrentBounds();
	SaveConfig();
	boundsInfoListWidget->RequestListRefresh();
}

void SLightMapUErWidget::FillBounds()
{
	boundsNames.Empty();
	BoundsPreset consPreset;
	consPreset.presetName = "Conservative";
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(4.f, 4)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(8.f, 4)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(16.f, 8)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(32.f, 8)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(64.f, 8)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(128.f, 16)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(256.f, 32)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(512.f, 32)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(1024.f, 64)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(2048.f, 128)));
	consPreset.currentBounds.Add(MakeShareable(new BoundsInfo(4096.f, 256)));
	boundsInfo.Add(consPreset);
	boundsNames.Add(MakeShareable(new FString(consPreset.presetName)));
	BoundsPreset defaultPreset;
	defaultPreset.presetName = "Greedy";
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(4.f, 4)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(16.f, 8)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(64.f, 8)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(128.f, 16)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(256.f, 32)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(512.f, 64)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(1024.f, 256)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(2048.f, 512)));
	defaultPreset.currentBounds.Add(MakeShareable(new BoundsInfo(4096.f, 1024)));
	boundsInfo.Add(defaultPreset);
	boundsNames.Add(MakeShareable(new FString(defaultPreset.presetName)));

}


#undef LOCTEXT_NAMESPACE
