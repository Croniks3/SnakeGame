#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Core/Game.h"
#include "Engine/DataTable.h"
#include "InputActionValue.h"
#include "SG_GameMode.generated.h"

class ASG_Grid;
class ASG_Snake;
class ASG_Food;
class AExponentialHeightFog;
class UInputAction;
class UInputMappingContext;

UCLASS()
class SNAKEGAME_API ASG_GameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ASG_GameMode();
	virtual void StartPlay() override;
	virtual void Tick(float deltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
	FUint32Point GridSize{10, 10};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "10", ClampMax = "100"), Category = "Settings")
	uint32 CellSize{10};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "4", ClampMax = "10"), Category = "Settings")
	uint32 SnakeDefaultSize{5};

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.05", ClampMax = "2.0"), Category = "Settings")
	float GameSpeed{1.0f};

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASG_Grid> GridVisualClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASG_Snake> SnakeVisualClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASG_Food> FoodVisualClass;
	
	UPROPERTY(EditDefaultsOnly, Category = "Design")
	TObjectPtr <UDataTable> ColorsTable;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> MoveForwardInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> MoveRightInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputAction> ResetGameInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "SnakeInput")
	TObjectPtr<UInputMappingContext> SnakeInputMappingContext;

private:
	UPROPERTY()
	ASG_Grid* GridVisual;

	UPROPERTY()
	ASG_Snake* SnakeVisual;

	UPROPERTY()
	ASG_Food* FoodVisual;

	UPROPERTY()
	AExponentialHeightFog* Fog;

	UFUNCTION(Exec, Category = "Console command")
	void NextColor();

private:
	TUniquePtr<SnakeGame::Game> Game;
	uint32 ColorTableIndex{0};
	SnakeGame::SnakeInput Input{SnakeGame::SnakeInput::Default};

	void UpdateColors();
	void FindFog();

	void SetupInput();
	void OnMoveForward(const FInputActionValue& Value);
	void OnMoveRight(const FInputActionValue& Value);
	void OnResetGame(const FInputActionValue& Value);

	SnakeGame::Settings CreateGameSettings() const;
};
