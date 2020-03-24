﻿// Copyright 2018-2020 wangjieest, Inc. All Rights Reserved.

#pragma once

#ifndef UNREAL_COMPATIBILITY_GUARD_H
#	define UNREAL_COMPATIBILITY_GUARD_H

#	include "Launch/Resources/Version.h"
#	include "UObject/UnrealType.h"

#	include <type_traits>

#	if ENGINE_MINOR_VERSION <= 20
namespace ITS
{
	template<typename E>
	using is_scoped_enum = std::integral_constant<bool, std::is_enum<E>::value && !std::is_convertible<E, int>::value>;
	/*
		GCC:
		void foo() [with T = {type}]
		clang:
		void foo() [T = {type}]
		MSVC:
		void __cdecl foo<{type}>(void)
		*/
#		if defined(_MSC_VER)
#			define Z_TEMPLATE_PARAMETER_NAME_ __FUNCSIG__
	constexpr unsigned int FRONT_SIZE = sizeof("static const char* __cdecl ITS::TypeStr<") - 1u;
	constexpr unsigned int BACK_SIZE = sizeof(">(void)") - 1u;
#		else
#			define Z_TEMPLATE_PARAMETER_NAME_ __PRETTY_FUNCTION__
#			if defined(__clang__)
	constexpr unsigned int FRONT_SIZE = sizeof("static const char* ITS::TypeStr() [T = ") - 1u;
	constexpr unsigned int BACK_SIZE = sizeof("]") - 1u;
#			else
	constexpr unsigned int FRONT_SIZE = sizeof("static const char* ITS::TypeStr() [with T = ") - 1u;
	constexpr unsigned int BACK_SIZE = sizeof("]") - 1u;
#			endif
#		endif

	template<typename EnumType>
	static const char* TypeStr(void)
	{
		static_assert(std::is_enum<EnumType>::value, "err");
		constexpr int32 size = sizeof(Z_TEMPLATE_PARAMETER_NAME_) - FRONT_SIZE - BACK_SIZE;
		static char typeName[size] = {};
		memcpy(typeName, Z_TEMPLATE_PARAMETER_NAME_ + FRONT_SIZE, size - 1u);

		return typeName;
	}
}  // namespace ITS

template<typename EnumType>
UEnum* StaticEnum()
{
	static UEnum* Ret = ::FindObject<UEnum>(ANY_PACKAGE, ANSI_TO_TCHAR(ITS::TypeStr<EnumType>()), true);
	return Ret;
}
template<typename ClassType>
UClass* StaticClass()
{
	return ClassType::StaticClass();
}
template<typename StructType>
UScriptStruct* StaticStruct()
{
	return TBaseStructure<StructType>::Get();
}
#	else
#		include "UObject/ReflectedTypeAccessors.h"
#	endif

using FUnsizedIntProperty = UE4Types_Private::TIntegerPropertyMapping<signed int>::Type;
using FUnsizedUIntProperty = UE4Types_Private::TIntegerPropertyMapping<unsigned int>::Type;

#	if ENGINE_MINOR_VERSION < 25
using FFieldPropertyType = UObject;
using FFieldClass = UClass;
using FField = UField;

using FProperty = UProperty;
#		define CASTCLASS_FProperty CASTCLASS_UProperty

using FBoolProperty = UBoolProperty;
#		define CASTCLASS_FBoolProperty CASTCLASS_UBoolProperty

using FNumericProperty = UNumericProperty;
#		define CASTCLASS_FNumericProperty CASTCLASS_UNumericProperty
using FInt8Property = UInt8Property;
#		define CASTCLASS_FInt8Property CASTCLASS_UInt8Property
using FByteProperty = UByteProperty;
#		define CASTCLASS_FByteProperty CASTCLASS_UByteProperty

using FInt16Property = UInt16Property;
#		define CASTCLASS_FInt16Property CASTCLASS_UInt16Property
using FUInt16Property = UUInt16Property;
#		define CASTCLASS_FUInt16Property CASTCLASS_UUInt16Property

using FIntProperty = UIntProperty;
#		define CASTCLASS_FIntProperty CASTCLASS_UIntProperty
using FUInt32Property = UUInt32Property;
#		define CASTCLASS_FUInt32Property CASTCLASS_UUInt32Property

using FInt64Property = UInt64Property;
#		define CASTCLASS_FInt64Property CASTCLASS_UInt64Property
using FUInt64Property = UUInt64Property;
#		define CASTCLASS_FUInt64Property CASTCLASS_UUInt64Property

using FEnumProperty = UEnumProperty;
#		define CASTCLASS_FEnumProperty CASTCLASS_UEnumProperty

