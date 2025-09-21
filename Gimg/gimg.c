/**************************************************************************************************

file:         Gimg
author:       Robbert de Groot
copyright:    2012-2012, Robbert de Groot

description:
Data structure for images.

**************************************************************************************************/

/**************************************************************************************************
include:
**************************************************************************************************/
#include "precompiled.h"

/**************************************************************************************************
local:
macro:
**************************************************************************************************/
#define GET_PIXEL_POINTER(IMG, X, Y)                                                               \
   (IMG)->valueData + (Y) * (IMG)->rowByteCount + (X) * (IMG)->valueByteCount

#define SET_PIXEL_VALUE(IMG, X, Y, COLOR)                                                          \
   SET_VALUE(                                                                                      \
      GET_PIXEL_POINTER(IMG, X, Y),                                                                \
      (IMG)->valueByteCount,                                                                       \
      COLOR);

#define SET_VALUE(PIXEL, COLOR_BYTE_COUNT, COLOR)                                                  \
   {                                                                                               \
      Gn1   *___pixel___ = (PIXEL);                                                                \
      Gn1   *___color___ = (Gn1 *) (COLOR).raw;                                                    \
      switch (COLOR_BYTE_COUNT)                                                                    \
      {                                                                                            \
      case 32: (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
      case 24: (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
      case 16: (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
      case 12: (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
      case 8:  (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
               (*___pixel___++) = (*___color___++);                                                \
      case 4:  (*___pixel___++) = (*___color___++);                                                \
      case 3:  (*___pixel___++) = (*___color___++);                                                \
      case 2:  (*___pixel___++) = (*___color___++);                                                \
      default: (*___pixel___++) = (*___color___++);                                                \
      }                                                                                            \
   }

#define GET_PIXEL_VALUE(IMG, X, Y, COLOR)                                                          \
   GET_VALUE(                                                                                      \
      GET_PIXEL_POINTER(IMG, X, Y),                                                                \
      (IMG)->valueByteCount,                                                                       \
      COLOR)

#define GET_VALUE(PIXEL, COLOR_BYTE_COUNT, COLOR)                                                  \
   {                                                                                               \
      Gn1   *___pixel___ = (PIXEL);                                                                \
      Gn1   *___color___ = (COLOR).raw;                                                            \
      switch (COLOR_BYTE_COUNT)                                                                    \
      {                                                                                            \
      case 32: (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
      case 24: (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
      case 16: (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
      case 12: (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
      case 8:  (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
               (*___color___++) = (*___pixel___++);                                                \
      case 4:  (*___color___++) = (*___pixel___++);                                                \
      case 3:  (*___color___++) = (*___pixel___++);                                                \
      case 2:  (*___color___++) = (*___pixel___++);                                                \
      default: (*___color___++) = (*___pixel___++);                                                \
      }                                                                                            \
   }

// Changing the GimgValue from the type value to an RGBA value.
#define VALUE_TYPE_TO_RGBA_N1(TYPE, COLOR)                                                         \
   {                                                                                               \
      switch (TYPE)                                                                                \
      {                                                                                            \
      default:                                                                                     \
      case gimgTypeRGBA_N1:                                                                        \
         /* Nothing to do */                                                                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeK_N1:                                                                           \
         /* Need to set A to 255. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_G]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = (COLOR).n1[gimgValueIndexK_K];                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeK_F1:                                                                           \
         /* Need to set A to 255. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_R]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_G] =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = _fToNTableN1[(COLOR).n1[gimgValueIndexK_K]];         \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKA_N1:                                                                          \
         /* Need to move A first. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = (COLOR).n1[gimgValueIndexK_A];                       \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_G]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = (COLOR).n1[gimgValueIndexK_K];                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKA_F1:                                                                          \
         /* Need to move A first. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = _fToNTableN1[(COLOR).n1[gimgValueIndexK_A]];         \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_R]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_G] =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = _fToNTableN1[(COLOR).n1[gimgValueIndexK_K]];         \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRGB_N1:                                                                         \
         /* Need to set A to 255. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         /* RGB are in the same location as RGB in RGBA. */                                        \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRGB_F1:                                                                         \
         /* RGB are in the same location as RGB in RGBA. */                                        \
         (COLOR).n1[gimgValueIndexRGB_R]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_R]];       \
         (COLOR).n1[gimgValueIndexRGB_G]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_G]];       \
         (COLOR).n1[gimgValueIndexRGB_B]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_B]];       \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRGBA_F1:                                                                        \
         /* RGB are in the same location as RGB in RGBA. */                                        \
         (COLOR).n1[gimgValueIndexRGB_R]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_R]];       \
         (COLOR).n1[gimgValueIndexRGB_G]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_G]];       \
         (COLOR).n1[gimgValueIndexRGB_B]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_B]];       \
         (COLOR).n1[gimgValueIndexRGB_A]    = _fToNTableN1[(COLOR).n1[gimgValueIndexRGB_A]];       \
         break;                                                                                    \
      }                                                                                            \
   }

// Changing the GimgValue from an RGBA value to a type value.
#define VALUE_RGBA_N1_TO_TYPE(TYPE, COLOR)                                                         \
   {                                                                                               \
      switch (TYPE)                                                                                \
      {                                                                                            \
      default:                                                                                     \
      case gimgTypeK_N1:                                                                           \
      case gimgTypeRGB_N1:                                                                         \
      case gimgTypeRGBA_N1:                                                                        \
         /* Nothing to do R and K are at the same location, RGB are at the same location */        \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeK_F1:                                                                           \
         (COLOR).n1[gimgValueIndexK_K]   = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_R]];          \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKA_N1:                                                                          \
         /* Nothing to do R and K are at the same location */                                      \
         (COLOR).n1[gimgValueIndexK_A]   = (COLOR).n1[gimgValueIndexRGB_A];                        \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKA_F1:                                                                          \
         (COLOR).n1[gimgValueIndexK_K]   = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_R]];          \
         (COLOR).n1[gimgValueIndexK_A]   = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_A]];          \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRGB_F1:                                                                         \
         (COLOR).n1[gimgValueIndexRGB_R] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_R]];          \
         (COLOR).n1[gimgValueIndexRGB_G] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_G]];          \
         (COLOR).n1[gimgValueIndexRGB_B] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_B]];          \
         (COLOR).n1[gimgValueIndexRGB_A] = 100;                                                    \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRGBA_F1:                                                                        \
         (COLOR).n1[gimgValueIndexRGB_R] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_R]];          \
         (COLOR).n1[gimgValueIndexRGB_G] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_G]];          \
         (COLOR).n1[gimgValueIndexRGB_B] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_B]];          \
         (COLOR).n1[gimgValueIndexRGB_A] = _nToFTableN1[(COLOR).n1[gimgValueIndexRGB_A]];          \
         break;                                                                                    \
      }                                                                                            \
   }

#define VALUE_SET_RGB_N1(COLOR, R, G, B)                                                           \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexRGB_R] = (Gn1) (R),                                                    \
   (COLOR).n1[gimgValueIndexRGB_G] = (Gn1) (G),                                                    \
   (COLOR).n1[gimgValueIndexRGB_B] = (Gn1) (B),                                                    \
   (COLOR).n1[gimgValueIndexRGB_A] = 255,                                                          \
   (COLOR)                                                                                         \
)
#define VALUE_SET_RGBA_N1(COLOR, R, G, B, A)                                                       \
(                                                                                                  \
   (COLOR).n1[gimgValueIndexRGB_R] = (Gn1) (R),                                                    \
   (COLOR).n1[gimgValueIndexRGB_G] = (Gn1) (G),                                                    \
   (COLOR).n1[gimgValueIndexRGB_B] = (Gn1) (B),                                                    \
   (COLOR).n1[gimgValueIndexRGB_A] = (Gn1) (A),                                                    \
   (COLOR)                                                                                         \
)
#define VALUE_GET_RGB_N1(COLOR, R, G, B)                                                           \
{                                                                                                  \
   R = (COLOR).n1[gimgValueIndexRGB_R];                                                            \
   G = (COLOR).n1[gimgValueIndexRGB_G];                                                            \
   B = (COLOR).n1[gimgValueIndexRGB_B];                                                            \
}
#define VALUE_GET_RGBA_N1(COLOR, R, G, B, A)                                                       \
{                                                                                                  \
   R = (COLOR).n1[gimgValueIndexRGB_R];                                                            \
   G = (COLOR).n1[gimgValueIndexRGB_G];                                                            \
   B = (COLOR).n1[gimgValueIndexRGB_B];                                                            \
   A = (COLOR).n1[gimgValueIndexRGB_A];                                                            \
}

