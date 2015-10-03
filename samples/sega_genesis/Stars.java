
import net.mikekohn.java_grinder.SegaGenesis;
import net.mikekohn.java_grinder.Memory;

public class Stars
{
  public static short[] palette =
  {
    0x000, 0xeee, 0x888, 0x444, 0x000
  };

  public static short[] stars_init =
  {
    // X0,  Y0,  DX,  DY   (160,112) is center
      140, 110,  -1,  -1,
      180,  80,   1,  -1,
      170,  120,  1,   2,
      190,  130,  2,   1,
      150,  130, -2,   1,
      150,  135, -2,   2,
      150,  90,  -1,  -2,
      170,  91,   1,  -2,
      165,  112,  2,   0,
      155,  110, -1,   0,
      140, 110,  -2,  -2,
      180,  80,   2,  -2,
      170,  120,  2,   3,
      190,  130,  3,   2,
      150,  130, -3,   2,
      150,  135, -3,   3,
      150,  90,  -2,  -3,
      170,  91,   2,  -3,
      165,  112,  3,   1,
      155,  110, -2,   1,
  };

  public static void moveStars(short[] stars)
  {
    int n;

    while(!SegaGenesis.inVerticalBlank());

    for (n = 0; n < stars.length; n += 4)
    {
      //SegaGenesis.plot(stars[n], stars[n + 1], 0);

      stars[n] += stars[n + 2];
      stars[n + 1] += stars[n + 3];

      // If star is beyond boundary, erase it and reset it
      if (stars[n] <= 0 || stars[n] >= 320 ||
          stars[n + 1] <= 0 || stars[n + 1] >= 224) 
      {
        int x = stars[n], y = stars[n + 1];

        for (int c = 1; c <= 4; c++)
        {
          SegaGenesis.plot(x, y, 0);
          x -= stars[n + 2];
          y -= stars[n + 3];
        }

        stars[n] = stars_init[n];
        stars[n + 1] = stars_init[n + 1];
      }

      // Draw a trail of stars
      int x = stars[n], y = stars[n + 1];

      for (int c = 1; c <= 4; c++)
      {
        SegaGenesis.plot(x, y, c);
        x -= stars[n + 2];
        y -= stars[n + 3];
      }
    }

    while(SegaGenesis.inVerticalBlank());
  }

  public static void run()
  {
    int state = 0;
    int delay = 200;
    int text_color = 0;
    int a,n;
    int dx;

    short[] stars = Memory.allocStackShorts(stars_init.length);

    // Text color starts out as black.
    SegaGenesis.setPalettePointer(49);
    SegaGenesis.setPaletteColor(0x000);

    // Need white and a few shades darker of pixels
    SegaGenesis.setPaletteColors(palette);

    // Copy stars to RAM
    for (n = 0; n < stars.length; n++)
    {
      stars[n] = stars_init[n];
    }

    // Main part of method to animate the stars
    for (a = 0; a < 1000; a++)
    {
      if (delay == 0)
      {
        if ((state & 0x3) == 1 && (a & 0x7) == 0)
        {
          SegaGenesis.setPalettePointer(49);
          SegaGenesis.setPaletteColor(text_color);
          if (text_color == 0xeee)
          {
            state++;
            delay = 150;
          }
          text_color += 0x222;
        }

        if ((state & 0x3) == 2 && (a & 0x7) == 0)
        {
          text_color -= 0x222;
          SegaGenesis.setPalettePointer(49);
          SegaGenesis.setPaletteColor(text_color);
          if (text_color == 0x000)
          {
            SegaGenesis.clearText();
            state += 2;
            delay = 150;
          }
        }

        if (state == 0)
        {
          SegaGenesis.setCursor(10, 11);
          SegaGenesis.print("THE[JAVA[GRINDER[TEAM");
          SegaGenesis.setCursor(16, 13);
          SegaGenesis.print("PRESENTS");
          state++;
        }
          else
        if (state == 4)
        {
          SegaGenesis.setCursor(14, 12);
          SegaGenesis.print("SEGA[GENESIS");
          state++;
        }
      }
        else
      {
        delay--;
      }

      moveStars(stars);
    }

    SegaGenesis.setHorizontalScrollAModeFull();
    n = 0;
    dx = 1;

    // Shift stars around
    for (a = 0; a < 1000; a++)
    {
      SegaGenesis.setHorizontalScrollB(0, n);
      SegaGenesis.setVerticalScrollB(n);
      n += dx;
      if (n > 8) { dx = -1; }
      if (n < -8) { dx = 1; }
      moveStars(stars);
    }

    SegaGenesis.setHorizontalScrollAModeLine();
    SegaGenesis.setHorizontalScrollB(0, 0);
    SegaGenesis.setVerticalScrollB(0);
  }
}
