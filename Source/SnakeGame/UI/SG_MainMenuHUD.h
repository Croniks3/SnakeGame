#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SG_MainMenuHUD.generated.h"

DECLARE_DELEGATE(FStartGameClick);
DECLARE_DELEGATE(FExitGameClick);

class USG_MainMenuWidget;


UCLASS()
class SNAKEGAME_API ASG_MainMenuHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	FStartGameClick StartGameClickEvent;
	FStartGameClick ExitGameClickEvent;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<USG_MainMenuWidget> MainMenuWidgetClass;

	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USG_MainMenuWidget> MainMenuWidget;

	void HandleStartGameClick();
	void HandleExitGameClick();
};