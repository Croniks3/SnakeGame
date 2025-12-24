#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorObjectPool.generated.h"


UCLASS()
class SNAKEGAME_API UActorObjectPool : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UWorld* World);
	TObjectPtr<AActor> GetActorFromPool();
	void ReturnActorToPool(TObjectPtr<AActor> Actor);

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AActor> ActorClass;
	
	UPROPERTY(EditDefaultsOnly)
	int32 PoolCapacity{8};

	UPROPERTY(EditDefaultsOnly)
	bool bFillFromStart{true};

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorPool;

	TWeakObjectPtr<UWorld> CachedWorld;
};