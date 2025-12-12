#include "Framework/SG_GameUserSettings.h"


void USG_GameUserSettings::SetSnakeSettings(EGameSpeed GameSpeed, EGridSize GridSize)
{
	CurrentSpeed = GameSpeeds[GameSpeed];
	CurrentGridSize = GridSizes[GridSize];
}

EGameSpeed USG_GameUserSettings::GetGameSpeedEnumByName(const FString& Name)
{
	return FindGameOptionEnumByName(GameSpeeds, Name, EGameSpeed::Normal);
}

EGridSize USG_GameUserSettings::GetGridSizeEnumByName(const FString& Name)
{
	return FindGameOptionEnumByName(GridSizes, Name, EGridSize::Size_8x8);
}

template<typename MapType, typename EnumType>
EnumType USG_GameUserSettings::FindGameOptionEnumByName(const MapType& Map, const FString& Name, EnumType Default)
{
	for(const auto& Element : Map)
	{
		if(Element.Value.Name == Name)
		{
			return Element.Key;
		}
	}

	return Default;
}