#include "UI/SG_MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Framework/SG_GameUserSettings.h"


void USG_MainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	
	if(StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalStartGameClick);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalExitGameClick);
	}

	if(!GEngine) { return; };
	USG_GameUserSettings* UserSettings = Cast<USG_GameUserSettings>(GEngine->GetGameUserSettings());
	if(!UserSettings) { return; } // @todo: make error

	SetupComboBoxByNames(GameSpeedComboBox, UserSettings->GetGameSpeedOptionNames(), UserSettings->GetCurrentGameSpeedOptionName());
	SetupComboBoxByNames(GridSizeComboBox, UserSettings->GetGridSizeOptionNames(), UserSettings->GetCurrentGridSizeOptionName());
}

void USG_MainMenuWidget::HandleInternalStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}

void USG_MainMenuWidget::HandleInternalExitGameClick()
{
	ExitGameClickEvent.ExecuteIfBound();
}

void USG_MainMenuWidget::SetupComboBoxByNames(TObjectPtr<UComboBoxString> ComboBox, const TArray<FString>& OptionNames, const FString& SelectedOption)
{
	if(ComboBox)
	{
		ComboBox->ClearOptions();

		for(const FString& OptionName : OptionNames)
		{
			ComboBox->AddOption(OptionName);
		}

		ComboBox->SetSelectedOption(SelectedOption);
		ComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::HandleInternalMenuOptionChanged);
	}
}

void USG_MainMenuWidget::HandleInternalMenuOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectionType != ESelectInfo::Direct)
	{
		SaveSettings();
	}
}

void USG_MainMenuWidget::SaveSettings()
{
	USG_GameUserSettings* UserSettings = Cast<USG_GameUserSettings>(GEngine->GetGameUserSettings());
	if(!UserSettings) { return; } 

	EGameSpeed GameSpeed = UserSettings->GetGameSpeedEnumByName(GameSpeedComboBox->GetSelectedOption());
	EGridSize GridSize = UserSettings->GetGridSizeEnumByName(GridSizeComboBox->GetSelectedOption());

	UserSettings->SetSnakeSettings(GameSpeed, GridSize);
}