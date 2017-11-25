#include "hexmap.h"
#include <stdlib.h>
#include <plist/plist.h>

#ifdef FIXEDPT
#else
#include <math.h>
static float M_PI = 3.14159265359f;
static float M_SQR3 = 1.7320508076;
#endif // !FIXEDPT

static struct CubeCoord hex_directions[6] = { {1, 0, -1}, {1, -1, 0}, {0, -1, 1}, {-1, 0, 1}, {-1, 1, 0}, {0, 1, -1} };
static struct CubeCoord hex_diagonals[6] = { {2, -1, -1}, {1, -2, 1}, {-1, -1, 2}, {-2, 1, 1}, {-1, 2, -1}, {1, 1, -2} };

//static struct Orientation layout_pointy = { M_SQR3, M_SQR3 / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5 };
//static struct Orientation layout_flat = { 3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0 };

static inline struct Orientation layout_pointy() {
#ifdef FIXEDPT
	fixedpt fp3 = fixedpt_fromint(3);
	fixedpt fp2 = fixedpt_fromint(2);
	struct Orientation o;
	o.f0 = fixedpt_sqrt(fp3);
	o.f1 = fixedpt_div(fixedpt_sqrt(fp3), fixedpt_fromint(2));
	o.f2 = 0;
	o.f3 = fixedpt_div(fp3, fp2);
	o.b0 = fixedpt_div(fixedpt_sqrt(fp3), fp3);
	o.b1 = fixedpt_div(fixedpt_sub(0, FIXEDPT_ONE), fp3);
	o.b2 = 0;
	o.b3 = fixedpt_div(fp2, fp3);
	o.start_angle = FIXEDPT_ONE_HALF;
	return o;
#else
	return (struct Orientation) { sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5 };
#endif // FIXEDPT
}

static inline struct Orientation layout_flat() {
#ifdef FIXEDPT
	fixedpt FIXEDPT3 = fixedpt_fromint(3);
	fixedpt FIXEDPT2 = fixedpt_fromint(2);

	struct Orientation o;
	o.f0 = fixedpt_div(FIXEDPT3, FIXEDPT2);
	o.f1 = 0;
	o.f2 = fixedpt_div(fixedpt_sqrt(FIXEDPT3), FIXEDPT2);
	o.f3 = fixedpt_sqrt(FIXEDPT3);

	o.b0 = fixedpt_div(FIXEDPT2, FIXEDPT3);
	o.b1 = 0;
	o.b2 = fixedpt_div(fixedpt_neg(FIXEDPT_ONE), FIXEDPT3);
	o.b3 = fixedpt_div(fixedpt_sqrt(FIXEDPT3), FIXEDPT2);

	o.start_angle = 0;

	return o;
#else
	return (struct Orientation) { 3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0 };
#endif // FIXEDPT
}

//static struct Orientation layout_pointy = { sqrt(3.0), sqrt(3.0) / 2.0, 0.0, 3.0 / 2.0, sqrt(3.0) / 3.0, -1.0 / 3.0, 0.0, 2.0 / 3.0, 0.5 };
//static struct Orientation layout_flat = { 3.0 / 2.0, 0.0, sqrt(3.0) / 2.0, sqrt(3.0), 2.0 / 3.0, 0.0, -1.0 / 3.0, sqrt(3.0) / 3.0, 0.0 };

static inline struct CubeCoord cubecoord_add(struct CubeCoord a, struct CubeCoord b) {
	return { a.q + b.q, a.r + b.r, a.s + b.s };
}

static inline struct CubeCoord cubecoord_subtract(struct CubeCoord a, struct CubeCoord b) {
	return { a.q - b.q, a.r - b.r, a.s - b.s };
}

static inline struct CubeCoord cubecoord_scale(struct CubeCoord a, int k) {
	return { a.q * k, a.r * k, a.s * k };
}

static inline struct CubeCoord cubecoord_direction(int direction) {
	return hex_directions[direction];
}

static inline struct CubeCoord cubecoord_neighbor(struct CubeCoord hex, int direction) {
	return cubecoord_add(hex, cubecoord_direction(direction));
}

static inline struct CubeCoord cubecoord_diagonal_neighbor(struct CubeCoord hex, int direction) {
	return cubecoord_add(hex, hex_diagonals[direction]);
}

