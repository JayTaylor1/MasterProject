// Generic popup text input box
// Copyright 2016-2018 Turfster / NT Entertainment
// All Rights Reserved.

#include "STurfInputBox.h"

EAppReturnType::Type STurfInputBox::ShowModal()
{
	GEditor->EditorAddModalWindow(SharedThis(this));
	return Response;
}

void STurfInputBox::Construct(const FArguments& InArgs)
{
	inputBoxText = InArgs._Input.Get().ToString();
	SWindow::Construct(SWindow::FArguments()
	.Title(InArgs._Title)
	.SupportsMinimize(false)
	.SupportsMaximize(false)
	.HasCloseButton(false)
	.ClientSize(FVector2D(400,50))
	[
		SNew(SBorder)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.AutoHeight()
			[
				SNew(SEditableTextBox)
				.Text(InArgs._Input)
				.OnTextCommitted(this, &STurfInputBox::ChangeText)
			]
			+SVerticalBox::Slot()
			.AutoHeight()
			[
				SNew(SUniformGridPanel)
				+SUniformGridPanel::Slot(0,0)
				[
					SNew(SButton)
					.Text(FText::FromString("OK"))
					.OnClicked(this, &STurfInputBox::OnButtonClick, EAppReturnType::Ok)
					.HAlign(HAlign_Center)
				]
				+ SUniformGridPanel::Slot(1, 0)
				[
					SNew(SButton)
					.Text(FText::FromString("Cancel"))
					.OnClicked(this, &STurfInputBox::OnButtonClick, EAppReturnType::Cancel)
					.HAlign(HAlign_Center)
				]
			]
		]
	]);
}

FReply STurfInputBox::OnButtonClick(EAppReturnType::Type ButtonID)
{
	Response = ButtonID;
	RequestDestroyWindow();
	return FReply::Handled();
}