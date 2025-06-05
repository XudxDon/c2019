#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define INPUT_SIZE 64
#define CLASSES 10
#define EPOCHS 1000
#define LR 0.1

static const char *DIGITS[10][8] = {
    {"00111100","01000010","10000001","10000001","10000001","10000001","01000010","00111100"},
    {"00010000","00110000","01010000","00010000","00010000","00010000","00010000","01111100"},
    {"00111100","01000010","00000010","00000100","00001000","00010000","00100000","01111110"},
    {"00111100","01000010","00000010","00011100","00000010","00000010","01000010","00111100"},
    {"00000100","00001100","00010100","00100100","01000100","01111110","00000100","00000100"},
    {"01111110","01000000","01000000","01111100","00000010","00000010","01000010","00111100"},
    {"00111100","01000010","01000000","01111100","01000010","01000010","01000010","00111100"},
    {"01111110","00000010","00000100","00001000","00010000","00100000","00100000","00100000"},
    {"00111100","01000010","01000010","00111100","01000010","01000010","01000010","00111100"},
    {"00111100","01000010","01000010","00111110","00000010","00000010","01000010","00111100"}
};

void make_dataset(double X[CLASSES][INPUT_SIZE], int y[CLASSES]) {
    for (int d=0; d<CLASSES; ++d) {
        y[d]=d;
        int idx=0;
        for(int r=0;r<8;++r) {
            for(int c=0;c<8;++c) {
                X[d][idx++] = DIGITS[d][r][c]-'0';
            }
        }
    }
}

double dot(const double *a, const double *b, int n) {
    double s=0; for(int i=0;i<n;++i) s+=a[i]*b[i]; return s;
}

void softmax(const double *scores, double *out, int n) {
    double m=scores[0]; for(int i=1;i<n;++i) if(scores[i]>m) m=scores[i];
    double sum=0; for(int i=0;i<n;++i){ out[i]=exp(scores[i]-m); sum+=out[i]; }
    for(int i=0;i<n;++i) out[i]/=sum;
}

void train(double X[][INPUT_SIZE], int y[], double W[][INPUT_SIZE], double b[]) {
    for(int j=0;j<CLASSES;++j){ for(int k=0;k<INPUT_SIZE;++k) W[j][k]=((rand()%2000)/1000.0-1)*0.01; b[j]=0; }
    for(int e=0;e<EPOCHS;++e) {
        for(int i=0;i<CLASSES;++i) {
            double scores[CLASSES];
            for(int j=0;j<CLASSES;++j) scores[j]=dot(W[j], X[i], INPUT_SIZE)+b[j];
            double probs[CLASSES];
            softmax(scores, probs, CLASSES);
            for(int j=0;j<CLASSES;++j) {
                double error = probs[j] - (y[i]==j?1.0:0.0);
                for(int k=0;k<INPUT_SIZE;++k) W[j][k]-=LR*error*X[i][k];
                b[j]-=LR*error;
            }
        }
    }
}

int predict(const double W[][INPUT_SIZE], const double b[], const double *x) {
    double scores[CLASSES];
    for(int j=0;j<CLASSES;++j) scores[j]=dot(W[j], x, INPUT_SIZE)+b[j];
    double probs[CLASSES];
    softmax(scores, probs, CLASSES);
    int best=0; for(int j=1;j<CLASSES;++j) if(probs[j]>probs[best]) best=j; return best;
}

int main(){
    srand(0);
    double X[CLASSES][INPUT_SIZE];
    int y[CLASSES];
    make_dataset(X, y);
    double W[CLASSES][INPUT_SIZE];
    double b[CLASSES];
    clock_t start = clock();
    train(X, y, W, b);
    clock_t end = clock();
    printf("pred:");
    for(int i=0;i<CLASSES;++i){ int p=predict(W,b,X[i]); printf(" %d", p); }
    printf("\ntruth:");
    for(int i=0;i<CLASSES;++i) printf(" %d", y[i]);
    printf("\ntraining time: %.3fs\n", (double)(end-start)/CLOCKS_PER_SEC);
    return 0;
}
