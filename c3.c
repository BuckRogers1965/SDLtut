
#include <SDL.h>
#include <math.h>

#define TRUE 1
#define FALSE 0

#define TOP  1
#define BOTTOM 2
#define RIGHT 4
#define LEFT 8

typedef struct ball *NODEPTR;

typedef struct ball
{
  SDL_Surface *bitmap;
  int x, y, delta_x, delta_y;
  float mass;

  NODEPTR next;
  NODEPTR lasthit;
} ball;

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;
const char *WINDOW_TITLE = "Multiple Ball Bounce Sprite Collision Demo";

void
drawSprite (SDL_Surface * imageSurface,
	    SDL_Surface * screenSurface,
	    int srcX, int srcY, int dstX, int dstY, int width, int height)
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

  SDL_BlitSurface (imageSurface, &srcRect, screenSurface, &dstRect);
}

ball *
NewBall (char *name, int start_x, int start_y, int delta_x, int delta_y, float mass)
{


  ball *instance = (ball *) malloc (sizeof (ball));

  if (instance)
    {

      instance->bitmap = SDL_LoadBMP (name);

      if (instance->bitmap)
	{

	  // set magenta to be the transparent color for the bitmap
	  SDL_SetColorKey (instance->bitmap, SDL_SRCCOLORKEY,
			   SDL_MapRGB (instance->bitmap->format, 255, 0,
				       255));

	  instance->x = start_x;
	  instance->y = start_y;
	  instance->delta_x = delta_x;
	  instance->delta_y = delta_y;

	  instance->next = NULL;
//         printf("found %s \n", name);
	  instance->mass = mass;

	}
      else
	{

	  free (instance);
	  return NULL;

	}
    }

  return instance;

}

void
DeleteBall (ball * theball)
{

  SDL_FreeSurface (theball->bitmap);

  theball->next = NULL;

  free (theball);

}

void
DrawBall (ball * theball, SDL_Surface * screen)
{
  drawSprite (theball->bitmap, screen, 0, 0, theball->x, theball->y,
	      theball->bitmap->w, theball->bitmap->h);
}

void
UpdateBall (ball * theball)
{

  theball->x += theball->delta_x;
  if (theball->x > (WINDOW_WIDTH - theball->bitmap->w))
    {
      theball->delta_x = -abs (theball->delta_x);
    };
  if (theball->x < 0)
    {
      theball->delta_x = abs (theball->delta_x);
      theball->lasthit = NULL;
    }

  theball->y += theball->delta_y;
  if (theball->y < 0)
    {
      theball->delta_y = abs (theball->delta_y);
      theball->lasthit = NULL;
    };
  if (theball->y > (WINDOW_HEIGHT - theball->bitmap->h))
    {
      theball->delta_y = -abs (theball->delta_y);
      theball->y -= 1;
    }
  ;
//else


// gravity
 theball->delta_y+=1;


}

void
DeleteBallz (ball * ballz)
{

  ball *head = ballz;
  ball *next;

  while (head)
    {
      next = head->next;
      DeleteBall (head);
      head = next;
    }

}

void
DrawBallz (ball * ballz, SDL_Surface * screen)
{

  ball *head = ballz;

  while (head)
    {
      DrawBall (head, screen);
      head = head->next;
    }
}

int
IsHit (ball * ball1, ball * ball2)
{

  int left1, left2;
  int right1, right2;
  int top1, top2;
  int bottom1, bottom2;
#define OFFSET 8

  if (ball1->lasthit == ball2)
    return (0);

  left1 = ball1->x + ball1->bitmap->w / OFFSET;
  left2 = ball2->x + ball2->bitmap->w / OFFSET;
  right1 = ball1->x + ball1->bitmap->w - ball1->bitmap->w / OFFSET;
  right2 = ball2->x + ball2->bitmap->w - ball2->bitmap->w / OFFSET;
  top1 = ball1->y + ball1->bitmap->w / OFFSET;
  top2 = ball2->y + ball2->bitmap->w / OFFSET;
  bottom1 = ball1->y + ball1->bitmap->h - ball1->bitmap->h / OFFSET;
  bottom2 = ball2->y + ball2->bitmap->h - ball2->bitmap->h / OFFSET;

  if (bottom1 < top2)
    return (0);
  if (top1 > bottom2)
    return (0);

  if (right1 < left2)
    return (0);
  if (left1 > right2)
    return (0);

  return (1);

}

float
DotVector (float ax, float ay, float bx, float by)
{
//returns the dot product between 2 vectors a and b.
  return ((ax * bx) + (ay * by));
}

void
NormalizeVector (float *nx, float *ny, float X1, float Y1, float X2, float Y2)
{

// normalizes the components of a vector derived
// from 2 points (x1,y1) and '(x2,y2)

  float dx, dy, dist;

  dx = X2 - X1;

  dy = Y2 - Y1;

  dist = sqrt (pow (dx, 2) + pow (dy, 2));

  *nx = dx / dist;

  *ny = dy / dist;
}

