// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheet/Public/CheatSheet.h"
#include "CheatSheet/Public/Settings/CheatSheetSettings.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatAsset.h>
#include <CheatSheetTypes/Public/Categories/CachedCheatBuilder.h>
#include <CheatSheetTypes/Public/Input/CheatSheetStaticBindings.h>
#include <CheatSheetUI/Public/UI/UI_CheatSheetHome.h>
#include <CheatSheetUI/Public/UI/UI_CheatView.h>
#include <Runtime/AssetRegistry/Public/AssetData.h>
#include <Runtime/AssetRegistry/Public/AssetRegistryModule.h>
#include <Runtime/CoreUObject/Public/UObject/Class.h>
#include <Runtime/CoreUObject/Public/UObject/Script.h>
#include <Runtime/CoreUObject/Public/UObject/UObjectIterator.h>
#include <Runtime/Engine/Classes/GameFramework/CheatManager.h>

#define LOCTEXT_NAMESPACE "FCheatSheetModule"

DEFINE_LOG_CATEGORY_STATIC(CheatSheetLog, Log, Log);

FCheatSheetModule::FCheatSheetModule()
	: CachedSettings(nullptr)
	, CachedPlayerController(nullptr)
	, HomeScreen(nullptr)
	, CurrentCheatManager(nullptr)
	, IsHomeScreenVisible(false)
	, Map()
	, OnCheatMapBuilt()
{}

void FCheatSheetModule::StartupModule()
{
	UE_LOG(CheatSheetLog, Log, TEXT("Spinning up CheatSheet"));

	UWorld* World = GWorld;

	CheatSheetStaticBindings::TempPushInputs();
}

void FCheatSheetModule::ShutdownModule()
{
	UE_LOG(CheatSheetLog, Log, TEXT("Shutting down CheatSheet"));
}

void FCheatSheetModule::Init(const APawn& InLocalPlayer)
{
	ICheatSheetInterface& CheatSheetInterface = FModuleManager::GetModuleChecked<ICheatSheetInterface>("CheatSheet");
	CheatSheetInterface.InitInternal(InLocalPlayer);
}

void FCheatSheetModule::Cleanup()
{
	ICheatSheetInterface& CheatSheetInterface = FModuleManager::GetModuleChecked<ICheatSheetInterface>("CheatSheet");
	CheatSheetInterface.CleanupInternal();
}

void FCheatSheetModule::InitInternal(const APawn& InLocalPawn)
{
	CachedPlayerController = Cast<APlayerController>(InLocalPawn.GetController());

	if (CachedPlayerController == nullptr)
	{
		UE_LOG(CheatSheetLog, Warning, TEXT("Attempting to pull a nullptr controller from %s in CheatSheet InitInternal - cannont build CheatSheet"), *InLocalPawn.GetName());
		return;
	}
	else
	{
		CacheSettings();

		if (CachedSettings != nullptr)
		{
			PreCreateUI();
			SetupBindings();

			CurrentCheatManager = CachedPlayerController->CheatManager;

			if (CurrentCheatManager != nullptr)
			{
				RebuildCheatMap();
			}
		}
		else
		{
			UE_LOG(CheatSheetLog, Error, TEXT("Attempting to spin up CheatSheet with invalid Settings - returning"));
		}
	}
}

void FCheatSheetModule::CleanupInternal()
{
	CachedPlayerController = nullptr;
}

void FCheatSheetModule::CacheSettings()
{
	if (CachedSettings == nullptr)
	{
		CachedSettings = GetMutableDefault<UCheatSheetSettings>();
	}
	else
	{
		UE_LOG(CheatSheetLog, Log, TEXT("CheatSheet settings alrady cached"));
	}
}

void FCheatSheetModule::PreCreateUI()
{
	if(TSubclassOf<UUI_CheatSheetHome> HomeScreenClass = CachedSettings->CheatSheetMenu)
	{
		HomeScreen = CreateWidget<UUI_CheatSheetHome>(CachedPlayerController, HomeScreenClass);
		OnCheatMapBuilt.AddUObject(HomeScreen.Get(), &UUI_CheatSheetHome::AddMap);
	}
}

void FCheatSheetModule::SetupBindings()
{
	//For actual key bindings, see CheatSheetStaticBindings
	//All keys are bound in code to prevent GameDefaults.ini from getting in the way
	if (UInputComponent* InputComponent = CachedPlayerController->GetPawn()->InputComponent)
	{
		if (HomeScreen.IsValid())
		{
			//Show Menu
			FInputActionBinding ShowMenuAB(CheatSheetStaticBindings::ShowMenu, IE_Pressed);
			ShowMenuAB.ActionDelegate.GetDelegateForManualSet().BindRaw(this, &FCheatSheetModule::ToggleListUI);
			InputComponent->AddActionBinding(ShowMenuAB);

			//Confirm
			FInputActionBinding ConfirmAB(CheatSheetStaticBindings::Confirm, IE_Pressed);
			ConfirmAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::ConfirmSelection);
			InputComponent->AddActionBinding(ConfirmAB);

			//Up
			FInputActionBinding UpAB(CheatSheetStaticBindings::Up, IE_Pressed);
			UpAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::UpSelection);
			InputComponent->AddActionBinding(UpAB);

			//Down
			FInputActionBinding DownAB(CheatSheetStaticBindings::Down, IE_Pressed);
			DownAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::DownSelection);
			InputComponent->AddActionBinding(DownAB);

			//Back
			FInputActionBinding BackAB(CheatSheetStaticBindings::Back, IE_Pressed);
			BackAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen.Get(), &UUI_CheatSheetHome::ShowPreviousCategory);
			InputComponent->AddActionBinding(BackAB);
		}
	}
}

