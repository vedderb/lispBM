#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "compression.h"




int main(void) {
  
  char test0[] = "((lambda (x) (+ x 1)) 1)";
  char test1[] = {
  0x0a, 0x28, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x72, 0x65, 0x76,
  0x65, 0x72, 0x73, 0x65, 0x0a, 0x20, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62,
  0x64, 0x61, 0x20, 0x28, 0x78, 0x73, 0x29, 0x0a, 0x20, 0x20, 0x20, 0x20,
  0x28, 0x6c, 0x65, 0x74, 0x20, 0x28, 0x28, 0x72, 0x65, 0x76, 0x61, 0x63,
  0x63, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x61,
  0x63, 0x63, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x20,
  0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x3d, 0x20, 0x6e, 0x69, 0x6c, 0x20,
  0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x61, 0x63, 0x63, 0x0a, 0x09,
  0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x72, 0x65, 0x76, 0x61,
  0x63, 0x63, 0x20, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28, 0x63, 0x61,
  0x72, 0x20, 0x78, 0x73, 0x29, 0x20, 0x61, 0x63, 0x63, 0x29, 0x20, 0x28,
  0x63, 0x64, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29,
  0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x72, 0x65, 0x76, 0x61,
  0x63, 0x63, 0x20, 0x6e, 0x69, 0x6c, 0x20, 0x78, 0x73, 0x29, 0x29, 0x29,
  0x29, 0x0a, 0x0a, 0x0a, 0x28, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20,
  0x69, 0x6f, 0x74, 0x61, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61,
  0x20, 0x28, 0x6e, 0x29, 0x0a, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x28, 0x6c, 0x65, 0x74, 0x20, 0x28, 0x28, 0x69, 0x61, 0x63, 0x63,
  0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x61, 0x63,
  0x63, 0x20, 0x69, 0x20, 0x6e, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x3e, 0x20, 0x69, 0x20,
  0x6e, 0x29, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x20, 0x61, 0x63, 0x63, 0x0a,
  0x09, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x69,
  0x61, 0x63, 0x63, 0x20, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28, 0x2d,
  0x20, 0x6e, 0x20, 0x69, 0x29, 0x20, 0x61, 0x63, 0x63, 0x29, 0x20, 0x28,
  0x2b, 0x20, 0x69, 0x20, 0x31, 0x29, 0x20, 0x6e, 0x29, 0x29, 0x29, 0x29,
  0x29, 0x0a, 0x09, 0x09, 0x20, 0x28, 0x69, 0x61, 0x63, 0x63, 0x20, 0x6e,
  0x69, 0x6c, 0x20, 0x30, 0x20, 0x6e, 0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a,
  0x28, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x6c, 0x65, 0x6e, 0x67,
  0x74, 0x68, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28,
  0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x28, 0x6c, 0x65, 0x74, 0x20,
  0x28, 0x28, 0x6c, 0x65, 0x6e, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64,
  0x61, 0x20, 0x28, 0x6c, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x09,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x3d,
  0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29, 0x0a, 0x09, 0x09, 0x09,
  0x09, 0x20, 0x20, 0x6c, 0x0a, 0x09, 0x09, 0x09, 0x09, 0x28, 0x6c, 0x65,
  0x6e, 0x20, 0x28, 0x2b, 0x20, 0x6c, 0x20, 0x31, 0x29, 0x20, 0x28, 0x63,
  0x64, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0a,
  0x09, 0x09, 0x20, 0x20, 0x20, 0x28, 0x6c, 0x65, 0x6e, 0x20, 0x30, 0x20,
  0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28, 0x64, 0x65, 0x66,
  0x69, 0x6e, 0x65, 0x20, 0x74, 0x61, 0x6b, 0x65, 0x20, 0x28, 0x6c, 0x61,
  0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x6e, 0x20, 0x78, 0x73, 0x29, 0x0a,
  0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x69, 0x66, 0x20,
  0x28, 0x6e, 0x75, 0x6d, 0x2d, 0x65, 0x71, 0x20, 0x6e, 0x20, 0x30, 0x29,
  0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x6e, 0x69, 0x6c, 0x0a, 0x09, 0x09,
  0x20, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20,
  0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x20, 0x28, 0x74, 0x61, 0x6b, 0x65, 0x20, 0x28, 0x2d, 0x20, 0x6e, 0x20,
  0x31, 0x29, 0x20, 0x28, 0x63, 0x64, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29,
  0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28, 0x64, 0x65, 0x66, 0x69, 0x6e,
  0x65, 0x20, 0x64, 0x72, 0x6f, 0x70, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62,
  0x64, 0x61, 0x20, 0x28, 0x6e, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x6e,
  0x75, 0x6d, 0x2d, 0x65, 0x71, 0x20, 0x6e, 0x20, 0x30, 0x29, 0x0a, 0x09,
  0x09, 0x20, 0x20, 0x20, 0x78, 0x73, 0x0a, 0x09, 0x09, 0x20, 0x28, 0x69,
  0x66, 0x20, 0x28, 0x3d, 0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29,
  0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x6e, 0x69, 0x6c, 0x0a,
  0x09, 0x09, 0x20, 0x20, 0x20, 0x28, 0x64, 0x72, 0x6f, 0x70, 0x20, 0x28,
  0x2d, 0x20, 0x6e, 0x20, 0x31, 0x29, 0x20, 0x28, 0x63, 0x64, 0x72, 0x20,
  0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28, 0x64,
  0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x7a, 0x69, 0x70, 0x20, 0x28, 0x6c,
  0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x78, 0x73, 0x20, 0x79, 0x73,
  0x29, 0x0a, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x69, 0x66,
  0x20, 0x28, 0x20, 0x3d, 0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29,
  0x0a, 0x09, 0x09, 0x20, 0x20, 0x6e, 0x69, 0x6c, 0x0a, 0x09, 0x09, 0x28,
  0x69, 0x66, 0x20, 0x28, 0x20, 0x3d, 0x20, 0x79, 0x73, 0x20, 0x6e, 0x69,
  0x6c, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x6e, 0x69, 0x6c,
  0x0a, 0x09, 0x09, 0x20, 0x20, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28,
  0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73,
  0x29, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x79, 0x73, 0x29, 0x29, 0x20,
  0x28, 0x7a, 0x69, 0x70, 0x20, 0x28, 0x63, 0x64, 0x72, 0x20, 0x78, 0x73,
  0x29, 0x20, 0x28, 0x63, 0x64, 0x72, 0x20, 0x79, 0x73, 0x29, 0x29, 0x29,
  0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28, 0x64, 0x65, 0x66, 0x69, 0x6e,
  0x65, 0x20, 0x6d, 0x61, 0x70, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64,
  0x61, 0x20, 0x28, 0x66, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x20, 0x20,
  0x20, 0x20, 0x20, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x3d, 0x20, 0x78,
  0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x6e,
  0x69, 0x6c, 0x0a, 0x09, 0x09, 0x28, 0x63, 0x6f, 0x6e, 0x73, 0x20, 0x28,
  0x66, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x20,
  0x28, 0x6d, 0x61, 0x70, 0x20, 0x66, 0x20, 0x28, 0x63, 0x64, 0x72, 0x20,
  0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28, 0x64,
  0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x6c, 0x6f, 0x6f, 0x6b, 0x75, 0x70,
  0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x78, 0x20,
  0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28,
  0x3d, 0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29, 0x0a, 0x09, 0x09,
  0x20, 0x20, 0x20, 0x20, 0x20, 0x6e, 0x69, 0x6c, 0x0a, 0x09, 0x09, 0x20,
  0x20, 0x20, 0x28, 0x69, 0x66, 0x20, 0x28, 0x3d, 0x20, 0x28, 0x63, 0x61,
  0x72, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x20,
  0x78, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
  0x28, 0x63, 0x64, 0x72, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73,
  0x29, 0x29, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x6c,
  0x6f, 0x6f, 0x6b, 0x75, 0x70, 0x20, 0x78, 0x20, 0x28, 0x63, 0x64, 0x72,
  0x20, 0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0a, 0x0a, 0x28,
  0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x66, 0x6f, 0x6c, 0x64, 0x72,
  0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28, 0x66, 0x20,
  0x69, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x28, 0x69, 0x66, 0x20,
  0x28, 0x3d, 0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29, 0x0a, 0x09,
  0x09, 0x20, 0x20, 0x20, 0x20, 0x69, 0x0a, 0x09, 0x09, 0x20, 0x20, 0x28,
  0x66, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73, 0x29, 0x20, 0x28,
  0x66, 0x6f, 0x6c, 0x64, 0x72, 0x20, 0x66, 0x20, 0x69, 0x20, 0x28, 0x63,
  0x64, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x29, 0x29, 0x29, 0x29, 0x0a,
  0x0a, 0x28, 0x64, 0x65, 0x66, 0x69, 0x6e, 0x65, 0x20, 0x66, 0x6f, 0x6c,
  0x64, 0x6c, 0x20, 0x28, 0x6c, 0x61, 0x6d, 0x62, 0x64, 0x61, 0x20, 0x28,
  0x66, 0x20, 0x69, 0x20, 0x78, 0x73, 0x29, 0x0a, 0x09, 0x09, 0x28, 0x69,
  0x66, 0x20, 0x28, 0x3d, 0x20, 0x78, 0x73, 0x20, 0x6e, 0x69, 0x6c, 0x29,
  0x0a, 0x09, 0x09, 0x20, 0x20, 0x20, 0x20, 0x69, 0x0a, 0x09, 0x09, 0x20,
  0x20, 0x28, 0x66, 0x6f, 0x6c, 0x64, 0x6c, 0x20, 0x66, 0x20, 0x28, 0x66,
  0x20, 0x69, 0x20, 0x28, 0x63, 0x61, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29,
  0x20, 0x28, 0x63, 0x64, 0x72, 0x20, 0x78, 0x73, 0x29, 0x29, 0x29, 0x29,
  0x29, 0x0a, 0x0};
  
  char test2[] = "\"Hello Wööld\"";
  char dest[8192];
  char *res;
  uint32_t n;
  
  int orig_n = strlen(test0); 
  res = compression_compress(test0, &n);

  printf("original size: %d\n", orig_n);
  printf("compressed size bytes : %d\n", n);
  printf("comressed: %f\n", ((double)n) / (double)orig_n);
  
  compression_decompress(dest, 8192, res);
  printf("DECOMP: %s\n", dest);
  free(res);
  
  orig_n = strlen(test1); 
  res = compression_compress(test1, &n);

  printf("original size: %d\n", orig_n);
  printf("compressed size bytes : %d\n", n);
  printf("comressed: %f\n", ((double)n) / (double)orig_n);
  
  compression_decompress(dest, 8192, res);
  printf("DECOMP: %s\n", dest);
  free(res);
  
  
  orig_n = strlen(test2); 
  res = compression_compress(test2, &n);

  printf("original size: %d\n", orig_n);
  printf("compressed size bytes : %d\n", n);
  printf("comressed: %f\n", ((double)n) / (double)orig_n);
  
  compression_decompress(dest, 8192, res);
  printf("DECOMP: %s\n", dest);
  free(res);


  return 0;
}
