#include "UI/SG_MainMenuHUD.h"
#include "UI/SG_MainMenuWidget.h"


void ASG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<USG_MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	check(MainMenuWidget);
	MainMenuWidget->AddToViewport();

}