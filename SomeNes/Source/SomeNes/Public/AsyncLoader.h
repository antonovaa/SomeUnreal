#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AsyncLoader.generated.h"

class UAnimMontage;
class USkeletalMesh;
class UTexture;

UDELEGATE()
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLoadComplete);
UCLASS(ClassGroup=(Custom))
class SOMENES_API UAsyncLoader: public UActorComponent
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;

	//Anim montage loader
	UFUNCTION(BlueprintCallable)
	void LoadAssetsAnim(const TMap<FString, TSoftObjectPtr<UAnimMontage>>& LoadsAnims);
	//Skeletal mesh loader
	UFUNCTION(BlueprintCallable)
	void LoadAssetsSkel(const TMap<FString, TSoftObjectPtr<USkeletalMesh>>& LoadsAnims);
	//Texture loader
	UFUNCTION(BlueprintCallable)
	void LoadAssetsTexture(const TMap<FString, TSoftObjectPtr< UTexture>>& LoadsAnims);
	//animmontage getter
	UFUNCTION(BlueprintCallable)
	TMap<FString, UAnimMontage*>&  GetLoadedAnimations();
	//mesh getter
	UFUNCTION(BlueprintCallable)
	TMap<FString,USkeletalMesh*>& GetLoadedMeshes();
	//Texture getter
	UFUNCTION(BlueprintCallable)
	TMap<FString, UTexture*>& GetLoadedTextures(); 

	//Complete delegate
	UPROPERTY(BlueprintAssignable)
	FLoadComplete OnLoadComplete;

	//Use this, if you want to delete assets from memory
	UFUNCTION()
	void DeleteAssets();
	
private:
	TMap<FString,FSoftObjectPath> ItemstoLoad;
	
	TMap<FString, UAnimMontage*> LoadedAnimations;
	TMap<FString, USkeletalMesh*> LoadedMeshes;
	TMap<FString, UTexture*> LoadedTexture;

	void LoadAnimEnd();
	void LoadSkelEnd();
	void LoadTextureEnd();
};
