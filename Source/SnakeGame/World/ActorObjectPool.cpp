#include "World/ActorObjectPool.h"
#include "World/SG_WorldTypes.h"
#include "LoggingConfig.h"

DEFINE_LOG_CATEGORY_STATIC(LogActorPool, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);


void UActorObjectPool::Initialize(UWorld* World, const UActorObjectPoolConfig* Config)
{
    check(World);
    if(!World) 
    {
        UE_LOG(LogActorPool, Error, TEXT("(%s::%s): A null pointer to the World was passed!"), 
            *GetNameSafe(this), TEXT(__FUNCTION__));
        return;
    } 
    
    CachedWorld = World;

    if(!Config || !Config->ActorClass)
    {
        UE_LOG(LogActorPool, Error, TEXT("(%s::%s): Invalid configuration!"), 
            *GetNameSafe(this), TEXT(__FUNCTION__));
        return;
    }

    ActorClass = Config->ActorClass;

    if(!Config->bFillFromStart)
    { 
        return; 
    }

    ActorPool.Reserve(Config->PoolCapacity);

    for(int32 i = 0; i < Config->PoolCapacity; ++i)
    {
        AActor* Actor = World->SpawnActor<AActor>(ActorClass, FTransform::Identity);
        if(!Actor) continue;

        Actor->SetActorHiddenInGame(true);
        ActorPool.Push(Actor);
    }
}

TObjectPtr<AActor> UActorObjectPool::GetActorFromPool()
{
    TObjectPtr<AActor> Actor = nullptr;

    while(ActorPool.Num() > 0)
    {
        Actor = ActorPool.Pop();
        if(!IsValid(Actor))
        {
            Actor = nullptr;
            continue;
        }

        break;
    }
    
    if(!IsValid(Actor))
    {
        if(!CachedWorld.IsValid())
        {
            UE_LOG(LogActorPool, Error, TEXT("(%s::%s): The field CachedWorld is a null pointer!")
                , *GetNameSafe(this), TEXT(__FUNCTION__));
            return nullptr;
        }

        if(!ActorClass)
        {
            UE_LOG(LogActorPool, Error, TEXT("(%s::%s): The field ActorClass is empty!")
                , *GetNameSafe(this), TEXT(__FUNCTION__));
            return nullptr;
        }

        Actor = CachedWorld->SpawnActor<AActor>(ActorClass, FTransform::Identity);
    }

    if(IsValid(Actor))
    {
        Actor->SetActorHiddenInGame(false);
        return Actor;
    }

	return nullptr;
}

void UActorObjectPool::ReturnActorToPool(TObjectPtr<AActor> Actor)
{
    if(IsValid(Actor))
    {
        Actor->SetActorHiddenInGame(true);
        ActorPool.Push(Actor);
    }
}