void
Collision (ball * ballz)
{
  ball *head = ballz;

  head = head->next;

  while (head)
    {
      if (IsHit (ballz, head))
	{

	  float dx, dy, w, h;
	  float n_x, n_y;
	  float hcx, hcy, bcx, bcy;
	  float hdx, hdy, bdx, bdy;

	  float impact_x, impact_y, impulse_x, impulse_y, impactSpeed;
//Formula:

// 1. Impact= balla-ballb         'vector subtaction
// 2. impulse = Normalize(p1,p2)  'see normalize sub
// 3. impactspeed = Dot!(impact,impulse)
// 4. newimpulse=impulse*impactspeed*mass1*mass2
// 5. newa=balla+newimpulse/mass1
// 6. newb=ballb-newimpulse/mass2
// PS the divide by 2 is for the frictionless model. ;*)


	  impact_x = head->delta_x - ballz->delta_x + 1;
	  impact_y = head->delta_y - ballz->delta_y + 1;

	  NormalizeVector (&impulse_x, &impulse_y,
			   ballz->x, ballz->y, head->x, head->y);

	  impactSpeed =
	    DotVector (impact_x, impact_y, impulse_x, impulse_y) + 10;

	  printf ("impact %f %f impulse %f %f %f \n", impact_x, impact_y,
		  impulse_x, impulse_y, impactSpeed);

	  impulse_x = impulse_x * impactSpeed * ballz->mass * head->mass / 2;

	  impulse_y = impulse_y * impactSpeed * ballz->mass * head->mass / 2;

	  printf ("impact %f %f impulse %f %f %f \n", impact_x, impact_y,
		  impulse_x, impulse_y, impactSpeed);

	  bdx = ballz->delta_x + impulse_x / ballz->mass;
	  bdy = ballz->delta_y + impulse_y / ballz->mass;

	  hdx = head->delta_x - impulse_x / head->mass;
	  hdy = head->delta_y - impulse_y / head->mass;

	  head->delta_x = bdx;
	  head->delta_y = bdy;

	  ballz->delta_x = hdx;
	  ballz->delta_y = hdy;

	  head->x += 2;
	  head->y += 2;

	  printf ("Impulse %f %f Impact %f %f head %d %d ballz %d %d  - \n\n",
		  impulse_x, impulse_y, impact_x, impact_y, head->delta_x,
		  head->delta_y, ballz->delta_x, ballz->delta_y);
//fflush(stdout);
	  head->lasthit = ballz;
	  ballz->lasthit = head;
	}
      else
	{
	  if (ballz->lasthit == head->lasthit)
	    {
	      ballz->lasthit = NULL;
	      head->lasthit = NULL;
	    }
	}
      Collision (head);
      head = head->next;
    }
}

void
UpdateBallz (ball * ballz)
{

  ball *head = ballz;

  while (head)
    {
      UpdateBall (head);
      head = head->next;
    }

  Collision (ballz);

}

ball *
LoadBallz ()
{

  ball *head = NULL;
  ball *current;

  if (current = NewBall ("ball.bmp", 100, 400, 5, -15, 1))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }

  if (current = NewBall ("BigBlueBall.bmp", 400, 100, -30, 4, 2))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }


  if (current = NewBall ("ball.bmp", 300, 300, -10, 2, 1))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }

  if (current = NewBall ("BigBlueBall.bmp", 64, 200, 15, 16, 2))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }


  if (current = NewBall ("ball.bmp", 32, 32, 3, 20, 1))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }

  if (current = NewBall ("BigBlueBall.bmp", 64, 128, 10, -2, 2))
    {
      if (head)
	{
	  current->next = head;
	}
      head = current;
    }

  return head;
}

int
main (int argc, char **argv)
{

  ball *ballz;

  SDL_Init (SDL_INIT_VIDEO);

  SDL_Surface *screen = SDL_SetVideoMode (WINDOW_WIDTH, WINDOW_HEIGHT, 0,
					  SDL_HWSURFACE | SDL_DOUBLEBUF);
  SDL_WM_SetCaption (WINDOW_TITLE, 0);

  ballz = LoadBallz ();

  SDL_Event event;
  int gameRunning = TRUE;

  while (gameRunning && ballz)
    {

      if (SDL_PollEvent (&event))
	{
	  if (event.type == SDL_QUIT)
	    {
	      gameRunning = FALSE;
	    }
	}

      // Clear the screen
      SDL_FillRect (screen, NULL, SDL_MapRGB (screen->format, 0, 0, 0));

      // Draw our balls.
      UpdateBallz (ballz);
      DrawBallz (ballz, screen);

      // Show the updated screen.
      SDL_Flip (screen);
      usleep (10000);
    }

  DeleteBallz (ballz);

  SDL_Quit ();

  return 0;
}
