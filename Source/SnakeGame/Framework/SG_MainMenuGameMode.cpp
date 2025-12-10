#include "Framework/SG_MainMenuGameMode.h"
#include "UI/SG_MainMenuHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

//#pragma optimize("", off)


void ASG_MainMenuGameMode::StartPlay()
{
	Super::StartPlay();

	UWorld* World = GetWorld();
	check(World);
	if(!World) { return; }

	APlayerController* PC = World->GetFirstPlayerController();
	check(PC)
	if(!PC) { return; }

	// Subsctibe to UI events
	MainMenuHUD = Cast<ASG_MainMenuHUD>(PC->GetHUD());
	check(MainMenuHUD)
	if(!MainMenuHUD) { return; }

	MainMenuHUD->StartGameClickEvent.BindUObject(this, &ThisClass::HandleStartGameClick);
	MainMenuHUD->ExitGameClickEvent.BindUObject(this, &ThisClass::HandleExitGameClick);

	// Mouse setup
	PC->bShowMouseCursor = true;
	PC->bEnableClickEvents = true;
	PC->bEnableMouseOverEvents = true;

	// Input mode setup: Game + UI
	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputMode);
}

void ASG_MainMenuGameMode::HandleStartGameClick()
{
	UWorld* World = GetWorld();
	if(!World || GameplayLevel.IsNull())
	{
		return;
	}

	UGameplayStatics::OpenLevelBySoftObjectPtr(World, GameplayLevel);
}

void ASG_MainMenuGameMode::HandleExitGameClick()
{
	UWorld* World = GetWorld();
	if(!World) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(World, 0);
	if(!PC) return;

	UKismetSystemLibrary::QuitGame(
		World,
		PC,
		EQuitPreference::Quit,
		false
	);
}