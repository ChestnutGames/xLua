#pragma once
#ifndef aabb_h
#define aabb_h

#include <config.h>

#ifdef FIXEDPT
#include <fixedpt.h>
#include <fixedptmath3d.h>
#else
#include <math3d.h>
#endif // FIXEDPT
#include <stdbool.h>

struct aabb {
	struct vector3 min;
	struct vector3 max;
};

static inline struct vector3
aabb_center(struct aabb *A) {
#ifdef FIXEDPT
	struct vector3 center;
	center.x = fixedpt_mul(fixedpt_add(A->min.x, A->max.x), FIXEDPT_ONE_HALF);
	center.y = fixedpt_mul(fixedpt_add(A->min.y, A->max.y), FIXEDPT_ONE_HALF);
	center.z = fixedpt_mul(fixedpt_add(A->min.z, A->max.z), FIXEDPT_ONE_HALF);

	return center;
#endif // FIXEDPT
}

static inline void
aabb_corners(struct vector3 corners[8]) {

}

static inline bool
aabb_intersects(struct aabb *A, struct aabb *B) {
	return ((A->min.x >= B->min.x && A->min.x <= B->max.x) || (B->min.x >= A->min.x && B->min.x <= B->max.x)) &&
		((A->min.y >= B->min.y && A->min.y <= B->max.y) || (B->min.y >= A->min.y &&B->min.y <= A->max.y)) &&
		((A->min.z >= B->min.z && A->min.z <= B->max.z) || (B->min.z >= A->min.z && B->min.z <= A->max.z));
}

static inline bool
aabb_contain_point(struct aabb *A, struct vector3 point) {
	if (point.x < A->min.x) return false;
	if (point.y < A->min.y) return false;
	if (point.z < A->min.z) return false;
	if (point.x > A->max.x) return false;
	if (point.y > A->max.y) return false;
	if (point.z > A->max.z) return false;
	return true;
}

static inline void 
aabb_merge(struct aabb *A, struct aabb *B) {
	A->min.x = fixedpt_min(A->min.x, B->min.x);
	A->min.y = fixedpt_min(A->min.y, B->min.y);
	A->min.z = fixedpt_min(A->min.z, B->min.z);

	A->max.x = fixedpt_max(A->max.x, B->max.x);
	A->max.y = fixedpt_max(A->max.y, B->max.y);
	A->max.z = fixedpt_max(A->max.z, B->max.z);
}

static inline void
aabb_reset(struct aabb *A) {
	
}

static inline bool
aabb_empty(struct aabb *A) {
	return A->min.x > A->max.x || A->min.y > A->max.y || A->min.z > A->max.z;
}


#endif // !aabb_h

