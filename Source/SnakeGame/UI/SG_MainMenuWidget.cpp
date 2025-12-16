#include "UI/SG_MainMenuWidget.h"
#include "Components/Button.h"
#include "Components/ComboBoxString.h"
#include "Framework/SG_GameUserSettings.h"


void USG_MainMenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if(!GEngine) { return; };
	USG_GameUserSettings* UserSettings = Cast<USG_GameUserSettings>(GEngine->GetGameUserSettings());
	if(!UserSettings) { return; } // @todo: add error

	if(StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalStartGameClick);
	}

	if(ExitButton)
	{
		ExitButton->OnClicked.AddDynamic(this, &ThisClass::HandleInternalExitGameClick);
	}

	if(GameSpeedComboBox)
	{
		GameSpeedComboBox->ClearOptions();

		for(const FString& GameSpeedOption : UserSettings->GetGameSpeedOptionNames())
		{
			GameSpeedComboBox->AddOption(GameSpeedOption);
		}
		
		GameSpeedComboBox->SetSelectedOption(UserSettings->GetCurrentGameSpeedOptionName());
		GameSpeedComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::HandleInternalGameSpeedOptionChanged);
	}

	if(GridSizeComboBox)
	{
		GridSizeComboBox->ClearOptions();

		for(const FString& GridSizeOption : UserSettings->GetGridSizeOptionNames())
		{
			GridSizeComboBox->AddOption(GridSizeOption);
		}

		GridSizeComboBox->SetSelectedOption(UserSettings->GetCurrentGridSizeOptionName());
		GridSizeComboBox->OnSelectionChanged.AddDynamic(this, &ThisClass::HandleInternalGridSizeOptionChanged);
	}
}

void USG_MainMenuWidget::HandleInternalStartGameClick()
{
	StartGameClickEvent.ExecuteIfBound();
}

void USG_MainMenuWidget::HandleInternalExitGameClick()
{
	ExitGameClickEvent.ExecuteIfBound();
}

void USG_MainMenuWidget::HandleInternalGameSpeedOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectionType != ESelectInfo::Direct)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *GameSpeedComboBox->GetSelectedOption());
		SaveSettings();
	}
}

void USG_MainMenuWidget::HandleInternalGridSizeOptionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
	if(SelectionType != ESelectInfo::Direct)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *GridSizeComboBox->GetSelectedOption());
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