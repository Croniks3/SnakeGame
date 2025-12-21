#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "SG_WorldUtils.h"
#include "LoggingConfig.h"

#pragma optimize("", off)

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

	for(auto SnakeLink : SnakeLinks)
	{
		AddSnakeLinkToPool(SnakeLink);
	}
	SnakeLinks.Empty();

	if(Snake.IsValid() == false || GetWorld() == nullptr) { return; }

	const auto& links = Snake.Pin()->getLinks();

	ReserveLinksInPool(SnakeLinkClass, LinksPoolCapacity);
	uint32 i = 0;
	for(const auto& link : links)
	{
		const bool isHead = i == 0;
		const FTransform transform = FTransform(SnakeGame::WorldUtils::GridPositionToVector(link, CellSize, GridDimensions));
		ASG_SnakeLink* snakeLink = GetSnakeLinkFromPool(isHead ? SnakeHeadClass : SnakeLinkClass, transform);
		snakeLink->SetScale(CellSize);
		SnakeLinks.Add(snakeLink);
		++i;
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
		ASG_SnakeLink* snakeLink = GetSnakeLinkFromPool(SnakeLinkClass, transform);
		snakeLink->SetScale(CellSize);
		snakeLink->SetColor(SnakeLinkColor);
		SnakeLinks.Add(snakeLink);
		modelLinkPtr = modelLinkPtr->GetNextNode();
	}
}

void ASG_Snake::ReserveLinksInPool(const TSubclassOf<ASG_SnakeLink>& InSnakeLinkClass, int32 Number)
{
	const int32 LinksNumber = SnakeLinksPool.Num();
	if(LinksNumber < Number)
	{
		SnakeLinksPool.Reserve(Number);
		for(int32 i = 0; i < Number - LinksNumber; ++i)
		{
			AddSnakeLinkToPool(GetWorld()->SpawnActor<ASG_SnakeLink>(InSnakeLinkClass, FTransform::Identity));
		}
	}
}

TObjectPtr<ASG_SnakeLink> ASG_Snake::GetSnakeLinkFromPool(const TSubclassOf<ASG_SnakeLink>& InSnakeLinkClass, const FTransform& Transform)
{
	TObjectPtr<ASG_SnakeLink> SnakeLink = (SnakeLinksPool.Num() > 0) ? 
												SnakeLinksPool.Pop() : 
												TObjectPtr<ASG_SnakeLink>(GetWorld()->SpawnActor<ASG_SnakeLink>(InSnakeLinkClass, Transform));
	SnakeLink->SetActorHiddenInGame(false);
	SnakeLink->SetActorTransform(Transform);

	return SnakeLink;
}

void ASG_Snake::AddSnakeLinkToPool(TObjectPtr<ASG_SnakeLink> SnakeLink)
{
	SnakeLinksPool.Push(SnakeLink);
	SnakeLink->SetActorHiddenInGame(true); 
}