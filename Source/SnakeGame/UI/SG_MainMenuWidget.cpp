#include "UI/SG_MainMenuWidget.h"
#include "Components/Button.h"


void USG_MainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalStartGameClick);
	}
}

void USG_MainMenuWidget::HandleInternalStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}