using FFloatProperty = UFloatProperty;
#		define CASTCLASS_FFloatProperty CASTCLASS_UFloatProperty
using FDoubleProperty = UDoubleProperty;
#		define CASTCLASS_FDoubleProperty CASTCLASS_UDoubleProperty

using FStructProperty = UStructProperty;
#		define CASTCLASS_FStructProperty CASTCLASS_UStructProperty

using FStrProperty = UStrProperty;
#		define CASTCLASS_FStrProperty CASTCLASS_UStrProperty
using FNameProperty = UNameProperty;
#		define CASTCLASS_FNameProperty CASTCLASS_UNameProperty
using FTextProperty = UTextProperty;
#		define CASTCLASS_FTextProperty CASTCLASS_UTextProperty

using FArrayProperty = UArrayProperty;
#		define CASTCLASS_FArrayProperty CASTCLASS_UArrayProperty
using FMapProperty = UMapProperty;
#		define CASTCLASS_FMapProperty CASTCLASS_UMapProperty
using FSetProperty = USetProperty;
#		define CASTCLASS_FSetProperty CASTCLASS_USetProperty

using FObjectPropertyBase = UObjectPropertyBase;
#		define CASTCLASS_FObjectPropertyBase CASTCLASS_UObjectPropertyBase
using FObjectProperty = UObjectProperty;
#		define CASTCLASS_FObjectProperty CASTCLASS_UObjectProperty
using FClassProperty = UClassProperty;
#		define CASTCLASS_FClassProperty CASTCLASS_UClassProperty
using FWeakObjectProperty = UWeakObjectProperty;
#		define CASTCLASS_FWeakObjectProperty CASTCLASS_UWeakObjectProperty
using FSoftObjectProperty = USoftObjectProperty;
#		define CASTCLASS_FSoftObjectProperty CASTCLASS_USoftObjectProperty
using FSoftClassProperty = USoftClassProperty;
#		define CASTCLASS_FSoftClassProperty CASTCLASS_USoftClassProperty
using FLazyObjectProperty = ULazyObjectProperty;
#		define CASTCLASS_FLazyObjectProperty CASTCLASS_ULazyObjectProperty
using FInterfaceProperty = UInterfaceProperty;
#		define CASTCLASS_FInterfaceProperty CASTCLASS_UInterfaceProperty

using FDelegateProperty = UDelegateProperty;
#		define CASTCLASS_FDelegateProperty CASTCLASS_UDelegateProperty
using FMulticastDelegateProperty = UMulticastDelegateProperty;
#		define CASTCLASS_FMulticastDelegateProperty CASTCLASS_UMulticastDelegateProperty

#		define CASTCLASS_FMulticastInlineDelegateProperty CASTCLASS_UMulticastInlineDelegateProperty
#		define CASTCLASS_FMulticastSparseDelegateProperty CASTCLASS_UMulticastSparseDelegateProperty

//////////////////////////////////////////////////////////////////////////

template<typename To, typename From>
FORCEINLINE auto CastField(From* Prop)
{
	return Cast<To>(Prop);
}
template<typename To, typename From>
FORCEINLINE auto CastFieldChecked(From* Prop)
{
	return CastChecked<To>(Prop);
}

FORCEINLINE EClassCastFlags GetPropCastFlags(FProperty* Prop)
{
	return (EClassCastFlags)Prop->GetClass()->ClassCastFlags;
}

template<typename T>
UClass* GetPropOwnerClass(T* Prop)
{
	return CastChecked<UClass>(Prop->GetOuter());
}

template<class T>
FORCEINLINE T* FindFProperty(const UStruct* Owner, FName FieldName)
{
	return FindField<T>(Owner, FieldName);
}

template<class T>
FORCEINLINE T* FindUField(const UStruct* Owner, FName FieldName)
{
	return FindField<T>(Owner, FieldName);
}

template<typename T = FField>
struct TFieldPath
{
public:
	TFieldPath(const T* InProp = nullptr)
		: Prop(const_cast<T*>(InProp))
	{
	}
	TFieldPath(const T& InProp)
		: Prop(const_cast<T*>(&InProp))
	{
	}

	void operator=(const T* InProp) { Prop = const_cast<T*>(InProp); }

	T* operator*() const { return Prop; }
	T* operator->() const { return Prop; }

	explicit operator bool() const { return !!Prop; }
	T* Get() const { return Prop; }

	T& GetOwnerVariant() const { return *Prop; }
	UClass* GetOwnerClass() const { return GetPropOwnerClass(Prop); }

