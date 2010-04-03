#include "SDL.h"
#include "SDL_ttf.h"

#define TRUE 1
#define FALSE 0

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "SDL Font Demo";

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
   SDL_Init( SDL_INIT_VIDEO );

   TTF_Init();

   TTF_Font* font = TTF_OpenFont("arial.ttf", 72);

   if (!font) {
      printf ("Font arial.ttf not found.\n");
      TTF_Quit();
      SDL_Quit();
      exit (0);
   }

   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      SDL_HWSURFACE | SDL_DOUBLEBUF );
   SDL_WM_SetCaption( WINDOW_TITLE, 0 );

   SDL_Color foregroundColor = { 222, 0, 0 };
   SDL_Color backgroundColor = { 255, 0, 255 };

   SDL_Surface* textSurface = TTF_RenderText_Blended(font, "Hello, World!",
      foregroundColor);
   // set magenta to be the transparent color for the bitmap
//   SDL_SetColorKey( textSurface, SDL_SRCCOLORKEY, SDL_MapRGB(textSurface->format, 255, 0, 255) );

   SDL_Surface* bitmap = SDL_LoadBMP("BigBlueBall.bmp");

   // set magenta to be the transparent color for the bitmap
   SDL_SetColorKey( bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(bitmap->format, 255, 0, 255) );


   // Pass zero for width and height to draw the whole surface
   SDL_Rect textLocation = { WINDOW_WIDTH/2-textSurface->w/2, WINDOW_HEIGHT/2-textSurface->h/2, 0, 0 };

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

      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

      drawSprite(bitmap, screen, 0, 0, 90, 160, 128, 128);

      SDL_BlitSurface(textSurface, NULL, screen, &textLocation);


      SDL_Flip(screen);
   }

   SDL_FreeSurface(textSurface);

   TTF_CloseFont(font);

   TTF_Quit();

   SDL_Quit();

   return 0;
}