static inline int cubecoord_length(struct CubeCoord hex) {
	return (int)((abs(hex.q) + abs(hex.r) + abs(hex.s)) / 2);
}

static inline int cubecoord_distance(struct CubeCoord a, struct CubeCoord b) {
	return cubecoord_length(cubecoord_subtract(a, b));
}


static const int EVEN = 1;
static const int ODD = -1;
static inline struct OffsetCoord qoffset_from_cube(int offset, struct CubeCoord h) {
	struct OffsetCoord coord;
	coord.c = h.q;
	coord.r = h.r + (int)((h.q + offset * (h.q & 1)) / 2);
	return coord;
}

static inline struct CubeCoord qoffset_to_cube(int offset, struct OffsetCoord h) {
	struct CubeCoord coord;
	coord.q = h.c;
	coord.r = h.r - (int)((h.c + offset * (h.c & 1)) / 2);
	coord.s = -coord.q - coord.r;
	return coord;
}

static inline struct OffsetCoord roffset_from_cube(int offset, struct CubeCoord h) {
	struct OffsetCoord coord;
	coord.c = h.q + (int)((h.r + offset * (h.r & 1)) / 2);
	coord.r = h.r;
	return coord;
}

static inline struct CubeCoord roffset_to_cube(int offset, struct OffsetCoord h) {
	struct CubeCoord coord;
	coord.q = h.c - (int)((h.r + offset * (h.r & 1)) / 2);
	coord.r = h.r;
	coord.s = -coord.q - coord.r;
	return coord;
}

static inline struct CubeCoord axial_to_cube(struct AxialCoord h) {
	struct CubeCoord coord;
	coord.q = h.q;
	coord.r = h.r;
	coord.s = -h.q - h.r;
	return coord;
}

static inline struct AxialCoord cube_to_axial(struct CubeCoord h) {
	struct AxialCoord coord;
	coord.q = h.q;
	coord.r = h.r;
	return coord;
}

static inline int64_t axial_to_index(struct AxialCoord h) {
	int64_t index = 0;
	index |= h.q << 32;
	index |= h.r;
	return index;
}

static inline struct CubeCoord hex_round(struct FractionalCubeCoord h) {
#ifdef FIXEDPT
	int q = (int)(fixedpt_round(h.q));
	int r = (int)(fixedpt_round(h.r));
	int s = (int)(fixedpt_round(h.s));
	fixedpt q_diff = fixedpt_abs(fixedpt_sub(fixedpt_fromint(q), h.q));
	fixedpt r_diff = fixedpt_abs(fixedpt_sub(fixedpt_fromint(r), h.r));
	fixedpt s_diff = fixedpt_abs(fixedpt_sub(fixedpt_fromint(s), h.s));
	if (q_diff > r_diff && q_diff > s_diff) {
		q = -r - s;
	}
	else
		if (r_diff > s_diff) {
			r = -q - s;
		}
		else {
			s = -q - r;
		}
		return { q, r, s };
#else
	int q = (int)(fixedpt_round(h.q));
	int r = (int)(fixedpt_round(h.r));
	int s = (int)(fixedpt_round(h.s));
	double q_diff = abs(q - h.q);
	double r_diff = abs(r - h.r);
	double s_diff = abs(s - h.s);
	if (q_diff > r_diff && q_diff > s_diff) {
		q = -r - s;
	}
	else
		if (r_diff > s_diff) {
			r = -q - s;
		}
		else {
			s = -q - r;
		}
		return (struct CubeCoord) { q, r, s };
#endif // FIXEDPT
}

/*
** @breif 此函数有问题
*/
//static inline struct FractionalCubeCoord hex_lerp(struct FractionalCubeCoord a, struct FractionalCubeCoord b, double t) {
//	return { a.q * (1 - t) + b.q * t, a.r * (1 - t) + b.r * t, a.s * (1 - t) + b.s * t };
//}

struct Hex *
	hex_create(struct HexMap *map_, void *ud_) {
	struct Hex *inst = (struct Hex *)malloc(sizeof(*inst));
	memset(inst, 0, sizeof(*inst));
	inst->map = map_;
	inst->ud = ud_;
	return inst;
}

struct Hex *
	hex_create_axial(struct HexMap *map_, struct AxialCoord axial_, void *ud_) {
	struct Hex *inst = (struct Hex *)malloc(sizeof(*inst));
	memset(inst, 0, sizeof(*inst));
	inst->map = map_;
	inst->axial = axial_;
	inst->ud = ud_;

	return inst;
}

