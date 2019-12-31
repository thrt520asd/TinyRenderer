#include "tgaimage.h"

const TGAColor white = TGAColor(255,255,255,255);
const TGAColor red = TGAColor(255,0,0,255);

int main(int args, char** argv){
    TGAImage image(10, 10, TGAImage::RGB);
    // image.set(52, 41, red);
    image.flip_vertically();
    image.write_tga_file("out.tga");
    
    return 0;
}