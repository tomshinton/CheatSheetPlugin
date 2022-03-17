// CheatSheet Plugin - Tom Shinton 2019

#include "CheatSheet/Public/CheatSheet.h"
#include "CheatSheet/Public/Settings/CheatSheetSettings.h"

#include <CheatSheetTypes/Public/Categories/CachedCheatAsset.h>
#include <CheatSheetTypes/Public/Categories/CachedCheatBuilder.h>
#include <CheatSheetUI/Public/UI/UI_CheatSheetHome.h>
#include <CheatSheetUI/Public/UI/UI_CheatView.h>
#include <Runtime/AssetRegistry/Public/AssetData.h>
#include <Runtime/AssetRegistry/Public/AssetRegistryModule.h>
#include <Runtime/CoreUObject/Public/UObject/Class.h>
#include <Runtime/CoreUObject/Public/UObject/Script.h>
#include <Runtime/CoreUObject/Public/UObject/UObjectIterator.h>
#include <Runtime/Engine/Classes/GameFramework/CheatManager.h>
#include <Runtime/Engine/Classes/GameFramework/GameModeBase.h>

#define LOCTEXT_NAMESPACE "FCheatSheetModule"

DEFINE_LOG_CATEGORY_STATIC(CheatSheetLog, Log, Log);

FCheatSheetModule::FCheatSheetModule()
	: CachedSettings(GetMutableDefault<UCheatSheetSettings>())
	, HomeScreen(nullptr)
	, WeakPlayerController(nullptr)
	, WeakCheatManager(nullptr)
	, IsHomeScreenVisible(false)
	, Map()
	, OnCheatMapBuilt()
	, WeakInputComponent(nullptr)
{}

void FCheatSheetModule::StartupModule()
{
	UE_LOG(CheatSheetLog, Log, TEXT("Spinning up CheatSheet"));

	FGameModeEvents::OnGameModePostLoginEvent().AddLambda([this](AGameModeBase* InGameMode, APlayerController* InNewPlayer)
	{
		CachedSettings = GetMutableDefault<UCheatSheetSettings>();
		WeakCheatManager = InNewPlayer->CheatManager;
		WeakPlayerController = InNewPlayer;

		SetupBindings(*InNewPlayer);
	});

	FWorldDelegates::OnWorldCleanup.AddRaw(this, &FCheatSheetModule::Cleanup);
}

void FCheatSheetModule::ShutdownModule()
{
	UE_LOG(CheatSheetLog, Log, TEXT("Shutting down CheatSheet"));

	Cleanup(nullptr, false, false);
}

void FCheatSheetModule::AddReferencedObjects(FReferenceCollector& Collector)
{
	Collector.AddReferencedObject(CachedSettings);
	Collector.AddReferencedObject(HomeScreen);
}

void FCheatSheetModule::Cleanup(UWorld* InWorld, bool bSessionEnded, bool bCleanupResources)
{
	WeakPlayerController = nullptr;
	CachedSettings = nullptr;
	IsHomeScreenVisible = false;
}

void FCheatSheetModule::CreateUI()
{
	if(TSubclassOf<UUI_CheatSheetHome> HomeScreenClass = CachedSettings->CheatSheetMenu)
	{
		HomeScreen = CreateWidget<UUI_CheatSheetHome>(WeakPlayerController.Get(), HomeScreenClass);
		OnCheatMapBuilt.AddUObject(HomeScreen, &UUI_CheatSheetHome::AddMap);

		RebuildCheatMap();
	}
}

