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
typedef enum
{
   gimgTypeNONE,

   gimgTypeKN1,                  // Gray,           8 bit,  1 byte
   gimgTypeKN2,                  // Gray,          16 bit,  2 byte
   gimgTypeKN4,                  // Gray,          32 bit,  4 byte
   gimgTypeKN8,                  // Gray,          64 bit,  8 byte
   gimgTypeKR4,                  // Gray,                   4 byte
   gimgTypeKR8,                  // Gray,                   8 byte
   gimgTypeKN1_AN1,              // Gray  + Alhpa,          2 byte
   gimgTypeKN2_AN2,              // Gray  + Alhpa,          4 byte
   gimgTypeKN4_AN4,              // Gray  + Alhpa,          8 byte
   gimgTypeKN8_AN8,              // Gray  + Alhpa,         16 byte
   gimgTypeKR4_AR4,              // Gray  + Alhpa,          8 byte
   gimgTypeKR8_AR8,              // Gray  + Alhpa,         16 byte
   gimgTypeRN1_GN1_BN1,          // Color,         24 bit,  3 byte
   gimgTypeRN2_GN2_BN2,          // Color,                  6 byte
   gimgTypeRN4_GN4_BN4,          // Color,                 12 byte
   gimgTypeRN8_GN8_BN8,          // Color,                 24 byte
   gimgTypeRR4_GR4_BR4,          // Color,                 12 byte
   gimgTypeRR8_GR8_BR8,          // Color,                 24 byte
   gimgTypeRN1_GN1_BN1_AN1,      // Color + Alhpa, 32 bit,  4 byte
   gimgTypeRN2_GN2_BN2_AN2,      // Color + Alhpa,          8 byte
   gimgTypeRN4_GN4_BN4_AN4,      // Color + Alhpa,         16 byte
   gimgTypeRN8_GN8_BN8_AN8,      // Color + Alhpa,         32 byte
   gimgTypeRR4_GR4_BR4_AR4,      // Color + Alhpa,         16 byte
   gimgTypeRR8_GR8_BR8_AR8,      // Color + Alhpa,         32 byte
   gimgTypePOINTER
} GimgType;

