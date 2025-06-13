#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "Engine/DataTable.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void StartPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
	FUint32Point GridSize{10, 10};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"))
	uint32 CellSize{ 10 };

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASG_Grid> GridVisualClass;

	UPROPERTY(EditDefaultsOnly, Category = "Design")
	UDataTable* ColorsTable;

private:
	TUniquePtr<Snake::Game> Game;
	
	UPROPERTY()
	ASG_Grid* GridVisual;

	void UpdateColors();
};
