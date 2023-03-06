/* TEI - HPC
 * Fabio Goncalves 17646
 * Sebastiao Pereira 21932
 * 16/05/2022
 * */

#include <stdio.h>
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>


int main() {

  int ix, iy;
  const int width = 3840;
  const int height = 2160;

  unsigned char ** * img_output = (unsigned char ** * ) malloc(sizeof(unsigned char ** ) * height);

  for (int line = 0; line < height; line++) {
    img_output[line] = (unsigned char ** ) malloc(sizeof(unsigned char * ) * width);

    for (int col = 0; col < width; col++) {
      img_output[line][col] = (unsigned char * ) malloc(sizeof(unsigned char) * 3);
    }
  }

  double cx, cy;

  const double cx_min = -2.5;
  const double cx_max = 1.5;

  const double cy_min = -2.0;
  const double cy_max = 2.0;

  double pixel_width = (cx_max - cx_min) / width;
  double pixel_height = (cy_max - cy_min) / height;

  const int max_color_component_value = 255;

  FILE * fp;
  char * filename = "Mandelbrot_OpenMP.ppm";
  char * comment = "# ";

  static unsigned char color[3];

  double zx, zy;
  double zx2, zy2;

  int iteration;
  const int iteration_max = 10000;

  double percentage;

  const double radius = 2;
  double r2 = radius * radius;

  fp = fopen(filename, "wb");

  fprintf(fp, "P6\n %s\n %d\n %d\n %d\n", comment, width, height, max_color_component_value);

  long int t1 = time(NULL);

  #pragma omp parallel for private(iy, cy)

  for (iy = 0; iy < height; iy++) {
    percentage++;
    printf("\rA processar o Conjunto Mandelbrot... [%.2f%]", percentage / 21.6);
    fflush(stdout);

    cy = cy_min + iy * pixel_height;
    if (fabs(cy) < pixel_height / 2)
      cy = 0.0;

    #pragma omp parallel for private(ix, cx, zx, zy, zx2, zy2)

    for (ix = 0; ix < width; ix++) {

      cx = cx_min + ix * pixel_width;
      zx = 0.0;
      zy = 0.0;
      zx2 = zx * zx;
      zy2 = zy * zy;

      for (iteration = 0; iteration < iteration_max && ((zx2 + zy2) < r2); iteration++) {
        zy = 2 * zx * zy + cy;
        zx = zx2 - zy2 + cx;
        zx2 = zx * zx;
        zy2 = zy * zy;

      };

      if (iteration == iteration_max) {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
      } else {
        color[0] = ((iteration * 60) % 256); 		// R
        color[1] = ((iteration * iteration) % 256); // G
        color[2] = ((iteration * 30) % 256); 		// B
      };

      img_output[iy][ix][0] = color[0]; // R

      img_output[iy][ix][1] = color[1]; // G

      img_output[iy][ix][2] = color[2]; // B

    }
  }
  long int t2 = time(NULL);

  printf("\nFinalizado com sucesso.\n");
  printf("\nTempo de execucao: %ld segundos.\n", t2 - t1);

  for (iy = 0; iy < height; iy++) {
    for (ix = 0; ix < width; ix++) {
      fwrite(img_output[iy][ix], 1, 3, fp);
    }
  }

  fclose(fp);

  for (int line = 0; line < height; line++) {
    for (int col = 0; col < width; col++) {
      free(img_output[line][col]);
    }
    free(img_output[line]);
  }
  free(img_output);

  return 0;
}