typedef enum
{
   gimgValueIndexK_K   = 0,
   gimgValueIndexK_A   = 1,

   gimgValueIndexRGB_R = 0,
   gimgValueIndexRGB_G = 1,
   gimgValueIndexRGB_B = 2,
   gimgValueIndexRGB_A = 3,
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

typedef union
{
   Gn1                raw[32];            // raw access.
   Gn1                n1[4];
   Gn2                n2[4];
   Gn4                n4[4];
   Gn8                n8[4];
   Gr4                r4[4];
   Gr8                r8[4];

   Gn1                kn1[1];             // 0 = k
   Gn2                kn2[1];
   Gn4                kn4[1];
   Gn8                kn8[1];
   Gr4                kr4[1];
   Gr8                kr8[1];

   Gn1                kn1_an1[2];         // 0 = k, 1 = a
   Gn2                kn2_an2[2];
   Gn4                kn4_an4[2];
   Gn8                kn8_an8[2];
   Gr4                kr4_ar4[2];
   Gr8                kr8_ar8[2];

   Gn1                rn1_gn1_bn1[3];     // 0 = r, 1 = g, 2 = b
   Gn2                rn2_gn2_bn2[3];
   Gn4                rn4_gn4_bn4[3];
   Gn8                rn8_gn8_bn8[3];
   Gr4                rr4_gr4_br4[3];
   Gr8                rr8_gr8_br8[3];

   Gn1                rn1_gn1_bn1_an1[4]; // 0 = r, 1 = g, 2 = b, 3 = a
   Gn2                rn2_gn2_bn2_an2[4];
   Gn4                rn4_gn4_bn4_an4[4];
   Gn8                rn8_gn8_bn8_an8[4];
   Gr4                rr4_gr4_br4_ar4[4];
   Gr8                rr8_gr8_br8_ar8[4];

   Gp                *p[1];               // 0 = p
} GimgValue;

/**************************************************************************************************
prototype:
**************************************************************************************************/
#define gimgCloc(VALUE_TYPE, W, H)                 (Gimg *) gleakCreate((void *) gimgCloc_(VALUE_TYPE, W, H), gsizeof(Gimg))

#define gimgGAMMAf                                 2.2f
#define gimgGAMMA                                  2.2

#define gimgN1ToPercent(  VALUE)                   (((Gr4) (VALUE)) / ((Gr4) Gn1MAX))
#define gimgN2ToPercent(  VALUE)                   (((Gr4) (VALUE)) / ((Gr4) Gn2MAX))
#define gimgN4ToPercent(  VALUE)                   (((Gr8) (VALUE)) / ((Gr8) Gn4MAX))
#define gimgN8ToPercent(  VALUE)                   (((Gr8) (VALUE)) / ((Gr8) Gn8MAX))
#define gimgSRGBtoLinear( PERCENT_O_TO_1)          (((PERCENT_O_TO_1) <  0.04045f)   ? (PERCENT_O_TO_1) / 12.92f : (Gr4) pow(((PERCENT_O_TO_1) + 0.055) / 1.055, 2.4))
#define gimgLinearToSRGB( PERCENT_O_TO_1)          (((PERCENT_O_TO_1) <= 0.0031308f) ? (PERCENT_O_TO_1) * 12.92f : (Gr4) pow( (PERCENT_O_TO_1),                  1.0 / 2.4) * (1.055) - 0.055)
#define gimgGammaToLinear(PERCENT_O_TO_1, GAMMA)   (pow((PERCENT_O_TO_1),       (GAMMA)))
#define gimgLinearToGamma(PERCENT_O_TO_1, GAMMA)   (pow((PERCENT_O_TO_1), 1.0 / (GAMMA)))


GIMG_API Gb              gimgClipLine(                Gimg       * const img, Gindex * const x1, Gindex * const y1, Gindex * const x2, Gindex * const y2);
GIMG_API Gb              gimgClipLineTest(            Gindex const denom, Gindex const num, Gr8 * const u1, Gr8 * const u2);

GIMG_API Gimg           *gimgCloc_(                                         GimgType const value, Gcount const width, Gcount const height);
GIMG_API Gb              gimgClocContent(           Gimg       * const img, GimgType const value, Gcount const width, Gcount const height);
GIMG_API Gn1            *gimgClocBufferBGR8(        Gimg const * const img);
GIMG_API Gn1            *gimgClocBufferBGRA8(       Gimg const * const img);
GIMG_API Gn1            *gimgClocBufferRGB8(        Gimg const * const img);
GIMG_API Gn1            *gimgClocBufferRGBA8(       Gimg const * const img);

GIMG_API void            gimgDloc(                 Gimg       * const img);
GIMG_API void            gimgDlocContent(          Gimg       * const img);

         Gr4            *gimgGetGammaTable(           void);
GIMG_API Gcount          gimgGetHeight(               Gimg const * const img);
         Gr4            *gimgGetPercentTable(         void);
GIMG_API Gb              gimgGetValue(                Gimg const * const img, Gindex const x, Gindex const y, GimgValue * const color);
GIMG_API Gcount          gimgGetWidth(                Gimg const * const img);

GIMG_API Gb              gimgIsStarted(               void);
GIMG_API Gb              gimgInit(                    Gimg       * const img);

GIMG_API Gimg           *gimgLoad(                    Gpath const * const filePath);

GIMG_API Gb              gimgResizeFixed(             Gimg const * const simage, Gimg * const dimage);

GIMG_API Gb              gimgSetCircle(               Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetCircleFill(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetClear(                Gimg       * const img, GimgValue const color);
GIMG_API Gb              gimgSetImage(                Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const value);
GIMG_API Gb              gimgSetImageAlpha(           Gimg       * const img, Gindex const x, Gindex const y, Gimg const * const value);
GIMG_API Gb              gimgSetLineH(                Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetLineHAlpha(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color);
GIMG_API Gb              gimgSetLineHInterpolate(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetLineV(                Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetLineVAlpha(           Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetLineVInterpolate(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetPixelColor(           Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelA(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelB(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelDepth(           Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelG(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelMask(            Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelR(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetValue(                Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetRect(                 Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectAlpha(            Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFill(             Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFillAlpha(        Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color);
GIMG_API Gb              gimgSetRectFillInterpolate(  Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const colortl, GimgValue const colortr, GimgValue const colorbl, GimgValue const colorbr);
GIMG_API Gb              gimgSetRectFillInterpolateH( Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetRectFillInterpolateV( Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetRectInterpolate(      Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const colortl, GimgValue const colortr, GimgValue const colorbl, GimgValue const colorbr);
GIMG_API Gb              gimgSetRectInterpolateH(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetRectInterpolateV(     Gimg       * const img, Gindex const x, Gindex const y, Gcount const w, Gcount const h, GimgValue const color1, GimgValue const color2);
GIMG_API Gb              gimgSetSize(                 Gimg       * const img, Gcount const width, Gcount const height);
GIMG_API Gb              gimgStart(                   void);
GIMG_API Gb              gimgStop(                    void);
GIMG_API Gb              gimgStore(                   Gimg const * const img, Gpath const * const filePath, Gr const compression);
GIMG_API Gb              gimgSwapColor(               Gimg       * const img, GimgValue const originalColor, GimgValue const newColor);

#define gimgValueSET_RGBA_N1(COLOR, R, G, B, A) \
(\
   (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_R] = (Gn1) (R),\
   (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_G] = (Gn1) (G),\
   (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_B] = (Gn1) (B),\
   (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_A] = (Gn1) (A),\
   (COLOR)\
)
#define gimgValueGET_RGBA_N1(R, G, B, A, COLOR) \
{\
   R = (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_R];\
   G = (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_G];\
   B = (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_B];\
   A = (COLOR).rn1_gn1_bn1_an1[gimgValueIndexRGB_A];\
}
#define gimgValueDecompileRGBA_N1A(COLOR) (COLOR).n1[gimgValueRGB_A_INDEX]

#define gimgValueGetTransparent(COLOR) gimgValueCompileRGBA_N1(COLOR,   0,   0,   0,   0)
#define gimgValueGetBlack(COLOR)       gimgValueCompileRGBA_N1(COLOR,   0,   0,   0, 255)
#define gimgValueGetWhite(COLOR)       gimgValueCompileRGBA_N1(COLOR, 255, 255, 255, 255)
#define gimgValueGetGrayDark(COLOR)    gimgValueCompileRGBA_N1(COLOR,  64,  64,  64, 255)
#define gimgValueGetGray(COLOR)        gimgValueCompileRGBA_N1(COLOR, 128, 128, 128, 255)
#define gimgValueGetGrayLight(COLOR)   gimgValueCompileRGBA_N1(COLOR, 192, 192, 192, 255)
#define gimgValueGetRed(COLOR)         gimgValueCompileRGBA_N1(COLOR, 255,   0,   0, 255)
#define gimgValueGetOrange(COLOR)      gimgValueCompileRGBA_N1(COLOR, 255, 128,   0, 255)
#define gimgValueGetYellow(COLOR)      gimgValueCompileRGBA_N1(COLOR, 255, 255,   0, 255)
#define gimgValueGetGreenYellow(COLOR) gimgValueCompileRGBA_N1(COLOR, 128, 255,   0, 255)
#define gimgValueGetGreen(COLOR)       gimgValueCompileRGBA_N1(COLOR,   0, 255,   0, 255)
#define gimgValueGetGreenCyan(COLOR)   gimgValueCompileRGBA_N1(COLOR,   0, 255, 128, 255)
#define gimgValueGetCyan(COLOR)        gimgValueCompileRGBA_N1(COLOR,   0, 255, 255, 255)
#define gimgValueGetBlueCyan(COLOR)    gimgValueCompileRGBA_N1(COLOR,   0, 128, 255, 255)
#define gimgValueGetBlue(COLOR)        gimgValueCompileRGBA_N1(COLOR,   0,   0, 255, 255)
#define gimgValueGetBluePurple(COLOR)  gimgValueCompileRGBA_N1(COLOR, 128,   0, 255, 255)
#define gimgValueGetPurple(COLOR)      gimgValueCompileRGBA_N1(COLOR, 255,   0, 255, 255)
#define gimgValueGetRedPurple(COLOR)   gimgValueCompileRGBA_N1(COLOR, 255,   0, 128, 255)

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
/*************************************************************************************************/

#endif
