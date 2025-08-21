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
type:
**************************************************************************************************/
#pragma pack(push, 1)

typedef struct
{
   Gn1  k;
} ValueKN1;

typedef struct
{
   Gn2  k;
} ValueKN2;

typedef struct
{
   Gn4  k;
} ValueKN4;

typedef struct
{
   Gn8  k;
} ValueKN8;

typedef struct
{
   Gr4  k;
} ValueKR4;

typedef struct
{
   Gr8  k;
} ValueKR8;


typedef struct
{
   Gn1  k;
   Gn1  a;
} ValueKAN1;

typedef struct
{
   Gn2  k;
   Gn2  a;
} ValueKAN2;

typedef struct
{
   Gn4  k;
   Gn4  a;
} ValueKAN4;

typedef struct
{
   Gn8  k;
   Gn8  a;
} ValueKAN8;

typedef struct
{
   Gr4  k;
   Gr4  a;
} ValueKAR4;

typedef struct
{
   Gr8  k;
   Gr8  a;
} ValueKAR8;


typedef struct
{
   Gn1  r;
   Gn1  g;
   Gn1  b;
} ValueRGBN1;

typedef struct
{
   Gn2  r;
   Gn2  g;
   Gn2  b;
} ValueRGBN2;

typedef struct
{
   Gn4  r;
   Gn4  g;
   Gn4  b;
} ValueRGBN4;

typedef struct
{
   Gn8  r;
   Gn8  g;
   Gn8  b;
} ValueRGBN8;

typedef struct
{
   Gr4  r;
   Gr4  g;
   Gr4  b;
} ValueRGBR4;

typedef struct
{
   Gr8  r;
   Gr8  g;
   Gr8  b;
} ValueRGBR8;


typedef struct
{
   Gn1  r;
   Gn1  g;
   Gn1  b;
   Gn1  a;
} ValueRGBAN1;

typedef struct
{
   Gn2  r;
   Gn2  g;
   Gn2  b;
   Gn2  a;
} ValueRGBAN2;

typedef struct
{
   Gn4  r;
   Gn4  g;
   Gn4  b;
   Gn4  a;
} ValueRGBAN4;

typedef struct
{
   Gn8  r;
   Gn8  g;
   Gn8  b;
   Gn8  a;
} ValueRGBAN8;

typedef struct
{
   Gr4  r;
   Gr4  g;
   Gr4  b;
   Gr4  a;
} ValueRGBAR4;

typedef struct
{
   Gr8  r;
   Gr8  g;
   Gr8  b;
   Gr8  a;
} ValueRGBAR8;


typedef struct
{
   Gp  *p;
} ValuePOINTER;

#pragma pack(pop)

/**************************************************************************************************
variable:
**************************************************************************************************/
static Gb    _isStarted = gbFALSE;
static Gr4   _percentTableN1[256];
static Gr4   _gammaTableN1[256];

/**************************************************************************************************
prototype:
**************************************************************************************************/
static Gb    _ResizeFixedK(   Gimg const * const simage, Gimg * const dimage);
static Gb    _ResizeFixedKA(  Gimg const * const simage, Gimg * const dimage);
static Gb    _ResizeFixedRGB( Gimg const * const simage, Gimg * const dimage);
static Gb    _ResizeFixedRGBA(Gimg const * const simage, Gimg * const dimage);

