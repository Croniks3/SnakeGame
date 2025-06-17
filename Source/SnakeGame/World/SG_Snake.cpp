#include "World/SG_Snake.h"


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

void ASG_Snake::BeginPlay()
{
	Super::BeginPlay();	

	if(Snake.IsValid() == false || GetWorld() == nullptr) { return; }

	const auto& links = Snake.Pin()->getLinks();
	
	uint32 i = 0;
	for(const auto& link : links)
	{
		const bool isHead = i == 0;
		const FTransform transform = FTransform(LinkPositionToVector(link, CellSize, GridDimensions));
		AActor* snakeLink = GetWorld()->SpawnActor<AActor>(isHead ? SnakeHeadClass : SnakeLinkClass, transform);
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
		actorLink->SetActorLocation(LinkPositionToVector(modelLinkPtr->GetValue(), CellSize, GridDimensions));
		modelLinkPtr = modelLinkPtr->GetNextNode();
	}
}

FVector ASG_Snake::LinkPositionToVector(const SnakeGame::Position& linkPosition, uint32 cellSize, const SnakeGame::Dimensions& gridDimensions) const
{
	return FVector(linkPosition.y * cellSize, linkPosition.x * cellSize, 0.0);
}
