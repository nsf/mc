#include "Math/Noise.h"
#include <random>

static inline float lerp(float a, float b, float v)
{
	return a * (1 - v) + b * v;
}

static inline float Smooth(float v)
{
	return v * v * (3 - 2 * v);
}

static inline float Gradient(const Vec3f &orig, const Vec3f &grad, const Vec3f &p)
{
	return dot(grad, p - orig);
}

static inline float Gradient(const Vec2f &orig, const Vec2f &grad, const Vec2f &p)
{
	return dot(grad, p - orig);
}

template <typename Rnd>
static inline Vec3f RandomGradient3D(Rnd &g)
{
	std::uniform_real_distribution<float> _2pi(0.0f, MATH_PI * 2.0f);
	const float x = _2pi(g);
	const float angle = _2pi(g);
	return {cosf(angle) * cosf(x), sinf(angle) * cosf(x), sinf(x)};
}

template <typename Rnd>
static inline Vec2f RandomGradient2D(Rnd &g)
{
	std::uniform_real_distribution<float> _2pi(0.0f, MATH_PI * 2.0f);
	const float angle = _2pi(g);
	return {cosf(angle), sinf(angle)};
}

Noise3D::Noise3D(int seed)
{
	std::default_random_engine rnd(seed);
	for (auto &g : m_gradients) {
		g = RandomGradient3D(rnd);
	}

	for (int i = 0; i < 256; i++) {
		int j = std::uniform_int_distribution<int>(0, i)(rnd);
		m_permutations[i] = m_permutations[j];
		m_permutations[j] = i;
	}
}

Vec3f Noise3D::get_gradient(int x, int y, int z) const
{
	int idx =
		m_permutations[x & 255] +
		m_permutations[y & 255] +
		m_permutations[z & 255];
	return m_gradients[idx & 255];
}

void Noise3D::get_gradients(Vec3f *origins, Vec3f *grads,
	float x, float y, float z) const
{
	float x0f = std::floor(x);
	float y0f = std::floor(y);
	float z0f = std::floor(z);
	int x0 = x0f;
	int y0 = y0f;
	int z0 = z0f;
	int x1 = x0 + 1;
	int y1 = y0 + 1;
	int z1 = z0 + 1;

	grads[0] = get_gradient(x0, y0, z0);
	grads[1] = get_gradient(x0, y0, z1);
	grads[2] = get_gradient(x0, y1, z0);
	grads[3] = get_gradient(x0, y1, z1);
	grads[4] = get_gradient(x1, y0, z0);
	grads[5] = get_gradient(x1, y0, z1);
	grads[6] = get_gradient(x1, y1, z0);
	grads[7] = get_gradient(x1, y1, z1);

	origins[0] = {x0f + 0.0f, y0f + 0.0f, z0f + 0.0f};
	origins[1] = {x0f + 0.0f, y0f + 0.0f, z0f + 1.0f};
	origins[2] = {x0f + 0.0f, y0f + 1.0f, z0f + 0.0f};
	origins[3] = {x0f + 0.0f, y0f + 1.0f, z0f + 1.0f};
	origins[4] = {x0f + 1.0f, y0f + 0.0f, z0f + 0.0f};
	origins[5] = {x0f + 1.0f, y0f + 0.0f, z0f + 1.0f};
	origins[6] = {x0f + 1.0f, y0f + 1.0f, z0f + 0.0f};
	origins[7] = {x0f + 1.0f, y0f + 1.0f, z0f + 1.0f};
}

float Noise3D::get(float x, float y, float z) const
{
	Vec3f origins[8];
	Vec3f grads[8];

	get_gradients(origins, grads, x, y, z);
	float vals[] = {
		Gradient(origins[0], grads[0], {x, y, z}),
		Gradient(origins[1], grads[1], {x, y, z}),
		Gradient(origins[2], grads[2], {x, y, z}),
		Gradient(origins[3], grads[3], {x, y, z}),
		Gradient(origins[4], grads[4], {x, y, z}),
		Gradient(origins[5], grads[5], {x, y, z}),
		Gradient(origins[6], grads[6], {x, y, z}),
		Gradient(origins[7], grads[7], {x, y, z}),
	};

	float fz = Smooth(z - origins[0].z);
	float vz0 = lerp(vals[0], vals[1], fz);
	float vz1 = lerp(vals[2], vals[3], fz);
	float vz2 = lerp(vals[4], vals[5], fz);
	float vz3 = lerp(vals[6], vals[7], fz);

	float fy = Smooth(y - origins[0].y);
	float vy0 = lerp(vz0, vz1, fy);
	float vy1 = lerp(vz2, vz3, fy);

	float fx = Smooth(x - origins[0].x);
	return lerp(vy0, vy1, fx);
}

Noise2D::Noise2D(int seed)
{
	std::default_random_engine rnd(seed);
	for (auto &g : m_gradients) {
		g = RandomGradient2D(rnd);
	}

	for (int i = 0; i < 256; i++) {
		int j = std::uniform_int_distribution<int>(0, i)(rnd);
		m_permutations[i] = m_permutations[j];
		m_permutations[j] = i;
	}
}

Vec2f Noise2D::get_gradient(int x, int y) const
{
	int idx =
		m_permutations[x & 255] +
		m_permutations[y & 255];
	return m_gradients[idx & 255];
}

void Noise2D::get_gradients(Vec2f *origins, Vec2f *grads, float x, float y) const
{
	float x0f = floorf(x);
	float y0f = floorf(y);
	int x0 = x0f;
	int y0 = y0f;
	int x1 = x0 + 1;
	int y1 = y0 + 1;

	grads[0] = get_gradient(x0, y0);
	grads[1] = get_gradient(x1, y0);
	grads[2] = get_gradient(x0, y1);
	grads[3] = get_gradient(x1, y1);

	origins[0] = {x0f + 0.0f, y0f + 0.0f};
	origins[1] = {x0f + 1.0f, y0f + 0.0f};
	origins[2] = {x0f + 0.0f, y0f + 1.0f};
	origins[3] = {x0f + 1.0f, y0f + 1.0f};
}

float Noise2D::get(float x, float y) const
{
	Vec2f origins[4];
	Vec2f grads[4];

	get_gradients(origins, grads, x, y);
	float vals[] = {
		Gradient(origins[0], grads[0], {x, y}),
		Gradient(origins[1], grads[1], {x, y}),
		Gradient(origins[2], grads[2], {x, y}),
		Gradient(origins[3], grads[3], {x, y}),
	};

	float fx = Smooth(x - origins[0].x);
	float vx0 = lerp(vals[0], vals[1], fx);
	float vx1 = lerp(vals[2], vals[3], fx);
	float fy = Smooth(y - origins[0].y);
	return lerp(vx0, vx1, fy);
}
