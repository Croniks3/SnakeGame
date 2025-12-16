#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SG_MainMenuWidget.generated.h"

DECLARE_DELEGATE(FStartGameClick);
DECLARE_DELEGATE(FExitGameClick);

class UButton;
class UComboBoxString;


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

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> GameSpeedComboBox;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UComboBoxString> GridSizeComboBox;

	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void HandleInternalStartGameClick();

	UFUNCTION()
	void HandleInternalExitGameClick();

	UFUNCTION()
	void HandleInternalMenuOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
	void SetupComboBoxByNames(TObjectPtr<UComboBoxString> ComboBox, const TArray<FString>& OptionNames, const FString& SelectedOption);
	void SaveSettings();
};