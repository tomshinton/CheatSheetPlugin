// CheatSheet Plugin - Tom Shinton 2019

#pragma once

#include <Developer/AssetTools/Public/AssetTypeActions_Base.h>

class ISlateStyle;

class FCachedCheatAssetActions : public FAssetTypeActions_Base
{
public:

	FCachedCheatAssetActions(EAssetTypeCategories::Type InAssetCategory);

	virtual bool CanFilter() override;
	virtual uint32 GetCategories() override;
	virtual FText GetName() const override;
	virtual UClass* GetSupportedClass() const override;
	virtual FColor GetTypeColor() const override;
	virtual bool HasActions(const TArray<UObject*>& InObjects) const override;

private:

	EAssetTypeCategories::Type AssetCategory;
};
