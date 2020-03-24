﻿/****************************************************************************
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

#include "CoreMinimal.h"

#include "Engine/DataTable.h"
#include "Templates/SubclassOf.h"
#include "UObject/WeakObjectPtr.h"
#include "UObject/WeakObjectPtrTemplates.h"

#include "DataTablePicker.generated.h"

//////////////////////////////////////////////////////////////////////////
USTRUCT(BlueprintType)
struct GENERICSTORAGES_API FDataTablePicker
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTablePicker")
	UDataTable* DataTable;
};

// Table+Row
USTRUCT(BlueprintType)
struct GENERICSTORAGES_API FDataTableRowPicker
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTablePicker")
	UDataTable* DataTable = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTablePicker")
	FName RowName;
};

USTRUCT(BlueprintType)
struct GENERICSTORAGES_API FDataTablePathPicker
{
	GENERATED_BODY()
public:
	// SoftPath
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTablePicker")
	TSoftObjectPtr<UDataTable> DataTablePath;
};

USTRUCT(BlueprintType)
struct GENERICSTORAGES_API FDataTableRowNamePicker
{
	GENERATED_BODY()
public:
	// RowName
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "DataTablePicker")
	FName RowName;

#if WITH_EDITORONLY_DATA
	// EditorOnly,Limit DataTable type
	UPROPERTY(EditAnywhere, Category = "DataTablePicker")
	TSoftObjectPtr<UDataTable> DataTablePath;
#endif
};
