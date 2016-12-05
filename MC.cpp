/***********************************************************
 * A Template for building OpenGL applications using GLUT
 *
 * Author: Perspective @ cprogramming.com
 * Date  : Jan, 2005
 *
 * Description:
 *   This code initializes an OpenGL ready window
 * using GLUT.  Some of the most common callbacks
 * are registered to empty or minimal functions.
 *
 * This code is intended to be a quick starting point
 * when building GLUT applications.
 *
 ***********************************************************/

#include <cstdio>
#include <cstdint>
#include "Math/Transform.h"
#include "Math/Noise.h"
#include "Core/Utils.h"
#include "Core/Vector.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
    #include <GLUT/glut.h>
#else
    #include <GL/glut.h>
#endif


//----------------------------------------------------------------------------
// Geometry
//----------------------------------------------------------------------------

struct Vertex {
	Vec3f position;
	Vec3f normal;
};

static Vector<float> voxels(65*65*65);
static Vector<Vertex> vertices;
static Vector<int> indices;

static inline int offset_3d(const Vec3i &p, const Vec3i &size)
{
	return (p.z * size.y + p.y) * size.x + p.x;
}

static inline int offset_3d_slab(const Vec3i &p, const Vec3i &size)
{
	return size.x * size.y * (p.z % 2) + p.y * size.x + p.x;
}

static void generate_voxels()
{
	Noise2D n2d(0);
	for (int z = 0; z < 65; z++) {
	for (int y = 0; y < 65; y++) {
	for (int x = 0; x < 65; x++) {
		const float fy = (float)y / 65.0f;
		const int offset = offset_3d({x, y, z}, Vec3i(65));
		const float v = n2d.get(x / 16.0f, z / 16.0f) * 0.25f;
		voxels[offset] = fy - 0.25f - v;
	}}}
}