static void  _SetCircleLines( Gimg       * const img, Gindex const x, Gindex const y, Gindex const xx, Gindex const yy, GimgValue const color, Gb const isEven);
static void  _SetCirclePixels(Gimg       * const img, Gindex const x, Gindex const y, Gindex const xx, Gindex const yy, GimgValue const color, Gb const isEven);

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
func: gimgGetPixel
**************************************************************************************************/
GIMG_API Gb gimgGetPixel(Gimg const * const img, Gindex const x, Gindex const y,
   GimgValue * const value)
{
   genter;

   greturnFalseIf(
      !img                       ||
      x < 0  || img->width  <= x ||
      y < 0  || img->height <= y);

   // Find the offset into the pixel array.
   gmemCopyOverTypeArray(
      (img->valueData + y * img->rowByteCount + x * img->valueByteCount),
      Gn1,
      img->valueByteCount,
      value);

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgGetGammaTable
**************************************************************************************************/
Gr4 *gimgGetGammaTable(void)
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
Gr4 *gimgGetPercentTable(void)
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
            gimgColorCompileRGBA_N1(
               color,
               N4ToN1(r),
               N4ToN1(g),
               N4ToN1(b),
               N4ToN1(a));
            gimgSetColorUNSAFE(img, pixel, row, color);
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
func: gimgResizeFixed

Resizes an image that is an exact multiple of another image.
**************************************************************************************************/
GIMG_API Gb gimgResizeFixed(Gimg const * const simage, Gimg * const dimage)
{
   returnFalseIf(
      !simage ||
      !dimage ||
      simage->valueType != dimage->valueType);

   switch (simage->valueType)
   {
   case gimgTypeKN1:
   case gimgTypeKN2:
   case gimgTypeKN4:
   case gimgTypeKN8:
   case gimgTypeKR4:
   case gimgTypeKR8:
      greturn _ResizeFixedK(simage, dimage);

   case gimgTypeKN1_AN1:
   case gimgTypeKN2_AN2:
   case gimgTypeKN4_AN4:
   case gimgTypeKN8_AN8:
   case gimgTypeKR4_AR4:
   case gimgTypeKR8_AR8:
      greturn _ResizeFixedKA(simage, dimage);

   case gimgTypeRN1_GN1_BN1:
   case gimgTypeRN2_GN2_BN2:
   case gimgTypeRN4_GN4_BN4:
   case gimgTypeRN8_GN8_BN8:
   case gimgTypeRR4_GR4_BR4:
   case gimgTypeRR8_GR8_BR8:
      greturn _ResizeFixedRGB(simage, dimage);

   case gimgTypeRN1_GN1_BN1_AN1:
   case gimgTypeRN2_GN2_BN2_AN2:
   case gimgTypeRN4_GN4_BN4_AN4:
   case gimgTypeRN8_GN8_BN8_AN8:
   case gimgTypeRR4_GR4_BR4_AR4:
   case gimgTypeRR8_GR8_BR8_AR8:
      greturn _ResizeFixedRGB(simage, dimage);
   }

   greturn gbFALSE;
}

/**************************************************************************************************
func: gimgSetCircle
**************************************************************************************************/
GIMG_API Gb gimgSetCircle(Gimg * const img, Gindex const inx, Gindex const iny, Gcount const w,
   GimgValue const color)
{
   Gindex x,
          y,
          xx,
          yy,
          d,
          de,
          dse;

   genter;

   x = inx + (w - 1) / 2;
   y = iny + (w - 1) / 2;

   xx = 0;
   yy = (w - 1) / 2;

   d   =  1 - yy;
   de  =  3;
   dse = -2 * yy + 5;

   for (xx = 0; xx < yy; xx++)
   {
      if (d < 0)
      {
         d   += de;
         de  += 2;
         dse += 2;
      }
      else
      {
         d   += dse;
         de  += 2;
         dse += 4;
         yy--;
      }

      _SetCirclePixels(img, x, y, xx, yy, color, !(w & 1));
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetCircleFill
**************************************************************************************************/
GIMG_API Gb gimgSetCircleFill(Gimg * const img, Gindex const inx, Gindex const iny, Gcount const w,
   GimgValue const color)
{
   Gindex x,
          y,
          xx,
          yy,
          d,
          de,
          dse;

   genter;

   x = inx + (w - 1) / 2;
   y = iny + (w - 1) / 2;

   xx = 0;
   yy = (w - 1) / 2;

   d   =  1 - yy;
   de  =  3;
   dse = -2 * yy + 5;

   for (xx = 0; xx < yy; xx++)
   {
      if (d < 0)
      {
         d   += de;
         de  += 2;
         dse += 2;
      }
      else
      {
         d   += dse;
         de  += 2;
         dse += 4;
         yy--;
      }

      _SetCircleLines(img, x, y, xx, yy, color, !(w & 1));
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetPixelColor
**************************************************************************************************/
GIMG_API Gb gimgSetPixelColor(Gimg *img, Gindex x, Gindex y, GimgValue p)
{
#if 0
   GimgValue *buffer;

   genter;

   greturnFalseIf(
      !image ||
      x < 0  || img->width  <= x ||
      y < 0  || img->height <= y);

   buffer = imageBUFFER(image);

   // Find the offset into the pixel array.
   *(buffer + y * img->width + x) = p;
#endif
   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetClear
**************************************************************************************************/
GIMG_API Gb gimgSetClear(Gimg * const img, GimgValue const color)
{
   Gindex     index;
   Gcount     count;
   GimgValue *cp;

   genter;

   greturnFalseIf(!img);

   cp    = img->valueData;
   count = img->height * img->width;
   forCount(index, count)
   {
      *cp = color;
      cp++;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImage
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
              vh;
   GimgValue *ibuffer,
             *vbuffer;

   genter;

   greturnFalseIf(
      !img                  ||
      !value                ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   ibuffer = gimgGetBuffer(img);
   vbuffer = gimgGetBuffer(value);

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
      gmemCopyOverTypeArrayAt(
         vbuffer,
         GimgValue,
         vw,
         vy * value->width + vx,
         ibuffer,
         iy * img->width + ix);
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetImageAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetImageAlpha(Gimg * const img, Gindex const x, Gindex const y,
   Gimg const * const value)
{
   Gindex     ix, iy,
              vx, vy,
              iindex, vindex;
   Gcount     iw, ih,
              vw, vh;
   Gn1        vr, vg, vb, va,
              ir, ig, ib, ia;
   GimgValue *ibuffer,
             *vbuffer;
   Gr4        vpercent,
              ipartPercent;

   genter;

   greturnFalseIf(
      !img                  ||
      !value                ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   ibuffer = gimgGetBuffer(img);
   vbuffer = gimgGetBuffer(value);

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
         gimgColorDecompileRGBA_N1(
            vr,
            vg,
            vb,
            va,
            vbuffer[vy * value->width + vindex]);
         continueIf(va == 0);

         if (va == 255)
         {
            gimgColorCompileRGBA_N1(
               ibuffer[iy * img->width + iindex],
               (Gn1) vr,
               (Gn1) vg,
               (Gn1) vb,
               (Gn1) va);
            continue;
         }

         gimgColorDecompileRGBA_N1(
            ir,
            ig,
            ib,
            ia,
            ibuffer[iy * img->width + iindex]);

         // Merge the colors
         vpercent     = ((Gr4) va) / (255.0f);
         ipartPercent = 1.0f - vpercent;

         ir = (Gn1) (ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr));
         ig = (Gn1) (ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg));
         ib = (Gn1) (ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb));

         gimgColorCompileRGBA_N1(
            ibuffer[iy * img->width + iindex],
            (Gn1) ir,
            (Gn1) ig,
            (Gn1) ib,
            (Gn1) ia);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgAlphaSetImageAlpha

Destination is an alphaed image.  Source image is an alphaed image.  Final
image will still be an alphaed image.
**************************************************************************************************/
GIMG_API Gb gimgAlphaSetImageAlpha(Gimg * const img, Gindex const x, Gindex const y,
   Gimg const * const value)
{
   Gindex     ix, iy,
              vx, vy,
              iindex, vindex;
   Gcount     iw, ih,
              vw, vh;
   Gn1        vr, vg, vb, va,
              ir, ig, ib, ia;
   GimgValue *ibuffer,
             *vbuffer;
   Gr4        vpercent,
              ipercent,
              ipartPercent,
              percent;

   genter;

   greturnFalseIf(
      !img                  ||
      !value                ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   ibuffer = gimgGetBuffer(img);
   vbuffer = gimgGetBuffer(value);

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
         gimgColorDecompileRGBA_N1(
            vr,
            vg,
            vb,
            va,
            vbuffer[vy * value->width + vindex]);
         continueIf(va == 0);

         if (va == 255)
         {
            gimgColorCompileRGBA_N1(
               ibuffer[iy * img->width + iindex],
               (Gn1) vr,
               (Gn1) vg,
               (Gn1) vb,
               (Gn1) va);
            continue;
         }

         gimgColorDecompileRGBA_N1(
            ir,
            ig,
            ib,
            ia,
            ibuffer[iy * img->width + iindex]);

         // Merge the colors
         vpercent     = ((Gr4) va) / (255.0f);
         ipercent     = ((Gr4) ia) / (255.0f);
         ipartPercent = (1.0f - vpercent) * ipercent;
         percent      = ipartPercent + vpercent;

         ir = (Gn1) ((ipartPercent * ((Gr4) ir) + vpercent * ((Gr4) vr)) / percent);
         ig = (Gn1) ((ipartPercent * ((Gr4) ig) + vpercent * ((Gr4) vg)) / percent);
         ib = (Gn1) ((ipartPercent * ((Gr4) ib) + vpercent * ((Gr4) vb)) / percent);
         ia = (Gn1) (255.0f * percent);

         gimgColorCompileRGBA_N1(
            ibuffer[iy * img->width + iindex],
            (Gn1) ir,
            (Gn1) ig,
            (Gn1) ib,
            (Gn1) ia);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgAlphaSetImageAlphaSub

Destination is an alphaed image.  Source image is an alphaed image.  Final
image will still be an alphaed image.

Source image is just removing, making transparent, parts of the destination
image.
**************************************************************************************************/
GIMG_API Gb gimgAlphaSetImageAlphaSub(Gimg * const img, Gindex const x,
   Gindex const y, Gimg const * const value)
{
   Gindex     ix, iy,
              vx, vy,
              iindex, vindex;
   Gcount     iw, ih,
              vw, vh;
   Gn1        vr, vg, vb, va,
              ir, ig, ib, ia;
   GimgValue *ibuffer,
             *vbuffer;
   Gr4        vpercent,
              ipercent;

   genter;

   greturnFalseIf(
      !img                  ||
      !value                ||
      x + value->width  < 0 || img->width  - 1 < x ||
      y + value->height < 0 || img->height - 1 < y);

   ibuffer = gimgGetBuffer(img);
   vbuffer = gimgGetBuffer(value);

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
         gimgColorDecompileRGBA_N1(
            vr,
            vg,
            vb,
            va,
            vbuffer[vy * value->width + vindex]);
         continueIf(va == 0);

         if (va == 255)
         {
            gimgColorCompileRGBA_N1(
               ibuffer[iy * img->width + iindex],
               (Gn1) vr,
               (Gn1) vg,
               (Gn1) vb,
               (Gn1) va);
            continue;
         }

         gimgColorDecompileRGBA_N1(
            ir,
            ig,
            ib,
            ia,
            ibuffer[iy * img->width + iindex]);

         // Adjust the alpha
         vpercent  = 1.0f     - (((Gr4) va) / (255.0f));
         ipercent  = vpercent * (((Gr4) ia) / (255.0f));

         ia = (Gn1) (255.0f * ipercent);

         gimgColorCompileRGBA_N1(
            ibuffer[iy * img->width + iindex],
            (Gn1) ir,
            (Gn1) ig,
            (Gn1) ib,
            (Gn1) ia);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLine
**************************************************************************************************/
GIMG_API Gb gimgSetLine(Gimg * const img, Gindex const x1, Gindex const y1, Gindex const x2,
   Gindex const y2, GimgValue const p)
{
   GinterpI i;

   genter;

   // line is visible after the clip.
   if (gimgClipLine(img, &x1, &y1, &x2, &y2))
   {
      // assume x is the longer axis.
      ginterpISet(&i, x1, y1, x2, y2);

      // plot the pixels.
      for (; !ginterpIIsDoneHalf(&i); ginterpIGetNext(&i))
      {
         gimgSetColor(
            img,
            ginterpIGetX1(&i),
            ginterpIGetY1(&i),
            p);
         gimgSetColor(
            img,
            ginterpIGetX2(&i),
            ginterpIGetY2(&i),
            p);
      }

      greturn gbTRUE;
   }

   greturn gbFALSE;
}

/**************************************************************************************************
func: gimgSetLineH
**************************************************************************************************/
GIMG_API Gb gimgSetLineH(Gimg * const img, Gindex const x, Gindex const y, Gcount const w,
   GimgValue const p)
{
   GimgValue *buffer;
   Gindex     a,
              index;
   Gcount     nw;

   genter;

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   // Clip.
   index = x;
   nw    = w;
   if (index < 0)
   {
      nw     = nw + index;
      index = 0;
   }

   // Find the start pixel.
   buffer = img->valueData + y * img->width + index;
   a      = index + gMIN(img->width - index, nw);

   for (; index < a; index++)
   {
      // Set the pixel
      *buffer = p;
      buffer++;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineHAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetLineHAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inw,
   GimgValue const color)
{
   GimgValue *buffer;
   Gindex     a,
              index;
   Gcount     w;
   Gn1        cr,  cg,  cb,  ca,
              sr,  sg,  sb,  sa;
   Gi4        cir, cig, cib, cia;

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
   buffer = img->valueData + y * img->width + index;
   a      = index + gMIN(img->width - index, w);

   // color to alpha in.
   gimgColorDecompileRGBA_N1(cr, cg, cb, ca, color);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      gimgColorDecompileRGBA_N1(sr, sg, sb, sa, *buffer);

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the pixel
      gimgColorCompileRGBA_N1(
         *buffer,
         sr,
         sg,
         sb,
         sa);
      buffer++;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineHInterpolate
**************************************************************************************************/
GIMG_API Gb gimgSetLineHInterpolate(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const inw, GimgValue const pa, GimgValue const pb)
{
   GimgValue    *buffer,
                 p;
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

   greturnFalseIf(
      !img  ||
      y < 0 || img->height - 1 < y);

   w = inw;

   // Get the color components of the input.
   gimgColorDecompileRGBA_N1(ra, ga, ba, aa, pa);
   gimgColorDecompileRGBA_N1(rb, gb, bb, ab, pb);

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
   buffer = img->valueData + y * img->width + index;
   a      = index + gMIN(img->width - index, w);

   for (; index < a; index++)
   {
      // Get the current color.
      gimgColorCompileRGBA_N1(
         p,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      *buffer = p;
      buffer++;

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
   GimgValue *buffer;
   Gindex     a,
              index;
   Goffset    rowOffset;
   Gcount     h;

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
   buffer    = img->valueData + index * img->width + x;
   a         = index + gMIN(img->height - index, h);
   rowOffset = img->width;

   for (; index < a; index++)
   {
      *buffer = p;
      buffer  = buffer + rowOffset;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineVAlpha
**************************************************************************************************/
GIMG_API Gb gimgSetLineVAlpha(Gimg * const img, Gindex const x, Gindex const y, Gcount const inh,
   GimgValue const color)
{
   GimgValue *buffer;
   Gindex     a,
              index;
   Goffset    rowOffset;
   Gcount     h;
   Gn1        cr,  cg,  cb,  ca,
              sr,  sg,  sb,  sa;
   Gi4        cir, cig, cib, cia;

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
   buffer    = img->valueData + index * img->width + x;
   a         = index + gMIN(img->height - index, h);
   rowOffset = img->width;

   // color to alpha in.
   gimgColorDecompileRGBA_N1(cr, cg, cb, ca, color);
   cir = cr * ca;
   cig = cg * ca;
   cib = cb * ca;
   cia = 256 - ca;
   for (; index < a; index++)
   {
      // Get the existing pixel
      gimgColorDecompileRGBA_N1(sr, sg, sb, sa, *buffer);

      // Merge the colors
      sr = (Gn1) ((((Gi4) sr) * cia + cir) >> 8);
      sg = (Gn1) ((((Gi4) sg) * cia + cig) >> 8);
      sb = (Gn1) ((((Gi4) sb) * cia + cib) >> 8);

      // Set the color.
      gimgColorCompileRGBA_N1(*buffer, sr, sg, sb, sa);
      buffer  = buffer + rowOffset;
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: gimgSetLineVInterpolate
**************************************************************************************************/
GIMG_API Gb gimgSetLineVInterpolate(Gimg * const img, Gindex const x, Gindex const y,
   Gcount const inh, GimgValue const pa, GimgValue const pb)
{
   GimgValue    *buffer,
                 p;
   Gindex        a,
                 index;
   Goffset       rowOffset;
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
      !img  ||
      x < 0 || img->width - 1 < x);

   h = inh;

   // Get the color components of the input.
   gimgColorDecompileRGBA_N1(ra, ga, ba, aa, pa);
   gimgColorDecompileRGBA_N1(rb, gb, bb, ab, pb);

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
   buffer    = img->valueData + index * img->width + x;
   a         = index + gMIN(img->height - index, h);
   rowOffset = img->width;

   for (; index < a; index++)
   {
      // Get the current color.
      gimgColorCompileRGBA_N1(
         p,
         (Gn1) ginterpIGetY1(&ir),
         (Gn1) ginterpIGetY1(&ig),
         (Gn1) ginterpIGetY1(&ib),
         aa);

      // Set the pixel
      *buffer = p;
      buffer  = buffer + rowOffset;

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
   gimgColorDecompileRGBA_N1(ra, ga, ba, aa, pa);
   gimgColorDecompileRGBA_N1(rb, gb, bb, ab, pb);

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
      gimgColorCompileRGBA_N1(
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
   gimgColorDecompileRGBA_N1(ra, ga, ba, aa, pa);
   gimgColorDecompileRGBA_N1(rb, gb, bb, ab, pb);

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
      gimgColorCompileRGBA_N1(
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
         gimgColorDecompileRGBA_N1(
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
   Gindex       index;
   Gcount       count;
   GimgValue   *ibuffer;

   genter;

   greturnFalseIf(!img);

   count   = img->width * img->height;
   ibuffer = gimgGetBuffer(img);

   for (index = 0; index < count; index++)
   {
      if (ibuffer[index].color == originalColor.color)
      {
         ibuffer[index] = newColor;
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
local:
function:
**************************************************************************************************/
/**************************************************************************************************
func: _ResizeFixedK
**************************************************************************************************/
static Gb _ResizeFixedK(Gimg const * const simage, Gimg * const dimage)
{
   genter;
   greturn gbFALSE;
}

/**************************************************************************************************
func: _ResizeFixedKA
**************************************************************************************************/
static Gb _ResizeFixedKA(Gimg const * const simage, Gimg * const dimage)
{
   genter;
   greturn gbFALSE;
}

/**************************************************************************************************
func: _ResizeFixedRGB
**************************************************************************************************/
static Gb _ResizeFixedRGB(Gimg const * const simage, Gimg * const dimage)
{
   genter;
   greturn gbFALSE;
}

/**************************************************************************************************
func: _ResizeFixedRGBA
**************************************************************************************************/
static Gb _ResizeFixedRGBA(Gimg const * const simage, Gimg * const dimage)
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

   sw = gimgGetWidth( simage);
   sh = gimgGetHeight(simage);
   dw = gimgGetWidth( dimage);
   dh = gimgGetHeight(dimage);

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
               gimgGetPixel(simage, dx * sampleX + sx, dy * sampleY + sy, &p);
               gimgColorDecompileRGBA_N1(r, g, b, a, p);

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

         gimgColorCompileRGBA_N1(
            p,
            (Gn1) sumr,
            (Gn1) sumg,
            (Gn1) sumb,
            (Gn1) suma);
         gimgSetColorUNSAFE(
            dimage,
            dx,
            dy,
            p);
      }
   }

   greturn gbTRUE;
}

/**************************************************************************************************
func: _SetCircleLines
**************************************************************************************************/
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

/**************************************************************************************************
func: _SetCirclePixels
**************************************************************************************************/
static void _SetCirclePixels(Gimg * const img, Gindex const x, Gindex const y,
   Gindex const xx, Gindex const yy, GimgValue const color, Gb const isEven)
{
   genter;
   gimgSetPixelColor(img, x + xx + isEven, y + yy + isEven, color);
   gimgSetPixelColor(img, x + yy + isEven, y + xx + isEven, color);
   gimgSetPixelColor(img, x + yy + isEven, y - xx,          color);
   gimgSetPixelColor(img, x + xx + isEven, y - yy,          color);
   gimgSetPixelColor(img, x - xx,          y - yy,          color);
   gimgSetPixelColor(img, x - yy,          y - xx,          color);
   gimgSetPixelColor(img, x - yy,          y + xx + isEven, color);
   gimgSetPixelColor(img, x - xx,          y + yy + isEven, color);
   greturn;
}
