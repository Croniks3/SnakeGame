#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_MainMenuWidget.generated.h"

DECLARE_DELEGATE(FStartGameClick);

class UButton;


UCLASS()
class SNAKEGAME_API USG_MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FStartGameClick StartGameClickEvent;

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> StartGameButton;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandleInternalStartGameClick();
};