static const uint64_t marching_cube_tris[256] = {
	0ULL, 33793ULL, 36945ULL, 159668546ULL,
	18961ULL, 144771090ULL, 5851666ULL, 595283255635ULL,
	20913ULL, 67640146ULL, 193993474ULL, 655980856339ULL,
	88782242ULL, 736732689667ULL, 797430812739ULL, 194554754ULL,
	26657ULL, 104867330ULL, 136709522ULL, 298069416227ULL,
	109224258ULL, 8877909667ULL, 318136408323ULL, 1567994331701604ULL,
	189884450ULL, 350847647843ULL, 559958167731ULL, 3256298596865604ULL,
	447393122899ULL, 651646838401572ULL, 2538311371089956ULL, 737032694307ULL,
	29329ULL, 43484162ULL, 91358498ULL, 374810899075ULL,
	158485010ULL, 178117478419ULL, 88675058979ULL, 433581536604804ULL,
	158486962ULL, 649105605635ULL, 4866906995ULL, 3220959471609924ULL,
	649165714851ULL, 3184943915608436ULL, 570691368417972ULL, 595804498035ULL,
	124295042ULL, 431498018963ULL, 508238522371ULL, 91518530ULL,
	318240155763ULL, 291789778348404ULL, 1830001131721892ULL, 375363605923ULL,
	777781811075ULL, 1136111028516116ULL, 3097834205243396ULL, 508001629971ULL,
	2663607373704004ULL, 680242583802939237ULL, 333380770766129845ULL, 179746658ULL,
	42545ULL, 138437538ULL, 93365810ULL, 713842853011ULL,
	73602098ULL, 69575510115ULL, 23964357683ULL, 868078761575828ULL,
	28681778ULL, 713778574611ULL, 250912709379ULL, 2323825233181284ULL,
	302080811955ULL, 3184439127991172ULL, 1694042660682596ULL, 796909779811ULL,
	176306722ULL, 150327278147ULL, 619854856867ULL, 1005252473234484ULL,
	211025400963ULL, 36712706ULL, 360743481544788ULL, 150627258963ULL,
	117482600995ULL, 1024968212107700ULL, 2535169275963444ULL, 4734473194086550421ULL,
	628107696687956ULL, 9399128243ULL, 5198438490361643573ULL, 194220594ULL,
	104474994ULL, 566996932387ULL, 427920028243ULL, 2014821863433780ULL,
	492093858627ULL, 147361150235284ULL, 2005882975110676ULL, 9671606099636618005ULL,
	777701008947ULL, 3185463219618820ULL, 482784926917540ULL, 2900953068249785909ULL,
	1754182023747364ULL, 4274848857537943333ULL, 13198752741767688709ULL, 2015093490989156ULL,
	591272318771ULL, 2659758091419812ULL, 1531044293118596ULL, 298306479155ULL,
	408509245114388ULL, 210504348563ULL, 9248164405801223541ULL, 91321106ULL,
	2660352816454484ULL, 680170263324308757ULL, 8333659837799955077ULL, 482966828984116ULL,
	4274926723105633605ULL, 3184439197724820ULL, 192104450ULL, 15217ULL,
	45937ULL, 129205250ULL, 129208402ULL, 529245952323ULL,
	169097138ULL, 770695537027ULL, 382310500883ULL, 2838550742137652ULL,
	122763026ULL, 277045793139ULL, 81608128403ULL, 1991870397907988ULL,
	362778151475ULL, 2059003085103236ULL, 2132572377842852ULL, 655681091891ULL,
	58419234ULL, 239280858627ULL, 529092143139ULL, 1568257451898804ULL,
	447235128115ULL, 679678845236084ULL, 2167161349491220ULL, 1554184567314086709ULL,
	165479003923ULL, 1428768988226596ULL, 977710670185060ULL, 10550024711307499077ULL,
	1305410032576132ULL, 11779770265620358997ULL, 333446212255967269ULL, 978168444447012ULL,
	162736434ULL, 35596216627ULL, 138295313843ULL, 891861543990356ULL,
	692616541075ULL, 3151866750863876ULL, 100103641866564ULL, 6572336607016932133ULL,
	215036012883ULL, 726936420696196ULL, 52433666ULL, 82160664963ULL,
	2588613720361524ULL, 5802089162353039525ULL, 214799000387ULL, 144876322ULL,
	668013605731ULL, 110616894681956ULL, 1601657732871812ULL, 430945547955ULL,
	3156382366321172ULL, 7644494644932993285ULL, 3928124806469601813ULL, 3155990846772900ULL,
	339991010498708ULL, 10743689387941597493ULL, 5103845475ULL, 105070898ULL,
	3928064910068824213ULL, 156265010ULL, 1305138421793636ULL, 27185ULL,
	195459938ULL, 567044449971ULL, 382447549283ULL, 2175279159592324ULL,
	443529919251ULL, 195059004769796ULL, 2165424908404116ULL, 1554158691063110021ULL,
	504228368803ULL, 1436350466655236ULL, 27584723588724ULL, 1900945754488837749ULL,
	122971970ULL, 443829749251ULL, 302601798803ULL, 108558722ULL,
	724700725875ULL, 43570095105972ULL, 2295263717447940ULL, 2860446751369014181ULL,
	2165106202149444ULL, 69275726195ULL, 2860543885641537797ULL, 2165106320445780ULL,
	2280890014640004ULL, 11820349930268368933ULL, 8721082628082003989ULL, 127050770ULL,
	503707084675ULL, 122834978ULL, 2538193642857604ULL, 10129ULL,
	801441490467ULL, 2923200302876740ULL, 1443359556281892ULL, 2901063790822564949ULL,
	2728339631923524ULL, 7103874718248233397ULL, 12775311047932294245ULL, 95520290ULL,
	2623783208098404ULL, 1900908618382410757ULL, 137742672547ULL, 2323440239468964ULL,
	362478212387ULL, 727199575803140ULL, 73425410ULL, 34337ULL,
	163101314ULL, 668566030659ULL, 801204361987ULL, 73030562ULL,
	591509145619ULL, 162574594ULL, 100608342969108ULL, 5553ULL,
	724147968595ULL, 1436604830452292ULL, 176259090ULL, 42001ULL,
	143955266ULL, 2385ULL, 18433ULL, 0ULL,
};

static void triangle(int a, int b, int c)
{
	Vertex &va = vertices[a];
	Vertex &vb = vertices[b];
	Vertex &vc = vertices[c];
	const Vec3f ab = va.position - vb.position;
	const Vec3f cb = vc.position - vb.position;
	const Vec3f n = cross(cb, ab);
	va.normal += n;
	vb.normal += n;
	vc.normal += n;
}