void FCheatSheetModule::SetupBindings(APlayerController& InPlayerController)
{
	UInputComponent* NewInputComponent = NewObject<UInputComponent>(&InPlayerController);
	WeakInputComponent = NewInputComponent;

	const TArray<FInputActionKeyMapping> Bindings = CachedSettings->GetShowBinding();
	for(const FInputActionKeyMapping& Binding : Bindings)
	{
		UPlayerInput::AddEngineDefinedActionMapping(Binding);
	}
	
	FInputActionBinding ShowMenuAB(CheatSheetBindingNames::ShowBinding, IE_Pressed);
	ShowMenuAB.ActionDelegate.GetDelegateForManualSet().BindRaw(this, &FCheatSheetModule::ToggleListUI);
	NewInputComponent->AddActionBinding(ShowMenuAB);	

	NewInputComponent->RegisterComponent();
	InPlayerController.PushInputComponent(NewInputComponent);
}

void FCheatSheetModule::SetupUINavigation()
{
	if (UInputComponent* StrongInputComponent = WeakInputComponent.Get())
	{
		TArray<FInputActionKeyMapping> Bindings;
		Bindings.Append(CachedSettings->GetConfirmBinding());
		Bindings.Append(CachedSettings->GetUpBinding());
		Bindings.Append(CachedSettings->GetDownBinding());
		Bindings.Append(CachedSettings->GetBackBinding());

		for(const FInputActionKeyMapping& Binding : Bindings)
		{
			UPlayerInput::AddEngineDefinedActionMapping(Binding);
		}
		
		//Up
		FInputActionBinding UpAB(CheatSheetBindingNames::UpBinding, IE_Pressed);
		UpAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::UpSelection);
		StrongInputComponent->AddActionBinding(UpAB);

		//Confirm
		FInputActionBinding ConfirmAB(CheatSheetBindingNames::ConfirmBinding, IE_Pressed);
		ConfirmAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::ConfirmSelection);
		StrongInputComponent->AddActionBinding(ConfirmAB);

		//Down
		FInputActionBinding DownAB(CheatSheetBindingNames::DownBinding, IE_Pressed);
		DownAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen->GetCheatView(), &UUI_CheatView::DownSelection);
		StrongInputComponent->AddActionBinding(DownAB);

		//Back
		FInputActionBinding BackAB(CheatSheetBindingNames::BackBinding, IE_Pressed);
		BackAB.ActionDelegate.GetDelegateForManualSet().BindUObject(HomeScreen, &UUI_CheatSheetHome::ShowPreviousCategory);
		StrongInputComponent->AddActionBinding(BackAB);
	}
}

void FCheatSheetModule::UnbindUINavigation()
{
	if (UInputComponent* StrongInputComponent = WeakInputComponent.Get())
	{
		StrongInputComponent->RemoveActionBinding(CheatSheetBindingNames::UpBinding, IE_Pressed);
		StrongInputComponent->RemoveActionBinding(CheatSheetBindingNames::ConfirmBinding, IE_Pressed);
		StrongInputComponent->RemoveActionBinding(CheatSheetBindingNames::DownBinding, IE_Pressed);
		StrongInputComponent->RemoveActionBinding(CheatSheetBindingNames::BackBinding, IE_Pressed);
	}
}