	bool IsA(const UClass* InClass) const { return Field && Field->IsA(InClass); }
	template<typename T>
	bool IsA() const
	{
		static_assert(sizeof(T) > 0, "T must not be an incomplete type");
		return IsA(T::StaticClass());
	}

protected:
	mutable T* Prop;
};

struct FFieldVariant : public TFieldPath<>
{
	using TFieldPath<>::TFieldPath;
};

template<typename T>
using TFieldPathType = T*;
template<typename T>
FORCEINLINE T* GetPropPtr(T* Prop)
{
	return Prop;
}
template<typename T>
FORCEINLINE T* GetPropPtr(const TFieldPath<T>& Prop)
{
	return Prop.Get();
}

FORCEINLINE UObject* GetPropOwnerUObject(FProperty* Prop)
{
	return Prop->GetOuter();
}
FORCEINLINE FString GetPropOwnerName(FProperty* Prop)
{
	return Prop->GetOuter()->GetName();
}

#	else  // not (ENGINE_MINOR_VERSION < 25)

//////////////////////////////////////////////////////////////////////////

#		include "UObject/FieldPath.h"
using FFieldPropertyType = FProperty;

FORCEINLINE EClassCastFlags GetPropCastFlags(FProperty* Prop)
{
	return (EClassCastFlags)Prop->GetCastFlags();
}
template<typename T>
using TFieldPathType = TFieldPath<T>;

template<typename T>
FORCEINLINE T* GetPropPtr(T* Prop)
{
	return Prop;
}
template<typename T>
FORCEINLINE FProperty* GetPropPtr(const TFieldPath<T>& Prop)
{
	return const_cast<TFieldPath<T>&>(Prop).Get();
}
FORCEINLINE UObject* GetPropOwnerUObject(FProperty* Prop)
{
	return Prop->GetOwner<UObject>();
}
template<typename T>
FORCEINLINE UClass* GetPropOwnerClass(T* Prop)
{
	return Prop->GetOwnerClass();
}
FORCEINLINE FString GetPropOwnerName(FProperty* Prop)
{
	return Prop->GetOwnerVariant().GetName();
}
#	endif

#	if ENGINE_MINOR_VERSION < 24
#		define UE_ARRAY_COUNT ARRAY_COUNT
FORCEINLINE bool IsEngineExitRequested()
{
	return GIsRequestingExit;
}
#	endif

#	if ENGINE_MINOR_VERSION < 23
#		include "DelegateInstanceInterface.h"
#		include "Templates/SharedPointer.h"
#		include "Templates/TypeWrapper.h"
#		include "Templates/UnrealTemplate.h"

#		define UE_DEPRECATED(VERSION, MESSAGE) DEPRECATED(VERSION, MESSAGE)
/**
 * Implements a weak object delegate binding for C++ functors, e.g. lambdas.
 */
template<typename UserClass, typename FuncType, typename FunctorType, typename... VarTypes>
class TWeakBaseFunctorDelegateInstance;

template<typename UserClass, typename WrappedRetValType, typename... ParamTypes, typename FunctorType, typename... VarTypes>
class TWeakBaseFunctorDelegateInstance<UserClass, WrappedRetValType(ParamTypes...), FunctorType, VarTypes...> : public IBaseDelegateInstance<typename TUnwrapType<WrappedRetValType>::Type(ParamTypes...)>
{
public:
	typedef typename TUnwrapType<WrappedRetValType>::Type RetValType;

private:
	static_assert(TAreTypesEqual<FunctorType, typename TRemoveReference<FunctorType>::Type>::Value, "FunctorType cannot be a reference");

	typedef IBaseDelegateInstance<typename TUnwrapType<WrappedRetValType>::Type(ParamTypes...)> Super;
	typedef TWeakBaseFunctorDelegateInstance<UserClass, RetValType(ParamTypes...), FunctorType, VarTypes...> UnwrappedThisType;

public:
	TWeakBaseFunctorDelegateInstance(UserClass* InContextObject, const FunctorType& InFunctor, VarTypes... Vars)
		: ContextObject(InContextObject)
		, Functor(InFunctor)
		, Payload(Vars...)
		, Handle(FDelegateHandle::GenerateNewHandle)
	{
	}

	TWeakBaseFunctorDelegateInstance(UserClass* InContextObject, FunctorType&& InFunctor, VarTypes... Vars)
		: ContextObject(InContextObject)
		, Functor(MoveTemp(InFunctor))
		, Payload(Vars...)
		, Handle(FDelegateHandle::GenerateNewHandle)
	{
	}

