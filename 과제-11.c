#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static long long cmp_count;

static void gap_insertion(int *a, int n, int h)
{
    for (int i = h; i < n; i++) {
        int tmp = a[i];
        int j = i;
        while (j >= h) {
            cmp_count++;
            if (a[j - h] > tmp) {
                a[j] = a[j - h];
                j -= h;
            } else {
                break;
            }
        }
        a[j] = tmp;
    }
}

static long long insertion_sort(int *a, int n)
{
    cmp_count = 0;
    gap_insertion(a, n, 1);
    return cmp_count;
}

static long long shell_sort_original(int *a, int n)
{
    cmp_count = 0;
    for (int h = n / 2; h >= 1; h /= 2)
        gap_insertion(a, n, h);
    return cmp_count;
}

static long long shell_sort_knuth(int *a, int n)
{
    int h = 1;
    cmp_count = 0;
    while (3 * h + 1 < n)
        h = 3 * h + 1;
    for (; h >= 1; h = (h - 1) / 3)
        gap_insertion(a, n, h);
    return cmp_count;
}

static int is_sorted(const int *a, int n)
{
    for (int i = 1; i < n; i++)
        if (a[i - 1] > a[i])
            return 0;
    return 1;
}

int main(void)
{
    int sizes[] = {1000, 2000, 4000, 8000, 16000, 20000};
    int count = sizeof(sizes) / sizeof(sizes[0]);

    srand(12345);

    for (int s = 0; s < count; s++) {
        int n = sizes[s];
        int *origin = (int *)malloc(sizeof(int) * n);
        int *work = (int *)malloc(sizeof(int) * n);
        long long c1, c2, c3;

        if (origin == NULL || work == NULL) {
            fprintf(stderr, "memory allocation failed\n");
            return 1;
        }

        for (int i = 0; i < n; i++)
            origin[i] = rand();

        memcpy(work, origin, sizeof(int) * n);
        c1 = insertion_sort(work, n);
        if (!is_sorted(work, n)) {
            fprintf(stderr, "insertion sort failed (n=%d)\n", n);
            return 1;
        }

        memcpy(work, origin, sizeof(int) * n);
        c2 = shell_sort_original(work, n);
        if (!is_sorted(work, n)) {
            fprintf(stderr, "shell sort (original) failed (n=%d)\n", n);
            return 1;
        }

        memcpy(work, origin, sizeof(int) * n);
        c3 = shell_sort_knuth(work, n);
        if (!is_sorted(work, n)) {
            fprintf(stderr, "shell sort (knuth) failed (n=%d)\n", n);
            return 1;
        }

        printf("Data Size : %d\n\n", n);
        printf("Insertion Sort       : %lld comparisons\n", c1);
        printf("Shell Sort (Original): %lld comparisons\n", c2);
        printf("Shell Sort (Knuth)   : %lld comparisons\n\n", c3);

        free(origin);
        free(work);
    }

    return 0;
}
