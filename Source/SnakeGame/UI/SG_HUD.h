#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_HUD.generated.h"

DECLARE_DELEGATE(FOnRestartClicked);
DECLARE_DELEGATE(FOnExitClicked);
DECLARE_DELEGATE_OneParam(FOnInputUpdated, FVector2D);

UENUM(BlueprintType)
enum class EGameState : uint8
{
	StartGame        UMETA(DisplayName = "StartGame"),
	GameInProgress   UMETA(DisplayName = "GameInProgress"),
	GameOver         UMETA(DisplayName = "GameOver"),
	GameCompleted    UMETA(DisplayName = "GameCompleted")
};

class USG_GameplayWidget;
class USG_GameOverWidget;
class USG_StartGameWidget;

namespace SnakeGame
{
	class Game;
}


UCLASS()
class SNAKEGAME_API ASG_HUD : public AHUD
{
	GENERATED_BODY()
	
public:
	FOnRestartClicked OnRestartClicked;
	FOnExitClicked OnExitClicked;
	FOnInputUpdated OnInputUdpated;

	void SetModel(const TSharedPtr<SnakeGame::Game>& Game);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USG_GameplayWidget> GameplayWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USG_GameOverWidget> GameOverWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USG_StartGameWidget> StartGameWidgetClass;
	
	virtual void BeginPlay() override;
	virtual void Tick(float deltaSeconds) override;

private:
	UPROPERTY()
	TObjectPtr<USG_GameplayWidget> GameplayWidget;

	UPROPERTY()
	TObjectPtr<USG_GameOverWidget> GameOverWidget;

	UPROPERTY()
	TObjectPtr<USG_StartGameWidget> StartGameWidget;

	UPROPERTY()
	TMap<EGameState, TObjectPtr<UUserWidget>> GameWidgets;

	UPROPERTY()
	TObjectPtr<UUserWidget> CurrentWidget;
	
	TWeakPtr<SnakeGame::Game> Game;

	FTimerHandle Timer;

	void HandleRestartClick();
	void HandleExitClick();
	void HandleInputUpdate(FVector2D InputVector);

	void SetMatchState(EGameState MatchState);
};