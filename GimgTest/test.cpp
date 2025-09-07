#include "pch.h"

/**************************************************************************************************
prototype:
**************************************************************************************************/

/**************************************************************************************************
test: libraryStart
**************************************************************************************************/
TEST(Gimg, LibraryStart)
{
   EXPECT_TRUE(grlStart());
   EXPECT_TRUE(gimgStart());

   gimgStop();
   grlStop();
}

/**************************************************************************************************
test: Cloc
**************************************************************************************************/
TEST(Gimg, Cloc)
{
   Gimg     *img;
   Gindex    pixelIndex;
   Gcount    pixelCount;
   Gn1      *data,
             r,
             g,
             b,
             a;

   grlStart();
   gimgStart();

   img = gimgCloc(gimgTypeRN1_GN1_BN1_AN1, 8, 8);
   EXPECT_TRUE(img != nullptr);

   EXPECT_TRUE(img->width           == 8);
   EXPECT_TRUE(img->height          == 8);
   EXPECT_TRUE(img->valueType       == gimgTypeRN1_GN1_BN1_AN1);
   EXPECT_TRUE(img->valueByteCount  == 4);
   EXPECT_TRUE(img->rowByteCount    == 8 * 4);

   data       = img->valueData;
   pixelCount = 8 * 8;
   forCount(pixelIndex, pixelCount)
   {
      gimgValueGET_RN1_GN1_BN1_AN1(r, g, b, a, *((GimgValue *) data));
      EXPECT_TRUE(r == 0 && g == 0 && b == 0 && a == 0);

      data += img->valueByteCount;
   }

   gimgDloc(img);

   img = gimgCloc(gimgTypeRN1_GN1_BN1, 8, 8);
   EXPECT_TRUE(img != nullptr);

   EXPECT_TRUE(img->valueByteCount  == 3);
   EXPECT_TRUE(img->rowByteCount    == 8 * 3);

   data = img->valueData;
   forCount(pixelIndex, pixelCount)
   {
      gimgValueGET_RN1_GN1_BN1(r, g, b, *((GimgValue *) data));
      EXPECT_TRUE(r == 0 && g == 0 && b == 0);

      data += img->valueByteCount;
   }

   gimgDloc(img);

   gimgStop();
   grlStop();
}

/**************************************************************************************************
test: SetClear
**************************************************************************************************/
TEST(Gimg, SetClear)
{
   Gimg     *img;
   Gindex    pixelIndex;
   Gcount    pixelCount;
   GimgValue color;
   Gn1      *data,
             r,
             g,
             b,
             a;

   grlStart();
   gimgStart();

   gmemClearType(&color, GimgValue);

   img = gimgCloc(gimgTypeRN1_GN1_BN1_AN1, 8, 8);

   gimgValueGET_WHITE_N1(color);
   gimgSetClear(img, color);

   data       = img->valueData;
   pixelCount = 8 * 8;
   forCount(pixelIndex, pixelCount)
   {
      gimgValueGET_RN1_GN1_BN1_AN1(r, g, b, a, *((GimgValue *) data));
      EXPECT_TRUE(r == 255 && g == 255 && b == 255 && a == 255);

      data += img->valueByteCount;
   }

   gimgDloc(img);

   gimgStop();
   grlStop();
}

/**************************************************************************************************
test: SetLineHAndV
**************************************************************************************************/
TEST(Gimg, SetLineHAndV)
{
   Gimg     *img;
   Gindex    pixelIndex;
   Gcount    pixelCount;
   GimgValue color;
   Gn1      *data,
             r,
             g,
             b,
             a;

   grlStart();
   gimgStart();

   gmemClearType(&color, GimgValue);

   img = gimgCloc(gimgTypeRN1_GN1_BN1_AN1, 8, 8);

   // Set the background to white.
   gimgValueGET_WHITE_N1(color);
   gimgSetClear(img, color);

   // Set the edges to black.
   gimgValueGET_BLACK_N1(color);
   gimgSetLineH(img, 0, 0, 8, color);
   gimgSetLineV(img, 0, 0, 8, color);
   gimgSetLineH(img, 0, 7, 8, color);
   gimgSetLineV(img, 7, 0, 8, color);

   data       = img->valueData;
   pixelCount = 8 * 8;
   forCount(pixelIndex, pixelCount)
   {
      gimgValueGET_RN1_GN1_BN1_AN1(r, g, b, a, *((GimgValue *) data));

      if (pixelIndex % 8 == 0 ||
          pixelIndex % 8 == 7 ||
          pixelIndex / 8 == 0 ||
          pixelIndex / 8 == 7)
      {
         EXPECT_TRUE(r == 0   && g == 0   && b == 0   && a == 255);
      }
      else
      {
         EXPECT_TRUE(r == 255 && g == 255 && b == 255 && a == 255);
      }

      data += img->valueByteCount;
   }

   gimgDloc(img);

   gimgStop();
   grlStop();
}

/**************************************************************************************************
test: SetCircle
**************************************************************************************************/
TEST(Gimg, SetCircle)
{
   Gimg     *img;
   Gindex    radius,
             pixelIndex;
   Gcount    pixelCount;
   GimgValue color;
   Gn1      *data,
             r,
             g,
             b,
             a;

   grlStart();
   gimgStart();

   gmemClearType(&color, GimgValue);

   img = gimgCloc(gimgTypeRN1_GN1_BN1_AN1, 8, 8);

   forCount(radius, 16)
   {
      // Set the background to white.
      gimgValueGET_WHITE_N1(color);
      gimgSetClear(img, color);

      // Set the edges to black.
      gimgValueGET_BLACK_N1(color);
      gimgSetCircle(img, 0, 0, radius, color);
   }

#if 0
   data       = img->valueData;
   pixelCount = 8 * 8;
   forCount(pixelIndex, pixelCount)
   {
      gimgValueGET_RN1_GN1_BN1_AN1(r, g, b, a, *((GimgValue *) data));

      if (pixelIndex % 8 == 0 ||
          pixelIndex % 8 == 7 ||
          pixelIndex / 8 == 0 ||
          pixelIndex / 8 == 7)
      {
         EXPECT_TRUE(r == 0   && g == 0   && b == 0   && a == 255);
      }
      else
      {
         EXPECT_TRUE(r == 255 && g == 255 && b == 255 && a == 255);
      }

      data += img->valueByteCount;
   }
#endif

   gimgDloc(img);

   gimgStop();
   grlStop();
}
