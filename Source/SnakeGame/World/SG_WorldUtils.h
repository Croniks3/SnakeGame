#pragma once

#include "CoreMinimal.h"
#include "Core/SG_Types.h"
#include "Components/StaticMeshComponent.h"

namespace SnakeGame
{
	class WorldUtils
	{
	public:
		static FVector GridPositionToVector(const Position& gridPosition, uint32 cellSize, const Dimensions& gridDimensions)
		{
			return FVector((gridDimensions.height - 1 - gridPosition.y) * cellSize, gridPosition.x * cellSize, 0.0) + FVector(cellSize * 0.5);
		}

		static void ScaleMeshByWorldSize(UStaticMeshComponent* Mesh, const FVector& WorldSize)
		{
			if(Mesh && Mesh->GetStaticMesh())
			{
				auto staticMesh = Mesh->GetStaticMesh();
				check(staticMesh);
				const FBox Box = staticMesh->GetBoundingBox();
				const auto BoxSize = Box.GetSize();

				check(BoxSize.X); check(BoxSize.Y);
				Mesh->SetRelativeScale3D(WorldSize / BoxSize);
			}
		}

		static FText FormatSeconds(float inSeconds)
		{
			const int32 InTotalSeconds = FMath::RoundToInt(inSeconds);
			//const int32 hours = inTotalSeconds / 3600;
			const int32 Minutes = InTotalSeconds / 60;
			const int32 Seconds = InTotalSeconds % 60;

			const FString FormattedTime = FString::Printf(TEXT("%02i:%02i"), Minutes, Seconds);
			return FText::FromString(FormattedTime);
		}

		static FText FormatScores(uint32 inScores)
		{
			return FText::FromString(FString::Printf(TEXT("%03i"), inScores));
		}
	};
}