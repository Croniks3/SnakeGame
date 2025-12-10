#include "UI/SG_MainMenuWidget.h"
#include "Components/Button.h"


void USG_MainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalStartGameClick);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalExitGameClick);
	}
}

void USG_MainMenuWidget::HandleInternalStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}

void USG_MainMenuWidget::HandleInternalExitGameClick()
{
	ExitGameClickEvent.ExecuteIfBound();
}