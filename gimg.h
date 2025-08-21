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

   gimgTypeKN1,
   gimgTypeKN2,
   gimgTypeKN4,
   gimgTypeKN8,
   gimgTypeKR4,
   gimgTypeKR8,
   gimgTypeKN1_AN1,
   gimgTypeKN2_AN2,
   gimgTypeKN4_AN4,
   gimgTypeKN8_AN8,
   gimgTypeKR4_AR4,
   gimgTypeKR8_AR8,
   gimgTypeRN1_GN1_BN1,          // 24 bit
   gimgTypeRN2_GN2_BN2,
   gimgTypeRN4_GN4_BN4,
   gimgTypeRN8_GN8_BN8,
   gimgTypeRR4_GR4_BR4,
   gimgTypeRR8_GR8_BR8,
   gimgTypeRN1_GN1_BN1_AN1,      // 32 bit
   gimgTypeRN2_GN2_BN2_AN2,
   gimgTypeRN4_GN4_BN4_AN4,
   gimgTypeRN8_GN8_BN8_AN8,
   gimgTypeRR4_GR4_BR4_AR4,
   gimgTypeRR8_GR8_BR8_AR8,
   gimgTypePOINTER
} GimgType;

typedef enum
{
   gimgValueK_K_INDEX   = 0,
   gimgValueK_A_INDEX   = 1,

   gimgValueRGB_R_INDEX = 0,
   gimgValueRGB_G_INDEX = 1,
   gimgValueRGB_B_INDEX = 2,
   gimgValueRGB_A_INDEX = 3,
};

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
   Gn1                n1[4];              // raw access.
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
#define gimgCloc(VALUE_TYPE, W, H)               (Gimg *) gleakCreate((void *) gimgCloc_(VALUE_TYPE, W, H), gsizeof(Gimg))

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
GIMG_API Gb              gimgGetPixel(                Gimg const * const img, Gindex const x, Gindex const y, GimgValue * const color);
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
GIMG_API Gb              gimgSetPixelColor(           Gimg       * const img, Gindex const x, Gindex const y, GimgValue const color);
GIMG_API Gb              gimgSetPixelA(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelB(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelDepth(           Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelG(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelMask(            Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelR(               Gimg       * const img, Gindex const x, Gindex const y, GimgValue const value);
GIMG_API Gb              gimgSetPixelValue(           Gimg       * const img, Gindex const x, Gindex const y, Gindex const valueIndex, GimgValue const value);
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

#define gimgColorCompileRGBA_N1(COLOR, R, G, B, A) \
(\
   (COLOR).n1[gimgValueRGB_R_INDEX] = R,\
   (COLOR).n1[gimgValueRGB_G_INDEX] = G,\
   (COLOR).n1[gimgValueRGB_B_INDEX] = B,\
   (COLOR).n1[gimgValueRGB_A_INDEX] = A,\
   (COLOR)\
)
#define gimgColorDecompileRGBA_N1(R, G, B, A, COLOR) \
{\
   R = (COLOR).n1[gimgValueRGB_R_INDEX];\
   G = (COLOR).n1[gimgValueRGB_G_INDEX];\
   B = (COLOR).n1[gimgValueRGB_B_INDEX];\
   A = (COLOR).n1[gimgValueRGB_A_INDEX];\
}
#define gimgColorDecompileRGBA_N1A(COLOR) (COLOR).n1[gimgValueRGB_A_INDEX]

#define gimgColorGetTransparent(COLOR) gimgColorCompileRGBA_N1(COLOR,   0,   0,   0,   0)
#define gimgColorGetBlack(COLOR)       gimgColorCompileRGBA_N1(COLOR,   0,   0,   0, 255)
#define gimgColorGetWhite(COLOR)       gimgColorCompileRGBA_N1(COLOR, 255, 255, 255, 255)
#define gimgColorGetGrayDark(COLOR)    gimgColorCompileRGBA_N1(COLOR,  64,  64,  64, 255)
#define gimgColorGetGray(COLOR)        gimgColorCompileRGBA_N1(COLOR, 128, 128, 128, 255)
#define gimgColorGetGrayLight(COLOR)   gimgColorCompileRGBA_N1(COLOR, 192, 192, 192, 255)
#define gimgColorGetRed(COLOR)         gimgColorCompileRGBA_N1(COLOR, 255,   0,   0, 255)
#define gimgColorGetOrange(COLOR)      gimgColorCompileRGBA_N1(COLOR, 255, 128,   0, 255)
#define gimgColorGetYellow(COLOR)      gimgColorCompileRGBA_N1(COLOR, 255, 255,   0, 255)
#define gimgColorGetGreenYellow(COLOR) gimgColorCompileRGBA_N1(COLOR, 128, 255,   0, 255)
#define gimgColorGetGreen(COLOR)       gimgColorCompileRGBA_N1(COLOR,   0, 255,   0, 255)
#define gimgColorGetGreenCyan(COLOR)   gimgColorCompileRGBA_N1(COLOR,   0, 255, 128, 255)
#define gimgColorGetCyan(COLOR)        gimgColorCompileRGBA_N1(COLOR,   0, 255, 255, 255)
#define gimgColorGetBlueCyan(COLOR)    gimgColorCompileRGBA_N1(COLOR,   0, 128, 255, 255)
#define gimgColorGetBlue(COLOR)        gimgColorCompileRGBA_N1(COLOR,   0,   0, 255, 255)
#define gimgColorGetBluePurple(COLOR)  gimgColorCompileRGBA_N1(COLOR, 128,   0, 255, 255)
#define gimgColorGetPurple(COLOR)      gimgColorCompileRGBA_N1(COLOR, 255,   0, 255, 255)
#define gimgColorGetRedPurple(COLOR)   gimgColorCompileRGBA_N1(COLOR, 255,   0, 128, 255)

#define gimgSetColorUNSAFE(IMAGE, X, Y, COLOR) \
   (IMAGE)->buffer[(Y) * (IMAGE)->width + (X)] = COLOR
#define gimgSetColor(IMAGE, X, Y, COLOR)    \
   if ((IMAGE)  &&                          \
       0 <= (X) && (X) < (IMAGE)->width &&  \
       0 <= (Y) && (Y) < (IMAGE)->height)   \
   {                                        \
      gimgSetColorUNSAFE(IMAGE, X, Y, COLOR); \
   }

/*************************************************************************************************/
#ifdef __cplusplus
}
#endif
/*************************************************************************************************/

#endif
