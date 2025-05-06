#pragma once


namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::Blue, int32 InKey = -1, float ShowTime = 3.f)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(InKey, ShowTime, Color, Msg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
		}
	}

	static void Print(const FString& FloatTitle, float FloatValueToPrint, int32 InKey = -1, const FColor& Color = FColor::Blue)
	{
		if (GEngine)
		{
			const FString FinalMsg = FloatTitle + TEXT(": ") + FString::SanitizeFloat(FloatValueToPrint);
			GEngine->AddOnScreenDebugMessage(InKey, 5.f, Color, FinalMsg);
			UE_LOG(LogTemp, Warning, TEXT("%s"), *FinalMsg);
		}
	}
}