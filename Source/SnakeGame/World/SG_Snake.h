#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Core/Snake.h"
#include "Core/SG_Types.h"
#include "SG_Snake.generated.h"

class ASG_SnakeLink;

UCLASS()
class SNAKEGAME_API ASG_Snake : public AActor
{
	GENERATED_BODY()
	
public:	
	ASG_Snake();

	void SetModel(const TSharedPtr<SnakeGame::Snake>& Snake, uint32 CellSize, const SnakeGame::Dimensions& gridDimensions);
	void SetColors(const FLinearColor& HeadColor, const FLinearColor& LinkColor);
	void Explode();

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASG_SnakeLink> SnakeHeadClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<ASG_SnakeLink> SnakeLinkClass;

public:	
	virtual void Tick(float DeltaTime) override;

private:
	TWeakPtr<SnakeGame::Snake> Snake;
	uint32 CellSize;
	SnakeGame::Dimensions GridDimensions;
	FLinearColor SnakeLinkColor;

	UPROPERTY()
	TArray<ASG_SnakeLink*> SnakeLinks;

private:
	// SnakeLink object pool
	UPROPERTY()
	TArray<TObjectPtr<ASG_SnakeLink>> SnakeLinksPool;

	TObjectPtr<ASG_SnakeLink> GetSnakeLinkFromPool(const TSubclassOf<ASG_SnakeLink>& SnakeLinkClass, const FTransform& Transform);
	void AddSnakeLinkToPool(TObjectPtr<ASG_SnakeLink>& SnakeLink);
};