#include "Animal.h"

int32_t AnimalJob::GetThreadId()
{
	return _animal->GetThreadKey();
}
