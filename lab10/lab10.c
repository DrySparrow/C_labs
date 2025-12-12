#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} Point;


int orientation(const Point p, const Point q, const Point r)
{
    long long a = (long long)(q.y) - (p.y);
    long long b = (long long)(r.x) - (q.x);
    long long c = (long long)(q.x) - (p.x);
    long long d = (long long)(r.y) - (q.y);

    int same_sign1 = 1;
    int same_sign2 = 1;

    if (a < 0ll) a = -a, same_sign1 = !same_sign1;
    if (b < 0ll) b = -b, same_sign1 = !same_sign1;
    if (c < 0ll) c = -c, same_sign2 = !same_sign2;
    if (d < 0ll) d = -d, same_sign2 = !same_sign2;

    if (a == 0ll || b == 0ll) same_sign1 = 1;
    if (c == 0ll || d == 0ll) same_sign2 = 1;

    if (same_sign1 == 1 && same_sign2 == 1){
        return (unsigned long long)(a) * (unsigned long long)(b) < (unsigned long long)(c) * (unsigned long long)(d);
    } else if (same_sign1 == 1 && same_sign2 == 0) {
        return 0;
    } else if (same_sign1 == 0 && same_sign2 == 1){
        return 1;
    } else {
        return (unsigned long long)(a) * (unsigned long long)(b) > (unsigned long long)(c) * (unsigned long long)(d);
    }
}


int compare(const void *a, const void *b)
{
    Point *p1 = (Point *)a;
    Point *p2 = (Point *)b;

    if (p1->x < p2->x) return -1;
    if (p1->x > p2->x) return 1;
    if (p1->y < p2->y) return -1;
    if (p1->y > p2->y) return 1;
    return 0;
}


int convexHull(Point points[], int n, Point hull[])
{
    int hullIndex = 0;

    // bottom part
    for (int i = 0; i < n; i++) {
        while (hullIndex >= 2 && orientation(hull[hullIndex - 2], hull[hullIndex - 1], points[i]) == 0) {
            hullIndex--;
        }
        hull[hullIndex++] = points[i];
    }

    // top part
    int prevHullIndex = hullIndex;
    for (int i = n - 2; i >= 0; i--) {
        while (hullIndex > prevHullIndex && hullIndex >= 2 && orientation(hull[hullIndex - 2], hull[hullIndex - 1], points[i]) == 0) {
            hullIndex--;
        }
        hull[hullIndex++] = points[i];
    }

    return hullIndex - 1;
}


int main(void)
{
    int N;
    if (scanf("%d", &N) != 1 || N < 0 || N > 100000) {
        printf("bad number of points\n");
        return 0;
    }
    
    Point *points = (Point *)malloc(N * sizeof(Point));
    if (points == NULL) {
        return 0;
    }

    for (int i = 0; i < N; i++) {
        if (scanf("%d %d", &points[i].x, &points[i].y) != 2) {
            printf("bad number of lines\n");
            free(points);
            return 0;
        }
    }

    if (N == 1) {
        printf("%d %d\n", points[0].x, points[0].y);
        free(points);
        return 0;
    } else if (N == 2) {
        printf("%d %d\n", points[0].x, points[0].y);
        printf("%d %d\n", points[1].x, points[1].y);
        free(points);
        return 0;
    }

   Point *hull = (Point *)malloc(2 * N * sizeof(Point)); // 2*N because an union top and bottom
    if (hull == NULL) {
        free(points);
        return 0;
    }

    qsort(points, N, sizeof(Point), compare);
    int hullSize = convexHull(points, N, hull);

    for (int i = 0; i < hullSize; i++) {
        printf("%d %d\n", hull[i].x, hull[i].y);
    }

    free(points);
    free(hull);
    
    return 0;
}
