#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ActorObjectPool.generated.h"

class UActorObjectPoolConfig;


UCLASS()
class SNAKEGAME_API UActorObjectPool : public UObject
{
	GENERATED_BODY()
	
public:
	void Initialize(UWorld* World, const UActorObjectPoolConfig* Config);
	TObjectPtr<AActor> GetActorFromPool();
	void ReturnActorToPool(TObjectPtr<AActor> Actor);
	
private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> ActorPool;

	UPROPERTY()
	TSubclassOf<AActor> ActorClass;

	TWeakObjectPtr<UWorld> CachedWorld;
};