#define N1_TO_PERCENT(  VALUE)                   (((Gr4) (VALUE)) / ((Gr4) Gn1MAX))
#define N2_TO_PERCENT(  VALUE)                   (((Gr4) (VALUE)) / ((Gr4) Gn2MAX))
#define N4_TO_PERCENT(  VALUE)                   (((Gr8) (VALUE)) / ((Gr8) Gn4MAX))
#define N8_TO_PERCENT(  VALUE)                   (((Gr8) (VALUE)) / ((Gr8) Gn8MAX))

#define SRGB_TO_LINEAR( PERCENT_O_TO_1)          (((PERCENT_O_TO_1) <  0.04045f)   ? (PERCENT_O_TO_1) / 12.92f : (Gr4) pow(((PERCENT_O_TO_1) + 0.055) / 1.055, 2.4))
#define LINEAR_TO_SRGB( PERCENT_O_TO_1)          (((PERCENT_O_TO_1) <= 0.0031308f) ? (PERCENT_O_TO_1) * 12.92f : (Gr4) pow( (PERCENT_O_TO_1),                  1.0 / 2.4) * (1.055) - 0.055)

#define GAMMA_TO_LINEAR(PERCENT_O_TO_1, GAMMA)   (pow((PERCENT_O_TO_1),       (GAMMA)))
#define LINEAR_TO_GAMMA(PERCENT_O_TO_1, GAMMA)   (pow((PERCENT_O_TO_1), 1.0 / (GAMMA)))


/**************************************************************************************************
variable:
**************************************************************************************************/
static Gb    _isStarted = gbFALSE;
static Gr    _percentTableN1[256];
static Gr    _gammaTableN1[256];
static Gn1   _nToFTableN1[256];
static Gn1   _fToNTableN1[256];

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
global:
function:
**************************************************************************************************/
/**************************************************************************************************
func: gimgClipLine
**************************************************************************************************/
GIMG_API Gb gimgClipLine(Gimg * const img, Gindex * const x1, Gindex * const y1, Gindex * const x2,
   Gindex * const y2)
{
   Gr     u1,
          u2;
   Gindex tx1,
          tx2,
          ty1,
          ty2,
          dx,
          dy;

   genter;

   u1  = 0.;
   u2  = 1.;
   tx1 = *x1;
   tx2 = *x2;
   ty1 = *y1;
   ty2 = *y2;

   dx = tx2 - tx1;
   dy = ty2 - ty1;

   // Line is a dot.  Trivial test.
   greturnTrueIf(
      !dx      &&
      !dy      &&
      0 <= tx1 && tx1 < img->width &&
      0 <= ty1 && ty1 < img->height);

   // More convoluted test.
   if (gimgClipLineTest(-dx, tx1,                   &u1, &u2) &&
       gimgClipLineTest( dx, img->width - tx1 - 1,  &u1, &u2) &&
       gimgClipLineTest(-dy, ty1,                   &u1, &u2) &&
       gimgClipLineTest( dy, img->height - ty1 - 1, &u1, &u2))
   {
      if (u2 < 1)
      {
         *x2 = tx1 + (Gindex) (u2 * dx - .5);
         *y2 = ty1 + (Gindex) (u2 * dy - .5);
      }
      if (u1 > 0)
      {
         *x1 += (Gindex) (u1 * dx + .5);
         *y1 += (Gindex) (u1 * dy + .5);
      }

      greturn gbTRUE;
   }

   greturn gbFALSE;
}

