#pragma once
#ifndef LI_SI_GPU_SORT_H
#define LI_SI_GPU_SORT_H

#include "ShaderInterop.h"

LI_CBUFFER(SortCB, 8)
{
	int3 job_params;
	uint counterReadOffset;
};

#endif
