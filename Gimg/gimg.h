/**************************************************************************************************

file:         Gimg
author:       Robbert de Groot
copyright:    2012-2025, Robbert de Groot

description:
Generic image library.  This library makes images that are at least 8 bit gray,
or 8 bit RGB(A).  It does not do 1 bit black and white, or palettized images.  You
will have to layer on top of this library to simulate those using a gray or
RGB 8 bit image.

This library can be used to simulate a depth buffer.  You would create a Gray scale image to use as
the depth buffer.

This library can be used to simulate a mask buffer.  You would create a Gray scale image to use as
the mask buffer.

This library can be used to create a 2 dimensional array of any specific values as long as they can
be represented by the value types.

gimgTypePOINTER is a special value.  If you have a 2 dimensional array of strings then you would
probably use a name table and insert the name table strings into this image.

**************************************************************************************************/

#if !defined(Gimg_HEADER)
#define      Gimg_HEADER

/**************************************************************************************************
include:
**************************************************************************************************/
// GWin is a static library normally.  Just in case someone wants it to be a dll.
#if defined(GIMG_IS_A_DYNAMIC_LIBRARY)
#if defined(GIMG_EXPORTS)
#define GIMG_API __declspec(dllexport)
#else defined(GIMG_IMPORTS)
#define GIMG_API __declspec(dllimport)
#endif
#else
#if defined(CPP)
#define GIMG_API extern "C"
#else
#define GIMG_API
#endif
#endif

/*************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif
/*************************************************************************************************/

