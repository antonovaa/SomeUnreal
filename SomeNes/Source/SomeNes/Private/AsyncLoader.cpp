#include "../Public/AsyncLoader.h"
#include "Animation/AnimMontage.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/Texture2D.h"

void UAsyncLoader::BeginPlay()
{
	Super::BeginPlay();
}

void UAsyncLoader::LoadAssetsAnim(const TMap<FString, TSoftObjectPtr<UAnimMontage>>& LoadsAnims)
{
	TArray<FSoftObjectPath> Loads;
	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	for(const auto& AnimPairs:LoadsAnims)
	{
		auto ref = AnimPairs.Value.ToSoftObjectPath();
		//caching asset reference
		ItemstoLoad.Add(AnimPairs.Key, ref); 
		Loads.AddUnique(ref);
	}
	Streamable.RequestAsyncLoad(Loads, FStreamableDelegate::CreateUObject(this, &UAsyncLoader::LoadAnimEnd));
}
void UAsyncLoader::LoadAssetsSkel(const TMap<FString, TSoftObjectPtr<USkeletalMesh>>& LoadsSkels)
{
	TArray<FSoftObjectPath> Loads;
	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	for(const auto& SkelPairs:LoadsSkels)
	{
		auto ref = SkelPairs.Value.ToSoftObjectPath();
		//caching asset reference
		ItemstoLoad.Add(SkelPairs.Key, ref);
		Loads.AddUnique(ref);
	}
	Streamable.RequestAsyncLoad(Loads, FStreamableDelegate::CreateUObject(this, &UAsyncLoader::LoadSkelEnd));
}

void UAsyncLoader::LoadAssetsTexture(const TMap<FString, TSoftObjectPtr< UTexture>>& LoadsTextures)
{
	TArray<FSoftObjectPath> Loads;
	FStreamableManager& Streamable = UAssetManager::Get().GetStreamableManager();
	for(const auto& TexturePairs:LoadsTextures)
	{
		auto ref = TexturePairs.Value.ToSoftObjectPath();
		ItemstoLoad.Add(TexturePairs.Key, ref);
		Loads.AddUnique(ref);
	}
	Streamable.RequestAsyncLoad(Loads, FStreamableDelegate::CreateUObject(this, &UAsyncLoader::LoadTextureEnd));
}

// Clear memory from loaded assets
void UAsyncLoader::DeleteAssets() 
{
	for(auto& LoadAnims:LoadedAnimations)
	{
		LoadAnims.Value->ConditionalBeginDestroy();
	}
	for(auto& LoadMeshs:LoadedMeshes)
	{
		LoadMeshs.Value->ConditionalBeginDestroy();
	}
	for(auto& LoadTexture:LoadedTexture)
	{
		LoadTexture.Value->ConditionalBeginDestroy();
	}
}

void UAsyncLoader::LoadAnimEnd()
{
	UAnimMontage* Asset;
	for(auto& LoadItem: ItemstoLoad)
	{
		const FString name = LoadItem.Value.ToString();
		auto obj = StaticLoadObject(UAnimMontage::StaticClass(), nullptr, *name);
		Asset = Cast<UAnimMontage>(obj);
		LoadedAnimations.Add(LoadItem.Key,Asset);
	}
	ItemstoLoad.Reset();
	//delegate if we have any observers
	if(OnLoadComplete.IsBound())
	{
		OnLoadComplete.Broadcast();
	}
}

void UAsyncLoader::LoadSkelEnd()
{
	USkeletalMesh* Asset;
	for(auto& LoadItem: ItemstoLoad)
	{
		const FString name = LoadItem.Value.ToString();
		auto obj = StaticLoadObject(USkeletalMesh::StaticClass(), nullptr, *name);
		Asset = Cast<USkeletalMesh>(obj);
		LoadedMeshes.Add(LoadItem.Key,Asset);
	}
	ItemstoLoad.Reset();
	//delegate if we have any observers
	if(OnLoadComplete.IsBound())
	{
		OnLoadComplete.Broadcast();
	}
}

void UAsyncLoader::LoadTextureEnd()
{
	UTexture* Asset;
	for(auto& LoadItem: ItemstoLoad)
	{
		const FString name = LoadItem.Value.ToString();
		auto obj = StaticLoadObject(UTexture::StaticClass(), nullptr, *name);
		Asset = Cast<UTexture>(obj);
		LoadedTexture.Add(LoadItem.Key,Asset);
	}
	ItemstoLoad.Reset();
	//delegate if we have any observers
	if(OnLoadComplete.IsBound()) 
	{
		OnLoadComplete.Broadcast();
	}
}

TMap<FString, UAnimMontage*>& UAsyncLoader::GetLoadedAnimations()
{
	return LoadedAnimations;
}

TMap<FString, USkeletalMesh*>& UAsyncLoader::GetLoadedMeshes()
{
	
	return LoadedMeshes;
}

TMap<FString,  UTexture*>& UAsyncLoader::GetLoadedTextures() 
{
	return LoadedTexture;
}