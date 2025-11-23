#include "World/SG_Snake.h"
#include "World/SG_SnakeLink.h"
#include "SG_WorldUtils.h"


ASG_Snake::ASG_Snake()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASG_Snake::SetModel(const TSharedPtr<SnakeGame::Snake>& InSnake, uint32 InCellSize, const SnakeGame::Dimensions& InGridDimensions)
{
	Snake = InSnake;
	CellSize = InCellSize;
	GridDimensions = InGridDimensions;
}

void ASG_Snake::SetColors(const FLinearColor& HeadColor, const FLinearColor& LinkColor)
{
	for(int32 i = 0; i < SnakeLinks.Num(); ++i)
	{
		const bool isHead = i == 0;
		ASG_SnakeLink* SnakeLink = SnakeLinks[i];
		SnakeLink->SetColor(isHead ? HeadColor : LinkColor);
	}
}

void ASG_Snake::BeginPlay()
{
	Super::BeginPlay();	

	if(Snake.IsValid() == false || GetWorld() == nullptr) { return; }

	const auto& links = Snake.Pin()->getLinks();
	
	uint32 i = 0;
	for(const auto& link : links)
	{
		const bool isHead = i == 0;
		const FTransform transform = FTransform(SnakeGame::WorldUtils::GridPositionToVector(link, CellSize, GridDimensions));
		ASG_SnakeLink* snakeLink = GetWorld()->SpawnActorDeferred<ASG_SnakeLink>(isHead ? SnakeHeadClass : SnakeLinkClass, transform);
		snakeLink->SetScale(CellSize);
		snakeLink->FinishSpawning(transform);
		SnakeLinks.Add(snakeLink);
		++i;
	}
}

void ASG_Snake::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(Snake.IsValid() == false) { return; }

	const auto& modelLinks = Snake.Pin()->getLinks();
	auto* modelLinkPtr = modelLinks.GetHead();

	for(auto* actorLink : SnakeLinks)
	{
		actorLink->SetActorLocation(SnakeGame::WorldUtils::GridPositionToVector(modelLinkPtr->GetValue(), CellSize, GridDimensions));
		modelLinkPtr = modelLinkPtr->GetNextNode();
	}
}