static void generate_geometry()
{
	for (int z = 0; z < 64; z++) {
	for (int y = 0; y < 64; y++) {
	for (int x = 0; x < 64; x++) {
		const float vs[8] = {
			voxels[offset_3d({x,   y,   z},   Vec3i(65))],
			voxels[offset_3d({x+1, y,   z},   Vec3i(65))],
			voxels[offset_3d({x,   y+1, z},   Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z},   Vec3i(65))],
			voxels[offset_3d({x,   y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x,   y+1, z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z+1}, Vec3i(65))],
		};

		const int config_n =
			((vs[0] < 0.0f) << 0) |
			((vs[1] < 0.0f) << 1) |
			((vs[2] < 0.0f) << 2) |
			((vs[3] < 0.0f) << 3) |
			((vs[4] < 0.0f) << 4) |
			((vs[5] < 0.0f) << 5) |
			((vs[6] < 0.0f) << 6) |
			((vs[7] < 0.0f) << 7);

		if (config_n == 0 || config_n == 255)
			continue;

		int edge_indices[12];
		auto do_edge = [&](int n_edge, float va, float vb, int axis, const Vec3f &base) {
			if ((va < 0.0) == (vb < 0.0))
				return;

			Vec3f v = base;
			v[axis] += va / (va - vb);
			edge_indices[n_edge] = vertices.length();
			vertices.append({v, Vec3f(0)});
		};

		do_edge(0,  vs[0], vs[1], 0, Vec3f(x, y,   z));
		do_edge(1,  vs[2], vs[3], 0, Vec3f(x, y+1, z));
		do_edge(2,  vs[4], vs[5], 0, Vec3f(x, y,   z+1));
		do_edge(3,  vs[6], vs[7], 0, Vec3f(x, y+1, z+1));

		do_edge(4,  vs[0], vs[2], 1, Vec3f(x,   y, z));
		do_edge(5,  vs[1], vs[3], 1, Vec3f(x+1, y, z));
		do_edge(6,  vs[4], vs[6], 1, Vec3f(x,   y, z+1));
		do_edge(7,  vs[5], vs[7], 1, Vec3f(x+1, y, z+1));

		do_edge(8,  vs[0], vs[4], 2, Vec3f(x,   y,   z));
		do_edge(9,  vs[1], vs[5], 2, Vec3f(x+1, y,   z));
		do_edge(10, vs[2], vs[6], 2, Vec3f(x,   y+1, z));
		do_edge(11, vs[3], vs[7], 2, Vec3f(x+1, y+1, z));

		const uint64_t config = marching_cube_tris[config_n];
		const int n_triangles = config & 0xF;
		const int n_indices = n_triangles * 3;
		const int index_base = indices.length();

		int offset = 4;
		for (int i = 0; i < n_indices; i++) {
			const int edge = (config >> offset) & 0xF;
			indices.append(edge_indices[edge]);
			offset += 4;
		}
		for (int i = 0; i < n_triangles; i++) {
			triangle(
				indices[index_base+i*3+0],
				indices[index_base+i*3+1],
				indices[index_base+i*3+2]);
		}
	}}}
	for (Vertex &v : vertices)
		v.normal = normalize(v.normal);
}