void FCheatSheetModule::RebuildCheatMap()
{
	Map = CheatMap();

	if (CurrentCheatManager.IsValid())
	{
		UE_LOG(CheatSheetLog, Log, TEXT("Building CheatSheet from %s"), *CurrentCheatManager->GetName());

		for (TFieldIterator<UFunction> Itr(CurrentCheatManager->GetClass(), EFieldIteratorFlags::IncludeSuper); Itr; ++Itr)
		{
			if (UFunction* Function = *Itr)
			{
				if ((Function->FunctionFlags & FUNC_Exec) && (Function->NumParms == 0))
				{
					Map.GetValue().AddCheat(FCachedCheat(
						*Function->GetName(), //Name actual
						*Function->GetName(), //Display name for autodiscovered cheats
						Function->GetMetaData(TEXT("Category")),
						Function->GetMetaData(TEXT("Tooltip")),
						Function));

					UE_LOG(CheatSheetLog, Log, TEXT("Function Found from CheatManager: %s();"), *Function->GetName());
				}
			}
		}
	}

	//Grab all CheatAssets
	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");

	TArray<FAssetData> CheatAssets;
	const UClass* CachedCheatClass = UCachedCheatAsset::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(CachedCheatClass->GetFName(), CheatAssets);
	for (const FAssetData& Asset : CheatAssets)
	{
		if (UCachedCheatAsset* CheatAsset = Cast<UCachedCheatAsset>(Asset.GetAsset()))
		{
			Map.GetValue().AddCheat(CheatAsset->GetCheat());
			UE_LOG(CheatSheetLog, Log, TEXT("Function Found from autodiscovered CheatAssets: %s();"), *CheatAsset->CheatString);
		}
	}

	TArray<FAssetData> BuilderAssets;
	const UClass* CachedCheatBuilder = UCachedCheatBuilder::StaticClass();
	AssetRegistryModule.Get().GetAssetsByClass(CachedCheatBuilder->GetFName(), BuilderAssets);
	for (const FAssetData& Asset : BuilderAssets)
	{
		if (UCachedCheatBuilder* BuilderAsset = Cast<UCachedCheatBuilder>(Asset.GetAsset()))
		{
			const TArray<FString> GeneratedParams = BuilderAsset->Generate();
			for (const FString& Param : GeneratedParams)
			{
				const FString RebuiltString = FString::Printf(TEXT("%s %s"), *BuilderAsset->CheatStringRoot, *Param);

				Map.GetValue().AddCheat(FCachedCheat(
					RebuiltString, //Name actual
					RebuiltString, //Display name for autodiscovered cheats
					BuilderAsset->Categories,
					BuilderAsset->Tooltip,
					nullptr));

				UE_LOG(CheatSheetLog, Log, TEXT("Function Found from autogenerated CheatBuilder: %s();"), *RebuiltString);
			}
		}
	}

	TArray< FString > ContentPaths;
	ContentPaths.Add(TEXT("/Game"));
	AssetRegistryModule.Get().ScanPathsSynchronous(ContentPaths);
	FName BaseClassName = CachedCheatBuilder->GetFName();

	// Use the asset registry to get the set of all class names deriving from Base
	TSet< FName > DerivedNames;
	{
		TArray< FName > BaseNames;
		BaseNames.Add(BaseClassName);

		TSet< FName > Excluded;
		AssetRegistryModule.Get().GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
	}

	FARFilter Filter;
	Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
	Filter.bRecursivePaths = true;

	TArray< FAssetData > AssetList;
	AssetRegistryModule.Get().GetAssets(Filter, AssetList);


	OnCheatMapBuilt.Broadcast(Map.GetValue());
}

#if WITH_EDITOR
void FCheatSheetModule::DebugPrintCheatMap()
{
	if (Map.IsSet())
	{
		for (const FCheatCategory& Cat : Map.GetValue().GetCategories())
		{
			Cat.Print(0);
		}
	}
}
#endif //WITH_EDITOR

void FCheatSheetModule::ToggleListUI()
{
	if (HomeScreen.IsValid())
	{
		IsHomeScreenVisible ? HideList() : ShowList();
	}
}

void FCheatSheetModule::ShowList()
{
	HomeScreen->AddToViewport(100); //Add it at Z100 so it's most likely infront of everything else

	HomeScreen->InitHomeView();

	IsHomeScreenVisible = true;
}

void FCheatSheetModule::HideList()
{
	HomeScreen->RemoveFromParent();
	IsHomeScreenVisible = false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCheatSheetModule, CheatSheet)