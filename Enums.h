#pragma once
enum Type
{
	NORMAL,
	SPECIAL,
	EMERGENCY,
	CANCELLATION //for patients only
};

enum CarStatus
{
	READY,
	ASSIGNED,
	LOADED,
	FAILED
};

enum UIModes
{
	INTERACTIVE,
	SILENT
};
