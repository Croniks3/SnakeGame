#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_GameOverWidget.generated.h"

DECLARE_DELEGATE(FOnRestartClicked);
DECLARE_DELEGATE(FOnBackClicked);

class UTextBlock;
class UButton;


UCLASS()
class SNAKEGAME_API USG_GameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetTotalGameTimeAndScores(float Seconds, uint32 Scores);

public:	
	FOnRestartClicked OnRestartClicked;
	FOnBackClicked OnBackClicked;
	
protected:
	virtual void NativeConstruct() override;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeTextValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoresTextValue;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> RestartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> BackButton;

private:
	UFUNCTION()
	void HandleInternalRestartClick();
	UFUNCTION()
	void HandleInternalBackClick();
};