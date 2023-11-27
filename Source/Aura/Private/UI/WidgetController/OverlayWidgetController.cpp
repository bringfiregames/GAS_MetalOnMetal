// Copyright bringfire games


#include "UI/WidgetController/OverlayWidgetController.h"

#include "AbilitySystem/AuraAbilitySystemComponent.h"
#include "AbilitySystem/AuraAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValues()
{

	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	OnHealthChanged.Broadcast(AuraAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AuraAttributeSet->GetMaxHealth());
	OnEnergyChanged.Broadcast(AuraAttributeSet->GetEnergy());
	OnMaxEnergyChanged.Broadcast(AuraAttributeSet->GetMaxEnergy());
	
	
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UAuraAttributeSet* AuraAttributeSet = Cast<UAuraAttributeSet>(AttributeSet);
	
	/*Lambdas are a way to create a function on the fly, without having to create a new class for it, or having to declare it in the header file, or having to write it in a cpp file, etc.
	 *The Lambdas below are used to bind the OnHealthChanged, OnMaxHealthChanged, OnEnergyChanged, and OnMaxEnergyChanged delegates to the corresponding Attributes in the AttributeSet.
	 *These Lambdas are functions that take in a const FOnAttributeChangeData& Data, and then call the corresponding Broadcast function with the NewValue of the Attribute.
	 *We use Lambdas here because we want to bind the delegates to the Attributes, but we don't want to have to create a new function for each Attribute.
	 **/
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
		{
		OnHealthChanged.Broadcast(Data.NewValue);
		}
		);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
	{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
	}
	);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetEnergyAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
{
OnEnergyChanged.Broadcast(Data.NewValue);
}
);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxEnergyAttribute()).AddLambda(
[this](const FOnAttributeChangeData& Data)
{
OnMaxEnergyChanged.Broadcast(Data.NewValue);
}
);
	
	
	Cast<UAuraAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for(const FGameplayTag& Tag : AssetTags)
			{
				//For example, say that Tag = Message.HealthPotion
				//"Message.HealthPotion".MatchesTag("Message") will return True, "Message".MatchesTag("Message.HealthPotion") will return False
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if(Tag.MatchesTag(MessageTag))
				{
					//IN THIS EXAMPLE: This will return True, because "Message.HealthPotion" is a child of "Message" -- however it is a general purpose Lambda function, so it will work for any Message Tag
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, Tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
					
				}
			}
			//OnEffectAssetTags.Broadcast(AssetTags);
		}

	);
	
}
