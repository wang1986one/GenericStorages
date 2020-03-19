/****************************************************************************
Copyright (c) 2017-2027 GenericStorages

author: wangjieest

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#pragma once
#ifndef WORLD_OBJECT_STORAGE_H
#	define WORLD_OBJECT_STORAGE_H
#	include "CoreMinimal.h"

#	include "Engine/GameEngine.h"
#	include "Engine/GameInstance.h"
#	include "Engine/World.h"

template<typename T>
struct FWorldObjectStorePairImpl
{
	TWeakObjectPtr<UWorld> WeakWorld;
	TWeakObjectPtr<T> Object;
};

class FWorldObjectStoreImpl
{
public:
	template<typename T>
	static T* GetObject(const UObject* WorldContextObj, bool bEnsure = true)
	{
		UWorld* World = WorldContextObj ? WorldContextObj->GetWorld() : nullptr;
		return GetObject<T>(World, bEnsure);
	}

	template<typename T>
	static T* GetObject(UWorld* World, bool bEnsure = true)
	{
		check(!World || IsValid(World));
		auto& Ptr = FindOrAdd<T>(World);
		if (!Ptr.IsValid())
		{
			if (!IsValid(World))
			{
				auto Instance = FindInstance();
				ensureMsgf(!bEnsure || Instance != nullptr, TEXT("Instance Error"));
				auto Obj = NewObject<T>();
				Ptr = Obj;
				if (Instance)
				{
					Instance->RegisterReferencedObject(Obj);
				}
				else
				{
					Ptr->AddToRoot();
				}
			}
			else
			{
				auto Obj = NewObject<T>(World);
				Ptr = Obj;
				World->ExtraReferencedObjects.AddUnique(Obj);
			}
			check(Ptr.IsValid());
		}
		return Ptr.Get();
	}

	template<typename T>
	static TWeakObjectPtr<T>& FindOrAdd(UWorld* InWorld)
	{
		for (int32 i = 0; i < Storage<T>.Num(); ++i)
		{
			auto World = Storage<T>[i].WeakWorld;
			if (!World.IsStale(true))
			{
				if (InWorld == World.Get())
					return Storage<T>[i].Object;
			}
			else
			{
				Storage<T>.RemoveAt(i);
				--i;
			}
		}
		auto& Ref = Storage<T>.AddDefaulted_GetRef();
		if (IsValid(InWorld))
			Ref.WeakWorld = InWorld;
		return Ref.Object;
	}

	template<typename T>
	static void Remove(UWorld* InWorld)
	{
		if (!IsValid(InWorld))
			return;

		for (int32 i = 0; i < Storage<T>.Num(); ++i)
		{
			auto World = Storage<T>[i].WeakWorld;
			if (!World.IsStale(true))
			{
				if (InWorld == World.Get())
				{
					Storage<T>.RemoveAt(i);
					return;
				}
			}
			else
			{
				Storage<T>.RemoveAt(i);
				--i;
			}
		}
	}

	template<typename T>
	static TArray<FWorldObjectStorePairImpl<T>, TInlineAllocator<4>> Storage;

	static UGameInstance* FindInstance()
	{
		UGameInstance* Instance = nullptr;
#	if WITH_EDITOR
		if (GIsEditor)
		{
			ensureAlwaysMsgf(!GIsInitialLoad && GEngine, TEXT("Is it needed to get singleton before engine initialized?"));
			UWorld* World = nullptr;
			for (const FWorldContext& Context : GEngine->GetWorldContexts())
			{
				auto CurWorld = Context.World();
				if (IsValid(CurWorld))
				{
					if (CurWorld->IsGameWorld())
					{
						if (Context.WorldType == EWorldType::PIE /*&& Context.PIEInstance == 0*/)
						{
							World = CurWorld;
							break;
						}

						if (Context.WorldType == EWorldType::Game)
						{
							World = CurWorld;
							break;
						}

						if (CurWorld->GetNetMode() == ENetMode::NM_Standalone || (CurWorld->GetNetMode() == ENetMode::NM_Client && Context.PIEInstance == 2))
						{
							World = CurWorld;
							break;
						}
					}
				}
			}
			Instance = World ? World->GetGameInstance() : nullptr;
		}
		else
#	endif
		{
			if (UGameEngine* GameEngine = Cast<UGameEngine>(GEngine))
			{
				Instance = GameEngine->GameInstance;
			}
		}
		return Instance;
	}
};

template<typename T>
TArray<FWorldObjectStorePairImpl<T>, TInlineAllocator<4>> FWorldObjectStoreImpl::Storage;

#endif
