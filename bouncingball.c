
#include "SDL.h"

#define TRUE 1
#define FALSE 0

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const int SPRITE_WIDTH = 64;
const int SPRITE_HEIGHT = 64;
const char* WINDOW_TITLE = "SDL Start";

void drawSprite(SDL_Surface* imageSurface,
                SDL_Surface* screenSurface,
                int srcX, int srcY,
                int dstX, int dstY,
                int width, int height)
{
   // Part of the bitmap that we want to draw
   SDL_Rect srcRect;
   srcRect.x = srcX;
   srcRect.y = srcY;
   srcRect.w = width;
   srcRect.h = height;

   // Part of the screen we want to draw the sprite to
   SDL_Rect dstRect;
   dstRect.x = dstX;
   dstRect.y = dstY;
   dstRect.w = width;
   dstRect.h = height;

   SDL_BlitSurface(imageSurface, &srcRect, screenSurface, &dstRect);
}

int main(int argc, char **argv)
{
	int x = SPRITE_WIDTH/2,
	    y = SPRITE_HEIGHT/2,
            delta_x = 5,
            delta_y = 5;

   SDL_Init( SDL_INIT_VIDEO );

   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      SDL_HWSURFACE | SDL_DOUBLEBUF );
   SDL_WM_SetCaption( WINDOW_TITLE, 0 );

   SDL_Surface* bitmap = SDL_LoadBMP("ball.bmp");

   // set magenta to be the transparent color for the bitmap
   SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) );


   SDL_Event event;
   int gameRunning = TRUE;

   while (gameRunning)
   {

      if (SDL_PollEvent(&event))
      {
         if (event.type == SDL_QUIT)
         {
            gameRunning = FALSE;
         }
      }

      // Clear the screen
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

      x=x+delta_x;
      if (x>(WINDOW_WIDTH-SPRITE_WIDTH)){ delta_x *= -1 ;};
      if (x< 0){ delta_x *= -1 ;};

      y=y+delta_y;
      if (y>(WINDOW_HEIGHT-SPRITE_HEIGHT)){ delta_y *= -1; };
      if (y< 0){ delta_y *= -1; };

      drawSprite(bitmap, screen, 0, 0, x, y, SPRITE_WIDTH, SPRITE_HEIGHT);

      SDL_Flip(screen);
//      printf("x %d %d  y %d %d\n", x, delta_x, y, delta_y);
//      fflush(stdout);
   }

   SDL_FreeSurface(bitmap);

   SDL_Quit();

   return 0;
}
