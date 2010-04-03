
#include "SDL.h"

#define TRUE 1
#define FALSE 0

typedef struct ball *NODEPTR; 

typedef struct ball{
   SDL_Surface* bitmap;
   int x,
       y,
       delta_x,
       delta_y;

   NODEPTR next;
   NODEPTR lasthit;
} ball;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char* WINDOW_TITLE = "Multiple Ball Bounce Demo";

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

ball * 
NewBall(char * name, int start_x, int start_y, int delta_x, int delta_y){


   ball * instance  = (ball *) malloc (sizeof(ball));

   if (instance) {

      instance->bitmap = SDL_LoadBMP(name);

      if (instance->bitmap){

         // set magenta to be the transparent color for the bitmap
         SDL_SetColorKey( instance->bitmap, SDL_SRCCOLORKEY, SDL_MapRGB(instance->bitmap->format, 255, 0, 255) );

         instance->x = start_x;
         instance->y = start_y;
         instance->delta_x=delta_x;
         instance->delta_y=delta_y;

         instance->next = NULL;
//         printf("found %s \n", name);

      } else {

         free(instance);
	 return NULL;

      }
   }

   return instance;

}

void 
DeleteBall(ball * theball){

   SDL_FreeSurface(theball->bitmap);

   theball->next = NULL;

   free(theball);

}

void
DrawBall(ball * theball, SDL_Surface* screen){
      drawSprite(theball->bitmap, screen, 0, 0, theball->x, theball->y, theball->bitmap->w, theball->bitmap->h);
}

void
UpdateBall(ball * theball){

   theball->x += theball->delta_x;
   if (theball->x>(WINDOW_WIDTH-theball->bitmap->w)){ theball->delta_x = -2 ; theball->lasthit = NULL; };
   if (theball->x< 0){ theball->delta_x = 2;  theball->lasthit = NULL;}

   theball->y += theball->delta_y;
   if (theball->y>(WINDOW_HEIGHT-theball->bitmap->h)){ theball->delta_y = -2;theball->lasthit = NULL ;};
   if (theball->y< 0){ theball->delta_y = 2; theball->lasthit = NULL;};
   
}

void
DeleteBallz(ball * ballz){

   ball * head = ballz;
   ball * next;

   while (head){
      next = head->next;
      DeleteBall(head);
      head = next;
   }

}

void
DrawBallz(ball * ballz, SDL_Surface* screen){

   ball * head = ballz;

   while (head){
      DrawBall(head, screen);
      head = head->next;
   }
}

int
IsHit(ball * ball1, ball * ball2){

    int left1, left2;
    int right1, right2;
    int top1, top2;
    int bottom1, bottom2;

    if (ball1->lasthit==ball2)
	return (0);

    left1 = ball1->x;
    left2 = ball2->x;
    right1 = ball1->x + ball1->bitmap->w - ball1->bitmap->w/8;
    right2 = ball2->x + ball2->bitmap->w - ball2->bitmap->w/8;
    top1 = ball1->y;
    top2 = ball2->y;
    bottom1 = ball1->y + ball1->bitmap->h - ball1->bitmap->h/8;
    bottom2 = ball2->y + ball2->bitmap->h - ball2->bitmap->h/8;

    if (bottom1 < top2) return(0);
    if (top1 > bottom2) return(0);

    if (right1 < left2) return(0);
    if (left1 > right2) return(0);

    return(1);

}

void 
Collision(ball* ballz){
   ball * head = ballz;
 
   head = head->next;

   while (head){
      if (IsHit(ballz, head)){
         head->delta_x *= -1 ;
         head->delta_y *= -1;
         head->lasthit = ballz;

         ballz->delta_x *= -1 ;
         ballz->delta_y *= -1;
	 ballz->lasthit = head;
      }
      Collision (head);
      head = head->next;
   }
}

void
UpdateBallz(ball * ballz){

   ball * head = ballz;

   while (head){
      UpdateBall(head);
      head = head->next;
   }

   Collision(ballz);

}

ball *
LoadBallz(){

    ball * head = NULL;
    ball * current;

    if (current = NewBall("ball.bmp", 100, 400, 2, -2)){
	if (head) {
	    current->next = head;
	}
	head = current;
    } 

    if (current = NewBall("BigBlueBall.bmp", 400, 100, -2, 2)){
	if (head) {
	    current->next = head;
	}
        head = current;
   }


    if (current = NewBall("ball.bmp", 300, 300, -2, 2)){
	if (head) {
	    current->next = head;
	}
	head = current;
    } 

    if (current = NewBall("BigBlueBall.bmp", 64, 200, 2, 2)){
	if (head) {
	    current->next = head;
	}
        head = current;
   }


    if (current = NewBall("ball.bmp", 32, 32, 2, 2)){
	if (head) {
	    current->next = head;
	}
	head = current;
    } 

    if (current = NewBall("BigBlueBall.bmp", 64, 128, 2, -2)){
	if (head) {
	    current->next = head;
	}
        head = current;
   }

   return head;
}

int main(int argc, char **argv)
{

   ball * ballz;

   SDL_Init( SDL_INIT_VIDEO );

   SDL_Surface* screen = SDL_SetVideoMode( WINDOW_WIDTH, WINDOW_HEIGHT, 0,
      SDL_HWSURFACE | SDL_DOUBLEBUF );
   SDL_WM_SetCaption( WINDOW_TITLE, 0 );

   ballz = LoadBallz();

   SDL_Event event;
   int gameRunning = TRUE;

   while (gameRunning && ballz)
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

      // Draw our balls.
      UpdateBallz(ballz);
      DrawBallz(ballz, screen);

      // Show the updated screen.
      SDL_Flip(screen);
      usleep (1000);
   }

   DeleteBallz(ballz);

   SDL_Quit();

   return 0;
}
