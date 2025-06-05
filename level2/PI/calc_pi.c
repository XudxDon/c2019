#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/resource.h>

void spigot_pi(int digits) {
    int len = digits * 10 / 3 + 1;
    int *a = (int*)calloc(len, sizeof(int));
    for (int i=0;i<len;i++) a[i] = 2;

    int nines = 0, predigit = 0;
    for (int j=0; j<digits; ++j) {
        int q = 0;
        for (int i=len-1;i>0;--i) {
            int x = a[i]*10 + q*i;
            a[i] = x % (2*i+1);
            q = x / (2*i+1);
        }
        int x = a[0]*10 + q*2;
        a[0] = x % 10;
        q = x / 10;
        if (q==9) {
            nines++;
        } else if (q==10) {
            printf("%d", predigit+1);
            for (int k=0;k<nines;k++) printf("0");
            predigit = 0; nines = 0;
        } else {
            printf("%d", predigit);
            predigit = q;
            if (nines) { for(int k=0;k<nines;k++) printf("9"); nines=0; }
        }
        if (j==0) putchar('.');
    }
    printf("%d\n", predigit);
    free(a);
}

int main(){
    int digits = 1000; // adjust as needed
    struct rusage u1; getrusage(RUSAGE_SELF, &u1);
    clock_t start = clock();
    spigot_pi(digits);
    clock_t end = clock();
    struct rusage u2; getrusage(RUSAGE_SELF, &u2);
    printf("digits: %d\n", digits);
    printf("time: %.3fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    printf("maxrss: %ld KB\n", u2.ru_maxrss);
    return 0;
}
