#pragma once

#ifndef CSTDAFX_H
#define CSTDAFX_H

#ifdef __cplusplus
extern "C" {
#endif

#include <config.h>
#include <binaryheap.h>
#include <utarray.h>
#include <uthash.h>
#include <utlist.h>
#include <utringbuffer.h>
#include <utstring.h>

#ifdef FIXEDPT
#include <fixedpt.h>
#include <fixedptmath3d.h>
#else
#include <math3d.h>
#endif // FIXEDPT

#ifdef __cplusplus
}
#endif
#endif // !STDAFX_H

