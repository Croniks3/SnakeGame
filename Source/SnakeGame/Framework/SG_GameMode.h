#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "SG_GameMode.generated.h"


UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
	FInt32Point GridSize{10, 10};

private:
	TUniquePtr<Snake::Game> Game;
};
