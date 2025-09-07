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
#define SET_PIXEL_VALUE(IMG, X, Y, COLOR)                                                          \
   SET_VALUE(                                                                                      \
      IMG->valueData + (Y) * IMG->rowByteCount + (X) * IMG->valueByteCount,                        \
      IMG->valueByteCount,                                                                         \
      COLOR);

#define SET_VALUE(PIXEL, COLOR_BYTE_COUNT, COLOR)                                                  \
   {                                                                                               \
      Gn1   *___pixel___ = (Gn1 *) (PIXEL);                                                        \
      Gindex ___index___ = 0;                                                                      \
      switch (COLOR_BYTE_COUNT)                                                                    \
      {                                                                                            \
      case 32: (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 24: (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 16: (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 12: (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 8:  (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
               (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 4:  (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 3:  (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      case 2:  (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      default: (*___pixel___++) = (COLOR).raw[___index___++];                                      \
      }                                                                                            \
   }

#define GET_PIXEL_VALUE(IMG, X, Y, COLOR)                                                          \
   GET_VALUE(                                                                                      \
      *(COLOR),                                                                                    \
      IMG->valueByteCount,                                                                         \
      IMG->valueData + (Y) * IMG->rowByteCount + (X) * IMG->valueByteCount)

#define GET_VALUE(COLOR, COLOR_BYTE_COUNT, PIXEL)                                                  \
   {                                                                                               \
      Gn1   *___pixel___ = (Gn1 *) (PIXEL);                                                        \
      Gindex ___index___ = 0;                                                                      \
      switch (COLOR_BYTE_COUNT)                                                                    \
      {                                                                                            \
      case 32: (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 24: (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 16: (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 12: (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 8:  (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
               (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 4:  (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 3:  (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      case 2:  (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      default: (COLOR).raw[___index___++] = (*___pixel___++);                                      \
      }                                                                                            \
   }

#define VALUE_TO_RGBA_N1(TYPE, COLOR)                                                              \
   {                                                                                               \
      switch (TYPE)                                                                                \
      {                                                                                            \
      default:                                                                                     \
      case gimgTypeRN1_GN1_BN1_AN1:                                                                \
         /* Nothing to do */                                                                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKN1:                                                                            \
         /* Need to set A to 255. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_G]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = (COLOR).n1[gimgValueIndexK_K];                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKN1_AN1:                                                                        \
         /* Need to move A first. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = (COLOR).n1[gimgValueIndexK_A];                       \
         /* Need to duplicate K over GB.  R and K are the same location */                         \
         (COLOR).n1[gimgValueIndexRGB_G]    =                                                      \
            (COLOR).n1[gimgValueIndexRGB_B] = (COLOR).n1[gimgValueIndexK_K];                       \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeRN1_GN1_BN1:                                                                    \
         /* Need to set A to 255. */                                                               \
         (COLOR).n1[gimgValueIndexRGB_A]    = 255;                                                 \
         /* RGB are in the same location as RGB in RGBA. */                                        \
         break;                                                                                    \
      }                                                                                            \
   }

#define VALUE_FROM_RGBA_N1(TYPE, COLOR)                                                            \
   {                                                                                               \
      GimgValue ___v___;                                                                           \
      switch (TYPE)                                                                                \
      {                                                                                            \
      default:                                                                                     \
      case gimgTypeKN1:                                                                            \
      case gimgTypeRN1_GN1_BN1:                                                                    \
      case gimgTypeRN1_GN1_BN1_AN1:                                                                \
         /* Nothing to do R and K are at the same location, RGB are at the same location */        \
         break;                                                                                    \
                                                                                                   \
      case gimgTypeKN1_AN1:                                                                        \
         /* R and K are at the same location.  A needs to move over. */                            \
         (COLOR).kn1_an1[gimgValueIndexK_A] = ___v___.n1[gimgValueIndexRGB_A];                     \
         break;                                                                                    \
      }                                                                                            \
   }


/**************************************************************************************************
variable:
**************************************************************************************************/
static Gb    _isStarted = gbFALSE;
static Gr    _percentTableN1[256];
static Gr    _gammaTableN1[256];

/**************************************************************************************************
prototype:
**************************************************************************************************/
#if 0
static void  _SetCirclePixelsEven(     Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width, Gindex const index, Gindex const step, GimgValue const color);
static void  _SetCirclePixelsOdd(      Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width, Gindex const index, Gindex const step, GimgValue const color);
static void  _SetCircleLinesOddFirst(  Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width, GimgValue const color);
static void  _SetCircleLines(          Gimg       * const img, Gindex const x, Gindex const y, Gindex const xx, Gindex const yy, GimgValue const color, Gb const isEven);
#endif
static void  _SetCirclePixels(         Gimg       * const img, Gindex const x, Gindex const y, Gindex const xx, Gindex const yy, GimgValue const color, Gb const isEven);
static void  _SetCirclePixelsEven(     Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width, Gindex const index, Gindex const step, GimgValue const color);
static void  _SetCirclePixelsOdd(      Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width, Gindex const index, Gindex const step, GimgValue const color);
static void  _SetCirclePixelsOddFirst( Gimg       * const img, Gindex const x, Gindex const y, Gindex const offset, Gcount const width,                                        GimgValue const color);

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
   case gimgTypeKN1:             img->valueByteCount = 1;            break;
   case gimgTypeKN2:             img->valueByteCount = 2;            break;
   case gimgTypeKN4:             img->valueByteCount = 4;            break;
   case gimgTypeKN8:             img->valueByteCount = 8;            break;
   case gimgTypeKR4:             img->valueByteCount = 4;            break;
   case gimgTypeKR8:             img->valueByteCount = 8;            break;
   case gimgTypeKN1_AN1:         img->valueByteCount = 2;            break;
   case gimgTypeKN2_AN2:         img->valueByteCount = 4;            break;
   case gimgTypeKN4_AN4:         img->valueByteCount = 8;            break;
   case gimgTypeKN8_AN8:         img->valueByteCount = 16;           break;
   case gimgTypeKR4_AR4:         img->valueByteCount = 8;            break;
   case gimgTypeKR8_AR8:         img->valueByteCount = 16;           break;
   case gimgTypeRN1_GN1_BN1:     img->valueByteCount = 3;            break;
   case gimgTypeRN2_GN2_BN2:     img->valueByteCount = 6;            break;
   case gimgTypeRN4_GN4_BN4:     img->valueByteCount = 12;           break;
   case gimgTypeRN8_GN8_BN8:     img->valueByteCount = 24;           break;
   case gimgTypeRR4_GR4_BR4:     img->valueByteCount = 12;           break;
   case gimgTypeRR8_GR8_BR8:     img->valueByteCount = 24;           break;
   case gimgTypeRN1_GN1_BN1_AN1: img->valueByteCount = 4;            break;
   case gimgTypeRN2_GN2_BN2_AN2: img->valueByteCount = 8;            break;
   case gimgTypeRN4_GN4_BN4_AN4: img->valueByteCount = 16;           break;
   case gimgTypeRN8_GN8_BN8_AN8: img->valueByteCount = 32;           break;
   case gimgTypeRR4_GR4_BR4_AR4: img->valueByteCount = 16;           break;
   case gimgTypeRR8_GR8_BR8_AR8: img->valueByteCount = 32;           break;
   case gimgTypePOINTER:         img->valueByteCount = sizeof(Gp *); break;
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

   // Find the offset into the pixel array.
   GET_PIXEL_VALUE(img, x, y, value);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgGetGammaTable
**************************************************************************************************/
Gr *gimgGetGammaTable(void)
{
   genter;

   greturn _gammaTableN1;
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
func: gimgGetPercentTable
**************************************************************************************************/
Gr *gimgGetPercentTable(void)
{
   genter;

   greturn _percentTableN1;
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
func: gimgResizeFixed_N1

Resizes an image that is an exact multiple of another image.
**************************************************************************************************/
GIMG_API Gb gimgResizeFixed_N1(Gimg const * const simage, Gimg * const dimage)
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
      !simage ||
      !dimage ||
      !(simage->valueType & gimgTypeN1 &&
        dimage->valueType & gimgTypeN1));

   sw = simage->width;
   sh = simage->height;
   dw = dimage->width;
   dh = dimage->height;

   sampleX     = sw / dw;
   sampleY     = sh / dh;
   sampleCount = sampleX * sampleY;

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
               GET_PIXEL_VALUE(simage, dx, dy, &p);
               VALUE_TO_RGBA_N1(simage->valueType, p);
               gimgValueGET_RN1_GN1_BN1_AN1(r, g, b, a, p);

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

         gimgValueSET_RN1_GN1_BN1_AN1(p, sumr, sumg, sumb, suma);
         VALUE_FROM_RGBA_N1(dimage->valueType, p);
         SET_PIXEL_VALUE(dimage, dx, dy, p);
      }
   }

   greturn gbTRUE;
}

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
          step;
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

      rsq = (Gr4) (radius * radius);
      xr  = 0;
      y1  = (Gr4)  radius;
      y2  = (Gr4) (radius - 1);

      // First iteration is 4 symetry.
      _SetCirclePixelsOddFirst(img, inx, iny, offset, w, color);

      step = 0;
      for (index = 1; index < offset; index++)
      {
         xr = (Gr4) index;
         y1 = (Gr4)  radius - step;
         y2 = (Gr4) (radius - step - 1);

         xrsq = xr * xr;
         breakIf(xrsq + 0.5 >= rsq);

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
         _SetCirclePixelsOdd(img, inx, iny, offset, w, index, step, color);
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

      rsq = (Gr4) (radius + 0.5f) * (radius + 0.5f);
      xr  = 0.5f;
      y1  = (Gr4) radius + 0.5f;
      y2  = (Gr4) radius - 0.5f;

      _SetCirclePixelsEven(img, inx, iny, offset, w, 0, 0, color);

      step = 0;
      for (index = 1; index < offset; index++)
      {
         xr = (Gr4) index + 0.5f;
         y1 = (Gr4) (radius - step) + 0.5f;
         y2 = (Gr4) (radius - step) - 0.5f;

          xrsq = xr * xr;
         breakIf(xrsq + 0.5 >= rsq);

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
         _SetCirclePixelsEven(img, inx, iny, offset, w, index, step, color);
      }
   }

   greturn gbTRUE;
}

#if 0
/**************************************************************************************************
func: gimgSetCircleFill
**************************************************************************************************/
GIMG_API Gb gimgSetCircleFill(Gimg * const img, Gindex const inx, Gindex const iny, Gcount const w,
   GimgValue const color)
{
   Gindex offset,
          radius,
          index,
          step;
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

   greturnIf(w == 1, gimgSetValue( img, inx, iny,    color));
   greturnIf(w == 2, gimgSetCircle(img, inx, iny, w, color));

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

      rsq = (Gr4) (radius * radius);
      xr  = 0;
      y1  = (Gr4)  radius;
      y2  = (Gr4) (radius - 1);

      // First iteration is 4 symetry.
      _SetCirclePixelsOddFirst(img, inx, iny, offset, w, color);

      step = 0;
      for (index = 1; index < offset; index++)
      {
         xr = (Gr4) index;
         y1 = (Gr4)  radius - step;
         y2 = (Gr4) (radius - step - 1);

         xrsq = xr * xr;
         breakIf(xrsq + 0.5 >= rsq);

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
         _SetCirclePixelsOdd(img, inx, iny, offset, w, index, step, color);
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

      rsq = (Gr4) (radius + 0.5f) * (radius + 0.5f);
      xr  = 0.5f;
      y1  = (Gr4) radius + 0.5f;
      y2  = (Gr4) radius - 0.5f;

      _SetCirclePixelsEven(img, inx, iny, offset, w, 0, 0, color);

      step = 0;
      for (index = 1; index < offset; index++)
      {
         xr = (Gr4) index + 0.5f;
         y1 = (Gr4) (radius - step) + 0.5f;
         y2 = (Gr4) (radius - step) - 0.5f;

          xrsq = xr * xr;
         breakIf(xrsq + 0.5 >= rsq);

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
         _SetCirclePixelsEven(img, inx, iny, offset, w, index, step, color);
      }
   }

   greturn gbTRUE;
}
#endif

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

#if 0
/**************************************************************************************************
func: gimgSetImage_N1

Copy the new image over an old image.  This doesn't merge images or do any alpha blending.  Both
images need to be of the same type.  Works with any image type.
**************************************************************************************************/
GIMG_API Gb gimgSetImage_N1(Gimg * const img, Gindex const x, Gindex const y, Gimg const * const value)
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
      !(img->valueType   & gimgTypeN1 &&
        value->valueType & gimgTypeN1)             ||
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
      gmemCopyOverTypeArrayAt(
         vValueData,
         Gn,
         vw                       * valueByteCount,
         (vy * value->width + vx) * valueByteCount,
         iValueData,
         (iy * img->width   + ix) * valueByteCount);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImageA

Treat the destination image as an opaque image.  The source image is an alpah image.  Only works
with RN1_GN1_BN1_AN1 images.
**************************************************************************************************/
GIMG_API Gb gimgSetImageA(Gimg * const img, Gindex const x, Gindex const y,
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
      img->valueType != value->valueType           ||
      !(img->valueType == gimgTypeRN1_GN1_BN1_AN1) ||
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
      for (iindex = ix, vindex = vx; iindex < iw; iindex++, vindex++)
      {
         GET_VALUE(
            color,
            valueByteCount,
            vValueData + vy * vrowByteCount + vindex * valueByteCount);

         gimgValueGET_RN1_GN1_BN1_AN1(vr, vg, vb, va, color);

         continueIf(va == 0);

         if (va == 255)
         {
            SET_VALUE(
               iValueData + iy * irowByteCount + iindex * valueByteCount,
               valueByteCount,
               color);
            continue;
         }

         GET_VALUE(color, valueByteCount, iValueData + iy * irowByteCount + iindex * valueByteCount);
         gimgValueGET_RN1_GN1_BN1_AN1(ir, ig, ib, ia, color);

         // Merge pixels
         vpercent     = ((Gr4) va) / (255.0f);
         ipartPercent = 1.0f - vpercent;


         gimgValueSET_RN1_GN1_BN1_AN1(
            color,
            (Gn1) (ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr)),
            (Gn1) (ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg)),
            (Gn1) (ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb)),
            (Gn1) ia);

         SET_VALUE(
            iValueData + iy * irowByteCount + iindex * valueByteCount,
            valueByteCount,
            color);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImageAA

Both Destination and Source image are alpha images.  Final image will still be an alpha image.
**************************************************************************************************/
GIMG_API Gb gimgSetImageAA(Gimg * const img, Gindex const x, Gindex const y,
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
      img->valueType != value->valueType           ||
      !(img->valueType == gimgTypeRN1_GN1_BN1_AN1) ||
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
         GET_VALUE(
            color,
            valueByteCount,
            vValueData + vy * vrowByteCount + vindex * valueByteCount);

         gimgValueGET_RN1_GN1_BN1_AN1(vr, vg, vb, va, color);

         continueIf(va == 0);

         // New pixel is opaque, overwrite the old pixel completely.
         if (va == 255)
         {
            SET_VALUE(
               iValueData + iy * irowByteCount + iindex * valueByteCount,
               valueByteCount,
               color);
            continue;
         }

         // Calculate the new pixel color by merging the two colors.
         GET_VALUE(color, valueByteCount, iValueData + iy * irowByteCount + iindex * valueByteCount);
         gimgValueGET_RN1_GN1_BN1_AN1(ir, ig, ib, ia, color);

         // Merge pixels.
         vpercent     = ((Gr4) va) / (255.0f);
         ipercent     = ((Gr4) ia) / (255.0f);
         ipartPercent = (1.0f - vpercent) * ipercent;
         percent      = ipartPercent + vpercent;

         gimgValueSET_RN1_GN1_BN1_AN1(
            color,
            (Gn1) ((ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr)) / percent),
            (Gn1) ((ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg)) / percent),
            (Gn1) ((ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb)) / percent),
            (Gn1) (255.0f * percent));

         SET_VALUE(
            iValueData + iy * irowByteCount + iindex * valueByteCount,
            valueByteCount,
            color);
      }
   }

   greturn gbTRUE;
}
#endif

/**************************************************************************************************
func: gimgSetLine
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
**************************************************************************************************/
GIMG_API Gb gimgSetLineH(Gimg * const img, Gindex const x, Gindex const y, Gcount const inw,
   GimgValue const p)
{
   Gn1   *buffer;
   Gindex a,
          index;
   Gcount nw;

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

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
      SET_VALUE(buffer, img->valueByteCount, p);
      buffer += img->valueByteCount;
   }

   greturn gbTRUE;
}

#if 0
/**************************************************************************************************
func: gimgSetLineHAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetLineHAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inw,
   GimgValue const color)
{
   Gn1   *buffer;
   Gindex a,
          index;
   Gcount w;
   Gn1    cr,  cg,  cb,  ca,
          sr,  sg,  sb,  sa;
   Gi4    cir, cig, cib, cia;

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
   gimgValueGET_RN1_GN1_BN1_AN1(cr, cg, cb, ca, color);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      gimgValueGET_RN1_GN1_BN1_AN1(sr, sg, sb, sa, *((GimgValue *) buffer));

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the pixel
      gimgValueSET_RN1_GN1_BN1_AN1(*((GimgValue *) buffer), sr, sg, sb, sa);

      buffer += img->valueByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineHInterpolate
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

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   w = inw;

   // Get the color components of the input.
   gimgValueGET_RN1_GN1_BN1_AN1(ra, ga, ba, aa, pa);
   gimgValueGET_RN1_GN1_BN1_AN1(rb, gb, bb, ab, pb);

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
      gimgValueSET_RN1_GN1_BN1_AN1(
         *((GimgValue *) buffer),
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      buffer += img->valueByteCount;

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}
#endif

/**************************************************************************************************
func: gimgSetLineV
**************************************************************************************************/
GIMG_API Gb gimgSetLineV(Gimg * const img, Gindex const x, Gindex const y, Gcount const inh,
   GimgValue const p)
{
   Gn1   *buffer;
   Gindex a,
          index;
   Gcount nh;

   genter;

   greturnTrueIf(
      !img  ||
      x < 0 || img->width - 1 < x);

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
      SET_VALUE(buffer, img->valueByteCount, p);
      buffer += img->rowByteCount;
   }

   greturn gbTRUE;
}

#if 0
/**************************************************************************************************
func: gimgSetLineVAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetLineVAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inh,
   GimgValue const color)
{
   Gn1   *buffer;
   Gindex a,
          index;
   Gcount h;
   Gn1    cr,  cg,  cb,  ca,
          sr,  sg,  sb,  sa;
   Gi4    cir, cig, cib, cia;

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
   gimgValueGET_RN1_GN1_BN1_AN1(cr, cg, cb, ca, color);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      gimgValueGET_RN1_GN1_BN1_AN1(sr, sg, sb, sa, *((GimgValue *) buffer));

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the color.
      gimgValueSET_RN1_GN1_BN1_AN1(*((GimgValue *) buffer), sr, sg, sb, sa);
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

   genter;

   greturnTrueIf(
      !img  ||
      x < 0 || img->width - 1 < x);

   h = inh;

   // Get the color components of the input.
   gimgValueGET_RN1_GN1_BN1_AN1(ra, ga, ba, aa, pa);
   gimgValueGET_RN1_GN1_BN1_AN1(rb, gb, bb, ab, pb);

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
      gimgValueSET_RN1_GN1_BN1_AN1(
         *((GimgValue *) buffer),
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      buffer += img->rowByteCount;

      // Interpolate the color step.
      ginterpIGetNext(&ir);
      ginterpIGetNext(&ig);
      ginterpIGetNext(&ib);
   }

   greturn gbTRUE;
}
#endif

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

#if 0
/**************************************************************************************************
func: gimgSetRectAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetRectAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const w,
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
#endif

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

#if 0
/**************************************************************************************************
func: gimgSetRectFillAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetRectFillAlpha(Gimg * const img, Gindex const inx, Gindex const y,
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
   gimgValueGET_RN1_GN1_BN1_AN1(ra, ga, ba, aa, pa);
   gimgValueGET_RN1_GN1_BN1_AN1(rb, gb, bb, ab, pb);

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
      gimgValueSET_RN1_GN1_BN1_AN1(
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
   gimgValueGET_RN1_GN1_BN1_AN1(ra, ga, ba, aa, pa);
   gimgValueGET_RN1_GN1_BN1_AN1(rb, gb, bb, ab, pb);

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
      gimgValueSET_RN1_GN1_BN1_AN1(
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
**************************************************************************************************/
GIMG_API Gb gimgSetValue(Gimg * const img, Gindex const x, Gindex const y, GimgValue const value)
{
   genter;

   greturnFalseIf(
      !img                       ||
      x < 0  || img->width  <= x ||
      y < 0  || img->height <= y);

   SET_PIXEL_VALUE(img, y, x, value);

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
      _percentTableN1[index] = gimgN1ToPercent(index);
      _gammaTableN1[index]   = gimgGammaToLinear(_percentTableN1[index], gimgGAMMA);
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
   GimgValue value;

   genter;

   greturnFalseIf(!img);

   gmemClearType(&value, GimgValue);

   valueByteCount = img->valueByteCount;
   count          = img->width * img->height;
   ibuffer        = img->valueData;

   for (index = 0; index < count; index++)
   {
      GET_VALUE(value, valueByteCount, ibuffer);

      if (gmemIsEqual(&value, &originalColor, valueByteCount))
      {
         SET_VALUE(ibuffer, valueByteCount, newColor);
      }

      ibuffer += valueByteCount;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/

#if 0
/**************************************************************************************************
func: _SetCircleLinesEven
**************************************************************************************************/
static void _SetCirclePixelsEven(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, Gindex const index, Gindex const step,
   GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset + index,     y             + step,   color);
   gimgSetValue(img, x + offset - index - 1, y             + step,   color);
   gimgSetValue(img, x + offset + index,     y + width - 1 - step,   color);
   gimgSetValue(img, x + offset - index - 1, y + width - 1 - step,   color);
   gimgSetValue(img, x             + step,   y + offset + index,     color);
   gimgSetValue(img, x             + step,   y + offset - index - 1, color);
   gimgSetValue(img, x + width - 1 - step,   y + offset + index,     color);
   gimgSetValue(img, x + width - 1 - step,   y + offset - index - 1, color);
   greturn;
}

/**************************************************************************************************
func: _SetCircleLinesOdd
**************************************************************************************************/
static void _SetCirclePixelsOdd(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, Gindex const index, Gindex const step,
   GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset + index,   y             + step, color);
   gimgSetValue(img, x + offset - index,   y             + step, color);
   gimgSetValue(img, x + offset + index,   y + width - 1 - step, color);
   gimgSetValue(img, x + offset - index,   y + width - 1 - step, color);
   gimgSetValue(img, x             + step, y + offset + index,   color);
   gimgSetValue(img, x             + step, y + offset - index,   color);
   gimgSetValue(img, x + width - 1 - step, y + offset + index,   color);
   gimgSetValue(img, x + width - 1 - step, y + offset - index,   color);
   greturn;
}

/**************************************************************************************************
func: _SetCircleLinesOddFirst
**************************************************************************************************/
static void _SetCircleLinesOddFirst(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset,    y,             color);
   gimgSetValue(img, x + offset,    y + width - 1, color);

   gimgSetLineH(img, x,             y + offset, width, color);
   greturn;
}

static void _SetCircleLines(Gimg * const img, Gindex const x, Gindex const y, Gindex const xx,
   Gindex const yy, GimgValue const color, Gb const isEven)
{
   genter;
   gimgSetLineH(img, x - xx, y + yy + isEven, 2 * (xx + isEven), color);
   gimgSetLineH(img, x - yy, y + xx + isEven, 2 * (yy + isEven), color);
   gimgSetLineH(img, x - yy, y - xx,          2 * (yy + isEven), color);
   gimgSetLineH(img, x - xx, y - yy,          2 * (xx + isEven), color);
   greturn;
}
#endif

/**************************************************************************************************
func: _SetCirclePixelsEven
**************************************************************************************************/
static void _SetCirclePixelsEven(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, Gindex const index, Gindex const step,
   GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset + index,     y             + step,   color);
   gimgSetValue(img, x + offset - index - 1, y             + step,   color);
   gimgSetValue(img, x + offset + index,     y + width - 1 - step,   color);
   gimgSetValue(img, x + offset - index - 1, y + width - 1 - step,   color);
   gimgSetValue(img, x             + step,   y + offset + index,     color);
   gimgSetValue(img, x             + step,   y + offset - index - 1, color);
   gimgSetValue(img, x + width - 1 - step,   y + offset + index,     color);
   gimgSetValue(img, x + width - 1 - step,   y + offset - index - 1, color);
   greturn;
}

/**************************************************************************************************
func: _SetCirclePixelsOdd
**************************************************************************************************/
static void _SetCirclePixelsOdd(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, Gindex const index, Gindex const step,
   GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset + index,   y             + step, color);
   gimgSetValue(img, x + offset - index,   y             + step, color);
   gimgSetValue(img, x + offset + index,   y + width - 1 - step, color);
   gimgSetValue(img, x + offset - index,   y + width - 1 - step, color);
   gimgSetValue(img, x             + step, y + offset + index,   color);
   gimgSetValue(img, x             + step, y + offset - index,   color);
   gimgSetValue(img, x + width - 1 - step, y + offset + index,   color);
   gimgSetValue(img, x + width - 1 - step, y + offset - index,   color);
   greturn;
}

/**************************************************************************************************
func: _SetCirclePixelsOddFirst
**************************************************************************************************/
static void _SetCirclePixelsOddFirst(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const offset, Gcount const width, GimgValue const color)
{
   genter;
   gimgSetValue(img, x + offset,    y,             color);
   gimgSetValue(img, x + offset,    y + width - 1, color);
   gimgSetValue(img, x,             y + offset,    color);
   gimgSetValue(img, x + width - 1, y + offset,    color);
   greturn;
}
