#include "UI/SG_MainMenuHUD.h"
#include "UI/SG_MainMenuWidget.h"


ASG_MainMenuHUD::ASG_MainMenuHUD()
{
	if(!HasAnyFlags(RF_ClassDefaultObject))
	{
		UE_LOG(LogTemp, Display, TEXT("ASG_MainMenuHUD ctor (INSTANCE)!"));
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("ASG_MainMenuHUD ctor (CDO)"));
	}
}

void ASG_MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("ASG_MainMenuHUD::BeginPlay World = %s"), *GetWorld()->GetName());

	MainMenuWidget = CreateWidget<USG_MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	check(MainMenuWidget);
	MainMenuWidget->AddToViewport();

	MainMenuWidget->StartGameClickEvent.BindUObject(this, &ThisClass::HandleStartGameClick);
}

void ASG_MainMenuHUD::HandleStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}