static void generate_geometry_smooth()
{
	static Vector<Vec3i> slab_inds(65*65*2);

	for (int z = 0; z < 64; z++) {
	for (int y = 0; y < 64; y++) {
	for (int x = 0; x < 64; x++) {
		const Vec3i p(x, y, z);
		const float vs[8] = {
			voxels[offset_3d({x,   y,   z},   Vec3i(65))],
			voxels[offset_3d({x+1, y,   z},   Vec3i(65))],
			voxels[offset_3d({x,   y+1, z},   Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z},   Vec3i(65))],
			voxels[offset_3d({x,   y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x,   y+1, z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z+1}, Vec3i(65))],
		};

		const int config_n =
			((vs[0] < 0.0f) << 0) |
			((vs[1] < 0.0f) << 1) |
			((vs[2] < 0.0f) << 2) |
			((vs[3] < 0.0f) << 3) |
			((vs[4] < 0.0f) << 4) |
			((vs[5] < 0.0f) << 5) |
			((vs[6] < 0.0f) << 6) |
			((vs[7] < 0.0f) << 7);

		if (config_n == 0 || config_n == 255)
			continue;

		auto do_edge = [&](int n_edge, float va, float vb, int axis, const Vec3i &p) {
			if ((va < 0.0) == (vb < 0.0))
				return;

			Vec3f v = ToVec3f(p);
			v[axis] += va / (va - vb);
			slab_inds[offset_3d_slab(p, Vec3i(65))][axis] = vertices.length();
			vertices.append({v, Vec3f(0)});
		};

		if (p.y == 0 && p.z == 0)
			do_edge(0,  vs[0], vs[1], 0, Vec3i(x, y,   z));
		if (p.z == 0)
			do_edge(1,  vs[2], vs[3], 0, Vec3i(x, y+1, z));
		if (p.y == 0)
			do_edge(2,  vs[4], vs[5], 0, Vec3i(x, y,   z+1));
		do_edge(3,  vs[6], vs[7], 0, Vec3i(x, y+1, z+1));

		if (p.x == 0 && p.z == 0)
			do_edge(4,  vs[0], vs[2], 1, Vec3i(x,   y, z));
		if (p.z == 0)
			do_edge(5,  vs[1], vs[3], 1, Vec3i(x+1, y, z));
		if (p.x == 0)
			do_edge(6,  vs[4], vs[6], 1, Vec3i(x,   y, z+1));
		do_edge(7,  vs[5], vs[7], 1, Vec3i(x+1, y, z+1));

		if (p.x == 0 && p.y == 0)
			do_edge(8,  vs[0], vs[4], 2, Vec3i(x,   y,   z));
		if (p.y == 0)
			do_edge(9,  vs[1], vs[5], 2, Vec3i(x+1, y,   z));
		if (p.x == 0)
			do_edge(10, vs[2], vs[6], 2, Vec3i(x,   y+1, z));
		do_edge(11, vs[3], vs[7], 2, Vec3i(x+1, y+1, z));

		int edge_indices[12];
		edge_indices[0]  = slab_inds[offset_3d_slab({p.x, p.y,   p.z  }, Vec3i(65))].x;
		edge_indices[1]  = slab_inds[offset_3d_slab({p.x, p.y+1, p.z  }, Vec3i(65))].x;
		edge_indices[2]  = slab_inds[offset_3d_slab({p.x, p.y,   p.z+1}, Vec3i(65))].x;
		edge_indices[3]  = slab_inds[offset_3d_slab({p.x, p.y+1, p.z+1}, Vec3i(65))].x;
		edge_indices[4]  = slab_inds[offset_3d_slab({p.x,   p.y, p.z  }, Vec3i(65))].y;
		edge_indices[5]  = slab_inds[offset_3d_slab({p.x+1, p.y, p.z  }, Vec3i(65))].y;
		edge_indices[6]  = slab_inds[offset_3d_slab({p.x,   p.y, p.z+1}, Vec3i(65))].y;
		edge_indices[7]  = slab_inds[offset_3d_slab({p.x+1, p.y, p.z+1}, Vec3i(65))].y;
		edge_indices[8]  = slab_inds[offset_3d_slab({p.x,   p.y,   p.z}, Vec3i(65))].z;
		edge_indices[9]  = slab_inds[offset_3d_slab({p.x+1, p.y,   p.z}, Vec3i(65))].z;
		edge_indices[10] = slab_inds[offset_3d_slab({p.x,   p.y+1, p.z}, Vec3i(65))].z;
		edge_indices[11] = slab_inds[offset_3d_slab({p.x+1, p.y+1, p.z}, Vec3i(65))].z;

		const uint64_t config = marching_cube_tris[config_n];
		const int n_triangles = config & 0xF;
		const int n_indices = n_triangles * 3;
		const int index_base = indices.length();

		int offset = 4;
		for (int i = 0; i < n_indices; i++) {
			const int edge = (config >> offset) & 0xF;
			indices.append(edge_indices[edge]);
			offset += 4;
		}
		for (int i = 0; i < n_triangles; i++) {
			triangle(
				indices[index_base+i*3+0],
				indices[index_base+i*3+1],
				indices[index_base+i*3+2]);
		}
	}}}
	for (Vertex &v : vertices)
		v.normal = normalize(v.normal);
}

