#include <ctime>

#include "dstat.h"

using namespace std;
using namespace dstat;

int main(int argc, char* argv[]) {
  if (argc < 6) {
    cout << "Usage: pre filein dimx dimy dimz block_size" << endl;
    return 1;
  }

  // global settings
  char* filein = argv[1];
  int dims[3];
  dims[0] = atoi(argv[2]); dims[1] = atoi(argv[3]); dims[2] = atoi(argv[4]);
  int dimtotal = dims[0] * dims[1] * dims[2];

  int block_size = atoi(argv[5]), block_size_it = block_size - 1, dimbs[3];
  dimbs[0] = static_cast<int>(ceil(static_cast<float>(dims[0]) / block_size_it));
  dimbs[1] = static_cast<int>(ceil(static_cast<float>(dims[1]) / block_size_it));
  dimbs[2] = static_cast<int>(ceil(static_cast<float>(dims[2]) / block_size_it));
  int dimbtotal = dimbs[0] * dimbs[1] * dimbs[2];

  // float dt = 1.0f;
  // int steps = 1000;

  time_t timer_1, timer_2;
  double diff;

  // generate spherical grids
  vector<int> bn; vector<float*> sg;
  gen_spgrid(4, &bn, &sg);
  int bnf = *(bn.end() - 1);
  float* sgf = *(sg.end() - 1);
  // log_tris_off(sg[0], bn[0], "../../results/temp/level0.off");
  // log_tris_off(sg[1], bn[1], "../../results/temp/level1.off");
  // log_tris_off(sg[2], bn[2], "../../results/temp/level2.off");
  // log_tris_off(sg[3], bn[3], "../../results/temp/level3.off");

  // load data
  FILE* file;
  file = fopen(filein, "rb");

  float* data = new float[dimtotal * 3];
  fread(data, sizeof(float), dimtotal * 3, file);

  fclose(file);
  cout << "Data loaded" << endl;

  // // overview
  // time(&timer_1);

  // list<float> trace; float seed[3];
  // for (int i = 0; i < 1000; ++i) {
  //   seed[0] = randf(0.0f, dims[0] - 1.0f);
  //   seed[1] = randf(0.0f, dims[1] - 1.0f);
  //   seed[2] = randf(0.0f, dims[2] - 1.0f);
  //   cpt(data, dims, seed, dt, steps, &trace);
  // }

  // log_trace_off(trace, "../../results/temp/overview.off");

  // time(&timer_2);
  // diff = difftime(timer_2, timer_1);
  // cout << "Overview generated, cost: " << diff << " seconds" << endl;

  // // streamline database - ground truth
  // time(&timer_1);

  // int sl_dims[3]; float slss = 20.0f;
  // sl_dims[0] = dims[0] / slss + 1;
  // sl_dims[1] = dims[1] / slss + 1;
  // sl_dims[2] = dims[2] / slss + 1;
  // list<float> sl;
  // float seed[3]; char prefix[] = "../../results/temp/", postfix[40], fn[100];

  // for (int k = 0; k != sl_dims[2]; ++k) {
  //   for (int j = 0; j != sl_dims[1]; ++j) {
  //     for (int i = 0; i != sl_dims[0]; ++i) {
  //       seed[0] = i * slss; seed[1] = j * slss; seed[2] = k * slss;
  //       sl.clear(); cpt(data, dims, seed, dt, steps, &sl);
  //       sl_name(seed[0], seed[1], seed[2], postfix);
  //       strcpy(fn, prefix);
  //       strcat(fn, postfix);
  //       log_trace_off(sl, fn);
  //     }
  //   }
  // }

  // time(&timer_2);
  // diff = difftime(timer_2, timer_1);
  // cout << "Ground truth generated, cost: " << diff << " seconds" << endl;

  // histogram computation
  time(&timer_1);

  // generate histograms
  float* datah[dimbtotal];
  for (int i = 0; i != dimbtotal; ++i)
    gen_hist(data, dims, dimbs, bn, sg, block_size, i, datah[i]);

  // output histogram data
  log_histogram_data(datah, dimbtotal, bnf,
                     "../../results/temp/isabel_16.hist");

  time(&timer_2);
  diff = difftime(timer_2, timer_1);
  cout << "Histograms computed, cost: " << diff << " seconds" << endl;

  return 0;
}
