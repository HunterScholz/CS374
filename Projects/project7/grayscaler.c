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
    int sum_r;
    int sum_g;
    int sum_b;
};

void *run (void *args){
    struct Parameters *params = (struct Parameters *)args;
    struct ppm *ppm = params->image;

    int h = params->slice_height;
    int w = ppm->width;

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

    out->sum_r = r_sum;
    out->sum_g = g_sum;
    out->sum_b = b_sum;

    // Output
    printf("Thread %d: %d %d\n", params->thread_id, params->start_y, params->slice_height);
    return out;
}

void usage(void){
    puts("usage: ./grayscaler int in.ppm out.ppm");
    exit(1);
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
    int total_reds = 0;
    int total_greens = 0;
    int total_blues = 0;

    for (int i = 0; i < n_threads; i++){
        void* out_ptr;
        pthread_join(thread[i], &out_ptr);

        // Handle Outputs
        struct Outputs* out = (struct Outputs*) out_ptr;

        total_reds += out->sum_r;
        total_greens += out->sum_g;
        total_blues += out->sum_b;

        free(out);
    }

    ppm_write(ppm, outfile);
    int n_pixels = ppm->height * ppm->width;

    printf("Average R: %d\n", total_reds / n_pixels);
    printf("Average G: %d\n", total_greens / n_pixels);
    printf("Average B: %d\n", total_blues / n_pixels);

    free(params);
    ppm_free(ppm);
}