#include "UI/SkillCardWidget.h"
#include "System/DataTableSubsystem.h"
#include "System/SkillDataRow.h" // 2차원 배열 구현을 위한 구조체 FSkillDeltaRow를 알아야 함. 
#include "Components/Button.h"
#include "Components/TextBlock.h"

void USkillCardWidget::SetupCard(FName InSkillFName, int32 InSkillLevel)
{
	Super::SetupCard(InSkillFName, InSkillLevel);

	if (UDataTableSubsystem* DTSubsystem = UDataTableSubsystem::Get(this))
	{
		// UI 설명용 FText
		FText Description = DTSubsystem->GetSkillDescription(InSkillFName, InSkillLevel);

		// UI 설명에 임베드 할 수치 로드
		const FSkillDeltaRow& DeltaRow = DTSubsystem->GetSkillDeltaRow(InSkillFName, InSkillLevel);
		const TArray<float>& Deltas = DeltaRow.Row;

		// 소수점 포맷 설정
		FNumberFormattingOptions IntegerFormat;
		FNumberFormattingOptions FractionalFormat;
		IntegerFormat.SetMaximumFractionalDigits(0);
		IntegerFormat.SetMinimumFractionalDigits(0);
		FractionalFormat.SetMaximumFractionalDigits(1);
		FractionalFormat.SetMinimumFractionalDigits(1);

		// 스킬 설명 부분 완성
		FFormatOrderedArguments Args;
		for (float Value : Deltas)
		{
			if (Value < 1.f)
			{
				// 수치가 1보다 작으면 소수점 한 자리 표시
				Args.Add(FText::AsNumber(Value, &FractionalFormat));
			}
			else
			{
				// 수치가 1보다 크면면 정수처럼 표시
				Args.Add(FText::AsNumber(Value, &IntegerFormat));
			}
		}
		FText FinalDescription = FText::Format(Description, Args);
		FText SkillName = FText::FromString(
			DTSubsystem->GetSkillUIName(InSkillFName).ToString() +
			FString::Printf(TEXT(" Lv.%d"), InSkillLevel + 1));
		NameText->SetText(SkillName);
		DescriptionText->SetText(FinalDescription);
	}
}
