#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_MainMenuWidget.generated.h"

DECLARE_DELEGATE(FStartGameClick);
DECLARE_DELEGATE(FExitGameClick);

class UButton;


UCLASS()
class SNAKEGAME_API USG_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FStartGameClick StartGameClickEvent;
	FStartGameClick ExitGameClickEvent;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartButton;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ExitButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandleInternalStartGameClick();

	UFUNCTION()
	void HandleInternalExitGameClick();
};