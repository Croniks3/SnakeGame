#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "Core/SG_Types.h"
#include "SG_GameUserSettings.generated.h"


UENUM()
enum class EGameSpeed : uint8
{
	Slow = 0,
	Normal,
	Fast
};

UENUM()
enum class EGridSize : uint8
{
	Size_6x6 = 0,
	Size_8x8,
	Size_10x10
};

UCLASS()
class SNAKEGAME_API USG_GameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	TArray<FString> GetGameSpeedOptions() const
	{
		TArray<FString> GameSpeedOtions;
		for(const TPair<EGameSpeed, FSpeedData>& GameSpeed : GameSpeeds)
		{
			GameSpeedOtions.Add(GameSpeed.Value.Name);
		}
		return GameSpeedOtions;
	}

	TArray<FString> GetGridSizeOptions() const
	{
		TArray<FString> GridSizeOtions;
		for(const TPair<EGridSize, FGridData>& GridSize : GridSizes)
		{
			GridSizeOtions.Add(GridSize.Value.Name);
		}
		return GridSizeOtions;
	}

	FString GetCurrentGameSpeedOption() const
	{
		return CurrentSpeed.Name;
	}

	FString GetCurrentGridSizeOption() const
	{
		return CurrentGridSize.Name;
	}

	void SetSnakeSettings(EGameSpeed GameSpeed, EGridSize GridSize);

	EGameSpeed GetGameSpeedEnumByName(const FString& Name);

	EGridSize GetGridSizeEnumByName(const FString& Name);

private:
	struct FSpeedData
	{
		FString Name;
		float Value;
	};

	const TMap<EGameSpeed, FSpeedData> GameSpeeds
	{
		{EGameSpeed::Slow,   {"Slow",   0.2f}},
		{EGameSpeed::Normal, {"Normal", 0.15f}},
		{EGameSpeed::Fast,   {"Fast",   0.10f}}
	};

	struct FGridData
	{
		FString Name;
		SnakeGame::Dimensions Dimensions;
	};

	const TMap<EGridSize, FGridData> GridSizes
	{
		{EGridSize::Size_6x6,   {"6x6",  SnakeGame::Dimensions{6, 6}}},
		{EGridSize::Size_8x8,   {"8x8", SnakeGame::Dimensions{8, 8}}},
		{EGridSize::Size_10x10, {"10x10",  SnakeGame::Dimensions{10, 10}}}
	};

	FSpeedData CurrentSpeed{GameSpeeds.FindChecked(EGameSpeed::Normal)};
	FGridData CurrentGridSize{GridSizes.FindChecked(EGridSize::Size_8x8)};

	template<typename MapType, typename EnumType>
	EnumType FindGameOptionEnumByName(const MapType& Map, const FString& Name, EnumType Default);
};