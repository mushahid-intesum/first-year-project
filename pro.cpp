/*This source code copyrighted by Lazy Foo' Productions (2004-2019)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, standard IO, and strings
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

//Screen dimension constants
const int SCREEN_WIDTH = 1600;
const int SCREEN_HEIGHT = 900;
int SH= SCREEN_HEIGHT;
int SW= SCREEN_WIDTH;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;
SDL_Surface* screenSurface=NULL;

int GOV=0;

//Current displayed texture
SDL_Texture* gStart = NULL;
SDL_Texture* gQuit = NULL;
SDL_Texture* gHS = NULL;
SDL_Texture* gRestart = NULL;
SDL_Texture* gTexture;
SDL_Texture* rect=NULL;
SDL_Texture* GOT = NULL;
SDL_Texture* GST = NULL;


bool initmenu()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

		//Create window
		gWindow = SDL_CreateWindow( "THE GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		
		



		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0x00 );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					success = false;
				}
			}
		}
	}

	return success;
}

bool loadMediaStart()
{
	//Loading success flag
	bool success = true;

	//Load texture
	gStart = loadTexture( "start.png" );
	gRestart = loadTexture("restart.png");
	gQuit = loadTexture ("quit.png");
	gHS = loadTexture("Hiscore.png");
	GOT = loadTexture("gameover.png");
	GST= loadTexture("play.png");
	return success;
}

void close()
{
	//Free loaded image
	SDL_DestroyTexture( gStart );
	SDL_DestroyTexture( gRestart );
	SDL_DestroyTexture( gQuit );
	SDL_DestroyTexture(gHS);

	gTexture = NULL;

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );
	}

	return newTexture;
}

int main( int argc, char* args[] )
{

	int flag = 1;


	//Start up SDL and create window
	if( !initmenu() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMediaStart() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;
			SDL_Texture *selTexture = SDL_CreateTexture( gRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 300, 80 );
			SDL_Rect optRect, optRect2;

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if (e.type == SDL_KEYDOWN)
					{
						switch( e.key.keysym.sym )
                        {
                            case SDLK_UP:
                            flag--;
                            flag %= 3;
                            break;

                            case SDLK_DOWN:
                            flag++;
                            flag %= 3;
                            break;

                            case SDLK_RETURN:
                            if( !flag )
                            	quit = true;
                            else if(flag == 1 || flag == -2) GOV= 1;
                            break;

                        }
					}

					SDL_Rect StartRect ={SW/2 - SW/8,SH /8,SW/4,SH/6};
					SDL_Rect QuitRect = {SW/2 - SW/8,SH /2 + SH /8,SW/4,SH/6};
			    	SDL_Rect HSRect   = {SW/2 - SW/8,SH /4+ SH/8 ,SW/4,SH/6};


					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x,y;
						SDL_GetMouseState(&x, &y);
						//click on quit
						if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/2 + SH /8 && y< SH /2 + SH /8 + SH/6)
							quit = true;
						//click on start/restart
						else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/8 && y< SH/8 +SH/6)
							{
								if (GOV== 0) GOV =1;
								else GOV = 0;
							}
						//click on hs
						else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y>SH/4 + SH/8 && y<SH/4 +SH/8+ SH/6 ) quit = true;
 

                       // cout << x <<" "<<y<<endl;
					}
					// if (e.type == SDL_MOUSEMOTION)
					// {
					// 	int x,y;
					// 	SDL_GetMouseState(&x, &y);
					// 	//click on quit
					// 	if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/2 + SH /8 && y< SH /2 + SH /8 + SH/6)
					// 	{
					// 		optRect = QuitRect;
					// 		SDL_RenderSetViewport( gRenderer, &HSRect );
					// 		SDL_SetTextureBlendMode( gHS, SDL_BLENDMODE_BLEND );
					// 	    SDL_SetTextureAlphaMod( gHS, 150 );
					// 		SDL_RenderCopy( gRenderer, gHS, NULL, NULL );
					// 	}

					// 	//click on start/restart
					// 	else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/8 && y< SH/8 +SH/6)
					// 	{

					// 		optRect= StartRect;

					// 		SDL_RenderSetViewport( gRenderer, &HSRect );
					// 		SDL_SetTextureBlendMode( gHS, SDL_BLENDMODE_BLEND );
					// 	    SDL_SetTextureAlphaMod( gHS, 150 );
					// 		SDL_RenderCopy( gRenderer, gHS, NULL, NULL );
					// 	}
					// 	//click on hs
					// 	else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y>SH/4 + SH/8 && y<SH/4 +SH/8+ SH/6 ) 
					// 	{
					// 		optRect= HSRect;
		 		// 			SDL_RenderSetViewport( gRenderer, &HSRect );
					// 		SDL_SetTextureBlendMode( gHS, SDL_BLENDMODE_BLEND );
					// 	    SDL_SetTextureAlphaMod( gHS, 150 );
					// 		SDL_RenderCopy( gRenderer, gHS, NULL, NULL );
					// 	}
     //                   // cout << x <<" "<<y<<endl;
					// }
				}

				//Clear screen
		    	SDL_RenderClear( gRenderer );
				SDL_Rect StartRect ={SW/2 - SW/8,SH /8,SW/4,SH/6};
				SDL_Rect QuitRect = {SW/2 - SW/8,SH /2 + SH /8,SW/4,SH/6};
			    SDL_Rect HSRect   = {SW/2 - SW/8,SH /4+ SH/8 ,SW/4,SH/6};


				if(!GOV) // GOV=  game over flag

				{			
					SDL_Rect go={0,0 ,SCREEN_WIDTH, SCREEN_HEIGHT };
					SDL_RenderSetViewport( gRenderer, &go );
					SDL_RenderCopy( gRenderer,GST , NULL, NULL );


					SDL_RenderSetViewport( gRenderer, &StartRect );
					SDL_SetTextureBlendMode( gStart, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gStart, 150 );
					SDL_RenderCopy( gRenderer, gStart, NULL, NULL );



					SDL_RenderSetViewport( gRenderer, &QuitRect);
					SDL_SetTextureBlendMode( gQuit, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gQuit, 150 );
					SDL_RenderCopy( gRenderer, gQuit, NULL, NULL );

				  

					SDL_RenderSetViewport( gRenderer, &HSRect );
					SDL_SetTextureBlendMode( gHS, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gHS, 150 );
					SDL_RenderCopy( gRenderer, gHS, NULL, NULL );
				}				
				else 	
				{	
					SDL_Rect go={0,0 ,SCREEN_WIDTH, SCREEN_HEIGHT };
					SDL_RenderSetViewport( gRenderer, &go );
					SDL_RenderCopy( gRenderer,GOT , NULL, NULL );
					

					SDL_RenderSetViewport( gRenderer, &StartRect );
					SDL_SetTextureBlendMode( gRestart, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gRestart, 150 );
					SDL_RenderCopy( gRenderer, gRestart, NULL, NULL );



					SDL_RenderSetViewport( gRenderer, &QuitRect);
					SDL_SetTextureBlendMode( gQuit, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gQuit, 150 );
					SDL_RenderCopy( gRenderer, gQuit, NULL, NULL );

				  

					SDL_RenderSetViewport( gRenderer, &HSRect );
					SDL_SetTextureBlendMode( gHS, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gHS, 150 );
					SDL_RenderCopy( gRenderer, gHS, NULL, NULL );

				


				}
				//SDL_SetRenderDrawColor( gRenderer, 0xFF, 0x00, 0xFF, 0x10 );
				
				if(flag==1 || flag == -2) optRect = StartRect;
				else if(flag==2 || flag == -1) optRect= HSRect;
				else optRect = QuitRect;
				
				optRect2 = {0,0,300,80};
				SDL_RenderSetViewport( gRenderer , NULL);
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0xff, 0x00, 0xFF );
				SDL_SetRenderTarget ( gRenderer, selTexture );
				SDL_RenderFillRect( gRenderer, &optRect2 );

				SDL_SetTextureBlendMode( selTexture, SDL_BLENDMODE_BLEND );
				SDL_SetTextureAlphaMod( selTexture, 100 );

				SDL_SetRenderTarget( gRenderer, NULL );
				SDL_RenderCopy( gRenderer, selTexture, NULL, &optRect );

				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}