/**************************************************************************************************
type:
**************************************************************************************************/
// N = Natural number.     0 - max value of the byte count.
//    1 byte = 0 - 255
//    2 byte = 0 - 65,535
//    4 byte = 0 - 4,294,967,295
//    8 byte = 0 - 18,446,744,073,709,551,615
// F = Fixed point number. 0 - 1.  Number of decimals depends on the byte count.
//    1 byte = 0.00        - 1.00                            (100 steps)
//    2 byte = 0.0000      - 1.0000                          (10,000 steps)
//    4 byte = 0.000000000 - 1.000000000                     (1,000,000,000 steps)
//    8 byte = 0.0000000000000000000 - 1.0000000000000000000 (10,000,000,000,000,000,000 steps)
// R = Real number.     -MAX - MAX.  Number of decimals standard for single and double precision.
//    Only option that can do HDR ranges where values can go below 0 and above 1.
typedef enum
{
   gimgTypeNONE,

   // What chanels are we storing.
   gimgTypeA            = 0x00000001,
   gimgTypeK            = 0x00000010,
   gimgTypeRGB          = 0x00011100,

   gimgTypeN            = 0x10000000,
   gimgTypeF            = 0x20000000,
   gimgTypeR            = 0x80000000,

   gimgType1            = 0x01000000,
   gimgType2            = 0x02000000,
   gimgType4            = 0x04000000,
   gimgType8            = 0x08000000,

   gimgTypeK_N1         = gimgTypeK   |             gimgTypeN | gimgType1,   // Gray,                   1 byte
   gimgTypeK_N2         = gimgTypeK   |             gimgTypeN | gimgType2,   // Gray,                   2 byte
   gimgTypeK_N4         = gimgTypeK   |             gimgTypeN | gimgType4,   // Gray,                   4 byte
   gimgTypeK_N8         = gimgTypeK   |             gimgTypeN | gimgType8,   // Gray,                   8 byte
   gimgTypeK_F1         = gimgTypeK   |             gimgTypeF | gimgType1,   // Gray,                   1 byte
   gimgTypeK_F2         = gimgTypeK   |             gimgTypeF | gimgType2,   // Gray,                   2 byte
   gimgTypeK_F4         = gimgTypeK   |             gimgTypeF | gimgType4,   // Gray,                   4 byte
   gimgTypeK_F8         = gimgTypeK   |             gimgTypeF | gimgType8,   // Gray,                   8 byte
   gimgTypeK_R4         = gimgTypeK   |             gimgTypeR | gimgType4,   // Gray,                   4 byte
   gimgTypeK_R8         = gimgTypeK   |             gimgTypeR | gimgType8,   // Gray,                   8 byte
   gimgTypeKA_N1        = gimgTypeK   | gimgTypeA | gimgTypeN | gimgType1,   // Gray  + Alhpa,          2 byte
   gimgTypeKA_N2        = gimgTypeK   | gimgTypeA | gimgTypeN | gimgType2,   // Gray  + Alhpa,          4 byte
   gimgTypeKA_N4        = gimgTypeK   | gimgTypeA | gimgTypeN | gimgType4,   // Gray  + Alhpa,          8 byte
   gimgTypeKA_N8        = gimgTypeK   | gimgTypeA | gimgTypeN | gimgType8,   // Gray  + Alhpa,         16 byte
   gimgTypeKA_F1        = gimgTypeK   | gimgTypeA | gimgTypeF | gimgType1,   // Gray  + Alhpa,          2 byte
   gimgTypeKA_F2        = gimgTypeK   | gimgTypeA | gimgTypeF | gimgType2,   // Gray  + Alhpa,          4 byte
   gimgTypeKA_F4        = gimgTypeK   | gimgTypeA | gimgTypeF | gimgType4,   // Gray  + Alhpa,          8 byte
   gimgTypeKA_F8        = gimgTypeK   | gimgTypeA | gimgTypeF | gimgType8,   // Gray  + Alhpa,         16 byte
   gimgTypeKA_R4        = gimgTypeK   | gimgTypeA | gimgTypeR | gimgType4,   // Gray  + Alhpa,          8 byte
   gimgTypeKA_R8        = gimgTypeK   | gimgTypeA | gimgTypeR | gimgType8,   // Gray  + Alhpa,         16 byte
   gimgTypeRGB_N1       = gimgTypeRGB |             gimgTypeN | gimgType1,   // Color,         24 bit,  3 byte
   gimgTypeRGB_N2       = gimgTypeRGB |             gimgTypeN | gimgType2,   // Color,                  6 byte
   gimgTypeRGB_N4       = gimgTypeRGB |             gimgTypeN | gimgType4,   // Color,                 12 byte
   gimgTypeRGB_N8       = gimgTypeRGB |             gimgTypeN | gimgType8,   // Color,                 24 byte
   gimgTypeRGB_F1       = gimgTypeRGB |             gimgTypeF | gimgType1,   // Color,                  3 byte
   gimgTypeRGB_F2       = gimgTypeRGB |             gimgTypeF | gimgType2,   // Color,                  6 byte
   gimgTypeRGB_F4       = gimgTypeRGB |             gimgTypeF | gimgType4,   // Color,                 12 byte
   gimgTypeRGB_F8       = gimgTypeRGB |             gimgTypeF | gimgType8,   // Color,                 24 byte
   gimgTypeRGB_R4       = gimgTypeRGB |             gimgTypeR | gimgType4,   // Color,                 12 byte
   gimgTypeRGB_R8       = gimgTypeRGB |             gimgTypeR | gimgType8,   // Color,                 24 byte
   gimgTypeRGBA_N1      = gimgTypeRGB | gimgTypeA | gimgTypeN | gimgType1,   // Color + Alhpa, 32 bit,  4 byte
   gimgTypeRGBA_N2      = gimgTypeRGB | gimgTypeA | gimgTypeN | gimgType2,   // Color + Alhpa,          8 byte
   gimgTypeRGBA_N4      = gimgTypeRGB | gimgTypeA | gimgTypeN | gimgType4,   // Color + Alhpa,         16 byte
   gimgTypeRGBA_N8      = gimgTypeRGB | gimgTypeA | gimgTypeN | gimgType8,   // Color + Alhpa,         32 byte
   gimgTypeRGBA_F1      = gimgTypeRGB | gimgTypeA | gimgTypeF | gimgType1,   // Color + Alhpa,          4 byte
   gimgTypeRGBA_F2      = gimgTypeRGB | gimgTypeA | gimgTypeF | gimgType2,   // Color + Alhpa,          8 byte
   gimgTypeRGBA_F4      = gimgTypeRGB | gimgTypeA | gimgTypeF | gimgType4,   // Color + Alhpa,         16 byte
   gimgTypeRGBA_F8      = gimgTypeRGB | gimgTypeA | gimgTypeF | gimgType8,   // Color + Alhpa,         32 byte
   gimgTypeRGBA_R4      = gimgTypeRGB | gimgTypeA | gimgTypeR | gimgType4,   // Color + Alhpa,         16 byte
   gimgTypeRGBA_R8      = gimgTypeRGB | gimgTypeA | gimgTypeR | gimgType8,   // Color + Alhpa,         32 byte
} GimgType;

typedef enum
{
   gimgValueIndexK_K    = 0,
   gimgValueIndexK_A    = 1,

   gimgValueIndexRGB_R  = 0,
   gimgValueIndexRGB_G  = 1,
   gimgValueIndexRGB_B  = 2,
   gimgValueIndexRGB_A  = 3,
} GimgValueIndex;

