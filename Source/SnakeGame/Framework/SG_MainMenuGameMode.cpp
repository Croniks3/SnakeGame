#include "Framework/SG_MainMenuGameMode.h"
#include "UI/SG_MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"

//#pragma optimize("", off)


void ASG_MainMenuGameMode::StartPlay()
{
	UWorld* World = GetWorld();
	check(World);
	if(!World) { return; }

	APlayerController* PC = World->GetFirstPlayerController();
	check(PC)
	if(!PC) { return; }

	MainMenuHUD = Cast<ASG_MainMenuHUD>(PC->GetHUD());
	check(MainMenuHUD)
	if(!MainMenuHUD) { return; }

	MainMenuHUD->StartGameClickEvent.BindUObject(this, &ThisClass::HandleStartGameClick);
}

void ASG_MainMenuGameMode::HandleStartGameClick()
{
	UWorld* World = GetWorld();
	if(!World || !GameplayLevel)
	{
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(World, GameplayLevel);
}