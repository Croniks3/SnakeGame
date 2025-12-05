#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SG_MainMenuGameMode.generated.h"

class ASG_MainMenuHUD;


UCLASS()
class SNAKEGAME_API ASG_MainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "Levels")
	TSoftObjectPtr<UWorld> GameplayLevel;

private:
	UPROPERTY()
	TObjectPtr<ASG_MainMenuHUD> MainMenuHUD;
};