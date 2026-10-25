// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/SaveGame.h"
#include "PlundercoreSaveGame.generated.h"

/** One stack of an item in the player's bag (mirrors AC_Inventory entries). */
USTRUCT(BlueprintType)
struct FPlundercoreSavedItem
{
	GENERATED_BODY()

	/** Blueprint class of the item (e.g. BP_Raw_Gold, BP_Item_Torch). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	TSubclassOf<AActor> ItemClass = nullptr;

	/** How many of this item the bag holds. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 Quantity = 0;
};

/**
 * Persistent state for one Plundercore run.
 *
 * Design rule (Milestone 1 guide, Item 1): save LEVELS and COUNTS, never derived stats.
 * Pickaxe damage and haste are stored as upgrade levels and re-derived on load,
 * so balance changes apply retroactively to old saves.
 */
UCLASS()
class FPS_TEST20_API UPlundercoreSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	/** Bump when the save format changes; add a migration path in UPlundercoreSaveLoadLibrary::LoadPlundercoreGame. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 SaveVersion = 1;

	/** Current money (source: AC_MoneyBag). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int64 PlayerMoney = 0;

	/** Current XP and level (source: AC_XP). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 PlayerXP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 PlayerLevel = 1;

	/** Current in-game day — the number that drives cave length and quota (source: BP_GameManager). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 DayCount = 1;

	/** The quota the player must meet today (source: BP_GameManager). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 CurrentQuota = 0;

	/** Upgrade LEVELS — re-derive damage/haste from these on load via the upgrade formulas. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 PickaxeDamageLevel = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	int32 HasteLevel = 0;

	/** Rebuild AC_Inventory from this on load: ForEach entry -> add Quantity of ItemClass. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	TArray<FPlundercoreSavedItem> BagContents;

	/** Player transform to restore (set from GetActorLocation/GetActorRotation at save time). */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	FVector PlayerLocation = FVector::ZeroVector;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	FRotator PlayerRotation = FRotator::ZeroRotator;

	/** True once the first-day tutorial is finished; suppress prompts on load. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Plundercore|Save")
	bool bTutorialCompleted = false;
};