typedef struct _Gimg Gimg;
struct _Gimg
{
   GTYPE_VAR

   GimgType           valueType;
   Gcount             valueByteCount,
                      rowByteCount;
   Gcount             height;
   Gcount             width;
   Gn1               *valueData;
};

#pragma pack(push, 1) // byte alignment.
typedef union
{
   Gn1                k;
} GimgValueK_N1;

typedef union
{
   Gn1                k,
                      a;
} GimgValueKA_N1;

typedef union
{
   Gn1                r,
                      g,
                      b;
} GimgValueRGB_N1;

typedef union
{
   Gn1                r,
                      g,
                      b,
                      a;
} GimgValueRGBA_N1;

typedef union
{
   Gn1                raw[32];            // raw access.
   Gn1                n1[4];
   Gn2                n2[4];
   Gn4                n4[4];
   Gn8                n8[4];
   Gn1                f1[4];
   Gn2                f2[4];
   Gn4                f4[4];
   Gn8                f8[4];
   Gr4                r4[4];
   Gr8                r8[4];

   GimgValueK_N1      k;
   GimgValueKA_N1     ka;
   GimgValueRGB_N1    rgb;
   GimgValueRGBA_N1   rgba;
} GimgValue;
#pragma pack(pop) // restore alignment.

/**************************************************************************************************
prototype:
**************************************************************************************************/
#define gimgCloc(VALUE_TYPE, W, H)                 (Gimg *) gleakCloc((void *) gimgCloc_(VALUE_TYPE, W, H), gsizeof(Gimg))

#define gimgGAMMAf                                 2.2f
#define gimgGAMMA                                  2.2

GIMG_API Gb              gimgClipLine(                Gimg       * const img, Gindex * const x1, Gindex * const y1, Gindex * const x2, Gindex * const y2);
GIMG_API Gb              gimgClipLineTest(            Gindex const denom, Gindex const num, Gr * const u1, Gr * const u2);

GIMG_API Gimg           *gimgCloc_(                                           GimgType const value, Gcount const width, Gcount const height);
GIMG_API Gimg           *gimgClocClone(               Gimg const * const img);
GIMG_API Gb              gimgClocContent(             Gimg       * const img, GimgType const value, Gcount const width, Gcount const height);

GIMG_API void            gimgDloc(                    Gimg       * const img);
GIMG_API void            gimgDlocContent(             Gimg       * const img);

GIMG_API Gcount          gimgGetHeight(               Gimg const * const img);
GIMG_API Gb              gimgGetValue(                Gimg const * const img, Gindex const x, Gindex const y, GimgValue * const color);
GIMG_API Gcount          gimgGetWidth(                Gimg const * const img);

GIMG_API Gb              gimgIsStarted(               void);

