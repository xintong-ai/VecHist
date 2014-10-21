#define _USE_MATH_DEFINES
//using namespace std;
#include <math.h>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <list>
#include <numeric>
#include <random>
#include <vector>
#include <fstream>

namespace dstat {

// constants
const float kEps = 0.000001f;
unsigned gSeed = std::chrono::system_clock::now().time_since_epoch().count();
std::default_random_engine gGenerator(gSeed);
std::uniform_real_distribution<float> gDistribution(0.0f, 1.0f);

// naming
void sl_name(float x, float y, float z, char* name) {
  sprintf(name, "%d_%d_%d.off", static_cast<int>(x), static_cast<int>(y),
          static_cast<int>(z));
}

// vec
void copy(const float* v1, float* v2) {
  v2[0] = v1[0]; v2[1] = v1[1]; v2[2] = v1[2];
}

void negate(const float* v, float* nv) {
  nv[0] = -v[0]; nv[1] = -v[1]; nv[2] = -v[2];
}

void add(const float* v1, const float* v2, float* v) {
  v[0] = v1[0] + v2[0]; v[1] = v1[1] + v2[1]; v[2] = v1[2] + v2[2];
}

void addequal(float* v1, const float* v2) {
  v1[0] += v2[0]; v1[1] += v2[1]; v1[2] += v2[2];
}

void sub(const float* v1, const float* v2, float* v) {
  v[0] = v1[0] - v2[0]; v[1] = v1[1] - v2[1]; v[2] = v1[2] - v2[2];
}

void multi(float* v, float s) {
  v[0] *= s; v[1] *= s; v[2] *= s;
}

float mag(const float* v) {
  return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
}

void normalize(float* v) {
  float mg = mag(v);
  if (mg > kEps) {
    v[0] /= mg; v[1] /= mg; v[2] /= mg;
  }
}

float dot(const float* v1, const float* v2) {
  return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

void cross(const float* v1, const float* v2, float* v) {
  v[0] = v1[1] * v2[2] - v1[2] * v2[1];
  v[1] = v1[2] * v2[0] - v1[0] * v2[2];
  v[2] = v1[0] * v2[1] - v1[1] * v2[0];
}

// coordinates
void stc(float theta, float phi, float* v) {
  v[0] = sin(theta) * cos(phi);
  v[1] = sin(theta) * sin(phi);
  v[2] = cos(theta);
}

void cts(const float* v, float* theta, float* phi) {
  *theta = acos(v[2]);
  *phi = atan2(v[1], v[0]);
}

// rand
float randf(float l, float h) {
  return l + static_cast<float>(rand()) / static_cast<float>(RAND_MAX) *
         (h - l);
}

// interpolation
int ttoo(const int* dims, int i, int j, int k) {
  return k * dims[0] * dims[1] + j * dims[0] + i;
}

int ttoo(const int* dims, const int* i3) {
  return i3[2] * dims[0] * dims[1] + i3[1] * dims[0] + i3[0];
}

void get_vec(const float* data, const int* dims,
             int i, int j, int k, float* v) {
  int i1 = ttoo(dims, i, j, k) * 3;
  v[0] = data[i1]; v[1] = data[i1 + 1]; v[2] = data[i1 + 2];
}

void get_vec(const float* data, const int* dims, const int* i3, float* v) {
  int i1 = ttoo(dims, i3) * 3;
  v[0] = data[i1]; v[1] = data[i1 + 1]; v[2] = data[i1 + 2];
}

void interpolate(float* v0, float* v1, float* v2, float* v3,
                 float* v4, float* v5, float* v6, float* v7,
                 const float* t, float* v) {
  float delta[3];

  sub(v4, v0, delta); multi(delta, t[0]); addequal(v0, delta);
  sub(v5, v1, delta); multi(delta, t[0]); addequal(v1, delta);
  sub(v6, v2, delta); multi(delta, t[0]); addequal(v2, delta);
  sub(v7, v3, delta); multi(delta, t[0]); addequal(v3, delta);

  sub(v2, v0, delta); multi(delta, t[1]); addequal(v0, delta);
  sub(v3, v1, delta); multi(delta, t[1]); addequal(v1, delta);

  sub(v1, v0, delta); multi(delta, t[2]); addequal(v0, delta);

  copy(v0, v);
}

void at_phys(const float* data, const int* dims, const float* pos, float* v) {
  int fx = floor(pos[0]), fy = floor(pos[1]), fz = floor(pos[2]);
  int cx = ceil(pos[0]), cy = ceil(pos[1]), cz = ceil(pos[2]);
  float v0[3], v1[3], v2[3], v3[3], v4[3], v5[3], v6[3], v7[3];
  get_vec(data, dims, fx, fy, fz, v0);
  get_vec(data, dims, fx, fy, cz, v1);
  get_vec(data, dims, fx, cy, fz, v2);
  get_vec(data, dims, fx, cy, cz, v3);
  get_vec(data, dims, cx, fy, fz, v4);
  get_vec(data, dims, cx, fy, cz, v5);
  get_vec(data, dims, cx, cy, fz, v6);
  get_vec(data, dims, cx, cy, cz, v7);
  float t[3]; t[0] = pos[0] - fx; t[1] = pos[1] - fy; t[2] = pos[2] - fz;
  interpolate(v0, v1, v2, v3, v4, v5, v6, v7, t, v);
}

// geodesic
bool rt_intersects(
    const float* d, const float* v0, const float* v1, const float* v2) {
  float e1[3], e2[3], h[3], s[3], q[3];
  float a, f, u, v, t;
  sub(v1, v0, e1);
  sub(v2, v0, e2);

  cross(d, e2, h);
  a = dot(e1, h);

  if (a > -kEps && a < kEps)
    return false;

  f = 1.0f / a;
  negate(v0, s);
  u = f * dot(s, h);

  if (u < -kEps || u > 1.0f + kEps)
    return false;

  cross(s, e1, q);
  v = f * dot(d, q);

  if (v < -kEps || u + v > 1.0f + kEps)
    return false;

  // at this stage we can compute t to find out where
  // the intersection point is on the line
  t = f * dot(e2, q);

  if (t > 0.0f)  // ray intersection
    return true;
  else  // this means that there is a line intersection,
        // but not a ray intersection
    return false;
}

void gen_spgrid(int dn, std::vector<int>* bn, std::vector<float*>* tris, 
	std::vector<std::vector<double> > &vV, std::vector<std::vector<int> > &vF) {
  for (int i = 0; i != dn; ++i) {
    bn->push_back(static_cast<int>(20 * pow(4.0f, i)));
    tris->push_back(new float[(*bn)[i] * 3 * 3]);
  }

  // level 0
  float* tris_l0 = (*tris)[0];
  const double kPhi = 2.0 * cos(M_PI / 5.0);
  double mg = sqrt(1.0 * 1.0 + kPhi * kPhi);

  using namespace std;

  std::vector<double> v{ -kPhi / mg, 1.0f / mg, 0.0f };
  vV.push_back(vector<double>{ -kPhi / mg, 1.0f / mg, 0.0f });	//0
  vV.push_back(vector<double>{ 0.0f, -kPhi / mg, -1.0f / mg });	//1
  vV.push_back(vector<double>{kPhi / mg, -1.0f / mg, 0.0f});	//2
  vV.push_back(vector<double>{1.0f / mg,	0.0f, kPhi / mg});	//3
  vV.push_back(vector<double>{ 0.0f, -kPhi / mg, 1.0f / mg});	//4
  vV.push_back(vector<double>{ -kPhi / mg, -1.0f / mg, 0.0f});	//5
  vV.push_back(vector<double>{ 1.0f / mg, 0.0f, -kPhi / mg});	//6
  vV.push_back(vector<double>{ kPhi / mg, 1.0f / mg, 0.0f});	//7
  vV.push_back(vector<double>{ -1.0f / mg, 0.0f, kPhi / mg});	//8
  vV.push_back(vector<double>{ 0.0f, kPhi / mg, 1.0f / mg});	//9
  vV.push_back(vector<double>{ 0.0f, kPhi / mg, -1.0f / mg});	//10
  vV.push_back(vector<double>{ -1.0f / mg, 0.0f, -kPhi / mg});	//11


  tris_l0[0] = 0.0f; tris_l0[1] = -kPhi / mg; tris_l0[2] = 1.0f / mg;		//4
  tris_l0[3] = 0.0f; tris_l0[4] = -kPhi / mg; tris_l0[5] = -1.0f / mg;		//1
  tris_l0[6] = kPhi / mg; tris_l0[7] = -1.0f / mg; tris_l0[8] = 0.0f;		//2
  vF.push_back(vector<int>{ 4, 1, 2});

  tris_l0[9] = 1.0f / mg; tris_l0[10] = 0.0f; tris_l0[11] = kPhi / mg;		//3
  tris_l0[12] = 0.0f; tris_l0[13] = -kPhi / mg; tris_l0[14] = 1.0f / mg;	//4
  tris_l0[15] = kPhi / mg; tris_l0[16] = -1.0f / mg; tris_l0[17] = 0.0f;	//2
  vF.push_back(vector<int>{ 3, 4, 2});

  tris_l0[18] = kPhi / mg; tris_l0[19] = 1.0f / mg; tris_l0[20] = 0.0f;		//7
  tris_l0[21] = 1.0f / mg; tris_l0[22] = 0.0f; tris_l0[23] = kPhi / mg;		//3
  tris_l0[24] = kPhi / mg; tris_l0[25] = -1.0f / mg; tris_l0[26] = 0.0f;	//2
  vF.push_back(vector<int>{ 7, 3, 2});

  tris_l0[27] = 1.0f / mg; tris_l0[28] = 0.0f; tris_l0[29] = -kPhi / mg;	//6
  tris_l0[30] = kPhi / mg; tris_l0[31] = 1.0f / mg; tris_l0[32] = 0.0f;		//7
  tris_l0[33] = kPhi / mg; tris_l0[34] = -1.0f / mg; tris_l0[35] = 0.0f;	//2
  vF.push_back(vector<int>{ 6, 7, 2});

  tris_l0[36] = kPhi / mg; tris_l0[37] = -1.0f / mg; tris_l0[38] = 0.0f;	//2
  tris_l0[39] = 0.0f; tris_l0[40] = -kPhi / mg; tris_l0[41] = -1.0f / mg;	//1
  tris_l0[42] = 1.0f / mg; tris_l0[43] = 0.0f; tris_l0[44] = -kPhi / mg;	//6
  vF.push_back(vector<int>{ 2, 1, 6});

  tris_l0[45] = 0.0f; tris_l0[46] = -kPhi / mg; tris_l0[47] = -1.0f / mg;	//1
  tris_l0[48] = 0.0f; tris_l0[49] = -kPhi / mg; tris_l0[50] = 1.0f / mg;	//4
  tris_l0[51] = -kPhi / mg; tris_l0[52] = -1.0f / mg; tris_l0[53] = 0.0f;	//5
  vF.push_back(vector<int>{ 1, 4, 5});

  tris_l0[54] = -1.0f / mg; tris_l0[55] = 0.0f; tris_l0[56] = kPhi / mg;	//8
  tris_l0[57] = -kPhi / mg; tris_l0[58] = -1.0f / mg; tris_l0[59] = 0.0f;	//5
  tris_l0[60] = 0.0f; tris_l0[61] = -kPhi / mg; tris_l0[62] = 1.0f / mg;	//4
  vF.push_back(vector<int>{ 8, 5, 4});

  tris_l0[63] = 0.0f; tris_l0[64] = -kPhi / mg; tris_l0[65] = 1.0f / mg;	//4
  tris_l0[66] = 1.0f / mg; tris_l0[67] = 0.0f; tris_l0[68] = kPhi / mg;		//3
  tris_l0[69] = -1.0f / mg; tris_l0[70] = 0.0f; tris_l0[71] = kPhi / mg;	//8
  vF.push_back(vector<int>{ 4, 3, 8});

  tris_l0[72] = 0.0f; tris_l0[73] = kPhi / mg; tris_l0[74] = 1.0f / mg;		//9
  tris_l0[75] = -1.0f / mg; tris_l0[76] = 0.0f; tris_l0[77] = kPhi / mg;	//8
  tris_l0[78] = 1.0f / mg; tris_l0[79] = 0.0f; tris_l0[80] = kPhi / mg;		//3
  vF.push_back(vector<int>{ 9, 8, 3});

  tris_l0[81] = 1.0f / mg; tris_l0[82] = 0.0f; tris_l0[83] = kPhi / mg;		//3
  tris_l0[84] = kPhi / mg; tris_l0[85] = 1.0f / mg; tris_l0[86] = 0.0f;		//7
  tris_l0[87] = 0.0f; tris_l0[88] = kPhi / mg; tris_l0[89] = 1.0f / mg;		//9
  vF.push_back(vector<int>{ 3, 7, 9});

  tris_l0[90] = 0.0f; tris_l0[91] = kPhi / mg; tris_l0[92] = -1.0f / mg;	//10
  tris_l0[93] = 0.0f; tris_l0[94] = kPhi / mg; tris_l0[95] = 1.0f / mg;		//9
  tris_l0[96] = kPhi / mg; tris_l0[97] = 1.0f / mg; tris_l0[98] = 0.0f;		//7
  vF.push_back(vector<int>{ 10, 9, 7});

  tris_l0[99] = kPhi / mg; tris_l0[100] = 1.0f / mg; tris_l0[101] = 0.0f;	//7
  tris_l0[102] = 1.0f / mg; tris_l0[103] = 0.0f; tris_l0[104] = -kPhi / mg;	//6
  tris_l0[105] = 0.0f; tris_l0[106] = kPhi / mg; tris_l0[107] = -1.0f / mg;	//10
  vF.push_back(vector<int>{ 7, 6, 10});

  tris_l0[108] = -1.0f / mg; tris_l0[109] = 0.0f; tris_l0[110] = -kPhi / mg;//11
  tris_l0[111] = 0.0f; tris_l0[112] = kPhi / mg; tris_l0[113] = -1.0f / mg;	//10
  tris_l0[114] = 1.0f / mg; tris_l0[115] = 0.0f; tris_l0[116] = -kPhi / mg;	//6
  vF.push_back(vector<int>{ 11, 10, 6});

  tris_l0[117] = 1.0f / mg; tris_l0[118] = 0.0f; tris_l0[119] = -kPhi / mg;	//6
  tris_l0[120] = 0.0f; tris_l0[121] = -kPhi / mg; tris_l0[122] = -1.0f / mg;//1
  tris_l0[123] = -1.0f / mg; tris_l0[124] = 0.0f; tris_l0[125] = -kPhi / mg;//11
  vF.push_back(vector<int>{ 6, 1, 11});

  tris_l0[126] = -kPhi / mg; tris_l0[127] = -1.0f / mg; tris_l0[128] = 0.0f;//5
  tris_l0[129] = -1.0f / mg; tris_l0[130] = 0.0f; tris_l0[131] = -kPhi / mg;//11
  tris_l0[132] = 0.0f; tris_l0[133] = -kPhi / mg; tris_l0[134] = -1.0f / mg;//1
  vF.push_back(vector<int>{ 5, 11, 1});

  tris_l0[135] = -kPhi / mg; tris_l0[136] = -1.0f / mg; tris_l0[137] = 0.0f;//5
  tris_l0[138] = -1.0f / mg; tris_l0[139] = 0.0f; tris_l0[140] = kPhi / mg;	//8
  tris_l0[141] = -kPhi / mg; tris_l0[142] = 1.0f / mg; tris_l0[143] = 0.0f;	//0
  vF.push_back(vector<int>{ 5, 8, 0});

  tris_l0[144] = -1.0f / mg; tris_l0[145] = 0.0f; tris_l0[146] = kPhi / mg;	//8
  tris_l0[147] = 0.0f; tris_l0[148] = kPhi / mg; tris_l0[149] = 1.0f / mg;	//9
  tris_l0[150] = -kPhi / mg; tris_l0[151] = 1.0f / mg; tris_l0[152] = 0.0f;	//0
  vF.push_back(vector<int>{ 8, 9, 0});

  tris_l0[153] = 0.0f; tris_l0[154] = kPhi / mg; tris_l0[155] = 1.0f / mg;	//9
  tris_l0[156] = 0.0f; tris_l0[157] = kPhi / mg; tris_l0[158] = -1.0f / mg;	//10
  tris_l0[159] = -kPhi / mg; tris_l0[160] = 1.0f / mg; tris_l0[161] = 0.0f;	//0
  vF.push_back(vector<int>{ 9, 10, 0});

  tris_l0[162] = 0.0f; tris_l0[163] = kPhi / mg; tris_l0[164] = -1.0f / mg;	//10
  tris_l0[165] = -1.0f / mg; tris_l0[166] = 0.0f; tris_l0[167] = -kPhi / mg;//11
  tris_l0[168] = -kPhi / mg; tris_l0[169] = 1.0f / mg; tris_l0[170] = 0.0f;	//0
  vF.push_back(vector<int>{ 10, 11, 0});

  tris_l0[171] = -kPhi / mg; tris_l0[172] = 1.0f / mg; tris_l0[173] = 0.0f;	//0
  tris_l0[174] = -1.0f / mg; tris_l0[175] = 0.0f; tris_l0[176] = -kPhi / mg;//11
  tris_l0[177] = -kPhi / mg; tris_l0[178] = -1.0f / mg; tris_l0[179] = 0.0f;//5
  vF.push_back(vector<int>{ 0, 11, 5});

  //for (int i = 0; i < vF.size(); i++)
  //for (int j = 0; j < vF[i].size(); j++)
	 // cout << vF[i][j]<<", ";

  // level 1 to (dn - 1)
  for (int i = 1; i != dn; ++i) {
    float* tris_ll = (*tris)[i - 1], *tris_lc = (*tris)[i];
    float v0[3], v1[3], v2[3], v3[3], v4[3], v5[3];

    for (int j = 0; j < (*bn)[i - 1]; ++j) {
      // copy old verts
      int j9 = j * 9;
      copy(tris_ll + j9, v0);
      copy(tris_ll + j9 + 3, v1);
      copy(tris_ll + j9 + 6, v2);
      // compute mid verts
      add(v0, v1, v3); add(v1, v2, v4); add(v2, v0, v5);
      normalize(v3); normalize(v4); normalize(v5);
      // construct subdivided triangles
      int j36 = j * 36;

      copy(v0, tris_lc + j36);
      copy(v3, tris_lc + j36 + 3);
      copy(v5, tris_lc + j36 + 6);

      copy(v1, tris_lc + j36 + 9);
      copy(v4, tris_lc + j36 + 12);
      copy(v3, tris_lc + j36 + 15);

      copy(v2, tris_lc + j36 + 18);
      copy(v5, tris_lc + j36 + 21);
      copy(v4, tris_lc + j36 + 24);

      copy(v3, tris_lc + j36 + 27);
      copy(v4, tris_lc + j36 + 30);
      copy(v5, tris_lc + j36 + 33);
    }
  }
}

// stat
int bin_id(const float* v, const std::vector<int>& bn,
           const std::vector<float*>& tris) {
  float mg = mag(v);
  if (mg >= kEps) {
    const int dn = bn.size(); int id = 0, id9 = 0;

    for (int i = 0; i != dn; ++i) {
      float* tris_lc = tris[i];
      id *= 4; id9 = id * 9;
      while (!rt_intersects(v, tris_lc + id9, tris_lc + id9 + 3,
                            tris_lc + id9 + 6)) {
        ++id; id9 = id * 9;
      }
    }

    return id;
  } else {
    return -1;
  }
}

void gen_hist(const float* data, const int* dims, const int* dimbs,
              const std::vector<int>& bn, const std::vector<float*>& tris,
              int bs, int i, float*& hist) {
  int bnf = *(bn.end() - 1);
  hist = new float[bnf];
  for (int i = 0; i != bnf; ++i)
    hist[i] = 0.0f;
  int bst = bs - 1;
  int bz = i / (dimbs[0] * dimbs[1]),
      by = (i - bz * dimbs[0] * dimbs[1]) / dimbs[0],
      bx = i % dimbs[0];
  float ss = static_cast<float>(bst) / 63.0f, pos[3];
  float v[3]; int bid = 0;

  // // type 1
  // for (int x = 0; x != bs; ++x) {
  //   for (int y = 0; y != bs; ++y) {
  //     for (int z = 0; z != bs; ++z) {
  //       pos[0] = bx * bst + x;
  //       pos[1] = by * bst + y;
  //       pos[2] = bz * bst + z;
  //       if (pos[0] <= dims[0] - 1 && pos[1] <= dims[1] - 1 &&
  //           pos[2] <= dims[2] - 1) {
  //         at_phys(data, dims, pos, v);
  //         bid = bin_id(v, bn, tris);
  //         if (bid > 0)
  //           hist[bid] += 1.0f;
  //       }
  //     }
  //   }
  // }

  // type 2
  for (int x = 0; x != 64; ++x) {
    for (int y = 0; y != 64; ++y) {
      for (int z = 0; z != 64; ++z) {
        pos[0] = bx * bst + std::min(static_cast<float>(x) * ss,
                                     static_cast<float>(bst));
        pos[1] = by * bst + std::min(static_cast<float>(y) * ss,
                                     static_cast<float>(bst));
        pos[2] = bz * bst + std::min(static_cast<float>(z) * ss,
                                     static_cast<float>(bst));
        if (pos[0] <= dims[0] - 1 && pos[1] <= dims[1] - 1 &&
            pos[2] <= dims[2] - 1) {
          at_phys(data, dims, pos, v);
          bid = bin_id(v, bn, tris);
          if (bid > 0)
            hist[bid] += 1.0f;
        }
      }
    }
  }

  float sum = std::accumulate(hist, hist + bnf, 0.0f);
  if (sum > kEps) {
    float rsum = 1.0f / sum;
    for (int i = 0; i != bnf; ++i)
      hist[i] *= rsum;
  }
}

void inverse_cdf(const float* h, int bn, const float* tris,
                 float* v) {
  // compute cdf
  float* ch = new float[bn]; ch[0] = h[0];
  for (int i = 1; i != bn; ++i)
    ch[i] = ch[i - 1] + h[i];
  // get index
  float u = gDistribution(gGenerator);
  auto it = std::lower_bound(ch, ch + bn, u);
  int id = it - ch, id9 = id * 9;
  // generate vector
  float w0, w1, w2;
  w0 = gDistribution(gGenerator);
  w1 = gDistribution(gGenerator);
  if (w0 + w1 > 1.0f) {
    w0 = 1.0f - w0;
    w1 = 1.0f - w1;
  }
  w2 = 1.0f - w0 - w1;
  float v0[3], v1[3], v2[3];
  copy(tris + id9, v0); copy(tris + id9 + 3, v1); copy(tris + id9 + 6, v2);
  multi(v0, w0); multi(v1, w1); multi(v2, w2);
  add(v0, v1, v); addequal(v, v2);
  normalize(v);
}

// particle tracing
bool in_boundary(const int* dims, const float* pos) {
  if (pos[0] < 0.0f || pos[0] > static_cast<float>(dims[0]) - 1.0f ||
      pos[1] < 0.0f || pos[1] > static_cast<float>(dims[1]) - 1.0f ||
      pos[2] < 0.0f || pos[2] > static_cast<float>(dims[2]) - 1.0f)
    return false;
  else
    return true;
}

void cpt(const float* data, const int* dims, const float* seed,
         float dt, int steps, std::list<float>* trace) {
  float x[3], v[3];
  copy(seed, x);
  trace->push_back(x[0]); trace->push_back(x[1]); trace->push_back(x[2]);
  while (in_boundary(dims, x) && steps > 0) {
    at_phys(data, dims, x, v);
    multi(v, dt); addequal(x, v);
    trace->push_back(x[0]); trace->push_back(x[1]); trace->push_back(x[2]);
    --steps;
  }
}

void mc_at_phys(const float* data, const int* dimbs, int bst, int bn,
                const float* tris, const float* pos, float* v) {
  int bx = static_cast<int>(pos[0] / bst),
      by = static_cast<int>(pos[1] / bst),
      bz = static_cast<int>(pos[2] / bst);
  int bid = ttoo(dimbs, bx, by, bz);
  inverse_cdf(data + bid * bn, bn, tris, v);
}

void mc(const float* data, const int* dims, const int* dimbs, int bst, int bn,
        const float* tris, const float* seed, float dt, int steps,
        std::list<float>* trace) {
  float x[3], v[3];
  copy(seed, x);
  trace->push_back(x[0]); trace->push_back(x[1]); trace->push_back(x[2]);
  while (in_boundary(dims, x) && steps > 0) {
    mc_at_phys(data, dimbs, bst, bn, tris, x, v);
    multi(v, dt); addequal(x, v);
    trace->push_back(x[0]); trace->push_back(x[1]); trace->push_back(x[2]);
    --steps;
  }
}

// log
void log_vec(const float* vec) {
  std::cout << vec[0] << "," << vec[1] << "," << vec[2] << std::endl;
}

void log_trace_off(const std::list<float>& trace, const char* of) {
  std::ofstream file;
  file.open(of);
  file << "OFF" << std::endl;
  file << trace.size() / 3 << " 0 0" << std::endl;
  int s = 0;
  for (auto it = trace.begin(); it != trace.end(); ++it) {
    if (s != 2) {
      file << *it << " ";
      ++s;
    } else {
      file << *it << std::endl;
      s = 0;
    }
  }
  file.close();
}

void log_tris_off(const float* verts, int fn, const char* of) {
  std::ofstream file;
  file.open(of);
  file << "OFF" << std::endl;
  int vn = fn * 3;
  file << vn << " " << fn << " 0" << std::endl;
  // log verts
  for (int i = 0; i != vn; ++i) {
    int i3 = i * 3;
    file << verts[i3] << " " << verts[i3 + 1] << " "
         << verts[i3 + 2] << std::endl;
  }
  // log faces
  for (int f = 0; f != fn; ++f) {
    int f3 = f * 3;
    file << "3 " << f3 << " " << f3 + 1 << " " << f3 + 2 << std::endl;
  }
  file.close();
}

void log_histogram_json(const float* h, int bn, const char* of) {
  std::ofstream file;
  file.open(of);
  file << "{" << std::endl;
  file << "\"p\" : [" << h[0];
  for (int i = 1; i != bn; ++i)
    file << "," << h[i];
  file << "]" << std::endl;
  file << "}" << std::endl;
  file.close();
}

void log_histogram_image(const float* h, const std::vector<int>& bn,
                         const std::vector<float*>& tris, const char* of) {
  // create histogram image
  float hm[2048], v[3], theta, phi;
  for (int i = 0; i != 32; ++i) {
    for (int j = 0; j != 64; ++j) {
      theta = M_PI / 32.0f * i + M_PI / 64.0f;
      phi = M_PI / 32.0f * j + M_PI / 64.0f - M_PI;
      stc(theta, phi, v);
      // std::cout << bin_id(v, bn, tris) << std::endl;
      hm[i * 64 + j] = h[bin_id(v, bn, tris)];
    }
  }

  // output the histogram image
  FILE* file;
  file = fopen(of, "wb");

  fwrite(hm, sizeof(float), 2048, file);

  fclose(file);
}

void log_histogram_data(float** h, int hn, int bn, const char* of) {
  //FILE* file;
  //file = fopen(of, "w");

  //for (int i = 0; i != hn; ++i)
  //  fwrite(h[i], sizeof(float), bn, file);

  //fclose(file);
	using namespace std;
	fstream fs;
	fs.open(of, std::fstream::out);
	for (int i = 0; i < hn; i++)
	{
		for (int j = 0; j < bn; j++)
		{
			fs << h[i][j] << " ";
		}
		fs << endl;
	}
	fs.close();
}

}
