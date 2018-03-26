#ifndef HEXMAP_H
#define HEXMAP_H

#ifdef __cplusplus
extern "C" {
#endif

#include <cstdafx.h>

#define MAX_PATH_NUM 20

typedef enum {
	NONE,
} MapState;

typedef enum {
	FLAT,
	POINTY,
} MapOrientation;

struct CubeCoord {
	int q;
	int r;
	int s;
};

#ifdef FIXEDPT
struct FractionalCubeCoord {
	fixedpt q; // x
	fixedpt r; // z
	fixedpt s; // y
};
#else
struct FractionalCubeCoord {
	double q; // x
	double r; // z
	double s; // y
};
#endif // FIXEDPT

struct OffsetCoord {
	int c;
	int r;
};

struct AxialCoord {
	int q;   // col
	int r;	 // row
};

struct HexMap;

struct HexWaypoint {
	int f;
	int g;
	int h;
	int free;
	struct Hex *hex;
	struct HexWaypoint *next, *prev;
};

/*
** cube coordinates
*/
struct Hex {
	struct HexMap     *map;
	struct CubeCoord   main;
	struct OffsetCoord offset;
	struct AxialCoord  axial;
	struct vector3     pos;
	float              height;
	MapState           state;
	void              *ud;

	// 双向链表构建格子，避免内存浪费，对于有很多空格的时候
	struct Hex        *neighbor[6];
	struct Hex *next, *prev;    // 内存
};

struct Hex *hex_create(struct HexMap *map_, void *ud_);
struct Hex *hex_create_axial(struct HexMap *map_, struct AxialCoord axial_, void *ud_);
struct Hex *hex_add(struct Hex *self, struct Hex *other);
struct Hex *hex_subtract(struct Hex *self, struct Hex *other);
int64_t     hex_toindex(struct Hex *self);

#ifdef FIXEDPT
struct Orientation {
	fixedpt f0;
	fixedpt f1;
	fixedpt f2;
	fixedpt f3;
	fixedpt b0;
	fixedpt b1;
	fixedpt b2;
	fixedpt b3;
	fixedpt start_angle;
};

struct Layout {
	struct Orientation orientation;
	struct vector3     origin;
	fixedpt            innerRadis;
	fixedpt            outerRadis;
};
#else
struct Orientation {
	double f0;
	double f1;
	double f2;
	double f3;
	double b0;
	double b1;
	double b2;
	double b3;
	double start_angle;
};

struct Layout {
	struct Orientation orientation;
	struct vector3     origin;
	float              innerRadis;
	float              outerRadis;
};
#endif // FIXEDPT

static int HexComp(struct Hex *lhs, struct Hex *rhs) {
	/*return left->pathState.*/
}

struct HexWaypointHead {
	int pathid;
	int free;
	struct vector3   startPos;
	struct vector3   exitPos;
	struct Hex      *nextHex;
	struct Hex      *exitHex;
	binary_heap_t   *open;
	struct HexWaypoint *closed;
};

struct HexMap {
	struct Layout layout;
	struct HexWaypointHead   pathState[MAX_PATH_NUM];

	struct Hex *hash;     // hash
	struct Hex *hexhead;  // pool
	struct HexWaypoint *hexshead;
};


struct HexMap * 
hexmap_create_from_plist(const char *src, int len);

#ifdef FIXEDPT
struct HexMap *
hexmap_create(MapOrientation o, fixedpt oradis);
#else
struct HexMap * 
hexmap_create(MapOrientation o, float oradis);
#endif

void            
hexmap_release(struct HexMap *self);

struct Hex *      
hexmap_create_hex(struct HexMap *self);

void              
hexmap_release_hex(struct HexMap *self, struct Hex *h);

struct HexWaypoint * 
hexmap_create_hexastar(struct HexMap *self);

void              
hexmap_release_hexastar(struct HexMap *self, struct HexWaypoint *h);

struct Hex *    
hexmap_find_hex(struct HexMap *self, struct CubeCoord coord);

struct vector3  
hexmap_to_position(struct HexMap *self, struct AxialCoord coord);

struct FractionalCubeCoord 
hexmap_to_cubcoord(struct HexMap *self, struct vector3 p);

int             
hexmap_h(struct HexMap *self, struct vector3 startPos, struct vector3 exitPos);

int             
hexmap_findpath(struct HexMap *self, struct vector3 startPos, struct vector3 exitPos);

int             
hexmap_findpath_update(struct HexMap *self, int pathid, struct Hex **h);

int             
hexmap_findpath_clean(struct HexMap *self, int pathid);

#ifdef __cplusplus
}
#endif

#endif