GIMG_API Gb              gimgSetCircle(               Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetCircleFill(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetClear(                Gimg       * const img, GimgValue const color);
GIMG_API Gb              gimgSetImage(                Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const valueImg);
GIMG_API Gb              gimgSetImage1(               Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const valueImg);
GIMG_API Gb              gimgSetImage1Blend(          Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const valueImg);
GIMG_API Gb              gimgSetImage1BlendA(         Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const valueImg);
GIMG_API Gb              gimgSetLineH(                Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetLineHAlpha(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetLineHInterpolate(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetLineV(                Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetLineVAlpha(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetLineVInterpolate(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetRect(                 Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectBlend(            Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFill(             Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFillBlend(        Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFillInterpolateH( Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetRectFillInterpolateV( Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetResizeFixed_1(        Gimg       * const img, Gimg const * const valueImg);
GIMG_API Gb              gimgSetValue(                Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgStart(                   void);
GIMG_API Gb              gimgStop(                    void);
GIMG_API Gb              gimgSwapColor(               Gimg       * const img, GimgValue const originalColor, GimgValue const newColor);

#define gimgValueGET_K_N1(COLOR, K)                                                                \
{                                                                                                  \
   K = (COLOR).n1[gimgValueIndexK_K];                                                              \
}

#define gimgValueSET_K_N1(COLOR, K)                                                                \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexK_K] = (Gn1) (K),                                                      \
   (COLOR)                                                                                         \
)

#define gimgValueGET_KA_N1(COLOR, K, A)                                                            \
{                                                                                                  \
   K = (COLOR).n1[gimgValueIndexK_K];                                                              \
   A = (COLOR).n1[gimgValueIndexK_A];                                                              \
}

#define gimgValueSET_KA_N1(COLOR, K, A)                                                            \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexK_K] = (Gn1) (K),                                                      \
   (COLOR).n1[gimgValueIndexK_A] = (Gn1) (A),                                                      \
   (COLOR)                                                                                         \
)

#define gimgValueGET_RGB_N1(COLOR, R, G, B)                                                        \
{                                                                                                  \
   R = (COLOR).n1[gimgValueIndexRGB_R];                                                            \
   G = (COLOR).n1[gimgValueIndexRGB_G];                                                            \
   B = (COLOR).n1[gimgValueIndexRGB_B];                                                            \
}

#define gimgValueSET_RGB_N1(COLOR, R, G, B)                                                        \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexRGB_R] = (Gn1) (R),                                                    \
   (COLOR).n1[gimgValueIndexRGB_G] = (Gn1) (G),                                                    \
   (COLOR).n1[gimgValueIndexRGB_B] = (Gn1) (B),                                                    \
   (COLOR)                                                                                         \
)

#define gimgValueGET_RGBA_N1(COLOR, R, G, B, A)                                                    \
{                                                                                                  \
   R = (COLOR).n1[gimgValueIndexRGB_R];                                                            \
   G = (COLOR).n1[gimgValueIndexRGB_G];                                                            \
   B = (COLOR).n1[gimgValueIndexRGB_B];                                                            \
   A = (COLOR).n1[gimgValueIndexRGB_A];                                                            \
}

#define gimgValueSET_RGBA_N1(COLOR, R, G, B, A)                                                    \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexRGB_R] = (Gn1) (R),                                                    \
   (COLOR).n1[gimgValueIndexRGB_G] = (Gn1) (G),                                                    \
   (COLOR).n1[gimgValueIndexRGB_B] = (Gn1) (B),                                                    \
   (COLOR).n1[gimgValueIndexRGB_A] = (Gn1) (A),                                                    \
   (COLOR)                                                                                         \
)

#define gimgValueSET_K_N1_BLACK(       COLOR)      gimgValueSET_K_N1(  COLOR,   0)
#define gimgValueSET_K_N1_GRAYDARK(    COLOR)      gimgValueSET_K_N1(  COLOR,  64)
#define gimgValueSET_K_N1_GRAY(        COLOR)      gimgValueSET_K_N1(  COLOR, 128)
#define gimgValueSET_K_N1_GRAYLIGHT(   COLOR)      gimgValueSET_K_N1(  COLOR, 192)
#define gimgValueSET_K_N1_WHITE(       COLOR)      gimgValueSET_K_N1(  COLOR, 255)

#define gimgValueSET_KA_N1_TRANSPARENT(COLOR)      gimgValueSET_KA_N1(  COLOR,   0,   0)
#define gimgValueSET_KA_N1_BLACK(      COLOR)      gimgValueSET_KA_N1(  COLOR,   0, 255)
#define gimgValueSET_KA_N1_GRAYDARK(   COLOR)      gimgValueSET_KA_N1(  COLOR,  64, 255)
#define gimgValueSET_KA_N1_GRAY(       COLOR)      gimgValueSET_KA_N1(  COLOR, 128, 255)
#define gimgValueSET_KA_N1_GRAYLIGHT(  COLOR)      gimgValueSET_KA_N1(  COLOR, 192, 255)
#define gimgValueSET_KA_N1_WHITE(      COLOR)      gimgValueSET_KA_N1(  COLOR, 255, 255)

#define gimgValueSET_RGB_N1_BLACK(       COLOR)    gimgValueSET_RGB_N1( COLOR,   0,   0,   0)
#define gimgValueSET_RGB_N1_WHITE(       COLOR)    gimgValueSET_RGB_N1( COLOR, 255, 255, 255)
#define gimgValueSET_RGB_N1_GRAYDARK(    COLOR)    gimgValueSET_RGB_N1( COLOR,  64,  64,  64)
#define gimgValueSET_RGB_N1_GRAY(        COLOR)    gimgValueSET_RGB_N1( COLOR, 128, 128, 128)
#define gimgValueSET_RGB_N1_GRAYLIGHT(   COLOR)    gimgValueSET_RGB_N1( COLOR, 192, 192, 192)
#define gimgValueSET_RGB_N1_RED(         COLOR)    gimgValueSET_RGB_N1( COLOR, 255,   0,   0)
#define gimgValueSET_RGB_N1_ORANGE(      COLOR)    gimgValueSET_RGB_N1( COLOR, 255, 128,   0)
#define gimgValueSET_RGB_N1_YELLOW(      COLOR)    gimgValueSET_RGB_N1( COLOR, 255, 255,   0)
#define gimgValueSET_RGB_N1_GREENYELLOW( COLOR)    gimgValueSET_RGB_N1( COLOR, 128, 255,   0)
#define gimgValueSET_RGB_N1_GREEN(       COLOR)    gimgValueSET_RGB_N1( COLOR,   0, 255,   0)
#define gimgValueSET_RGB_N1_GREENCYAN(   COLOR)    gimgValueSET_RGB_N1( COLOR,   0, 255, 128)
#define gimgValueSET_RGB_N1_CYAN(        COLOR)    gimgValueSET_RGB_N1( COLOR,   0, 255, 255)
#define gimgValueSET_RGB_N1_BLUECYAN(    COLOR)    gimgValueSET_RGB_N1( COLOR,   0, 128, 255)
#define gimgValueSET_RGB_N1_BLUE(        COLOR)    gimgValueSET_RGB_N1( COLOR,   0,   0, 255)
#define gimgValueSET_RGB_N1_BLUEPURPLE(  COLOR)    gimgValueSET_RGB_N1( COLOR, 128,   0, 255)
#define gimgValueSET_RGB_N1_PURPLE(      COLOR)    gimgValueSET_RGB_N1( COLOR, 255,   0, 255)
#define gimgValueSET_RGB_N1_REDPURPLE(   COLOR)    gimgValueSET_RGB_N1( COLOR, 255,   0, 128)

#define gimgValueSET_RGBA_N1_TRANSPARENT(COLOR)    gimgValueSET_RGBA_N1(COLOR,   0,   0,   0,   0)
#define gimgValueSET_RGBA_N1_BLACK(      COLOR)    gimgValueSET_RGBA_N1(COLOR,   0,   0,   0, 255)
#define gimgValueSET_RGBA_N1_WHITE(      COLOR)    gimgValueSET_RGBA_N1(COLOR, 255, 255, 255, 255)
#define gimgValueSET_RGBA_N1_GRAYDARK(   COLOR)    gimgValueSET_RGBA_N1(COLOR,  64,  64,  64, 255)
#define gimgValueSET_RGBA_N1_GRAY(       COLOR)    gimgValueSET_RGBA_N1(COLOR, 128, 128, 128, 255)
#define gimgValueSET_RGBA_N1_GRAYLIGHT(  COLOR)    gimgValueSET_RGBA_N1(COLOR, 192, 192, 192, 255)
#define gimgValueSET_RGBA_N1_RED(        COLOR)    gimgValueSET_RGBA_N1(COLOR, 255,   0,   0, 255)
#define gimgValueSET_RGBA_N1_ORANGE(     COLOR)    gimgValueSET_RGBA_N1(COLOR, 255, 128,   0, 255)
#define gimgValueSET_RGBA_N1_YELLOW(     COLOR)    gimgValueSET_RGBA_N1(COLOR, 255, 255,   0, 255)
#define gimgValueSET_RGBA_N1_GREENYELLOW(COLOR)    gimgValueSET_RGBA_N1(COLOR, 128, 255,   0, 255)
#define gimgValueSET_RGBA_N1_GREEN(      COLOR)    gimgValueSET_RGBA_N1(COLOR,   0, 255,   0, 255)
#define gimgValueSET_RGBA_N1_GREENCYAN(  COLOR)    gimgValueSET_RGBA_N1(COLOR,   0, 255, 128, 255)
#define gimgValueSET_RGBA_N1_CYAN(       COLOR)    gimgValueSET_RGBA_N1(COLOR,   0, 255, 255, 255)
#define gimgValueSET_RGBA_N1_BLUECYAN(   COLOR)    gimgValueSET_RGBA_N1(COLOR,   0, 128, 255, 255)
#define gimgValueSET_RGBA_N1_BLUE(       COLOR)    gimgValueSET_RGBA_N1(COLOR,   0,   0, 255, 255)
#define gimgValueSET_RGBA_N1_BLUEPURPLE( COLOR)    gimgValueSET_RGBA_N1(COLOR, 128,   0, 255, 255)
#define gimgValueSET_RGBA_N1_PURPLE(     COLOR)    gimgValueSET_RGBA_N1(COLOR, 255,   0, 255, 255)
#define gimgValueSET_RGBA_N1_REDPURPLE(  COLOR)    gimgValueSET_RGBA_N1(COLOR, 255,   0, 128, 255)

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
/*************************************************************************************************/

#endif
