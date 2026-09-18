#include "UI/SkillBarWidget.h"
#include "UI/SkillSlotWidget.h"
#include "Player/Skill/SkillComponent.h"
#include "Player/Skill/ActiveSkillBase.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Engine/Texture2D.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"

void USkillBarWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	ActiveSlotWidgets.SetNum(USkillComponent::MaxSkillSlots);
	PassiveSlotWidgets.SetNum(USkillComponent::MaxPassiveSlots);
}

void USkillBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = GetOwningPlayer();
	if (!PC)
	{
		return;
	}

	PC->OnPossessedPawnChanged.AddUniqueDynamic(this, &USkillBarWidget::HandlePossessedPawnChanged);
	BindSkillComponent(PC->GetPawn());
}

void USkillBarWidget::NativeDestruct()
{
	UnbindSkillComponent();

	if (APlayerController* PC = GetOwningPlayer())
	{
		PC->OnPossessedPawnChanged.RemoveDynamic(this, &USkillBarWidget::HandlePossessedPawnChanged);
	}

	Super::NativeDestruct();
}

void USkillBarWidget::HandlePossessedPawnChanged(APawn* OldPawn, APawn* NewPawn)
{
	BindSkillComponent(NewPawn);
}

void USkillBarWidget::BindSkillComponent(APawn* Pawn)
{
	UnbindSkillComponent();
	if (!Pawn)
	{
		return;
	}

	CachedSkillComp = Pawn->FindComponentByClass<USkillComponent>();
	if (!CachedSkillComp)
	{
		return;
	}

	CachedSkillComp->OnSkillSlotChanged.AddDynamic(this, &USkillBarWidget::HandleSkillSlotChanged);
	CachedSkillComp->OnPassiveSlotChanged.AddDynamic(this, &USkillBarWidget::HandlePassiveSlotChanged);
	CachedSkillComp->OnSelectedSlotChanged.AddDynamic(this, &USkillBarWidget::HandleSelectedSlotChanged);

	for (int32 i = 0; i < ActiveSlotWidgets.Num(); ++i)
	{
		UpdateSlot(ActiveBox, ActiveSlotWidgets, i, CachedSkillComp->GetSkillInSlot(i));
	}
	for (int32 i = 0; i < PassiveSlotWidgets.Num(); ++i)
	{
		UpdateSlot(PassiveBox, PassiveSlotWidgets, i, CachedSkillComp->GetPassiveInSlot(i));
	}
	HandleSelectedSlotChanged(CachedSkillComp->GetSelectedSlotIndex());
}

void USkillBarWidget::UnbindSkillComponent()
{
	if (!CachedSkillComp)
	{
		return;
	}

	CachedSkillComp->OnSkillSlotChanged.RemoveDynamic(this, &USkillBarWidget::HandleSkillSlotChanged);
	CachedSkillComp->OnPassiveSlotChanged.RemoveDynamic(this, &USkillBarWidget::HandlePassiveSlotChanged);
	CachedSkillComp->OnSelectedSlotChanged.RemoveDynamic(this, &USkillBarWidget::HandleSelectedSlotChanged);
	CachedSkillComp = nullptr;
}

void USkillBarWidget::HandleSkillSlotChanged(int32 SlotIndex, USkillBase* Skill)
{
	UpdateSlot(ActiveBox, ActiveSlotWidgets, SlotIndex, Skill);
}

void USkillBarWidget::HandlePassiveSlotChanged(int32 SlotIndex, USkillBase* Skill)
{
	UpdateSlot(PassiveBox, PassiveSlotWidgets, SlotIndex, Skill);
}

void USkillBarWidget::HandleSelectedSlotChanged(int32 NewSlotIndex)
{
	for (int32 i = 0; i < ActiveSlotWidgets.Num(); ++i)
	{
		if (ActiveSlotWidgets[i])
		{
			ActiveSlotWidgets[i]->SetSelected(i == NewSlotIndex);
		}
	}
}

void USkillBarWidget::UpdateSlot(UHorizontalBox* Box, TArray<TObjectPtr<USkillSlotWidget>>& Slots, int32 SlotIndex, USkillBase* Skill)
{
	if (!Slots.IsValidIndex(SlotIndex) || !SlotWidgetClass)
	{
		return;
	}

	USkillSlotWidget* SlotWidget = Slots[SlotIndex];

	if (!Skill)
	{
		if (SlotWidget)
		{
			SlotWidget->RemoveFromParent();
			Slots[SlotIndex] = nullptr;
		}
		return;
	}

	if (!SlotWidget)
	{
		SlotWidget = CreateWidget<USkillSlotWidget>(this, SlotWidgetClass);
		if (!SlotWidget)
		{
			return;
		}
		Box->AddChildToHorizontalBox(SlotWidget)->SetPadding(SlotPadding);
		Slots[SlotIndex] = SlotWidget;
	}

	const TSoftObjectPtr<UTexture2D>* Icon = SkillIcons.Find(Skill->GetSkillName());
	UTexture2D* IconTexture = Icon ? Icon->LoadSynchronous() : nullptr;

	double CooldownEndTime = 0.0;
	if (const UActiveSkillBase* ActiveSkill = Cast<UActiveSkillBase>(Skill))
	{
		CooldownEndTime = GetWorld()->GetTimeSeconds() + ActiveSkill->GetRemainingCooldown();
	}

	SlotWidget->SetSkill(IconTexture, Skill->GetLevel(), CooldownEndTime);
}
