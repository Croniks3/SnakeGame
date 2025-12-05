#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_GameplayWidget.generated.h"

DECLARE_DELEGATE_OneParam(FJoystickInputUpdateEvent, FVector2D);

class UTextBlock;
class USG_Joystick;


UCLASS()
class SNAKEGAME_API USG_GameplayWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FJoystickInputUpdateEvent InputUpdateEvent;

	virtual void SetVisibility(ESlateVisibility InVisibility) override;

	void SetGameTime(float seconds);
	void UpdateScores(uint32 scores);
	
protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> TimeText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> ScoresText;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USG_Joystick> JoystickWidget;

	virtual void NativeConstruct() override;

private:
	void OnInputUpdated(FVector2D InputVector);
};