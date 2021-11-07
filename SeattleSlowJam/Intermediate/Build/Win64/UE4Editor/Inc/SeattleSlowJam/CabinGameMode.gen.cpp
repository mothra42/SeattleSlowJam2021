// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SeattleSlowJam/CabinGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeCabinGameMode() {}
// Cross Module References
	SEATTLESLOWJAM_API UClass* Z_Construct_UClass_ACabinGameMode_NoRegister();
	SEATTLESLOWJAM_API UClass* Z_Construct_UClass_ACabinGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_SeattleSlowJam();
// End Cross Module References
	void ACabinGameMode::StaticRegisterNativesACabinGameMode()
	{
	}
	UClass* Z_Construct_UClass_ACabinGameMode_NoRegister()
	{
		return ACabinGameMode::StaticClass();
	}
	struct Z_Construct_UClass_ACabinGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_ACabinGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_SeattleSlowJam,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_ACabinGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "CabinGameMode.h" },
		{ "ModuleRelativePath", "CabinGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_ACabinGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<ACabinGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_ACabinGameMode_Statics::ClassParams = {
		&ACabinGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_ACabinGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_ACabinGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_ACabinGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_ACabinGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(ACabinGameMode, 1200176018);
	template<> SEATTLESLOWJAM_API UClass* StaticClass<ACabinGameMode>()
	{
		return ACabinGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_ACabinGameMode(Z_Construct_UClass_ACabinGameMode, &ACabinGameMode::StaticClass, TEXT("/Script/SeattleSlowJam"), TEXT("ACabinGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(ACabinGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
