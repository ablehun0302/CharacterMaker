// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleMainWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

void UTitleMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Button_CreateServer)
	{
		Button_CreateServer->OnClicked.AddDynamic(this, &UTitleMainWidget::OnClickedCreateServerBtn);
	}
	if (Button_Join)
	{
		Button_Join->OnClicked.AddDynamic(this, &UTitleMainWidget::OnClickedJoinBtn);
	}
}

void UTitleMainWidget::OnClickedCreateServerBtn()
{
	UGameplayStatics::OpenLevel(GetOwningPlayer(), "InGame01", true, TEXT("listen"));
}

void UTitleMainWidget::OnClickedJoinBtn()
{
	UGameplayStatics::OpenLevel(GetOwningPlayer(), FName(TextBox_IPInput->GetText().ToString()));
}
