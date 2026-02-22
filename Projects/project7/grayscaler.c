#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "ppm.h"

struct Parameters {
    struct ppm *image;
    int thread_id;
    int start_y;
    int slice_height;
};

struct Outputs {
    int avg_r;
    int avg_g;
    int avg_b;
};

void *run (void *args){
    struct Parameters *params = (struct Parameters *)args;
    struct ppm *ppm = params->image;

    int h = params->slice_height;
    int w = ppm->width;
    int n_pixels = w * h;

    int r_sum = 0;
    int g_sum = 0;
    int b_sum = 0;

    // Greyscale the slice
    for (int y = params->start_y; y < params->start_y + h; y++){
        for (int x = 0; x < w; x++){

            // Get a pixel's colors
            int pixel = ppm_get_pixel(ppm, x, y);

            int r = PPM_PIXEL_R(pixel);
            int g = PPM_PIXEL_G(pixel);
            int b = PPM_PIXEL_B(pixel);

            r_sum += r;
            g_sum += g;
            b_sum += b;

            // Greyscale a pixel
            int gray = (299*r + 587*g + 114*b) / 1000;
            int gray_pixel = PPM_PIXEL(gray, gray, gray);

            ppm_set_pixel(ppm, x, y, gray_pixel);

        }
    }

    struct Outputs *out = malloc(sizeof(struct Outputs));
    if (!out){
        perror("malloc failed");
        exit(1);
    }

    out->avg_r = r_sum / n_pixels;
    out->avg_g = g_sum / n_pixels;
    out->avg_b = b_sum / n_pixels;

    // Output
    printf("Thread %d: %d %d\n", params->thread_id, params->start_y, params->slice_height);
    return out;
}

void usage(void){
    puts("usage: ./grayscaler int in.ppm out.ppm");
    exit(1);
}

int getMean (int arr[], int n){
    int sum = 0;
    for (int i = 0; i < n; i++){
        sum += arr[i];
    }
    return sum / n;
}

int main (int argc, char *argv[]){
    if (argc < 4){
        usage();
    }

    int n_threads = atoi(argv[1]);
    char *filename = argv[2];
    char *outfile = argv[3];

    struct Parameters *params = malloc(sizeof(struct Parameters) * n_threads);
    if (!params){
        perror("malloc failed");
        exit(1);
    }

    struct ppm *ppm = ppm_read(filename);
    int img_h = ppm->height;

    // Create Threads
    pthread_t thread[n_threads];

    for (int i = 0; i < n_threads; i++){
        params[i].thread_id = i;
        params[i].image = ppm;
        
        int slice = img_h / n_threads;
        // Remainder goes to the last slice
        if (i == n_threads-1){
            params[i].slice_height = slice + (img_h % n_threads);
        } 
        // Otherwise image is sliced up normally
        else {
            params[i].slice_height = slice;
        }

        params[i].start_y = slice * i;

        pthread_create(thread+i, NULL, run, params+i);
    }

    // Join Threads
    int avg_reds[n_threads];
    int avg_greens[n_threads];
    int avg_blues[n_threads];

    for (int i = 0; i < n_threads; i++){
        void* out_ptr;
        pthread_join(thread[i], &out_ptr);

        // Handle Outputs
        struct Outputs* out = (struct Outputs*) out_ptr;

        avg_reds[i] = out->avg_r;
        avg_greens[i] = out->avg_g;
        avg_blues[i] = out->avg_b;

        free(out);
    }

    ppm_write(ppm, outfile);

    printf("Average R: %d\n", getMean(avg_reds, n_threads));
    printf("Average G: %d\n", getMean(avg_greens, n_threads));
    printf("Average B: %d\n", getMean(avg_blues, n_threads));

    free(params);
    ppm_free(ppm);
}