void FCheatSheetModule::RebuildCheatMap()
{
	Map.Reset();

	//Parse CheatManager, if possible
	if (WeakCheatManager.IsValid())
	{
		UE_LOG(CheatSheetLog, Log, TEXT("Building CheatSheet from %s"), *WeakCheatManager->GetName());

		for (TFieldIterator<UFunction> Itr(WeakCheatManager->GetClass(), EFieldIteratorFlags::IncludeSuper); Itr; ++Itr)
		{
			if (UFunction* Function = *Itr)
			{
				if ((Function->FunctionFlags & FUNC_Exec) && (Function->NumParms == 0))
				{
					Map.AddCheat(FCachedCheat(
						*Function->GetName(),
						*Function->GetName(), 
						Function->GetMetaData(TEXT("Category")),
						Function->GetMetaData(TEXT("Tooltip")),
						Function,
						true));
				}
			}
		}
	}

	FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
	IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	
	//Find Cheat Assets
	{
		TArray<FAssetData> CheatAssets;
		const UClass* CachedCheatClass = UCachedCheatAsset::StaticClass();
		AssetRegistry.GetAssetsByClass(CachedCheatClass->GetFName(), CheatAssets);
		
		if(CheatAssets.Num() > 0)
		{
			UE_LOG(CheatSheetLog, Log, TEXT("Autodiscovered %i additional CheatSheets"), CheatAssets.Num());
		}
		
		for (const FAssetData& Asset : CheatAssets)
		{
			if (UCachedCheatAsset* CheatAsset = Cast<UCachedCheatAsset>(Asset.GetAsset()))
			{
				UE_LOG(CheatSheetLog, Log, TEXT("Autodiscovered CheatAssets: %s"), *CheatAsset->CheatString);
				Map.AddCheat(CheatAsset->GetCheat());
			}
		}
	}

	//Find Cheat Builder Assets
	{
		TSet< FName > DerivedNames;
		{
			TArray<FName> BaseNames;
			BaseNames.Add("CachedCheatBuilder");
			TSet<FName> Excluded;
			AssetRegistry.GetDerivedClassNames(BaseNames, Excluded, DerivedNames);
		}
		
		FARFilter Filter;
		Filter.ClassNames.Add(UBlueprint::StaticClass()->GetFName());
		Filter.bRecursiveClasses = true;
		Filter.bRecursivePaths = true;

		TArray< FAssetData > AssetList;
		AssetRegistry.GetAssets(Filter, AssetList);
		for(const FAssetData& Asset : AssetList)
		{
			const FAssetTagValueRef Tag = Asset.TagsAndValues.FindTag(TEXT("GeneratedClass"));
			if(Tag.IsSet())
			{
				const FString ClassObjectPath = FPackageName::ExportTextPathToObjectPath(*Tag.AsString());
				const FString ClassName = FPackageName::ObjectPathToObjectName(ClassObjectPath);

				if(DerivedNames.Contains(*ClassName))
				{
					if (UBlueprint* CheatAsset = Cast<UBlueprint>(Asset.GetAsset()))
					{
						if (UCachedCheatBuilder* CheatAssetCDO = Cast<UCachedCheatBuilder>(CheatAsset->GeneratedClass->GetDefaultObject()))
						{
							const TArray<FString> GeneratedCheats = CheatAssetCDO->Generate();
							for (const FString& GeneratedCheat : GeneratedCheats)
							{
								const FString RebuiltString = FString::Printf(TEXT("%s %s"), *CheatAssetCDO->CheatStringRoot, *GeneratedCheat);

								Map.AddCheat(FCachedCheat(
									RebuiltString,
									RebuiltString,
									CheatAssetCDO->Categories,
									CheatAssetCDO->Tooltip,
									nullptr,
									true));
							}
						}
					}
				}
			}
		}
	}
	
	Map.Sort();
	OnCheatMapBuilt.Broadcast(Map);
}

void FCheatSheetModule::ToggleListUI()
{
	if (UInputComponent* StrongInputComponent = WeakInputComponent.Get())
	{
		if (IsHomeScreenVisible)
		{
			UnbindUINavigation();
			HideList();

			if(UWorld* World = GWorld)
			{
				World->GetTimerManager().SetTimerForNextTick(FTimerDelegate::CreateLambda([WeakInputComponent = WeakInputComponent]()
				{
					if (UInputComponent* StrongInputComponent = WeakInputComponent.Get())
					{
						StrongInputComponent->bBlockInput = false;
					}
				}));
			}
		}
		else
		{
			StrongInputComponent->bBlockInput = true;
			
			CreateUI();
			SetupUINavigation();
			ShowList();
		}
	}
}

void FCheatSheetModule::ShowList()
{
	HomeScreen->AddToViewport(100);
	HomeScreen->InitHomeView();
	IsHomeScreenVisible = true;
}

void FCheatSheetModule::HideList()
{
	HomeScreen->RemoveFromViewport();
	IsHomeScreenVisible = false;
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCheatSheetModule, CheatSheet)