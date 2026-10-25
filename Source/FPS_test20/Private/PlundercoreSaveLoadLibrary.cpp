// Fill out your copyright notice in the Description page of Project Settings.

#include "PlundercoreSaveLoadLibrary.h"

#include "Kismet/GameplayStatics.h"
#include "PlundercoreSaveGame.h"

namespace PlundercoreSave
{
	/** Highest save format version this build understands. Bump with each migration. */
	constexpr int32 CurrentSaveVersion = 1;
}

int32 UPlundercoreSaveLoadLibrary::GetCurrentSaveVersion()
{
	return PlundercoreSave::CurrentSaveVersion;
}

bool UPlundercoreSaveLoadLibrary::SavePlundercoreGame(UPlundercoreSaveGame* SaveObject, const FString& SlotName, int32 UserIndex)
{
	if (!IsValid(SaveObject))
	{
		UE_LOG(LogTemp, Error, TEXT("SavePlundercoreGame: SaveObject is null."));
		return false;
	}

	if (SlotName.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("SavePlundercoreGame: SlotName is empty."));
		return false;
	}

	if (!UGameplayStatics::SaveGameToSlot(SaveObject, SlotName, UserIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("SavePlundercoreGame: SaveGameToSlot failed for slot '%s'."), *SlotName);
		return false;
	}

	return true;
}

UPlundercoreSaveGame* UPlundercoreSaveLoadLibrary::LoadPlundercoreGame(const FString& SlotName, int32 UserIndex)
{
	if (!DoesPlundercoreSaveExist(SlotName, UserIndex))
	{
		return nullptr;
	}

	USaveGame* LoadedRaw = UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex);
	if (!LoadedRaw)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadPlundercoreGame: LoadGameFromSlot failed for slot '%s'."), *SlotName);
		return nullptr;
	}

	UPlundercoreSaveGame* Loaded = Cast<UPlundercoreSaveGame>(LoadedRaw);
	if (!Loaded)
	{
		UE_LOG(LogTemp, Error, TEXT("LoadPlundercoreGame: slot '%s' is not a Plundercore save."), *SlotName);
		return nullptr;
	}

	// A save written by a NEWER build may contain fields this build cannot
	// interpret — refuse rather than silently corrupt it.
	if (Loaded->SaveVersion > GetCurrentSaveVersion())
	{
		UE_LOG(LogTemp, Warning,
			TEXT("LoadPlundercoreGame: save v%d in slot '%s' is newer than this build (v%d); refusing to load."),
			Loaded->SaveVersion, *SlotName, GetCurrentSaveVersion());
		return nullptr;
	}

	// Migration hook: when the format changes, bump PlundercoreSave::CurrentSaveVersion
	// and upgrade older saves here, e.g.:
	//   if (Loaded->SaveVersion < 2) { Loaded->NewField = Default; Loaded->SaveVersion = 2; }

	return Loaded;
}

bool UPlundercoreSaveLoadLibrary::DoesPlundercoreSaveExist(const FString& SlotName, int32 UserIndex)
{
	return !SlotName.IsEmpty() && UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex);
}

bool UPlundercoreSaveLoadLibrary::DeletePlundercoreSave(const FString& SlotName, int32 UserIndex)
{
	if (!DoesPlundercoreSaveExist(SlotName, UserIndex))
	{
		return false;
	}

	if (!UGameplayStatics::DeleteGameInSlot(SlotName, UserIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("DeletePlundercoreSave: DeleteGameInSlot failed for slot '%s'."), *SlotName);
		return false;
	}

	return true;
}
