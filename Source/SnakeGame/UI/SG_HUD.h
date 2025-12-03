#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_HUD.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnRestartClicked);
DECLARE_MULTICAST_DELEGATE(FOnExitClicked);

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

	TWeakPtr<SnakeGame::Game> Game;

	FTimerHandle Timer;

	void HandleRestartClick();
	void HandleExitClick();
};
