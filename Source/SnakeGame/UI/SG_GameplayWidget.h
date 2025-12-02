#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_GameplayWidget.generated.h"

class UTextBlock;


UCLASS()
class SNAKEGAME_API USG_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetGameTime(float seconds);
	void UpdateScores(uint32 scores);

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoresText;
};
