// Generic popup text input box
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LevelEditor.h"
#include "Editor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Layout/SUniformGridPanel.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SEditableTextBox.h"
#include "Widgets/Input/SButton.h"
#include "Widgets/Input/SCheckBox.h"
#include "Widgets/Input/SComboBox.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"

class STurfInputBox:public SWindow
{
	public:
		SLATE_BEGIN_ARGS(STurfInputBox)	{}
			SLATE_ATTRIBUTE(FText, Input)
			SLATE_ATTRIBUTE(FText, Title)
		SLATE_END_ARGS()

		EAppReturnType::Type ShowModal();
		
		void Construct(const FArguments& InArgs);

		FString GetInputBoxText()
		{
			return inputBoxText;
		}

	protected:
		FReply OnButtonClick(EAppReturnType::Type ButtonID);
	
	private:
		FString inputBoxText;
		EAppReturnType::Type Response;

		void ChangeText(const FText& inLabel, ETextCommit::Type TextType)
		{
			//if (TextType == ETextCommit::OnEnter || TextType == ETextCommit::OnCleared || TextType == ETextCommit::OnUserMovedFocus) // commented this out for the users that don't want to do the whole "hit enter then hit ok"
			inputBoxText = inLabel.ToString();
		}

};