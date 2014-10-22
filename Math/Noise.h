#pragma once

#include "Math/Vec.h"

struct Noise3D {
	Vec3f m_gradients[256];
	int  m_permutations[256];

	explicit Noise3D(int seed);
	Vec3f get_gradient(int x, int y, int z) const;
	void get_gradients(Vec3f *origins, Vec3f *grads,
		float x, float y, float z) const;

	float get(float x, float y, float z) const;
};

struct Noise2D {
	Vec2f m_gradients[256];
	int  m_permutations[256];

	explicit Noise2D(int seed);
	Vec2f get_gradient(int x, int y) const;
	void get_gradients(Vec2f *origins, Vec2f *grads, float x, float y) const;
	float get(float x, float y) const;
};
