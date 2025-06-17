#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Snake.h"
#include "Core/SG_Types.h"
#include "SG_Snake.generated.h"

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
	GENERATED_BODY()
	
public:	
	ASG_Snake();

	void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 CellSize, const SnakeGame::Dimensions& gridDimensions);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> SnakeHeadClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<AActor> SnakeLinkClass;

	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TWeakPtr<SnakeGame::Snake> Snake;
	uint32 CellSize;
	SnakeGame::Dimensions GridDimensions;

	UPROPERTY()
	TArray<AActor*> SnakeLinks;

	FVector LinkPositionToVector(const SnakeGame::Position& linkPosition, uint32 CellSize, const SnakeGame::Dimensions& GridDimensions) const;
};