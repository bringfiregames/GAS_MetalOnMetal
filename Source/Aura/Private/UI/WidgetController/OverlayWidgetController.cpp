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
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::OnMaxHealthChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetEnergyAttribute()).AddUObject(this, &UOverlayWidgetController::OnEnergyChangedCallback);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AuraAttributeSet->GetMaxEnergyAttribute()).AddUObject(this, &UOverlayWidgetController::OnMaxEnergyChangedCallback);
	
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

void UOverlayWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnEnergyChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnEnergyChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::OnMaxEnergyChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxEnergyChanged.Broadcast(Data.NewValue);
}
