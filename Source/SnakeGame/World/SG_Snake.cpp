#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "World/ActorObjectPool.h"
#include "SG_WorldUtils.h"
#include "LoggingConfig.h"


DEFINE_LOG_CATEGORY_STATIC(LogSGSnake, LOG_DEFAULT_VERBOSITY, LOG_COMPILETIME_VERBOSITY);

ASG_Snake::ASG_Snake()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dimensions& InGridDimensions)
{
	Snake = InSnake;
	CellSize = InCellSize;
	GridDimensions = InGridDimensions;

	if(Snake.IsValid() == false || GetWorld() == nullptr)
	{ 
		return; 
	}

	if(!SnakeLinkPool)
	{
		SnakeLinkPool = NewObject<UActorObjectPool>(this);
		SnakeLinkPool->Initialize(GetWorld(), SnakeLinkPoolConfig);
	}

	for(auto SnakeLink : SnakeLinks)
	{
		SnakeLinkPool->ReturnActorToPool(SnakeLink);
	}
	SnakeLinks.Empty();
	
	const auto& links = Snake.Pin()->getLinks();
	for(const auto& link : links)
	{
		const FTransform transform = FTransform(SnakeGame::WorldUtils::GridPositionToVector(link, CellSize, GridDimensions));
		TObjectPtr<ASG_SnakeLink> snakeLink = Cast<ASG_SnakeLink>(SnakeLinkPool->GetActorFromPool());

		if(snakeLink)
		{
			snakeLink->SetActorTransform(transform);
			snakeLink->SetScale(CellSize);
			SnakeLinks.Add(snakeLink);
		}
	}
}

void ASG_Snake::SetColors(const FLinearColor& HeadColor, const FLinearColor& LinkColor)
{
	SnakeLinkColor = LinkColor;

	for(int32 i = 0; i < SnakeLinks.Num(); ++i)
	{
		const bool isHead = i == 0;
		ASG_SnakeLink* SnakeLink = SnakeLinks[i];
		SnakeLink->SetColor(isHead ? HeadColor : LinkColor);
	}
}

void ASG_Snake::Explode()
{
	if(Snake.IsValid())
	{
		for(int32 i = 0; i < SnakeLinks.Num(); ++i)
		{
			SnakeLinks[i]->Explode();
		}
	}
	else
	{
		UE_LOG(LogSGSnake, Warning, TEXT("Snake actor is not valid!"));
	}
}

void ASG_Snake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Snake.IsValid() == false) { return; }

	const auto& modelLinks = Snake.Pin()->getLinks();
	auto* modelLinkPtr = modelLinks.GetHead();

	for(auto actorLink : SnakeLinks)
	{
		actorLink->SetActorLocation(SnakeGame::WorldUtils::GridPositionToVector(modelLinkPtr->GetValue(), CellSize, GridDimensions));
		modelLinkPtr = modelLinkPtr->GetNextNode();
	}

	while(modelLinkPtr) 
	{
		const FTransform transform = FTransform(SnakeGame::WorldUtils::GridPositionToVector(modelLinkPtr->GetValue(), CellSize, GridDimensions));
		ASG_SnakeLink* snakeLink = Cast<ASG_SnakeLink>(SnakeLinkPool->GetActorFromPool());

		if(snakeLink)
		{
			snakeLink->SetActorTransform(transform);
			snakeLink->SetScale(CellSize);
			snakeLink->SetColor(SnakeLinkColor);

			SnakeLinks.Add(snakeLink);
		}
		
		modelLinkPtr = modelLinkPtr->GetNextNode();
	}
}