#pragma once

#include "Math/Vec.h"

struct Rect {
	Vec2i min, max;

	Rect() = default;
	Rect(const Vec2i &min, const Vec2i &max): min(min), max(max) {}
	Rect(int x0, int y0, int x1, int y1): min(x0, y0), max(x1, y1) {}

	int width() const { return max.x - min.x + 1; }
	int height() const { return max.y - min.y + 1; }
	Vec2i size() const { return Vec2i(width(), height()); }

	int top() const { return min.y; }
	int bottom() const { return max.y; }
	int left() const { return min.x; }
	int right() const { return max.x; }

	Vec2i top_left() const { return Vec2i(min.x, min.y); }
	Vec2i top_right() const { return Vec2i(max.x, min.y); }
	Vec2i bottom_left() const { return Vec2i(min.x, max.y); }
	Vec2i bottom_right() const { return Vec2i(max.x, max.y); }

	void set_top(int v) { min.y = v; }
	void set_bottom(int v) { max.y = v; }
	void set_left(int v) { min.x = v; }
	void set_right(int v) { max.x = v; }

	void set_top_left(const Vec2i &v) { min.x = v.x; min.y = v.y; }
	void set_top_right(const Vec2i &v) { max.x = v.x; min.y = v.y; }
	void set_bottom_left(const Vec2i &v) { min.x = v.x; max.y = v.y; }
	void set_bottom_right(const Vec2i &v) { max.x = v.x; max.y = v.y; }

	bool valid() const { return min <= max; }
};

static inline bool operator==(const Rect &l, const Rect &r) { return l.min == r.min && l.max == r.max; }
static inline bool operator!=(const Rect &l, const Rect &r) { return l.min != r.min || l.max != r.max; }

static inline Rect Rect_WH(const Vec2i &p, const Vec2i &size) { return Rect(p, p+size-Vec2i(1)); }
static inline Rect Rect_WH(int x, int y, int w, int h) { return Rect_WH(Vec2i(x, y), Vec2i(w, h)); }
static inline Rect Rect_Intersection(const Rect &r1, const Rect &r2) { return Rect(max(r1.min, r2.min), min(r1.max, r2.max)); }
static inline Rect Rect_Valid(const Rect &r)
{
	if (!r.valid())
		return Rect(r.min, r.min);
	return r;
}

static inline bool contains(const Rect &r, const Vec2i &p) { return r.min <= p && p <= r.max; }
static inline bool intersects(const Rect &r1, const Rect &r2)
{
	return !(
		r1.max.x < r2.min.x ||
		r1.max.y < r2.min.y ||
		r1.min.x > r2.max.x ||
		r1.min.y > r2.max.y
	);
}
static inline bool contains(const Rect &r1, const Rect &r2)
{
	return Rect_Intersection(r1, r2) == r2;
}

#define RECT(r) (r).min.x, (r).min.y, (r).max.x, (r).max.y
