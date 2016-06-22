#ifndef KMEANSROSETTA_H
#define KMEANSROSETTA_H

#define M_PI 3.14f

typedef struct { double x, y; int group; } point_t, *point;

double randf(double m);

point gen_xy(int count, double radius);

inline double dist2(point a, point b);

inline int nearest(point pt, point cent, int n_cluster, double *d2);

void kpp(point pts, int len, point cent, int n_cent);

point lloyd(point pts, int len, int n_cluster);

void print_eps(point pts, int len, point cent, int n_cluster);

#define PTS 50
#define K 5

int mainPP();

#endif // KMEANS