struct Hex *
	hex_add(struct Hex *self, struct Hex *other) {
	struct CubeCoord main = cubecoord_add(self->main, other->main);
	return hexmap_find_hex(self->map, main);
}

struct Hex *
	hex_subtract(struct Hex *self, struct Hex *other) {
	struct CubeCoord main = cubecoord_subtract(self->main, other->main);
	return hexmap_find_hex(self->map, main);
}

int64_t
hex_toindex(struct Hex *self) {
	return axial_to_index(self->axial);
}


//vector<Hex> hex_linedraw(Hex a, Hex b) {
//	int N = hex_distance(a, b);
//	FractionalHex a_nudge = FractionalHex(a.q + 0.000001, a.r + 0.000001, a.s - 0.000002);
//	FractionalHex b_nudge = FractionalHex(b.q + 0.000001, b.r + 0.000001, b.s - 0.000002);
//	vector<Hex> results = {};
//	double step = 1.0 / max(N, 1);
//	for (int i = 0; i <= N; i++) {
//		results.push_back(hex_round(hex_lerp(a_nudge, b_nudge, step * i)));
//	}
//	return results;
//}

struct HexMap *
hexmap_create_from_plist(const char *src, int len) {
	plist_t pl;
	plist_from_xml(src, len, &pl);

	plist_t value = plist_dict_get_item(pl, "");

	/*plist_dic
	plist_dict_get_item(p, "name");*/

	return NULL;
}

#ifdef FIXEDPT

struct HexMap *
	hexmap_create(MapOrientation o, fixedpt oradis) {
	fixedpt FIXEDPT3 = fixedpt_fromint(3);
	fixedpt FIXEDPT2 = fixedpt_fromint(2);
	fixedpt innerRadis = fixedpt_div(fixedpt_mul(oradis, fixedpt_sqrt(FIXEDPT3)), FIXEDPT2);

	struct vector3 origin = { FIXEDPT_ZERO, FIXEDPT_ZERO, FIXEDPT_ZERO };
	struct Layout l;
	if (o == FLAT) {
		l.orientation = layout_flat();
		l.origin = origin;
		l.innerRadis = innerRadis;
		l.outerRadis = oradis;
	}
	else {
		l.orientation = layout_pointy();
		l.origin = origin;
		l.innerRadis = innerRadis;
		l.outerRadis = oradis;
	}
	struct HexMap *inst = (struct HexMap *)malloc(sizeof(*inst));
	memset(inst, 0, sizeof(*inst));
	inst->layout = l;
	return inst;
}

#else
struct HexMap * hexmap_create(MapOrientation o, float oradis) {
#if defined(_DEBUG)
	float innerRadis = oradis * sqrt(3.0f) / 2.0f;
#else
	float innerRadis = oradis * 0.8660254038f;
#endif // 

	struct vector3 origin = { 0.0, 0.0, 0.0 };
	struct Layout l;
	if (o == FLAT) {
		l.orientation = layout_flat();
		l.origin = origin;
		l.innerRadis = innerRadis;
		l.outerRadis = oradis;
	}
	else {
		l.orientation = layout_pointy();
		l.origin = origin;
		l.innerRadis = innerRadis;
		l.outerRadis = oradis;
	}
	struct HexMap *inst = (struct HexMap *)malloc(sizeof(*inst));
	memset(inst, 0, sizeof(*inst));
	inst->layout = l;
	return inst;
}
#endif

void
hexmap_release(struct HexMap *self) {
	free(self);
}

struct Hex *
	hexmap_create_hex(struct HexMap *self) {
	struct Hex *elt, *res;
	int count;
	LL_COUNT(self->hexhead, elt, count);
	if (count > 0)
	{
		res = self->hexhead->next;
		LL_DELETE(self->hexhead, self->hexhead->next);
		return res;
	}
	res = (struct Hex *)malloc(sizeof(*res));
	return res;
}

void
hexmap_release_hex(struct HexMap *self, struct Hex *h) {
	assert(self != NULL && h != NULL);
	LL_APPEND(self->hexhead, h);
}

