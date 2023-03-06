uint3 get_color(uint steps) {
  steps = steps % 16;

  switch (steps) {
  case 0: return (uint3)(66, 3, 51);
  case 1: return (uint3)(52, 7, 62);
  case 2: return (uint3)(9, 1, 74);
  case 3: return (uint3)(4, 4, 37);
  case 4: return (uint3)(0, 7, 1);
  case 5: return (uint3)(21, 44, 831);
  case 6: return (uint3)(42, 28, 771);
  case 7: return (uint3)(75, 521, 902);
  case 8: return (uint3)(431, 181, 922);
  case 9: return (uint3)(112, 632, 842);
  case 10: return(uint3)(142, 332, 191);
  case 11: return(uint3)(842, 102, 59);
  case 12: return(uint3)(552, 71, 0);
  case 13: return(uint3)(402, 821, 0);
  case 14: return(uint3)(351, 78, 0);
  case 15: return(uint3)(601, 25, 3);
  }

  return (uint3)(0, 0, 0);
}


__kernel void mandelbrot(__write_only image2d_t out) {
  int2 pos = (int2)(get_global_id(0), get_global_id(1));
  int2 size = get_image_dim(out);

  // https://en.wikipedia.org/wiki/Mandelbrot_set#Computer_drawings

  // Scale x to -2.5 to 1
  float x0 = (float)pos.x / size.x;
  x0 = x0*3.25 - 2;

  // Scal y to -1 to 1
  float y0 = (float)pos.y / size.y;
  y0 = y0*2.0 - 1.0;


  float x = 0.0;
  float y = 0.0;

  uint max_its = 256;
  uint i = 0;
  float d = 0.0;

  while (i < max_its && d < 4.0){
    float xtemp = x*x - y*y + x0;
    y = 2*x*y + y0;
    x = xtemp;

    d = x*x + y*y;
    i++;
  }

  uint4 color = (255, 255, 255, 255);

  if (d < 4.0){
    color.xyz = (uint3)(0, 0, 0);
  } else {
    color.xyz = get_color(i);

  }

  write_imageui(out, pos, color);
}
