// Copyright bringfire games


#include "AbilitySystem/AuraAbilitySystemComponent.h"

void UAuraAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UAuraAbilitySystemComponent::EffectApplied);
}

void UAuraAbilitySystemComponent::EffectApplied(UAbilitySystemComponent* AbilitySystemComponent,
                                                const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	for(const FGameplayTag& Tag : TagContainer)
	{
		//TODO: Broadcast to Widget Controller in UI
		GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Blue, FString::Printf(TEXT("GE Tag: %s"), *Tag.ToString()));
	}
}