struct HexWaypoint *
	hexmap_create_hexastar(struct HexMap *self) {
	struct HexWaypoint *elt, *res;
	int count;
	LL_COUNT(self->hexshead, elt, count);
	if (count > 0)
	{
		res = self->hexshead->next;
		LL_DELETE(self->hexshead, self->hexshead->next);
		return res;
	}
	res = (struct HexWaypoint *)malloc(sizeof(*res));
	return res;
}

void
hexmap_release_hexastar(struct HexMap *self, struct HexWaypoint *h) {
	assert(self != NULL && h != NULL);
	LL_APPEND(self->hexshead, h);
}

struct Hex *    hexmap_find_hex(struct HexMap *self, struct CubeCoord coord) {
	struct AxialCoord axialcoord = cube_to_axial(coord);
	//struct Hex *** quad = hexmap_find_quad(self, axialcoord);
	//struct Hex *h = quad[axialcoord.q][axialcoord.r];
	/*return h;*/
	return NULL;
}

/*              ***
**            *     *
			***     *
		  *     ***
		  *     *
			***
*/
struct vector3
	hexmap_to_position(struct HexMap *self, struct AxialCoord coord) {
	struct Orientation M = self->layout.orientation;
	struct vector3 origin = self->layout.origin;
	struct AxialCoord h = coord;

#ifdef FIXEDPT
	fixedpt x = fixedpt_mul(fixedpt_add(fixedpt_mul(M.f0, fixedpt_fromint(h.q)), fixedpt_mul(M.f1, fixedpt_fromint(h.r))), self->layout.innerRadis);
	fixedpt z = fixedpt_mul(fixedpt_add(fixedpt_mul(M.f2, fixedpt_fromint(h.q)), fixedpt_mul(M.f3, fixedpt_fromint(h.r))), self->layout.innerRadis);

	struct vector3 res;
	res.x = x;
	res.y = FIXEDPT_ZERO;
	res.z = z;
#else
	float x = (M.f0 * h.q + M.f1 * h.r) * self->layout.innerRadis;
	float z = (M.f2 * h.q + M.f3 * h.r) * self->layout.innerRadis;
	struct vector3 res;
	res.x = x;
	res.y = 0.0f;
	res.z = z;
#endif // FIXEDPT
	return res;
}

int
hexmap_get_pathid(struct HexMap *self) {
	int i = 0;
	for (; i < MAX_PATH_NUM; i++)
	{
		if (self->pathState[i].free == 0) {
			return i;
		}
	}
	return -1;
}

/*
** -        -
** | f0, f1 |    * | q |  = | x |
** | f2, f3 |      | r |    | y |
** -        -
** 求逆矩阵
** | q | = | f0, f1 |-1 * | x |
** | r |   | f2, f3 |     | y |
*/
struct FractionalCubeCoord
	hexmap_to_cubcoord(struct HexMap *self, struct vector3 p) {
	struct Orientation M = self->layout.orientation;
	struct vector3 origin = self->layout.origin;

#ifdef FIXEDPT
	struct vector3 pt;
	pt.x = fixedpt_div(fixedpt_sub(p.x, origin.x), self->layout.innerRadis);
	pt.z = fixedpt_div(fixedpt_sub(p.z, origin.z), self->layout.innerRadis);

	fixedpt q = fixedpt_add(fixedpt_mul(M.b0, pt.x), fixedpt_mul(M.b1, pt.z));
	fixedpt r = fixedpt_add(fixedpt_mul(M.b2, pt.x), fixedpt_mul(M.b3, pt.z));

	struct FractionalCubeCoord coord;
	coord.q = q;
	coord.r = r;
	coord.s = fixedpt_sub(fixedpt_neg(q), r);
	return coord;

#else
	struct vector3 pt;
	pt.x = fixedpt_div(fixedpt_sub(p.x, origin.x), self->layout.innerRadis);
	pt.z = fixedpt_div(fixedpt_sub(p.z, origin.z), self->layout.innerRadis);

	fixedpt q = fixedpt_add(fixedpt_mul(M.b0, pt.x), fixedpt_mul(M.b1, pt.z));
	fixedpt r = fixedpt_add(fixedpt_mul(M.b2, pt.x), fixedpt_mul(M.b3, pt.z));

	double q = M.b0 * pt.x + M.b1 * pt.z;
	double r = M.b2 * pt.x + M.b3 * pt.z;
	return  (struct FractionalCubeCoord) { q, r, -q - r };
#endif // FIXEDPT
}