	// IDelegateInstance interface

#		if USE_DELEGATE_TRYGETBOUNDFUNCTIONNAME

	virtual FName TryGetBoundFunctionName() const override final { return NAME_None; }

#		endif

	virtual UObject* GetUObject() const override final { return ContextObject.Get(); }

	// virtual const void* GetObjectForTimerManager() const final { return ContextObject.Get(); }
	// virtual uint64 GetBoundProgramCounterForTimerManager() const final { return 0; }

	// Deprecated
	virtual bool HasSameObject(const void* InContextObject) const override final { return GetUObject() == InContextObject; }

	virtual bool IsCompactable() const override final { return !ContextObject.Get(true); }

	virtual bool IsSafeToExecute() const override final { return ContextObject.IsValid(); }

public:
	// IBaseDelegateInstance interface
	virtual void CreateCopy(FDelegateBase& Base) override final { new (Base) UnwrappedThisType(*(UnwrappedThisType*)this); }

	virtual RetValType Execute(ParamTypes... Params) const override final { return Payload.ApplyAfter(Functor, Params...); }

	virtual FDelegateHandle GetHandle() const override final { return Handle; }

public:
	/**
	 * Creates a new static function delegate binding for the given function pointer.
	 *
	 * @param InFunctor C++ functor
	 * @return The new delegate.
	 */
	FORCEINLINE static void Create(FDelegateBase& Base, UserClass* InContextObject, const FunctorType& InFunctor, VarTypes... Vars) { new (Base) UnwrappedThisType(InContextObject, InFunctor, Vars...); }
	FORCEINLINE static void Create(FDelegateBase& Base, UserClass* InContextObject, FunctorType&& InFunctor, VarTypes... Vars) { new (Base) UnwrappedThisType(InContextObject, MoveTemp(InFunctor), Vars...); }

private:
	// Context object - the validity of this object controls the validity of the lambda
	TWeakObjectPtr<UserClass> ContextObject;

	// C++ functor
	// We make this mutable to allow mutable lambdas to be bound and executed.  We don't really want to
	// model the Functor as being a direct subobject of the delegate (which would maintain transivity of
	// const - because the binding doesn't affect the substitutability of a copied delegate.
	mutable typename TRemoveConst<FunctorType>::Type Functor;

	// Payload member variables, if any.
	TTuple<VarTypes...> Payload;

	// The handle of this delegate
	FDelegateHandle Handle;
};

template<typename UserClass, typename FunctorType, typename... ParamTypes, typename... VarTypes>
class TWeakBaseFunctorDelegateInstance<UserClass, void(ParamTypes...), FunctorType, VarTypes...> : public TWeakBaseFunctorDelegateInstance<UserClass, TTypeWrapper<void>(ParamTypes...), FunctorType, VarTypes...>
{
	typedef TWeakBaseFunctorDelegateInstance<UserClass, TTypeWrapper<void>(ParamTypes...), FunctorType, VarTypes...> Super;

public:
	/**
	 * Creates and initializes a new instance.
	 *
	 * @param InFunctor C++ functor
	 */
	TWeakBaseFunctorDelegateInstance(UserClass* InContextObject, const FunctorType& InFunctor, VarTypes... Vars)
		: Super(InContextObject, InFunctor, Vars...)
	{
	}

	TWeakBaseFunctorDelegateInstance(UserClass* InContextObject, FunctorType&& InFunctor, VarTypes... Vars)
		: Super(InContextObject, MoveTemp(InFunctor), Vars...)
	{
	}

	virtual bool ExecuteIfSafe(ParamTypes... Params) const override final
	{
		if (Super::IsSafeToExecute())
		{
			Super::Execute(Params...);
			return true;
		}

		return false;
	}
};
#	endif

// void_t
template<typename... T>
struct MakeVoid
{
	using type = void;
};
template<typename... T>
using VoidType = typename MakeVoid<T...>::type;

// Init Once
template<typename Type>
bool TrueOnFirstCall(const Type&)
{
	static bool bValue = true;
	bool Result = bValue;
	bValue = false;
	return Result;
}

// CreateWeakLabmda
template<typename DelegateType, typename UserClass, typename FunctorType, typename... VarTypes>
inline DelegateType CreateWeakLambda(UserClass* InUserObject, FunctorType&& InFunctor, VarTypes... Vars)
{
	DelegateType Result;
	TWeakBaseFunctorDelegateInstance<UserClass, typename DelegateType::TFuncType, typename TRemoveReference<FunctorType>::Type, VarTypes...>::Create(Result, InUserObject, Forward<FunctorType>(InFunctor), Vars...);
	return Result;
}
#endif
