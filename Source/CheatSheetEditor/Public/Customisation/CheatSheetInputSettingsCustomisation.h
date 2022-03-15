#pragma once

#include <Editor/PropertyEditor/Public/IDetailCustomization.h>
#include <Editor/PropertyEditor/Public/DetailCategoryBuilder.h>
#include <Editor/PropertyEditor/Public/DetailLayoutBuilder.h>
#include <Editor/PropertyEditor/Public/DetailWidgetRow.h>
#include <Editor/PropertyEditor/Public/PropertyCustomizationHelpers.h>
#include <Editor/DetailCustomizations/Private/InputSettingsDetails.h>
#include <Runtime/Core/Public/Misc/AssertionMacros.h>
#include <Runtime/Core/Public/Templates/SharedPointer.h>
#include <CheatSheet/Public/Settings/CheatSheetSettings.h>
#include <Editor/PropertyEditor/Public/PropertyEditorModule.h>
#include "c:/Program Files/Epic Games/UE_4.27/Engine/Source/Editor/PropertyEditor/Public/IDetailGroup.h"

class FCheatSheetInputSettingsCustomisation : public IDetailCustomization
{
public:

	//IDetailCustomization
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override final
	{
		TArray<TSharedRef<IPropertyHandle>> Handles;
		Handles.Reserve(5);
		Handles.Emplace(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCheatSheetSettings, ShowBinding)));
		Handles.Emplace(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCheatSheetSettings, ConfirmBinding)));
		Handles.Emplace(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCheatSheetSettings, UpBinding)));
		Handles.Emplace(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCheatSheetSettings, DownBinding)));
		Handles.Emplace(DetailBuilder.GetProperty(GET_MEMBER_NAME_CHECKED(UCheatSheetSettings, BackBinding)));

		static const FName BindingsCategory = TEXT("Bindings");
		IDetailCategoryBuilder& MappingsDetailCategoryBuilder = DetailBuilder.EditCategory(BindingsCategory);

		for (const TSharedRef<IPropertyHandle>& Handle : Handles)
		{	
			IDetailGroup& NewGroup = MappingsDetailCategoryBuilder.AddGroup(Handle->GetDefaultCategoryName(), Handle->GetPropertyDisplayName());
			NewGroup.AddPropertyRow(Handle);
			/*IDetailPropertyRow& NewProp = MappingsDetailCategoryBuilder.AddProperty(Handle);

			TSharedPtr<SWidget> NameWidget;
			TSharedPtr<SWidget> ValueWidget;
			NewProp.GetDefaultWidgets(NameWidget, ValueWidget);

			MappingsDetailCategoryBuilder.AddCustomRow(Handle->GetPropertyDisplayName())
				.NameContent()
				[
					NameWidget.ToSharedRef()
				]
			.ValueContent()
				[
					ValueWidget.ToSharedRef()
				];*/
		}
	}
	//~IDetailCustomization

	static TSharedRef<IDetailCustomization> MakeInstance()
	{
		return MakeShareable(new FCheatSheetInputSettingsCustomisation());
	}
};