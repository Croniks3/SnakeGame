#include "UI/SG_MainMenuHUD.h"
#include "UI/SG_MainMenuWidget.h"


void ASG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	
	MainMenuWidget = CreateWidget<USG_MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	check(MainMenuWidget);
	MainMenuWidget->AddToViewport();

	MainMenuWidget->StartGameClickEvent.BindUObject(this, &ThisClass::HandleStartGameClick);
	MainMenuWidget->ExitGameClickEvent.BindUObject(this, &ThisClass::HandleExitGameClick);
}

void ASG_MainMenuHUD::HandleStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}

void ASG_MainMenuHUD::HandleExitGameClick()
{
	ExitGameClickEvent.ExecuteIfBound();
}