static void quad(bool flip, int ia, int ib, int ic, int id)
{
	if (flip)
		std::swap(ib, id);

	Vertex &va = vertices[ia];
	Vertex &vb = vertices[ib];
	Vertex &vc = vertices[ic];
	Vertex &vd = vertices[id];

	const Vec3f ab = va.position - vb.position;
	const Vec3f cb = vc.position - vb.position;
	const Vec3f n1 = cross(cb, ab);
	va.normal += n1;
	vb.normal += n1;
	vc.normal += n1;

	const Vec3f ac = va.position - vc.position;
	const Vec3f dc = vd.position - vc.position;
	const Vec3f n2 = cross(dc, ac);
	va.normal += n2;
	vc.normal += n2;
	vd.normal += n2;

	indices.append(ia);
	indices.append(ib);
	indices.append(ic);

	indices.append(ia);
	indices.append(ic);
	indices.append(id);
}


static void generate_geometry_naive_surface_nets()
{
	static Vector<int> inds(65*65*2);

	for (int z = 0; z < 64; z++) {
	for (int y = 0; y < 64; y++) {
	for (int x = 0; x < 64; x++) {
		const Vec3i p(x, y, z);
		const float vs[8] = {
			voxels[offset_3d({x,   y,   z},   Vec3i(65))],
			voxels[offset_3d({x+1, y,   z},   Vec3i(65))],
			voxels[offset_3d({x,   y+1, z},   Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z},   Vec3i(65))],
			voxels[offset_3d({x,   y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y,   z+1}, Vec3i(65))],
			voxels[offset_3d({x,   y+1, z+1}, Vec3i(65))],
			voxels[offset_3d({x+1, y+1, z+1}, Vec3i(65))],
		};

		const int config_n =
			((vs[0] < 0.0f) << 0) |
			((vs[1] < 0.0f) << 1) |
			((vs[2] < 0.0f) << 2) |
			((vs[3] < 0.0f) << 3) |
			((vs[4] < 0.0f) << 4) |
			((vs[5] < 0.0f) << 5) |
			((vs[6] < 0.0f) << 6) |
			((vs[7] < 0.0f) << 7);

		if (config_n == 0 || config_n == 255)
			continue;

		Vec3f average(0);
		int average_n = 0;
		auto do_edge = [&](float va, float vb, int axis, const Vec3i &p) {
			if ((va < 0.0) == (vb < 0.0))
				return;

			Vec3f v = ToVec3f(p);
			v[axis] += va / (va - vb);
			average += v;
			average_n++;
		};

		do_edge(vs[0], vs[1], 0, Vec3i(x, y,     z));
		do_edge(vs[2], vs[3], 0, Vec3i(x, y+1,   z));
		do_edge(vs[4], vs[5], 0, Vec3i(x, y,     z+1));
		do_edge(vs[6], vs[7], 0, Vec3i(x, y+1,   z+1));
		do_edge(vs[0], vs[2], 1, Vec3i(x,   y,   z));
		do_edge(vs[1], vs[3], 1, Vec3i(x+1, y,   z));
		do_edge(vs[4], vs[6], 1, Vec3i(x,   y,   z+1));
		do_edge(vs[5], vs[7], 1, Vec3i(x+1, y,   z+1));
		do_edge(vs[0], vs[4], 2, Vec3i(x,   y,   z));
		do_edge(vs[1], vs[5], 2, Vec3i(x+1, y,   z));
		do_edge(vs[2], vs[6], 2, Vec3i(x,   y+1, z));
		do_edge(vs[3], vs[7], 2, Vec3i(x+1, y+1, z));

		const Vec3f v = average / Vec3f(average_n);
		inds[offset_3d_slab(p, Vec3i(65))] = vertices.length();
		vertices.append({v, Vec3f(0)});

		const bool flip = vs[0] < 0.0f;
		if (p.y > 0 && p.z > 0 && (vs[0] < 0.0f) != (vs[1] < 0.0f)) {
			quad(flip,
				inds[offset_3d_slab(Vec3i(p.x, p.y,   p.z),   Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x, p.y,   p.z-1), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x, p.y-1, p.z-1), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x, p.y-1, p.z),   Vec3i(65))]
			);
		}
		if (p.x > 0 && p.z > 0 && (vs[0] < 0.0f) != (vs[2] < 0.0f)) {
			quad(flip,
				inds[offset_3d_slab(Vec3i(p.x,   p.y, p.z),   Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x-1, p.y, p.z),   Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x-1, p.y, p.z-1), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x,   p.y, p.z-1), Vec3i(65))]
			);
		}
		if (p.x > 0 && p.y > 0 && (vs[0] < 0.0f) != (vs[4] < 0.0f)) {
			quad(flip,
				inds[offset_3d_slab(Vec3i(p.x,   p.y,   p.z), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x,   p.y-1, p.z), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x-1, p.y-1, p.z), Vec3i(65))],
				inds[offset_3d_slab(Vec3i(p.x-1, p.y,   p.z), Vec3i(65))]
			);
		}
	}}}
	for (Vertex &v : vertices)
		v.normal = normalize(v.normal);
}

