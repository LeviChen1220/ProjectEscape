#pragma once

UENUM()
enum class EEscapeConfirmType : uint8
{
	Yes,
	No
};

UENUM()
enum class EEscapeValidType : uint8
{
	Valid,
	Invalid
};

UENUM()
enum class EEscapeSuccessType : uint8
{
	Successful,
	Failed
};
