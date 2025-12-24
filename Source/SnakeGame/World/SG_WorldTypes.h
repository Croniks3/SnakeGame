#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Engine/DataAsset.h"
#include "SG_WorldTypes.generated.h"


USTRUCT(BlueprintType)
struct FSnakeColors : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GridBackgroundColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GridWallColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor GridLineColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor SkyAtmosphereColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor SnakeHeadColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor SnakeLinkColor{};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FLinearColor FoodColor{};
};

UCLASS(BlueprintType)
class UActorObjectPoolConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass;

	UPROPERTY(EditDefaultsOnly)
	int32 PoolCapacity = 8;

	UPROPERTY(EditDefaultsOnly)
	bool bFillFromStart = true;
};