//----------------------------------------------------------------------------
// Camera
//----------------------------------------------------------------------------

enum PlayerCameraState {
	PCS_MOVING_FORWARD  = 1 << 0,
	PCS_MOVING_BACKWARD = 1 << 1,
	PCS_MOVING_LEFT     = 1 << 2,
	PCS_MOVING_RIGHT    = 1 << 3,
};

static unsigned camera_state = 0;
static Transform camera(
	Vec3f(33.755638, 49.220379, 72.422722),
	Quat(-0.362434, 0.002032, 0.000791, 0.931997));
static bool rotating_camera = false;

static Quat mouse_rotate(const Quat &in, float x, float y, float sensitivity)
{
	const Quat xq(Vec3f_Y(), -x * sensitivity);
	const Quat yq(Vec3f_X(), -y * sensitivity);
	return xq * (in * yq);
}

static void get_camera_vectors(Vec3f *look_dir, Vec3f *up, Vec3f *right, const Quat &orient)
{
	NG_ASSERT(look_dir != nullptr);
	NG_ASSERT(up != nullptr);
	NG_ASSERT(right != nullptr);
	const Mat4 m = to_mat4(inverse(orient));
	*right    = { m[0],  m[4],  m[8]};
	*up       = { m[1],  m[5],  m[9]};
	*look_dir = {-m[2], -m[6], -m[10]};
}

static Vec3f get_walk_direction()
{
	constexpr float sincos_45 = 0.7071067f;

	Vec3f look_dir, up, right;
	get_camera_vectors(&look_dir, &up, &right, camera.orientation);

	float fb_move = 0.0f;
	float lr_move = 0.0f;
	if (camera_state & PCS_MOVING_FORWARD)
		fb_move += 1.0f;
	if (camera_state & PCS_MOVING_BACKWARD)
		fb_move -= 1.0f;
	if (camera_state & PCS_MOVING_LEFT)
		lr_move -= 1.0f;
	if (camera_state & PCS_MOVING_RIGHT)
		lr_move += 1.0f;

	if (camera_state & (PCS_MOVING_FORWARD | PCS_MOVING_BACKWARD) &&
		camera_state & (PCS_MOVING_LEFT | PCS_MOVING_RIGHT))
	{
		fb_move *= sincos_45;
		lr_move *= sincos_45;
	}

	return look_dir * Vec3f(fb_move) + right * Vec3f(lr_move);
}

//----------------------------------------------------------------------------
// GLUT
//----------------------------------------------------------------------------

static void keyboardDown(unsigned char key, int x, int y) {
	Vec3f look_dir, up, right;

	switch (key) {
	case 'Q':
	case 'q':
	case  27:
		exit(0);
		break;
	case 'w':
		camera_state |= PCS_MOVING_FORWARD;
		break;
	case 'a':
		camera_state |= PCS_MOVING_LEFT;
		break;
	case 's':
		camera_state |= PCS_MOVING_BACKWARD;
		break;
	case 'd':
		camera_state |= PCS_MOVING_RIGHT;
		break;
	case 'p':
		printf("pos: %f %f %f\n", VEC3(camera.translation));
		printf("orient %f %f %f %f\n", VEC4(camera.orientation));
		break;
	}
}