/**************************************************************************************************
func: gimgClipLineTest
**************************************************************************************************/
GIMG_API Gb gimgClipLineTest(Gindex const denom, Gindex const num, Gr * const u1, Gr * const u2)
{
   Gr r;

   genter;

   if      (denom < 0)
   {
      r = (Gr) num / denom;

      greturnFalseIf(r > *u2);

      if (r > *u1)
      {
         *u1 = r;
      }
   }
   else if (denom > 0)
   {
      r = (Gr) num / denom;

      greturnFalseIf(r < *u1);

      if (r < *u2)
      {
         *u2 = r;
      }
   }
   else if (num < 0)
   {
      greturn gbFALSE;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgCloc
**************************************************************************************************/
GIMG_API Gimg *gimgCloc_(GimgType const value, Gcount const width, Gcount const height)
{
   Gimg *img;

   genter;

   greturnNullIf(!_isStarted);

   img = gmemClocType(Gimg);
   greturnNullIf(!img);

   if (!gimgClocContent(img, value, width, height))
   {
      gimgDloc(img);
   }

   greturn img;
}

/**************************************************************************************************
func: gimgClocClone
**************************************************************************************************/
GIMG_API Gimg *gimgClocClone(Gimg const * const img)
{
   Gimg *clone;

   genter;

   greturnNullIf(
      !_isStarted ||
      !img);

   // Create the clone image.
   clone = gimgCloc(img->valueType, img->width, img->height);
   greturnNullIf(!clone);

   // Copy the image data.
   if (!gmemCopyOverTypeArray(
         clone->valueData,
         Gn1,
         img->height * img->rowByteCount,
         img->valueData))
   {
      gimgDloc(clone);
      greturn NULL;
   }

   greturn clone;
}

/**************************************************************************************************
func: gimgClocContent
**************************************************************************************************/
GIMG_API Gb gimgClocContent(Gimg * const img, GimgType const value, Gcount const width,
   Gcount const height)
{
   genter;

   greturnFalseIf(
      !_isStarted ||
      !img);

   GTYPE_SET(img, "Gimg");

   gmemClearType(img, Gimg);

   img->valueType = value;

   switch (value)
   {
   case gimgTypeK_N1:
   case gimgTypeK_F1:      img->valueByteCount = 1;   break;
   case gimgTypeK_N2:
   case gimgTypeK_F2:      img->valueByteCount = 2;   break;
   case gimgTypeK_N4:
   case gimgTypeK_F4:      img->valueByteCount = 4;   break;
   case gimgTypeK_N8:
   case gimgTypeK_F8:      img->valueByteCount = 8;   break;
   case gimgTypeK_R4:      img->valueByteCount = 4;   break;
   case gimgTypeK_R8:      img->valueByteCount = 8;   break;
   case gimgTypeKA_N1:
   case gimgTypeKA_F1:     img->valueByteCount = 2;   break;
   case gimgTypeKA_N2:
   case gimgTypeKA_F2:     img->valueByteCount = 4;   break;
   case gimgTypeKA_N4:
   case gimgTypeKA_F4:     img->valueByteCount = 8;   break;
   case gimgTypeKA_N8:
   case gimgTypeKA_F8:     img->valueByteCount = 16;  break;
   case gimgTypeKA_R4:     img->valueByteCount = 8;   break;
   case gimgTypeKA_R8:     img->valueByteCount = 16;  break;
   case gimgTypeRGB_N1:
   case gimgTypeRGB_F1:    img->valueByteCount = 3;   break;
   case gimgTypeRGB_N2:
   case gimgTypeRGB_F2:    img->valueByteCount = 6;   break;
   case gimgTypeRGB_N4:
   case gimgTypeRGB_F4:    img->valueByteCount = 12;  break;
   case gimgTypeRGB_N8:
   case gimgTypeRGB_F8:    img->valueByteCount = 24;  break;
   case gimgTypeRGB_R4:    img->valueByteCount = 12;  break;
   case gimgTypeRGB_R8:    img->valueByteCount = 24;  break;
   case gimgTypeRGBA_N1:
   case gimgTypeRGBA_F1:   img->valueByteCount = 4;   break;
   case gimgTypeRGBA_N2:
   case gimgTypeRGBA_F2:   img->valueByteCount = 8;   break;
   case gimgTypeRGBA_N4:
   case gimgTypeRGBA_F4:   img->valueByteCount = 16;  break;
   case gimgTypeRGBA_N8:
   case gimgTypeRGBA_F8:   img->valueByteCount = 32;  break;
   case gimgTypeRGBA_R4:   img->valueByteCount = 16;  break;
   case gimgTypeRGBA_R8:   img->valueByteCount = 32;  break;
   }

   img->rowByteCount = width * img->valueByteCount;
   img->width        = width;
   img->height       = height;

   // Create the image buffer as one contiguous block of memory.
   img->valueData = gmemClocTypeArray(Gn1, img->height * img->width * img->valueByteCount);
   greturnFalseIf(!img->valueData);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgDloc
**************************************************************************************************/
GIMG_API void gimgDloc(Gimg * const img)
{
   genter;

   greturnVoidIf(
      !_isStarted ||
      !img);

   gimgDlocContent(img);

   gmemDloc(img);

   greturn;
}

/**************************************************************************************************
func: gimgDestoryContent
**************************************************************************************************/
GIMG_API void gimgDlocContent(Gimg * const img)
{
   genter;

   greturnVoidIf(
      !_isStarted ||
      !img);

   // Clean up memory.
   gmemDloc(img->valueData);

   greturn;
}

/**************************************************************************************************
func: gimgGetHeight
**************************************************************************************************/
GIMG_API Gcount gimgGetHeight(Gimg const * const img)
{
   genter;

   greturn0If(
      !_isStarted ||
      !img);

   greturn img->height;
}

/**************************************************************************************************
func: gimgGetValue
**************************************************************************************************/
GIMG_API Gb gimgGetValue(Gimg const * const img, Gindex const x, Gindex const y,
   GimgValue * const value)
{
   genter;

   greturnFalseIf(
      !img                       ||
      !value                     ||
      x < 0  || img->width  <= x ||
      y < 0  || img->height <= y);

   // Clear out the value.
   value->n8[0]    =
      value->n8[1] =
      value->n8[2] =
      value->n8[3] = 0;

   // Find the offset into the pixel array.
   GET_PIXEL_VALUE(img, x, y, *value);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgGetWidth
**************************************************************************************************/
GIMG_API Gcount gimgGetWidth(Gimg const * const img)
{
   genter;

   greturn0If(
      !_isStarted ||
      !img);

   greturn img->width;
}

/**************************************************************************************************
func: gimgIsStarted
**************************************************************************************************/
GIMG_API Gb gimgIsStarted(void)
{
   genter;

   greturn _isStarted;
}

#if 0
/**************************************************************************************************
func: gimgLoad

Load in an image using gimgio.  So supported images are PNG, BMP, TIF, JPEG and
GRAW
**************************************************************************************************/
GIMG_API Gimg *gimgLoad(Gpath const * const filePath)
{
   Gindex    loopIndex;
   Gb        result;
   Gcount    w,
             h;
   Gindex    row,
             pixel;
   Gn4       r,
             g,
             b,
             a;
   Gn1      *buffer;
   Gimg     *img;
   Gimgio   *imgio;
   GimgValue color;

   genter;

   img    = NULL;
   imgio  = NULL;
   buffer = NULL;
   result = gbFALSE;

   loopOnce(loopIndex)
   {
      // Open the image file.
      imgio = gimgioOpen(
         filePath,
         gimgioOpenREAD,
         gimgioGetFormatFromName(filePath));
      breakIf(!imgio);

      // Set the pixel type we want to read in.
      gimgioSetTypePixel(imgio, gimgioTypeRGB | gimgioTypeALPHA | gimgioTypeN1);

      // Create the image.
      w   = gimgioGetWidth( imgio);
      h   = gimgioGetHeight(imgio);
      img = gimgCloc(w, h);
      breakIf(!img);

      // for each
      buffer = gmemClocTypeArray(
         Gn1,
         gimgioGetPixelSize(gimgioTypeRGB | gimgioTypeALPHA | gimgioTypeN1, w));
      breakIf(!buffer);
      forCount(row, h)
      {
         gimgioSetRow(imgio, row);
         breakIf(!gimgioGetPixelRow(imgio, buffer));

         for (pixel = 0; pixel < w; pixel++)
         {
            gimgioGetPixelAtN(
               gimgioTypeRGB | gimgioTypeALPHA | gimgioTypeN1,
               pixel,
               buffer,
               &r, &g, &b, &a);
            gimgValueSET_RN1_GN1_BN1_AN1(
               color,
               N4ToN1(r),
               N4ToN1(g),
               N4ToN1(b),
               N4ToN1(a));
            SET_PIXEL_VALUE(img, pixel, row, color);
         }
      }

      result = gbTRUE;
   }

   if (!result)
   {
      gimgDloc(img);
      img = NULL;
   }
   gmemDloc(buffer);
   gimgioClose(imgio);

   greturn img;
}
#endif

/**************************************************************************************************
func: gimgSetCircle

Why am I not using a midpoint integer base scan conversion circle routine?  I did have this
implemented using a Bresenham algoritm but it didn't quite do what I wanted on the even or odd case
so I did this for now to get a result that I am looking for.  Will speed this up later with an
integer bases solution.
**************************************************************************************************/
GIMG_API Gb gimgSetCircle(Gimg * const img, Gindex const inx, Gindex const iny, Gcount const w,
   GimgValue const color)
{
   Gindex offset,
          radius,
          index,
          step,
          xCenter,
          yCenter,
          xMax,
          yMax;
   Gr4    xr,
          xrsq,
          y1,
          y1sq,
          y2,
          y2sq,
          d1,
          d2,
          rsq;

   genter;

   greturnFalseIf(w <= 0);

   greturnIf(w == 1, gimgSetValue(img, inx, iny, color));

   // Dealing with an odd width.  Symetric around a central pixel.
   //     +---------------+ w (3, 5, 7, ...)
   //             +-------+ radius
   //     +-------+         offset
   //     x
   //   +---+---+---+---+---+
   // y |   |   |   |   |   | + y1 = radius
   //   +---+---+---+---+---+ |
   //   |   |   |   |   |   | | y2 = radius - 1
   //   +---+---+---+---+---+ | |
   //   |   |   | o |   |   | + +
   //   +---+---+---+---+---+
   //   |   |   |   |   |   |
   //   +---+---+---+---+---+
   //   |   |   |   |   |   |
   //   +---+---+---+---+---+
   //             xr->
   //
   // o = center of circle
   if (w % 2)
   {
      offset    =
         radius = (w - 1) / 2;

      xCenter   = inx + offset;
      yCenter   = iny + offset;
      xMax      = inx + w - 1;
      yMax      = iny + w - 1;

      rsq = (Gr4) (radius * radius);
      xr  = 0;
      y1  = (Gr4)  radius;
      y2  = (Gr4) (radius - 1);

      // First iteration is 4 symetry.
      gimgSetValue(img, xCenter, iny,     color);
      gimgSetValue(img, xCenter, yMax,    color);
      gimgSetValue(img, inx,     yCenter, color);
      gimgSetValue(img, xMax,    yCenter, color);

      step = 0;
      for (index = 1; index + step <= offset; index++)
      {
         xr = (Gr4) index;
         y1 = (Gr4)  radius - step;
         y2 = (Gr4) (radius - step - 1);

         xrsq = xr * xr;
         y1sq = y1 * y1;
         y2sq = y2 * y2;

         d1 = (xrsq + y1sq) - rsq;
         d2 = rsq - (xrsq + y2sq);

         // sqrt(x^2 + y^2) = h
         //      x^2 + y^2  = h^2
         // x starts off as being 0 and increments.
         // y starts off being the radius.
         // d1 is the diff of the radius^2 with using the x^2 + y^2 assuming y doesn't change from iteration
         // d2 is the diff ... assuming y decreases by 1 from last interation.
         // If d2 is closer to 0, closer to the radius, then we need to step y down.
         if (d2 < d1)
         {
            step++;
         }
         gimgSetValue(img, xCenter + index, iny  + step,     color);
         gimgSetValue(img, xCenter - index, iny  + step,     color);
         gimgSetValue(img, xCenter + index, yMax - step,     color);
         gimgSetValue(img, xCenter - index, yMax - step,     color);
         gimgSetValue(img, inx  + step,     yCenter + index, color);
         gimgSetValue(img, inx  + step,     yCenter - index, color);
         gimgSetValue(img, xMax - step,     yCenter + index, color);
         gimgSetValue(img, xMax - step,     yCenter - index, color);
      }
   }
   // Dealing with an even width.  Symetric around a pixel boundary.
   //      +-----------+ w = even (2, 4, 6, ...)
   //            +---+   radius (+ 0.5f)
   //      +-------+     offset
   //      x
   //    +---+---+---+---+
   //  y |   |   |   |   | + y1 = radius (+ 0.5f)
   //    +---+---+---+---+ |
   //    |   |   |   |   | | + y2 = radius - 1 (+ 0.5f)
   //    +---+---o---+---+ + +
   //    |   |   |   |   |
   //    +---+---+---+---+
   //    |   |   |   |   |
   //    +---+---+---+---+
   //            xr->
   else
   {
      offset    =  w      / 2;
         radius = (w - 1) / 2;

      xCenter = inx + offset;
      yCenter = iny + offset;
      xMax    = inx + w - 1;
      yMax    = iny + w - 1;

      rsq = (Gr4) (radius + 0.5f) * (radius + 0.5f);
      xr  = 0.5f;
      y1  = (Gr4) radius + 0.5f;
      y2  = (Gr4) radius - 0.5f;

      gimgSetValue(img, xCenter,     iny,         color);
      gimgSetValue(img, xCenter - 1, iny,         color);
      gimgSetValue(img, xCenter,     yMax,        color);
      gimgSetValue(img, xCenter - 1, yMax,        color);
      gimgSetValue(img, inx,         yCenter,     color);
      gimgSetValue(img, inx,         yCenter - 1, color);
      gimgSetValue(img, xMax,        yCenter,     color);
      gimgSetValue(img, xMax,        yCenter - 1, color);

      step = 0;
      for (index = 1; index + step < offset; index++)
      {
         xr = (Gr4) index + 0.5f;
         y1 = (Gr4) (radius - step) + 0.5f;
         y2 = (Gr4) (radius - step) - 0.5f;

         xrsq = xr * xr;
         y1sq = y1 * y1;
         y2sq = y2 * y2;

         d1 = (xrsq + y1sq) - rsq;
         d2 = rsq - (xrsq + y2sq);

         // sqrt(x^2 + y^2) = h
         //      x^2 + y^2  = h^2
         // x starts off as being 0 and increments.
         // y starts off being the radius.
         // d1 is the diff of the radius^2 with using the x^2 + y^2 assuming y doesn't change from iteration
         // d2 is the diff ... assuming y decreases by 1 from last interation.
         // If d2 is closer to 0, closer to the radius, then we need to step y down.
         if (d2 < d1)
         {
            step++;
         }
         gimgSetValue(img, xCenter + index,     iny  + step,         color);
         gimgSetValue(img, xCenter - index - 1, iny  + step,         color);
         gimgSetValue(img, xCenter + index,     yMax - step,         color);
         gimgSetValue(img, xCenter - index - 1, yMax - step,         color);
         gimgSetValue(img, inx  + step,         yCenter + index,     color);
         gimgSetValue(img, inx  + step,         yCenter - index - 1, color);
         gimgSetValue(img, xMax - step,         yCenter + index,     color);
         gimgSetValue(img, xMax - step,         yCenter - index - 1, color);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetCircleFill

See note in gimgSetCircle above.

Also, I know this is inefficient.  This routing draws over what it drew before at the top and
bottoms of the circle.  Will fix later.
**************************************************************************************************/
GIMG_API Gb gimgSetCircleFill(Gimg * const img, Gindex const inx, Gindex const iny, Gcount const w,
   GimgValue const color)
{
   Gindex offset,
          radius,
          index,
          step,
          xCenter,
          yCenter,
          xMax,
          yMax,
          xPending,
          yTopPending,
          yBotPending,
          wPending;
   Gr4    xr,
          xrsq,
          y1,
          y1sq,
          y2,
          y2sq,
          d1,
          d2,
          rsq;

   genter;

   greturnFalseIf(w <= 0);

   greturnIf(w == 1, gimgSetValue(img, inx, iny, color));

   // Dealing with an odd width.  Symetric around a central pixel.
   //     +---------------+ w (3, 5, 7, ...)
   //             +-------+ radius
   //     +-------+         offset
   //     x
   //   +---+---+---+---+---+
   // y |   |   |   |   |   | + y1 = radius
   //   +---+---+---+---+---+ |
   //   |   |   |   |   |   | | y2 = radius - 1
   //   +---+---+---+---+---+ | |
   //   |   |   | o |   |   | + +
   //   +---+---+---+---+---+
   //   |   |   |   |   |   |
   //   +---+---+---+---+---+
   //   |   |   |   |   |   |
   //   +---+---+---+---+---+
   //             xr->
   //
   // o = center of circle
   if (w % 2)
   {
      offset    =
         radius = (w - 1) / 2;

      xCenter   = inx + offset;
      yCenter   = iny + offset;
      xMax      = inx + w - 1;
      yMax      = iny + w - 1;

      rsq = (Gr4) (radius * radius);
      xr  = 0;
      y1  = (Gr4)  radius;
      y2  = (Gr4) (radius - 1);

      // First iteration is 4 symetry.
      xPending    = xCenter;
      yTopPending = iny;
      yBotPending = yMax;
      wPending    = 1;
      gimgSetLineH(img, inx,     yCenter, w, color);

      step = 0;
      for (index = 1; index + step <= offset; index++)
      {
         xr = (Gr4) index;
         y1 = (Gr4)  radius - step;
         y2 = (Gr4) (radius - step - 1);

         xrsq = xr * xr;
         y1sq = y1 * y1;
         y2sq = y2 * y2;

         d1 = (xrsq + y1sq) - rsq;
         d2 = rsq - (xrsq + y2sq);

         // sqrt(x^2 + y^2) = h
         //      x^2 + y^2  = h^2
         // x starts off as being 0 and increments.
         // y starts off being the radius.
         // d1 is the diff of the radius^2 with using the x^2 + y^2 assuming y doesn't change from iteration
         // d2 is the diff ... assuming y decreases by 1 from last interation.
         // If d2 is closer to 0, closer to the radius, then we need to step y down.
         if (d2 < d1)
         {
            gimgSetLineH(img, xPending, yTopPending, wPending, color);
            gimgSetLineH(img, xPending, yBotPending, wPending, color);
            step++;
         }
         xPending    = xCenter - index;
         yTopPending = iny  + step;
         yBotPending = yMax - step;
         wPending    = index * 2 + 1;
         gimgSetLineH(img, inx  + step,     yCenter + index, w - 2 * step,  color);
         gimgSetLineH(img, inx  + step,     yCenter - index, w - 2 * step,  color);
      }
   }
   // Dealing with an even width.  Symetric around a pixel boundary.
   //      +-----------+ w = even (2, 4, 6, ...)
   //            +---+   radius (+ 0.5f)
   //      +-------+     offset
   //      x
   //    +---+---+---+---+
   //  y |   |   |   |   | + y1 = radius (+ 0.5f)
   //    +---+---+---+---+ |
   //    |   |   |   |   | | + y2 = radius - 1 (+ 0.5f)
   //    +---+---o---+---+ + +
   //    |   |   |   |   |
   //    +---+---+---+---+
   //    |   |   |   |   |
   //    +---+---+---+---+
   //            xr->
   else
   {
      offset    =  w      / 2;
         radius = (w - 1) / 2;

      xCenter = inx + offset;
      yCenter = iny + offset;
      xMax    = inx + w - 1;
      yMax    = iny + w - 1;

      rsq = (Gr4) (radius + 0.5f) * (radius + 0.5f);
      xr  = 0.5f;
      y1  = (Gr4) radius + 0.5f;
      y2  = (Gr4) radius - 0.5f;

      xPending    = xCenter - 1;
      yTopPending = iny;
      yBotPending = yMax;
      wPending    = 2;
      gimgSetLineH(img, inx,         yCenter,     w, color);
      gimgSetLineH(img, inx,         yCenter - 1, w, color);

      step = 0;
      for (index = 1; index + step < offset; index++)
      {
         xr = (Gr4) index + 0.5f;
         y1 = (Gr4) (radius - step) + 0.5f;
         y2 = (Gr4) (radius - step) - 0.5f;

         xrsq = xr * xr;
         y1sq = y1 * y1;
         y2sq = y2 * y2;

         d1 = (xrsq + y1sq) - rsq;
         d2 = rsq - (xrsq + y2sq);

         // sqrt(x^2 + y^2) = h
         //      x^2 + y^2  = h^2
         // x starts off as being 0 and increments.
         // y starts off being the radius.
         // d1 is the diff of the radius^2 with using the x^2 + y^2 assuming y doesn't change from iteration
         // d2 is the diff ... assuming y decreases by 1 from last interation.
         // If d2 is closer to 0, closer to the radius, then we need to step y down.
         if (d2 < d1)
         {
            gimgSetLineH(img, xPending, yTopPending, wPending, color);
            gimgSetLineH(img, xPending, yBotPending, wPending, color);
            step++;
         }
         xPending    = xCenter - index - 1;
         yTopPending = iny  + step;
         yBotPending = yMax - step;
         wPending    = index * 2 + 2;
         gimgSetLineH(img, inx  + step,         yCenter + index,     w - 2 * step,  color);
         gimgSetLineH(img, inx  + step,         yCenter - index - 1, w - 2 * step,  color);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetClear
**************************************************************************************************/
GIMG_API Gb gimgSetClear(Gimg * const img, GimgValue const color)
{
   Gindex     indexPixel;
   Gcount     countPixel,
              countColor;
   Gn1       *imagePointer;

   genter;

   greturnFalseIf(!img);

   // For all pixels...
   imagePointer = img->valueData;
   countPixel   = img->height * img->width;
   countColor   = img->valueByteCount;

   forCount(indexPixel, countPixel)
   {
      SET_VALUE(imagePointer, countColor, color);
      imagePointer = imagePointer + countColor;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImage

Copy the new image over an old image.  This completely overwrites the old image regardless of alpha
channel value.  Both images must be of the same type.
**************************************************************************************************/
GIMG_API Gb gimgSetImage(Gimg * const img, Gindex const x, Gindex const y, Gimg const * const value)
{
   Gindex     ix,
              iy,
              vx,
              vy;
   Gcount     iw,
              ih,
              vw,
              vh,
              valueByteCount;
   Gn1       *iValueData,
             *vValueData;

   genter;

   greturnFalseIf(
      !img                                         ||
      !value                                       ||
      img->valueType != value->valueType           ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   valueByteCount = img->valueByteCount;
   iValueData     = img->valueData;
   vValueData     = value->valueData;

   vx             = 0;
   vy             = 0;
   vw             = value->width;
   vh             = value->height;

   // Clip off the under flow
   ix = x;
   if (ix < 0)
   {
      ix  = 0;
      vx  = -x;
      vw -= -x;
   }
   iy = y;
   if (iy < 0)
   {
      iy  = 0;
      vy  = -y;
      vh -= -y;
   }

   // Clip off the over flow
   vw = gMIN(img->width  - ix, vw);
   vh = gMIN(img->height - iy, vh);
   iw = ix + vw;
   ih = iy + vh;

   // Copy the image information.
   for (; iy < ih; iy++, vy++)
   {
      gmemCopyOverTypeArray(
         iValueData + iy * img->rowByteCount + ix * img->valueByteCount,
         Gn1,
         vw * img->valueByteCount,
         vValueData + vy * img->rowByteCount + vx * img->valueByteCount);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImage

Copy the new image over an old image.  This completely overwrites the old image regardless of alpha
channel value.  Both images need not be of the same type.  This is slower than above because
convertions that may be needed.  (Currently only works with gimgType1 types.)
**************************************************************************************************/
GIMG_API Gb gimgSetImage1(Gimg * const img, Gindex const x, Gindex const y, Gimg const * const value)
{
   Gindex     ix,
              iy,
              ixIndex,
              iyIndex,
              vx,
              vy,
              vxIndex,
              vyIndex;
   Gcount     iw,
              ih,
              vw,
              vh,
              valueByteCount;
   Gn1       *iValueData,
             *vValueData;
   GimgValue  vValue;

   genter;

   greturnFalseIf(
      !img                                         ||
      !value                                       ||
      !(img->valueType   & gimgType1)              ||
      !(value->valueType & gimgType1)              ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   valueByteCount = img->valueByteCount;
   iValueData     = img->valueData;
   vValueData     = value->valueData;

   vx             = 0;
   vy             = 0;
   vw             = value->width;
   vh             = value->height;
   ix             = x;
   iy             = y;


   // Clip off the under flow
   if (ix < 0)
   {
      ix  = 0;
      vx  = -x;
      vw -= -x;
   }
   if (iy < 0)
   {
      iy  = 0;
      vy  = -y;
      vh -= -y;
   }

   // Clip off the over flow
   vw = gMIN(img->width  - ix, vw);
   vh = gMIN(img->height - iy, vh);
   iw = ix + vw;
   ih = iy + vh;

   // Copy the image information.
   for (iyIndex = iy, vyIndex = vy;
        iyIndex < ih;
        iyIndex++,    vyIndex++)
   {
      for (ixIndex = ix, vxIndex = vx;
           ixIndex < iw;
           ixIndex++,    vxIndex++)
      {
         // Get the source pixel value.
         GET_PIXEL_VALUE(value, vxIndex, vyIndex, vValue);
         // Convert the source pixel value to RGBA
         VALUE_TYPE_TO_RGBA_N1(value->valueType, vValue);
         // Convert the RGBA to the destination pixel value.
         VALUE_RGBA_N1_TO_TYPE(img->valueType, vValue);
         // Set the destination pixel value.
         SET_PIXEL_VALUE(img, ixIndex, iyIndex, vValue);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImage1Blend

Treat the destination image as an opaque image.  The source image is an alpah image.  Only works
with RN1_GN1_BN1_AN1 images.

Assumes destination image is opaque.
**************************************************************************************************/
GIMG_API Gb gimgSetImage1Blend(Gimg * const img, Gindex const x, Gindex const y,
   Gimg const * const value)
{
   Gindex     ix, iy,
              vx, vy,
              iindex, vindex;
   Gcount     iw, ih,
              vw, vh,
              valueByteCount,
              irowByteCount,
              vrowByteCount;
   Gn1        vr, vg, vb, va,
              ir, ig, ib, ia;
   Gn1       *iValueData,
             *vValueData;
   Gr4        vpercent,
              ipartPercent;
   GimgValue  color;

   genter;

   greturnFalseIf(
      !img                                         ||
      !value                                       ||
      !(img->valueType   & gimgType1)              ||
      !(value->valueType & gimgType1)              ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   irowByteCount  = img->rowByteCount;
   vrowByteCount  = value->rowByteCount;
   valueByteCount = img->valueByteCount;
   iValueData     = img->valueData;
   vValueData     = value->valueData;

   vx = 0;
   vy = 0;
   vw = value->width;
   vh = value->height;

   // Clip off the under flow
   ix = x;
   if (ix < 0)
   {
      ix  = 0;
      vx  = -x;
      vw -= -x;
   }
   iy = y;
   if (iy < 0)
   {
      iy  = 0;
      vy  = -y;
      vh -= -y;
   }

   // Clip off the over flow
   vw = gMIN(img->width  - ix, vw);
   vh = gMIN(img->height - iy, vh);
   iw = ix + vw;
   ih = iy + vh;

   // Copy the image information.
   for (; iy < ih; iy++, vy++)
   {
      for (iindex = ix, vindex = vx;
           iindex < iw;
           iindex++,    vindex++)
      {
         GET_PIXEL_VALUE(value, vindex, vy, color);
         VALUE_TYPE_TO_RGBA_N1(value->valueType, color);
         VALUE_GET_RGBA_N1(color, vr, vg, vb, va);

         continueIf(va == 0);

         if (va == 255)
         {
            VALUE_RGBA_N1_TO_TYPE(img->valueType, color);
            SET_PIXEL_VALUE(img, iindex, iy, color);
            continue;
         }

         GET_PIXEL_VALUE(img, iindex, iy, color);
         VALUE_TYPE_TO_RGBA_N1(img->valueType, color);
         VALUE_GET_RGBA_N1(color, ir, ig, ib, ia);

         // Merge pixels
         vpercent     = ((Gr4) va) / (255.0f);
         ipartPercent = 1.0f - vpercent;

         VALUE_SET_RGBA_N1(
            color,
            (Gn1) (ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr)),
            (Gn1) (ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg)),
            (Gn1) (ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb)),
            (Gn1) ia);
         VALUE_RGBA_N1_TO_TYPE(img->valueType, color);
         SET_PIXEL_VALUE(img, iindex, iy, color);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImage1BlendA

Both Destination and Source image are alpha images.  Final image will still be an alpha image.
**************************************************************************************************/
GIMG_API Gb gimgSetImage1BlendA(Gimg * const img, Gindex const x, Gindex const y,
   Gimg const * const value)
{
   Gindex     ix, iy,
              vx, vy,
              iindex, vindex;
   Gcount     iw, ih,
              vw, vh,
              valueByteCount,
              irowByteCount,
              vrowByteCount;
   Gn1        vr, vg, vb, va,
              ir, ig, ib, ia;
   Gn1       *iValueData,
             *vValueData;
   Gr4        vpercent,
              ipercent,
              ipartPercent,
              percent;
   GimgValue  color;

   genter;

   greturnFalseIf(
      !img                                         ||
      !value                                       ||
      !(img->valueType   & gimgType1)              ||
      !(value->valueType & gimgType1)              ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   irowByteCount  = img->rowByteCount;
   vrowByteCount  = value->rowByteCount;
   valueByteCount = img->valueByteCount;
   iValueData     = img->valueData;
   vValueData     = value->valueData;

   vx             = 0;
   vy             = 0;
   vw             = value->width;
   vh             = value->height;

   // Clip off the under flow
   ix = x;
   if (ix < 0)
   {
      ix  = 0;
      vx  = -x;
      vw -= -x;
   }
   iy = y;
   if (iy < 0)
   {
      iy  = 0;
      vy  = -y;
      vh -= -y;
   }

   // Clip off the over flow
   vw = gMIN(img->width  - ix, vw);
   vh = gMIN(img->height - iy, vh);
   iw = ix + vw;
   ih = iy + vh;

   // Copy the image information.
   for (; iy < ih; iy++, vy++)
   {
      for (iindex = ix, vindex = vx; iindex < iw; iindex++, vindex++)
      {
         GET_PIXEL_VALUE(value, vindex, vy, color);
         VALUE_TYPE_TO_RGBA_N1(value->valueType, color);
         VALUE_GET_RGBA_N1(color, vr, vg, vb, va);

         continueIf(va == 0);

         // New pixel is opaque, overwrite the old pixel completely.
         if (va == 255)
         {
            VALUE_RGBA_N1_TO_TYPE(img->valueType, color);
            SET_PIXEL_VALUE(img, iindex, iy, color);
            continue;
         }

         // Calculate the new pixel color by merging the two colors.
         GET_PIXEL_VALUE(img, iindex, iy, color);
         VALUE_TYPE_TO_RGBA_N1(img->valueType, color);
         VALUE_GET_RGBA_N1(color, ir, ig, ib, ia);

         // Merge pixels.
         vpercent     = ((Gr4) va) / (255.0f);
         ipercent     = ((Gr4) ia) / (255.0f);
         ipartPercent = (1.0f - vpercent) * ipercent;
         percent      = ipartPercent + vpercent;

         VALUE_SET_RGBA_N1(
            color,
            (Gn1) ((ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr)) / percent),
            (Gn1) ((ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg)) / percent),
            (Gn1) ((ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb)) / percent),
            (Gn1) (255.0f * percent));
         VALUE_RGBA_N1_TO_TYPE(img->valueType, color);
         SET_PIXEL_VALUE(img, iindex, iy, color);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLine

Value is copied directly.  No alpha blending.
**************************************************************************************************/
GIMG_API Gb gimgSetLine(Gimg * const img, Gindex const ix1, Gindex const iy1, Gindex const ix2,
   Gindex const iy2, GimgValue const p)
{
   GinterpI i;
   Gindex   x1, x2,
            y1, y2;

   genter;

   x1 = ix1;
   x2 = ix2;
   y1 = iy1;
   y2 = iy2;

   // line is visible after the clip.
   if (gimgClipLine(img, &x1, &y1, &x2, &y2))
   {
      // assume x is the longer axis.
      ginterpISet(&i, x1, y1, x2, y2);

      // plot the pixels.
      for (; !ginterpIIsDoneHalf(&i); ginterpIGetNext(&i))
      {
         // Get the point from the start.
         SET_PIXEL_VALUE(img, ginterpIGetX1(&i), ginterpIGetY1(&i), p);

         // Get the point from the end.  Lines are symetrical around the middle.
         SET_PIXEL_VALUE(img, ginterpIGetX2(&i), ginterpIGetY2(&i), p);
      }

      greturn gbTRUE;
   }

   greturn gbFALSE;
}

/**************************************************************************************************
func: gimgSetLineH

Value is copied directly.  No alpha blending.
**************************************************************************************************/
GIMG_API Gb gimgSetLineH(Gimg * const img, Gindex const x, Gindex const y, Gcount const inw,
   GimgValue const p)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    nw;
   GimgValue value;

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   // Get the value in image terms.
   value = p;
   VALUE_RGBA_N1_TO_TYPE(img->valueType, value);

   // Clip.
   index = x;
   nw    = inw;
   if (index < 0)
   {
      nw    = nw + index;
      index = 0;
   }

   // Find the start pixel.
   buffer = img->valueData + y * img->rowByteCount + index * img->valueByteCount;
   a      = index + gMIN(img->width - index, nw);

   for (; index < a; index++)
   {
      // Set the pixel
      SET_VALUE(buffer, img->valueByteCount, value);
      buffer += img->valueByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineHAlpha

Color is RGBA (n1 at the moment.)
**************************************************************************************************/
GIMG_API Gb gimgSetLineHAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inw,
   GimgValue const color)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    w;
   Gn1       cr,  cg,  cb,  ca,
             sr,  sg,  sb,  sa;
   Gi4       cir, cig, cib, cia;
   GimgValue value;

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   // Clip.
   index = x;
   w     = inw;
   if (index < 0)
   {
      w     = w + index;
      index = 0;
   }

   // Find the start pixel.
   buffer = img->valueData + y * img->rowByteCount + index * img->valueByteCount;
   a      = index + gMIN(img->width - index, w);

   // color to alpha in.
   VALUE_GET_RGBA_N1(color, cr, cg, cb, ca);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      GET_VALUE(buffer, img->valueByteCount, value);
      VALUE_TYPE_TO_RGBA_N1(img->valueType, value);
      VALUE_GET_RGBA_N1(value, sr, sg, sb, sa);

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the pixel
      VALUE_SET_RGBA_N1(value, sr, sg, sb, sa);
      VALUE_RGBA_N1_TO_TYPE(img->valueType, value);
      SET_VALUE(buffer, img->valueByteCount, value);

      buffer += img->valueByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineHInterpolate

pa and pb are RGBA (n1 at the moment.)
**************************************************************************************************/
GIMG_API Gb gimgSetLineHInterpolate(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const inw, GimgValue const pa, GimgValue const pb)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    w;
   Gn1       ra, rb,
             ga, gb,
             ba, bb,
             aa, ab;
   GinterpI  ir,
             ig,
             ib;
   GimgValue value;

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   w = inw;

   // Get the color components of the input.
   VALUE_GET_RGBA_N1(pa, ra, ga, ba, aa);
   VALUE_GET_RGBA_N1(pb, rb, gb, bb, ab);

   // Set up the interpolators.
   ginterpISet(&ir, 0, (Gi4) ra, w, (Gi4) rb);
   ginterpISet(&ig, 0, (Gi4) ga, w, (Gi4) gb);
   ginterpISet(&ib, 0, (Gi4) ba, w, (Gi4) bb);

   // Clip.
   index = x;
   if (index < 0)
   {
      ginterpISkip(&ir, -index);
      ginterpISkip(&ig, -index);
      ginterpISkip(&ib, -index);
      w     = w + index;
      index = 0;
   }

   // Find the start pixel.
   buffer = img->valueData + y * img->rowByteCount + index * img->valueByteCount;
   a      = index + gMIN(img->width - index, w);

   for (; index < a; index++)
   {
      // Get the current color.
      VALUE_SET_RGBA_N1(
         value,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);
      VALUE_RGBA_N1_TO_TYPE(img->valueType, value);
      SET_VALUE(buffer, img->valueByteCount, value);

      // Set the pixel
      buffer += img->valueByteCount;

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineV
**************************************************************************************************/
GIMG_API Gb gimgSetLineV(Gimg * const img, Gindex const x, Gindex const y, Gcount const inh,
   GimgValue const p)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    nh;
   GimgValue value;

   genter;

   greturnTrueIf(
      !img  ||
      x < 0 || img->width - 1 < x);

   // Get the value in image terms.
   value = p;
   VALUE_RGBA_N1_TO_TYPE(img->valueType, value);

   // Clip.
   index = y;
   nh    = inh;
   if (index < 0)
   {
      nh    = nh + index;
      index = 0;
   }

   // Set the buffer pointer.
   buffer = img->valueData + index * img->rowByteCount + x * img->valueByteCount;
   a      = index + gMIN(img->height - index, nh);

   for (; index < a; index++)
   {
      SET_VALUE(buffer, img->valueByteCount, value);
      buffer += img->rowByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineVAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetLineVAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inh,
   GimgValue const color)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    h;
   Gn1       cr,  cg,  cb,  ca,
             sr,  sg,  sb,  sa;
   Gi4       cir, cig, cib, cia;
   GimgValue value;

   genter;

   greturnTrueIf(
      !img  ||
      x < 0 || img->width - 1 < x);

   // Clip.
   index = y;
   h     = inh;
   if (index < 0)
   {
      h     = h + index;
      index = 0;
   }

   // Set the buffer pointer.
   buffer    = img->valueData + index * img->rowByteCount + x * img->valueByteCount;
   a         = index + gMIN(img->height - index, h);

   // color to alpha in.
   VALUE_GET_RGBA_N1(color, cr, cg, cb, ca);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      GET_VALUE(buffer, img->valueByteCount, value);
      VALUE_TYPE_TO_RGBA_N1(img->valueType, value);
      VALUE_GET_RGBA_N1(value, sr, sg, sb, sa);

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the color.
      VALUE_SET_RGBA_N1(value, sr, sg, sb, sa);
      VALUE_RGBA_N1_TO_TYPE(img->valueType, value);
      SET_VALUE(buffer, img->valueByteCount, value);
            // Move to the next pixel.
      buffer += img->rowByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineVInterpolate
**************************************************************************************************/
GIMG_API Gb gimgSetLineVInterpolate(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const inh, GimgValue const pa, GimgValue const pb)
{
   Gn1      *buffer;
   Gindex    a,
             index;
   Gcount    h;
   Gn1       ra, rb,
             ga, gb,
             ba, bb,
             aa, ab;
   GinterpI  ir,
             ig,
             ib;
   GimgValue value;

   genter;

   greturnTrueIf(
      !img  ||
      x < 0 || img->width - 1 < x);

   h = inh;

   // Get the color components of the input.
   VALUE_GET_RGBA_N1(pa, ra, ga, ba, aa);
   VALUE_GET_RGBA_N1(pb, rb, gb, bb, ab);

   // Set up the interpolators.
   ginterpISet(&ir, 0, (Gi4) ra, h, (Gi4) rb);
   ginterpISet(&ig, 0, (Gi4) ga, h, (Gi4) gb);
   ginterpISet(&ib, 0, (Gi4) ba, h, (Gi4) bb);

   // Clip.
   index = y;
   if (index < 0)
   {
      ginterpISkip(&ir, -index);
      ginterpISkip(&ig, -index);
      ginterpISkip(&ib, -index);
      h     = h + index;
      index = 0;
   }

   // Set the buffer pointer.
   buffer    = img->valueData + index * img->rowByteCount + x * img->valueByteCount;
   a         = index + gMIN(img->height - index, h);

   for (; index < a; index++)
   {
      // Get the current color.
      VALUE_SET_RGBA_N1(
         value,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);
      VALUE_RGBA_N1_TO_TYPE(img->valueType, value);
      SET_VALUE(buffer, img->valueByteCount, value);

      // Set the pixel
      buffer += img->rowByteCount;

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRect
**************************************************************************************************/
GIMG_API Gb gimgSetRect(Gimg * const img, Gindex const x, Gindex const y, Gcount const w,
   Gcount const h, GimgValue const p)
{
   genter;

   greturnFalseIf(!img);

   gimgSetLineH(img, x, y,         w, p);
   gimgSetLineH(img, x, y + h - 1, w, p);

   gimgSetLineV(img, x,         y, h, p);
   gimgSetLineV(img, x + w - 1, y, h, p);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRectBlend
**************************************************************************************************/
GIMG_API Gb gimgSetRectBlend(Gimg * const img, Gindex const x, Gindex const y, Gcount const w,
   Gcount const h, GimgValue const color)
{
   genter;

   greturnFalseIf(!img);

   gimgSetLineHAlpha(img, x, y,         w, color);
   gimgSetLineHAlpha(img, x, y + h - 1, w, color);

   gimgSetLineVAlpha(img, x,         y + 1, h - 2, color);
   gimgSetLineVAlpha(img, x + w - 1, y + 1, h - 2, color);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRectFill
**************************************************************************************************/
GIMG_API Gb gimgSetRectFill(Gimg * const img, Gindex const x, Gindex const y, Gcount const w,
   Gcount const h, GimgValue const p)
{
   Gindex a,
          index,
          nx;
   Gcount nw,
          nh;

   genter;

   // clip
   nx = x;
   nw = w;
   if (nx < 0)
   {
      nw = nw + nx;
      nx = 0;
   }

   index = y;
   nh    = h;
   if (index < 0)
   {
      nh    = nh + index;
      index = 0;
   }

   nw =         gMIN(img->width  - nx, nw);
   a  = index + gMIN(img->height - y,  nh);
   for (; index < a; index++)
   {
      gimgSetLineH(img, nx, index, nw, p);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRectFillBlend
**************************************************************************************************/
GIMG_API Gb gimgSetRectFillBlend(Gimg * const img, Gindex const inx, Gindex const y,
   Gcount const inw, Gcount const inh, GimgValue const color)
{
   Gindex a,
          index,
          x;
   Gcount nw,
          h,
          w;

   genter;

   x = inx;
   h = inh;
   w = inw;

   // clip
   if (x < 0)
   {
      w = w + x;
      x = 0;
   }

   index = y;
   if (index < 0)
   {
      h     = h + index;
      index = 0;
   }

   nw =         gMIN(img->width  - x, w);
   a  = index + gMIN(img->height - y, h);
   for (; index < a; index++)
   {
      gimgSetLineHAlpha(img, x, index, nw, color);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRectFillInterpolateH
**************************************************************************************************/
GIMG_API Gb gimgSetRectFillInterpolateH(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const inw, Gcount const h, GimgValue const pa, GimgValue const pb)
{
   GimgValue     p;
   Gindex        a,
                 index;
   Gcount        w;
   Gn1           ra, rb,
                 ga, gb,
                 ba, bb,
                 aa, ab;
   GinterpI      ir,
                 ig,
                 ib;

   genter;

   greturnTrueIf(
      !img        ||
      x + inw < 0 || img->width - 1 < x);

   w = inw;

   // Get the color components of the input.
   VALUE_GET_RGBA_N1(pa, ra, ga, ba, aa);
   VALUE_GET_RGBA_N1(pb, rb, gb, bb, ab);

   // Set up the interpolators.
   ginterpISet(&ir, 0, (Gi4) ra, w, (Gi4) rb);
   ginterpISet(&ig, 0, (Gi4) ga, w, (Gi4) gb);
   ginterpISet(&ib, 0, (Gi4) ba, w, (Gi4) bb);

   // Clip.
   index = x;
   if (index < 0)
   {
      ginterpISkip(&ir, -index);
      ginterpISkip(&ig, -index);
      ginterpISkip(&ib, -index);
      w     = w + index;
      index = 0;
   }

   // Set the buffer pointer.
   a = index + gMIN(img->width - index, w);

   for (; index < a; index++)
   {
      // Get the current color.
      VALUE_SET_RGBA_N1(
         p,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      gimgSetLineV(img, index, y, h, p);

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetRectFillInterpolateV
**************************************************************************************************/
GIMG_API Gb gimgSetRectFillInterpolateV(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const w, Gcount const inh, GimgValue const pa, GimgValue const pb)
{
   GimgValue     p;
   Gindex        a,
                 index;
   Gcount        h;
   Gn1           ra, rb,
                 ga, gb,
                 ba, bb,
                 aa, ab;
   GinterpI      ir,
                 ig,
                 ib;

   genter;

   greturnTrueIf(
      !img      ||
      x + w < 0 || img->width - 1 < x);

   h = inh;

   // Get the color components of the input.
   VALUE_GET_RGBA_N1(pa, ra, ga, ba, aa);
   VALUE_GET_RGBA_N1(pb, rb, gb, bb, ab);

   // Set up the interpolators.
   ginterpISet(&ir, 0, (Gi4) ra, h, (Gi4) rb);
   ginterpISet(&ig, 0, (Gi4) ga, h, (Gi4) gb);
   ginterpISet(&ib, 0, (Gi4) ba, h, (Gi4) bb);

   // Clip.
   index = y;
   if (index < 0)
   {
      ginterpISkip(&ir, -index);
      ginterpISkip(&ig, -index);
      ginterpISkip(&ib, -index);
      h     = h + index;
      index = 0;
   }

   // Set the buffer pointer.
   a = index + gMIN(img->height - index, h);

   for (; index < a; index++)
   {
      // Get the current color.
      VALUE_SET_RGBA_N1(
         p,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      gimgSetLineH(img, x, index, w, p);

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetResizeFixed_1

Resizes an image that is an exact multiple of another image.
Images need to be of 1 byte type per channel.
**************************************************************************************************/
GIMG_API Gb gimgSetResizeFixed_1(Gimg * const img, Gimg const * const valueImg)
{
   Gn1         r, g, b, a;
   GimgValue   p;
   Gindex      dx,  dy,
               sx,  sy,
               sampleX, sampleY;
   Gcount      sampleCount,
               dw,  dh,
               sw,  sh,
               unsampleCount;
   Gn4         sumr, sumg, sumb, suma;

   genter;

   greturnFalseIf(
      !img      ||
      !valueImg ||
      !(img->valueType      & gimgType1 &&
        valueImg->valueType & gimgType1));

   sw = valueImg->width;
   sh = valueImg->height;
   dw = img->width;
   dh = img->height;

   sampleX     = sw / dw;
   sampleY     = sh / dh;
   sampleCount = sampleX * sampleY;

   // Parrallelize this loop
   forCount(dy, dh)
   {
      forCount(dx, dw)
      {
         unsampleCount = 0;
         sumr          =
            sumg       =
            sumb       =
            suma       = 0;

         forCount(sy, sampleY)
         {
            forCount(sx, sampleX)
            {
               GET_PIXEL_VALUE(valueImg, dx * sampleX + sx, dy * sampleY + sy, p);
               VALUE_TYPE_TO_RGBA_N1(valueImg->valueType, p);
               VALUE_GET_RGBA_N1(p, r, g, b, a);

               if (a == 0)
               {
                  unsampleCount++;
               }
               else
               {
                  sumr += r;
                  sumg += g;
                  sumb += b;
               }
               suma += a;
            }
         }

         // Average the samples.
         if (unsampleCount == sampleCount)
         {
            sumr = 0;
            sumg = 0;
            sumb = 0;
            suma = 0;
         }
         else
         {
            sumr /= (sampleCount - unsampleCount);
            sumg /= (sampleCount - unsampleCount);
            sumb /= (sampleCount - unsampleCount);
            suma /= sampleCount;
         }

         VALUE_SET_RGBA_N1(p, sumr, sumg, sumb, suma);
         VALUE_RGBA_N1_TO_TYPE(img->valueType, p);
         SET_PIXEL_VALUE(img, dx, dy, p);
      }
   }

   greturn gbTRUE;
}

#if 0
/**************************************************************************************************
func: gimgStore

Store in an image using gimgio.  So supported images are PNG, BMP, TIF, JPEG and
GRAW
**************************************************************************************************/
GIMG_API Gb gimgStore(Gimg const * const img, Gpath const * const filePath, Gr const incompression)
{
   Gb           result;
   Gimgio      *iio;
   GimgioFormat iiof;
   Gindex       row,
                pixel;
   Gn1         *buffer;
   GimgValue    color;
   Gr           compression;

   genter;

   compression = incompression;

   result = gbFALSE;

   iiof = gimgioGetFormatFromName(filePath);
   iio  = gimgioOpen(filePath, gimgioOpenWRITE, iiof);
   greturnFalseIf(!iio);

   // set the specifics
   gimgioSetWidth(      iio, gimgGetWidth( img));
   gimgioSetHeight(     iio, gimgGetHeight(img));
   gimgioSetTypeFile(   iio, gimgioTypeRGB | gimgioTypeALPHA | gimgioTypeN1);
   gimgioSetTypePixel(  iio, gimgioTypeRGB | gimgioTypeALPHA | gimgioTypeN1);

   // A compression is given.
   if (compression >= 0.)
   {
      gimgioSetCompression(iio, compression);
   }
   // Use a default compression.
   else
   {
      switch (iiof)
      {
      case gimgioFormatBMP:
      case gimgioFormatGIF:
      case gimgioFormatGRAW:
      case gimgioFormatPPM:
      case gimgioFormatRLE:
      case gimgioFormatTRG:
      case gimgioFormatTIFF:
         // full compression for these formats.
         compression = 100.;
         break;

      case gimgioFormatPNG:
         // Good and fast compression (more common).
         compression = 60.;
         break;

      case gimgioFormatJPG:
      default:
         // Best image quality.
         compression = 0.;
      }
   }


   buffer = (Gn1 *) gmemClocTypeArray(GimgValue, gimgGetWidth(img));
   gotoIf(!buffer, gimgStoreERROR);

   forCount(row, gimgGetHeight(img))
   {
      forCount(pixel, gimgGetWidth(img))
      {
         gimgGetColor(img, pixel, row, &color);
         gimgValueGET_RN1_GN1_BN1_AN1(
            buffer[pixel * 4 + 0],
            buffer[pixel * 4 + 1],
            buffer[pixel * 4 + 2],
            buffer[pixel * 4 + 3],
            color);
      }

      gimgioSetRow(iio, row);
      gotoIf(!gimgioSetPixelRow(iio, buffer), gimgStoreERROR);
   }

   result = gbTRUE;

gimgStoreERROR:
   gmemDloc(buffer);
   gimgioClose(iio);

   greturn result;
}
#endif

/**************************************************************************************************
func: gimgSetValue

value is RGBA (n1 at the moment.)
**************************************************************************************************/
GIMG_API Gb gimgSetValue(Gimg * const img, Gindex const x, Gindex const y, GimgValue const value)
{
   GimgValue iValue;

   genter;

   greturnFalseIf(
      !img                       ||
      x < 0  || img->width  <= x ||
      y < 0  || img->height <= y);

   iValue = value;
   VALUE_RGBA_N1_TO_TYPE(img->valueType, iValue);
   SET_PIXEL_VALUE(img, y, x, iValue);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgStart
**************************************************************************************************/
GIMG_API Gb gimgStart(void)
{
   Gindex index;

   genter;

   greturnTrueIf(_isStarted);

   // Initialize the gamma table.  Going one way.
   forCount(index, 256)
   {
      _percentTableN1[index] = N1_TO_PERCENT(index);
      _gammaTableN1[index]   = GAMMA_TO_LINEAR(_percentTableN1[index], gimgGAMMA);
      _fToNTableN1[index]    = (Gn1) ((((Gr8) index) / 255.0) * 100);
      _nToFTableN1[index]    = (Gn1) ((((Gr8) index) / 100.0) * 255);
   }

   _isStarted = gbTRUE;

   greturn _isStarted;
}

/**************************************************************************************************
func: gimgStop
**************************************************************************************************/
GIMG_API Gb gimgStop(void)
{
   genter;

   greturnTrueIf(!_isStarted);

   _isStarted = gbFALSE;

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSwapColor
**************************************************************************************************/
GIMG_API Gb gimgSwapColor(Gimg * const img, GimgValue const originalColor,
   GimgValue const newColor)
{
   Gindex    index;
   Gcount    count,
             valueByteCount;
   Gn1      *ibuffer;
   GimgValue value,
             newColorValue;

   genter;

   greturnFalseIf(!img);

   gmemClearType(&value, GimgValue);

   newColorValue = newColor;
   VALUE_RGBA_N1_TO_TYPE(img->valueType, newColorValue);

   valueByteCount = img->valueByteCount;
   count          = img->width * img->height;
   ibuffer        = img->valueData;

   for (index = 0; index < count; index++)
   {
      GET_VALUE(ibuffer, valueByteCount, value);
      VALUE_TYPE_TO_RGBA_N1(img->valueType, value);

      if (gmemIsEqual(&value, &originalColor, valueByteCount))
      {
         SET_VALUE(ibuffer, valueByteCount, newColorValue);
      }

      ibuffer += valueByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