int
hexmap_h(struct HexMap *self, struct vector3 startPos, struct vector3 exitPos) {
#ifdef FIXEDPT
	return fixedpt_add(fixedpt_abs(fixedpt_sub(exitPos.x, startPos.x)), fixedpt_abs(fixedpt_sub(exitPos.z, startPos.z)));
#else
	return abs(exitPos.x - startPos.x) + abs(exitPos.z - startPos.z);
#endif // FIXEDPT
}

static int
hexastar_compare(void *lhs, void *rhs) {
	return (((struct HexWaypoint *)(lhs))->f < ((struct HexWaypoint *)(rhs))->f);
}

static int
hexastar_equal(void *lhs, void *rhs) {

	if (((struct HexWaypoint *)lhs)->hex == ((struct HexWaypoint *)rhs)->hex && ((struct HexWaypoint *)lhs)->hex != NULL)
	{
		return 0;
	}
	return 1;
}

int
hexmap_findpath(struct HexMap *self, struct vector3 startPos, struct vector3 exitPos) {

	int pathid = hexmap_get_pathid(self);
	self->pathState[pathid].free = 1;
	self->pathState[pathid].pathid = pathid;
	self->pathState[pathid].startPos = startPos;
	self->pathState[pathid].exitPos = exitPos;
	struct CubeCoord coord = hex_round(hexmap_to_cubcoord(self, exitPos));
	self->pathState[pathid].exitHex = hexmap_find_hex(self, coord);
	binary_heap_new(&self->pathState[pathid].open, hexastar_compare);
	self->pathState[pathid].closed = NULL;

	struct HexWaypoint *tmp = hexmap_create_hexastar(self);
	tmp->g = 0;
	tmp->h = hexmap_h(self, startPos, exitPos);
	tmp->f = tmp->g + tmp->h;

	coord = hex_round(hexmap_to_cubcoord(self, startPos));
	tmp->hex = hexmap_find_hex(self, coord);

	LL_APPEND(self->pathState[pathid].closed, tmp);

	return pathid;
}

static void hexastar_visit_free(void *h) {
	struct HexWaypoint *ptr = (struct HexWaypoint *)(h);
	if (ptr)
	{
		hexmap_release_hexastar(ptr->hex->map, ptr);
	}
}

int
hexmap_findpath_update(struct HexMap *self, int pathid, struct Hex **h) {
	int i = 0;
	for (; i < MAX_PATH_NUM; ++i)
	{
		struct HexWaypointHead *path = &self->pathState[i];
		if (path->free == 1) // 占用
		{
			if (binary_heap_size(path->open) > 0)
			{
				struct HexWaypoint *top = NULL;
				binary_heap_peek(path->open, (void **)&top);
				LL_APPEND(path->closed, top);

				assert(top != NULL);
				struct HexWaypoint *elt, etmp;
				int j = 0;
				for (; j < 6; j++) {
					if (top->hex->neighbor[i] == NULL) continue;

					etmp.hex = top->hex->neighbor[i];
					LL_SEARCH(path->closed, elt, &etmp, hexastar_equal);
					if (elt) // found
					{
						continue;
					}

					void *ret = NULL;
					binary_heap_search(path->open, &ret, (void *)&etmp, hexastar_equal);
					if (ret != NULL)
					{
						continue;
					}


					struct HexWaypoint *tmp = hexmap_create_hexastar(self);
					int cost = hexmap_h(self, top->hex->pos, top->hex->neighbor[i]->pos);
					tmp->g = top->g + cost;
					tmp->h = hexmap_h(self, top->hex->neighbor[i]->pos, self->pathState[pathid].exitPos);
					tmp->f = tmp->g + tmp->h;

					binary_heap_push(path->open, tmp);
				}
				path->nextHex = top->hex;
			}
		}
	}
	return 0;
}

int
hexmap_findpath_clean(struct HexMap *self, int pathid) {
	struct HexWaypointHead *path = &self->pathState[pathid];

	binary_heap_traverse(path->open, hexastar_visit_free);
	binary_heap_destroy(path->open);

	struct HexWaypoint *elt, *tmp, etmp;
	LL_FOREACH_SAFE(path->closed, elt, tmp) {
		hexmap_release_hexastar(self, elt);
	}

	return 0;
}