static void keyboardUp(unsigned char key, int x, int y) {
	switch (key) {
	case 'w':
		camera_state &= ~PCS_MOVING_FORWARD;
		break;
	case 'a':
		camera_state &= ~PCS_MOVING_LEFT;
		break;
	case 's':
		camera_state &= ~PCS_MOVING_BACKWARD;
		break;
	case 'd':
		camera_state &= ~PCS_MOVING_RIGHT;
		break;
	}
}

static void keyboardSpecialDown(int k, int x, int y) {
}

static void keyboardSpecialUp(int k, int x, int y) {
}

static void reshape(int width, int height)
{
	GLfloat fieldOfView = 90.0f;
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fieldOfView, (GLfloat)width/height, 0.1, 500.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

static void mouseClick(int button, int state, int x, int y)
{
	rotating_camera = state == GLUT_DOWN;
}

static void mouseMotion(int x, int y)
{
	static int last_x = 0, last_y = 0;
	const int dx = x - last_x;
	const int dy = y - last_y;
	last_x = x;
	last_y = y;

	if (rotating_camera)
		camera.orientation = mouse_rotate(camera.orientation, dx, dy, 0.25);
}

static void draw()
{
	static int last_time = 0;
	const int current_time = glutGet(GLUT_ELAPSED_TIME);
	const float delta = float(current_time - last_time) / 1000.0f;
	camera.translation += get_walk_direction() * Vec3f(delta) * Vec3f(0.05);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(to_mat4(inverse(camera)).data);

	// LIGHT SETUP HERE
	const Vec3f tmp = normalize(Vec3f(1, 1, 0));
	const Vec4f light_dir(tmp.x, tmp.y, tmp.z, 0);
	const Vec4f ambient(0.4, 0.4, 0.4, 1);
	const Vec4f diffuse(1, 1, 1, 1);
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient.data);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse.data);
	glLightfv(GL_LIGHT0, GL_POSITION, light_dir.data);

	// RENDER HERE
	glBegin(GL_TRIANGLES);
		for (int idx : indices) {
			const Vertex &v = vertices[idx];
			glNormal3fv(v.normal.data);
			glVertex3fv(v.position.data);
		}
	glEnd();

	glutSwapBuffers();
	glutPostRedisplay();
}

static void idle()
{
}

/* initialize OpenGL settings */
static void initGL(int width, int height)
{
	reshape(width, height);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0f);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}

static void menu(int choice)
{
	static bool wireframe = false;

	switch (choice) {
	case 'f':
		vertices.clear();
		indices.clear();
		generate_geometry();
		break;
	case 's':
		vertices.clear();
		indices.clear();
		generate_geometry_smooth();
		break;
	case 'n':
		vertices.clear();
		indices.clear();
		generate_geometry_naive_surface_nets();
		break;
	case 'w':
		if (!wireframe) {
			// enable
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		} else {
			// disable
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		wireframe = !wireframe;
		break;
	}
}

int main(int argc, char** argv)
{
	generate_voxels();
	generate_geometry();

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 600);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Perspective's GLUT Template");

	// register glut call backs
	glutKeyboardFunc(keyboardDown);
	glutKeyboardUpFunc(keyboardUp);
	glutSpecialFunc(keyboardSpecialDown);
	glutSpecialUpFunc(keyboardSpecialUp);
	glutMouseFunc(mouseClick);
	glutMotionFunc(mouseMotion);
	glutPassiveMotionFunc(mouseMotion);
	glutReshapeFunc(reshape);
	glutDisplayFunc(draw);
	glutIdleFunc(idle);
	glutIgnoreKeyRepeat(true); // ignore keys held down

	glutCreateMenu(menu);
	glutAddMenuEntry("Marching Cubes (flat shading)", 'f');
	glutAddMenuEntry("Marching Cubes (smooth shading)", 's');
	glutAddMenuEntry("Naive Surface Nets (smooth shading)", 'n');
	glutAddMenuEntry("Toggle Wireframe", 'w');
	glutAttachMenu(GLUT_RIGHT_BUTTON);

	initGL(800, 600);

	glutMainLoop();
	return 0;
}
