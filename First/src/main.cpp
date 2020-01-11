#include "tgaimage.h"
#include <stdlib.h>
#include <vector>
#include <cmath>
#include "model.h"
#include "geometry.h"
#include <iostream>
// #include <stdio.h>
const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red   = TGAColor(255, 0,   0,   255);
const TGAColor green   = TGAColor(0, 255,   0,   255);
Model *model = NULL;
const int width = 200;
const int height = 200;
void line2(int x0, int y0, int x1, int y1, TGAImage &img, TGAColor color){
    bool steep = false; 
    if (std::abs(x0-x1)<std::abs(y0-y1)) { 
        std::swap(x0, y0); 
        std::swap(x1, y1); 
        steep = true; 
    } 
    if (x0>x1) { 
        std::swap(x0, x1); 
        std::swap(y0, y1); 
    } 
    int dx = x1-x0; 
    int dy = y1-y0; 
    int derror2 = std::abs(dy)*2; 
    int error2 = 0; 
    int y = y0; 
    if(steep) {
        for(int x = x0; x<=x1; ++x) {
            img.set(y, x, color);
            error2 += derror2;
            if(error2 > dx) {
                y += (y1>y0? 1 : -1);
                error2 -= dx*2;
            }
        }
    } else {
        for(int x = x0; x<=x1; ++x) {
            img.set(x, y, color);
            error2 += derror2;
            if(error2 > dx) {
                y += (y1>y0? 1 : -1);
                error2 -= dx*2;
            }
        }
    }
}
void line(Vec2i t0, Vec2i t1, TGAImage &img, TGAColor color) { 
	int x0, y0, x1, y1;
	x0 = t0.x;
	y0 = t0.y;
	x1 = t1.x;
	y1 = t1.y;
    line2(t0.x, t0.y, t1.x, t1.y, img, color);
} 



void triangle(Vec2i t0, Vec2i t1, Vec2i t2, TGAImage &image, TGAColor color){
    
	if(t0.y>t1.y) std::swap(t0, t1);
	if(t0.y>t2.y) std::swap(t0, t2);
	if(t1.y>t2.y) std::swap(t1, t2);
	// line(t0, t1, image, green);
	// line(t1, t2, image, green);
	// line(t2, t0, image, red);
    int total_height = t2.y - t0.y;
    float invslope1 = (t1.x - t0.x) / (float)(t1.y - t0.y);
    float invslope2 = (t2.x - t0.x) / (float)(t2.y - t0.y);
    int len = t1.y - t0.y;
    for (int i = 0; i <= len; i++)
    {
        line2(t0.x + (int)(i * invslope1), t0.y + i, t0.x +(int)( i * invslope2 ), t0.y + i, image, color);
    }
    float invslope3 = (t0.x - t2.x)/(float)(t0.y - t2.y);
    float invslope4 = (t1.x - t2.x)/(float)(t1.y - t2.y);
    len = t2.y - t1.y;
    for (int i = 0; i < len; i++)
    {
        line2(t2.x - (int)(i*invslope3), t2.y-i, t2.x-(int)(i*invslope4), t2.y-i, image, color);
    }
    
}

Vec3f cross(Vec3f v1 ,Vec3f v2){
    return Vec3f(v1.y*v2.z-v1.z*v2.y,v1.z*v2.x*v1.x*v2.z,v1.x*v2.y-v1.y*v2.x);
}

Vec3f barycentric(Vec2i *pts, Vec2i p){
    Vec3f u = cross(Vec3f(pts[2].x-pts[0].x,pts[1].x-pts[0].x,pts[0].x-p.x),Vec3f(pts[2].y-pts[0].y,pts[1].y-pts[0].y,pts[0].y-p.y));
    if(abs(u.z)<1) {
        return Vec3f(-1,1,1);
    }
    return Vec3f(1.f-(u.x+u.y)/u.z,u.y/u.z,u.x/u.z);
}

int max(int x,int y){
    return x>y?x:y;
}

int min(int x,int y){
    return x>y?y:x;
}
void triangle2(Vec2i *pts, TGAImage &image, TGAColor color){
    Vec2i clamp(image.get_width()-1, image.get_height()-1);
    int minx,miny,maxx,maxy;
    minx=clamp.x;
    miny=clamp.y;
    maxx=0;
    maxy=0;
    for (int i = 0; i < 3; i++)
    {
        minx = min(minx,pts[i].x);
        maxx = max(maxx,pts[i].x);
        miny = min(miny,pts[i].y);
        maxy = max(maxy,pts[i].y);
    }
    std::cerr<<minx<<","<<miny<<";"<<maxx<<","<<maxy;
    Vec2i bboxmin = Vec2i(max(0,minx),max(0,maxx));
    Vec2i bboxmax = Vec2i(min(clamp.x,maxx),min(clamp.y,maxy));
    Vec2i p;
    for (p.x = bboxmin.x; p.x <= bboxmax.x; p.x++)
    {
        for (p.y = bboxmin.y; p.y <= bboxmax.y; p.y++)
        {
            Vec3f bc_screen = barycentric(pts,p);
            if(bc_screen.x<0 || bc_screen.y<0 || bc_screen.z<0) continue;
            image.set(p.x,p.y,color);
        }
    }
    
    
    
}



int main(int argc, char** argv) {
	TGAImage image(width, height, TGAImage::RGB);
    
    Model *model = new Model("D:/project/TinyRenderer/First/src/obj/african_head.obj");
    Vec2i screen_coords[3];
    Vec3f world_coords[3];
    Vec3f light_dir(0,0,-1);
    for (int i = 0; i < model->nfaces(); i++)
    {
        std::vector<int> face = model->face(i);
        Vec2i screen_corrds[3];
        for (int j = 0; j < 3; j++)
        {
            Vec3f v = model->vert(face[j]);
            screen_corrds[j] = Vec2i((v.x+1.)*width/2.,(v.y+1.)*height/2.);
            world_coords[j] = v;
        }
        //法线
        Vec3f n = (world_coords[2] - world_coords[0])^(world_coords[1]-world_coords[0]);
        n.normalize();
        float intensity = n*light_dir;

        if(intensity>0){
            triangle(screen_corrds[0],screen_corrds[1],screen_corrds[2],image,TGAColor(intensity*255,intensity*255,intensity*255,255));
        }   
    }
    
	image.flip_vertically();
	image.write_tga_file("output_color_face.tga" , false);
	system("Pause");
	return 0;
}


