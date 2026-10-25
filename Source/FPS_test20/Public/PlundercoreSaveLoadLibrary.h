// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PlundercoreSaveLoadLibrary.generated.h"

class UPlundercoreSaveGame;

/**
 * Blueprint-callable save/load entry points for Plundercore.
 *
 * Implements Item 1 of the Milestone 1 Implementation Guide. The intended slot
 * name is "PlundercoreSaveSlot" with UserIndex 0; settings (audio, resolution,
 * keybinds) should use a SEPARATE slot so wiping a run never wipes settings.
 *
 * Typical Blueprint flow:
 *   Save: build a UPlundercoreSaveGame (Create Save Game Object) -> fill fields
 *         from AC_MoneyBag / AC_XP / AC_Inventory / BP_GameManager -> SavePlundercoreGame.
 *   Load: DoesPlundercoreSaveExist -> LoadPlundercoreGame -> apply fields in
 *         BP_GameManager first, then player components, then transform.
 */
UCLASS()
class FPS_TEST20_API UPlundercoreSaveLoadLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/** Serializes SaveObject to the slot. Returns false if the save fails (e.g. invalid object or disk error). */
	UFUNCTION(BlueprintCallable, Category = "Plundercore|Save")
	static bool SavePlundercoreGame(UPlundercoreSaveGame* SaveObject, const FString& SlotName, int32 UserIndex);

	/**
	 * Loads and returns the save from the slot, or nullptr if the slot is empty,
	 * unreadable, or was written by a NEWER build than this one.
	 */
	UFUNCTION(BlueprintCallable, Category = "Plundercore|Save")
	static UPlundercoreSaveGame* LoadPlundercoreGame(const FString& SlotName, int32 UserIndex);

	/** True when a save exists in the slot. Use this to enable/grey the Continue button. */
	UFUNCTION(BlueprintPure, Category = "Plundercore|Save")
	static bool DoesPlundercoreSaveExist(const FString& SlotName, int32 UserIndex);

	/** Deletes the slot's save. Returns true when a save was deleted. */
	UFUNCTION(BlueprintCallable, Category = "Plundercore|Save")
	static bool DeletePlundercoreSave(const FString& SlotName, int32 UserIndex);

	/** Highest SaveVersion this build knows how to read. */
	UFUNCTION(BlueprintPure, Category = "Plundercore|Save")
	static int32 GetCurrentSaveVersion();
};
