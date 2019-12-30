#ifndef _IMAGE_H__
#define _IMAGE_H__

#include <fstream>

#pragma pack(push,1)
struct TGA_Header{
    char idLength;//图像信息字段（见本子表的后面）的字符数。本字段是1字节无符号整型，指出了图像格式区别字段长度，其取值范围是0到255，当它为0时表示没有图像的信息字段
    char colormaptype;//颜色表的类型。该字段为表示对应格式1的图像而包含一个二进制1
    char datatypecode;//图像类型码。该字段总为1，这也是此类型为格式1的原因
    short colormaporigin;//颜色表规格，颜色表首址
    short colormaplength;//颜色表首元表项的整型（低位-高位）索引
    char colormapdepth;//颜色表的长度。颜色表的表项总数，整型（低位-高位）
    short x_origin;//图像X坐标的起始位置。图像左下角X坐标的整型（低位-高位）值
    short y_origin;//图像Y坐标的起始位置。图像左下角Y坐标的整型（低位-高位）值
    short width;//图像宽度。以像素为单位，图像宽度的整型（低位-高位）值
    short height;//图像高度。以像素为单位，图像高度的整型（低位-高位）值
    short bitsperpixel;//图像每像素存储占用位（bit）数
    char imagedescriptor;/*
    图像描述符字节
bits 3~0：每像素的属性位（bit）数
bit 4：保留，必须为0
bit 5：屏幕起始位置标志
0：原点在左下角
1：原点在左上角
Truevision图像必须为0bits 7~6：交叉数据存储标志
00：无交叉
01：两路奇/偶交叉
10：四路交叉
11：保留
    */
};
#pragma pack(pop)

struct TGAColor{
    union{
        struct 
        {
            unsigned char b,g,r,a;
        };
        unsigned char raw[4];
        unsigned int val;
    };
    int bytespp;
    TGAColor(): val(0),bytespp(1){

    }
    TGAColor(unsigned char R, unsigned char G,unsigned char B , unsigned char A):b(B),g(G),r(R),a(A),bytespp(4){

    }
    TGAColor(int v, int bpp):val(v) , bytespp(bpp){

    }
    TGAColor(const TGAColor &c):val(c.val),bytespp(c.bytespp){

    }
    TGAColor(const unsigned char *p , int bpp):val(0),bytespp(bpp){
        for (int i = 0; i < bpp; i++)
        {
            raw[i] = p[i];
        }
    }
    TGAColor & operator = (const TGAColor &c){
        if(this != &c){
            bytespp = c.bytespp;
            val = c.val;
        }
        return *this;
    }
};

class TGAImage{
    protected:
            unsigned char* data;
            int width;
            int height;
            int bytespp;

            bool load_rle_data(std::ifstream &in);
            bool unload_rle_data(std::ofstream &out);
    public:
            enum Format{
                GRAYSCALE = 1,RGB=3,RGBA=4
            };
            TGAImage();
            TGAImage(int w,int h,int bpp);
            TGAImage(const TGAImage &img);
            bool read_tga_file(const char *filename);
            bool write_tga_file(const char *firlename,bool rle=true);
            bool flip_horizontally();
            bool flip_vertically();
            bool scale(int w , int h);
            TGAColor get(int x , int y);
            bool set(int x , int y ,TGAColor c);
            ~TGAImage();
            TGAImage & operator = (const TGAImage &image);
            int get_width();
            int get_height();
            int get_bytespp();
            unsigned char *buffer();
            void clear();
};

#endif //__IMAGE_H_