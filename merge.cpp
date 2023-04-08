#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_mouse.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;
int GOV=0;


const int TILE_RED = 0;
const int TILE_GREEN = 1;
const int TILE_BLUE = 2;
const int TILE_CENTER = 3;
const int TILE_TOP = 4;
const int TILE_TOPRIGHT = 5;
const int TILE_RIGHT = 6;
const int TILE_BOTTOMRIGHT = 7;
const int TILE_BOTTOM = 8;
const int TILE_BOTTOMLEFT = 9;
const int TILE_LEFT = 10;
const int TILE_TOPLEFT = 11;

bool Caught = false;
int Score = 0;

class Tile
{
    public:
		Tile( int x, int y, int tileType );

		void render();

		int getType();

		SDL_Rect getBox();

    private:
		SDL_Rect mBox;

		int mType;
};

class Texture
{
	public:
		Texture();
		~Texture();

		bool LoadFromFile(string path);

		#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
		bool LoadFromRenderedText(std::string TextureText, SDL_Color TextColor);
		#endif

		void free();
		void setColor( Uint8 red, Uint8 green, Uint8 blue );
		void setBlendMode( SDL_BlendMode blending );
		void setAlpha( Uint8 alpha );
		void render( int x, int y, SDL_Rect* clip = NULL, double angle = 0.0, SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE );
		
		int getWidth();
		int getHeight();

	private:
		SDL_Texture* lTexture;

		int lWidth;
		int lHeight;
};

class Player
{
	public:
		static const int PLAYER_WIDTH = 20;
		static const int PLAYER_HEIGHT = 20;

		Player();
		void HandleEvent(SDL_Event& e);
		void Move(Tile *tiles[]);
		void render();

	private:
		double pVelX, pVelY;
		double Vel;
};

class Enemy
{
	public:
		static const int ENEMY_HEIGHT = 20;
		static const int ENEMY_WIDTH = 20;

		Enemy(int , int);
		void Move(Tile *tiles[], char arr[][1000]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
};

class Items
{
	public:
		Items(int x, int y);
		void render();
		void Destroy();
		bool Collect();

	public:
		int itemPosX;
		int itemPosY;
		SDL_Rect a;
		bool collected;
};

bool init();
bool loadMedia(Tile* tiles[] );
void close(Tile* tiles[] );
void Move();
bool CheckCollision(SDL_Rect a, SDL_Rect b);
double DistanceSquared(double x1, double y1, double x2, double y2);
void MouseEvent(SDL_Event &e);
bool touchesWall( SDL_Rect box, Tile* tiles[] );
bool setTiles( Tile *tiles[] );
void mapToArr(char[][1000]);

bool Alerted = false;
double pPosX, pPosY;
char maparr[1000][1000];

int lastTime = 0;

SDL_Texture* loadTexture( std::string path );
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Texture PlayerTexture;
Texture EnemyTexture;
Texture gTileTexture;
Texture ItemTexture;

SDL_Rect eCollider;
SDL_Rect pCollider;
SDL_Rect Detector;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

void mapToArr(char maparr[1000][1000])
{
	FILE *i;
	i = fopen("mappy.map","r");

	for(int j = 0;j<12;j++)
		fgets(maparr[j], 1000, i);
}

Tile::Tile( int x, int y, int tileType )
{
    mBox.x = x;
    mBox.y = y;

    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    mType = tileType;
}

void Tile::render()
{
    gTileTexture.render(mBox.x, mBox.y, &gTileClips[ mType ]);
}

int Tile::getType()
{
    return mType;
}

SDL_Rect Tile::getBox()
{
    return mBox;
}

Texture::Texture()
{
	lTexture = NULL;
	lWidth = 0;
	lHeight = 0;
}

Texture::~Texture()
{
	free();
}

bool Texture::LoadFromFile(string path)
{
	free();

	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path.c_str());

	if(loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());

	else
	{
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );

		if(newTexture == NULL)
			printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());

		else
		{
			lWidth = loadedSurface->w;
			lHeight = loadedSurface->h;
		}
		SDL_FreeSurface(loadedSurface);
	}
	lTexture = newTexture;
	return lTexture != NULL;
}

void Texture::free()
{
	if(lTexture != NULL)
	{
		SDL_DestroyTexture(lTexture);
		lTexture = NULL;
		lWidth = 0;
		lHeight = 0;
	}
}

void Texture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
	SDL_SetTextureColorMod(lTexture, red, green, blue);
}

void Texture::setBlendMode(SDL_BlendMode blending)
{
	SDL_SetTextureBlendMode(lTexture, blending);
}

void Texture::setAlpha(Uint8 alpha)
{
	SDL_SetTextureAlphaMod(lTexture, alpha);
}

void Texture::render(int x, int y, SDL_Rect* clip, double angle, SDL_Point* center, SDL_RendererFlip flip)
{
	SDL_Rect renderQuad = {x, y, lWidth, lHeight};

	if(clip != NULL)
	{
		renderQuad.w = clip -> w;
		renderQuad.h = clip -> h;
	}
	SDL_RenderCopyEx(gRenderer, lTexture, clip, &renderQuad, angle, center, flip);
}

int Texture::getWidth()
{
	return lWidth;
}

int Texture::getHeight()
{
	return lHeight;
}

Player::Player()
{
	pPosX = 0;
	pPosY = 0;

	pCollider.w = PLAYER_WIDTH;
	pCollider.h = PLAYER_HEIGHT;

	Detector.w = PLAYER_WIDTH+10;
	Detector.h = PLAYER_HEIGHT+10;

	pVelX = 0;
	pVelY = 0;
	Vel = 0.05;
}

void Player::HandleEvent(SDL_Event& e)
{
	//Implement sprinting and stop the acceleration
	if(e.type == SDL_KEYDOWN && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP: 	pVelY -= Vel; break;
			case SDLK_DOWN: pVelY += Vel; break;
			case SDLK_RIGHT:pVelX += Vel; break;
			case SDLK_LEFT: pVelX -= Vel; break;
		}
	}

	else if(e.type == SDL_KEYUP && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_UP: 	pVelY += Vel; break;
			case SDLK_DOWN: pVelY -= Vel; break;
			case SDLK_RIGHT:pVelX -= Vel; break;
			case SDLK_LEFT: pVelX += Vel; break;
		}
	}
}

void Player::Move(Tile *tiles[])
{
	pPosX += pVelX;
	pCollider.x = pPosX;
	Detector.x = pPosX-5;

	if((pPosX < 0) || (pPosX + PLAYER_WIDTH > SCREEN_WIDTH) || touchesWall( pCollider, tiles ))
	{
		pPosX -= pVelX;
		pCollider.x = pPosX;
		Detector.x = pPosX-5;
	}

	pPosY += pVelY;
	pCollider.y = pPosY;
	Detector.y = pPosY-5;

	if((pPosY < 0) || (pPosY + PLAYER_HEIGHT > SCREEN_HEIGHT) || touchesWall( pCollider, tiles ))
	{
		pPosY -= pVelY;
		pCollider.y = pPosY;
		Detector.y = pPosY-5;
	}
}

void Player::render()
{
	PlayerTexture.render(pPosX, pPosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &Detector);
}

Enemy::Enemy(int x, int y)
{
	ePosX = x;//SCREEN_WIDTH/2;
	ePosY = y;//SCREEN_HEIGHT/2;

	eCollider.w = ENEMY_WIDTH+50;
	eCollider.h = ENEMY_HEIGHT;

	eCollider.y = ePosY;
	eCollider.x = ePosX;

	eVelX = 0;
	eVelY = 0;
	eVel = 0.02;

	detector = 50;

	dir = -1;
}

void Enemy::Move(Tile *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosX += eVel*dir;
		int currentTime = SDL_GetTicks();
		if(currentTime >= lastTime+1000)
		{
			dir *= -1;
			//currentTime = SDL_GetTicks();
			lastTime = currentTime;
			//cout << lastTime << " " << dir << endl;
		}

		if(dir < 1)
			eCollider.x = ePosX + detector*dir;

		else
			eCollider.x = ePosX;

		if((ePosX < 0) || (ePosX + ENEMY_WIDTH > SCREEN_WIDTH) || touchesWall( eCollider, tiles ))
			dir *= -1;

		eCollider.y = ePosY;
	}

	if(Alerted)
	{
		// FILE *map;
		// map = fopen("lazy.map","r");

		// char arr[1000][1000];

		// for(int j = 0;j<12;j++)
		// 	fgets(arr[j], 1000, map);

		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;

		eVel = 0.03;

		int jaegaX1;
		int jaegaY1;

		int jaegaX2;
		int jaegaY2;

		int jaegaX3;
		int jaegaY3;

		int jaegaX4;
		int jaegaY4;

		// cout << ePosX << " " << ePosY << " " << eCollider.x << " " << eCollider.y << endl;

		int initX = ePosX;
		int initY = ePosY;

		int destX = pCollider.x;
		int destY = pCollider.y;
		// cout << destX << " " << destY << endl;

		jaegaX1 = (ePosX+ENEMY_WIDTH+1)/40;
		jaegaY1 = ePosY/40;

		jaegaX2 = (ePosX-1)/40;
		jaegaY2 = ePosY/40;

		jaegaX3 = (ePosX)/40;
		jaegaY3 = (ePosY+ENEMY_HEIGHT+1)/40;

		jaegaX4 = (ePosX)/40;
		jaegaY4 = (ePosY-1)/40;

		// cout << jaegaX << " " << jaegaY << endl;

		// cout << arr[jaegaX+ENEMY_WIDTH+1][jaegaY] << endl;
		// cout << arr[jaegaX-1][jaegaY] << endl;

		if(touchesWall(eCollider, tiles))
		{
			ePosX -= eVel;
			ePosY -= eVel;
			// cout << arr[jaegaX+ENEMY_WIDTH+1][jaegaY] << endl;
			// cout << arr[jaegaX-1][jaegaY] << endl;
			if(arr[jaegaX1][jaegaY1] == '3')
			{
				ePosY += eVel;
			}

			else if(arr[jaegaX2][jaegaY2] == '3')
			{
				ePosY -= eVel;
			}

			else if(arr[jaegaX3][jaegaY3] == '3')
			{
				ePosX += eVel;
			}

			else if(arr[jaegaX4][jaegaY4] == '3')
			{
				ePosX -= eVel;
			}

			// if(arr[(ePosX-1)/40][jaegaY] == '3')
			// {
			// 	cout << "right" << endl;
			// 	ePosY += eVel; 
			// }
		}

		else
		{
			if(ePosX > destX)
				ePosX -= eVel;

			if(ePosY > destY)
				ePosY -= eVel;

			if(ePosX < destX)
				ePosX += eVel;

			if(ePosY < destY)
				ePosY += eVel;

			// if(CheckCollision(eCollider, Detector))
			// 	cout << SDL_GetTicks() << endl;
		}
		eCollider.x = ePosX;// + (double)eCollider.w/2;
		eCollider.y = ePosY;// + (double)eCollider.h/2;

		//if((ePosX+ENEMY_WIDTH >= Detector.x) || (ePosX <= Detector.x+ENEMY_WIDTH) && (ePosY+ENEMY_WIDTH >= Detector.y) || (ePosY <= Detector.y+ENEMY_WIDTH))
		if(CheckCollision(Detector, eCollider))
		{
			Caught = true;
			// cout << "Detected" << endl;
		}

		// cout << ePosX << " " << ePosY << endl;
		// cout << eCollider.x << " " << eCollider.y << endl;

		// cout << signX << " " << signY << endl;
	}
}

void Enemy::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider, a))
		Alerted = true;
}

void Enemy::render()
{
	EnemyTexture.render(ePosX, ePosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &eCollider);
}

double DistanceSquared( double x1, double y1, double x2, double y2 )
{
	double deltaX = x2 - x1;
	double deltaY = y2 - y1;
	return deltaX*deltaX + deltaY*deltaY;
}

Items::Items(int x, int y)
{
	itemPosX = x;
	itemPosY = y;
	a.w = 20;
	a.h = 20;
	a.x = itemPosX;
	a.y = itemPosY;

	collected = false;
}

void Items::render()
{
	ItemTexture.render(itemPosX, itemPosY);
}

void Items::Destroy()
{
	if(CheckCollision(a, pCollider))
	{
		Score++;
		ItemTexture.free();
	}
}

bool Items::Collect()
{
	// cout << collected << endl;
	return collected;
}

bool setTiles( Tile* tiles[] )
{
	bool tilesLoaded = true;

    int x = 0, y = 0;

    std::ifstream map( "lazy.map" );

    if( map.fail() )
    {
		printf( "Unable to load map file!\n" );
		tilesLoaded = false;
    }
	else
	{
		for( int i = 0; i < TOTAL_TILES; ++i )
		{
			int tileType = -1;

			map >> tileType;

			if( map.fail() )
			{
				printf( "Error loading map: Unexpected end of file!\n" );
				tilesLoaded = false;
				break;
			}

			if( ( tileType >= 0 ) && ( tileType < TOTAL_TILE_SPRITES ) )
			{
				tiles[ i ] = new Tile( x, y, tileType );
				// if(tileType == 3)
				// {
				// 	tilePosX[ix] = x;
				// 	tilePosY[iy] = y;
				// 	cout << ix << " " << iy << endl;
				// 	ix++;
				// 	iy++;
				// }	
			}
			else
			{
				printf( "Error loading map: Invalid tile type at %d!\n", i );
				tilesLoaded = false;
				break;
			}

			x += TILE_WIDTH;

			if( x >= SCREEN_WIDTH )
			{
				x = 0;

				y += TILE_HEIGHT;
			}
		}
		
		if( tilesLoaded )
		{
			gTileClips[ TILE_RED ].x = 0;
			gTileClips[ TILE_RED ].y = 0;
			gTileClips[ TILE_RED ].w = TILE_WIDTH;
			gTileClips[ TILE_RED ].h = TILE_HEIGHT;

			gTileClips[ TILE_GREEN ].x = 0;
			gTileClips[ TILE_GREEN ].y = 80;
			gTileClips[ TILE_GREEN ].w = TILE_WIDTH;
			gTileClips[ TILE_GREEN ].h = TILE_HEIGHT;

			gTileClips[ TILE_BLUE ].x = 0;
			gTileClips[ TILE_BLUE ].y = 160;
			gTileClips[ TILE_BLUE ].w = TILE_WIDTH;
			gTileClips[ TILE_BLUE ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPLEFT ].x = 80;
			gTileClips[ TILE_TOPLEFT ].y = 0;
			gTileClips[ TILE_TOPLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_LEFT ].x = 80;
			gTileClips[ TILE_LEFT ].y = 80;
			gTileClips[ TILE_LEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_LEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMLEFT ].x = 80;
			gTileClips[ TILE_BOTTOMLEFT ].y = 160;
			gTileClips[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOP ].x = 160;
			gTileClips[ TILE_TOP ].y = 0;
			gTileClips[ TILE_TOP ].w = TILE_WIDTH;
			gTileClips[ TILE_TOP ].h = TILE_HEIGHT;

			gTileClips[ TILE_CENTER ].x = 160;
			gTileClips[ TILE_CENTER ].y = 80;
			gTileClips[ TILE_CENTER ].w = TILE_WIDTH;
			gTileClips[ TILE_CENTER ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOM ].x = 160;
			gTileClips[ TILE_BOTTOM ].y = 160;
			gTileClips[ TILE_BOTTOM ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOM ].h = TILE_HEIGHT;

			gTileClips[ TILE_TOPRIGHT ].x = 240;
			gTileClips[ TILE_TOPRIGHT ].y = 0;
			gTileClips[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_RIGHT ].x = 240;
			gTileClips[ TILE_RIGHT ].y = 80;
			gTileClips[ TILE_RIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_RIGHT ].h = TILE_HEIGHT;

			gTileClips[ TILE_BOTTOMRIGHT ].x = 240;
			gTileClips[ TILE_BOTTOMRIGHT ].y = 160;
			gTileClips[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			gTileClips[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
		}
	}

    map.close();

    return tilesLoaded;
}

bool touchesWall( SDL_Rect box, Tile* tiles[] )
{
    for( int i = 0; i < TOTAL_TILES; ++i )
    {
        if( ( tiles[ i ]->getType() == TILE_CENTER ) )
        {
            if( CheckCollision( box, tiles[ i ]->getBox() ) )
            {
                return true;
            }
        }
    }

    return false;
}

bool init()
{
	bool success = true;

	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}
		SDL_DestroyWindow(gWindow);
		gWindow = SDL_CreateWindow( "A Stelthi Bdeo Gaem!", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				success = false;
			}
			else
			{
				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

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
		SDL_DestroyWindow(gWindow);

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

bool loadMedia(Tile* tiles[])
{
	bool success = true;

	if(!PlayerTexture.LoadFromFile("dot.bmp"))
	{
		printf( "Failed to load dot texture!\n" );
		success = false;		
	}

	if(!EnemyTexture.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}

	//Load tile texture
	if( !gTileTexture.LoadFromFile( "tiles.png" ) )
	{
		printf( "Failed to load tile set texture!\n" );
		success = false;
	}

	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	if(!ItemTexture.LoadFromFile("dot.bmp"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}
	return success;
}

bool CheckCollision(SDL_Rect a, SDL_Rect b)
{
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x+a.w;
	topA = a.y;
	bottomA = a.y+a.h;

	leftB = b.x;
	rightB = b.x+b.w;
	topB = b.y;
	bottomB = b.y+b.h;

	if(bottomA <= topB)
		return false;

	if(topA >= bottomB)
		return false;

	if(rightA <= leftB)
		return false;

	if(leftA >= rightB)
		return false;

	return true;
}

void close(Tile* tiles[])
{
	for( int i = 0; i < TOTAL_TILES; ++i )
	{
		 if( tiles[ i ] == NULL )
		 {
			delete tiles[ i ];
			tiles[ i ] = NULL;
		 }
	}
	PlayerTexture.free();
	gTileTexture.free();

	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}

SDL_Texture* loadTexture( std::string path )
{
	SDL_Texture* newTexture = NULL;

	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to  image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
        newTexture = SDL_CreateTextureFromSurface( gRenderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		SDL_FreeSurface( loadedSurface );
	}
	return newTexture;
}

int game( )
{	
	mapToArr(maparr);
	// cout << ix << endl;

	Tile* tileSet[ TOTAL_TILES ];
	Player player;
	Enemy enemy2(SCREEN_WIDTH/20-14, SCREEN_HEIGHT/20);
	Items item1(137, 100);

	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		if(!loadMedia(tileSet))
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			bool quit = false;

			SDL_Event e;

			while( !quit )
			{
				int x,y;
				SDL_GetMouseState(&x, &y);

				// cout << x << " " << y << endl;
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					player.HandleEvent(e);
				}

				player.Move(tileSet);

				enemy2.Move(tileSet, maparr);

				enemy2.Detected(pCollider);

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				for( int i = 0; i < TOTAL_TILES; ++i )
					tileSet[ i ]->render();

				player.render();

				// enemy.render();
				enemy2.render();

				if(!item1.Collect())
					item1.render();

				item1.Destroy();

				SDL_RenderPresent( gRenderer );

				if(Caught)
				{
					GOV = 1;
					close(tileSet);
					return 1;
				}
				// Time();
			}
		}
	}
	cout << Score << endl;
	close(tileSet);

}



//Screen dimension constants

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
//SDL_Window* gWindow = NULL;

//The window renderer
// SDL_Renderer* gRenderer = NULL;
// SDL_Surface* screenSurface=NULL;


//Current displayed texture
SDL_Texture* gStart = NULL;
SDL_Texture* gQuit = NULL;
SDL_Texture* gHS = NULL;
SDL_Texture* gRestart = NULL;
SDL_Texture* gTexture;
SDL_Texture* rect=NULL;
SDL_Texture* GOT = NULL;
SDL_Texture* GST = NULL;



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

void closeMenu()
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


int menu()
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
						return 0;
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
                            	return 0;
                            else if(flag == 1 || flag == -2) 
                           	{
								return 1;
							}
							else return 2;
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
						closeMenu();
						//click on quit
						if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/2 + SH /8 && y< SH /2 + SH /8 + SH/6)
							return 0;
						//click on start/restart
						else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y> SH/8 && y< SH/8 +SH/6)
							{	
								return 1;
							}
						//click on hs
						else if(x> SW/2 - SW/8 && x< SW/2 - SW/8 + SW/4 && y>SH/4 + SH/8 && y<SH/4 +SH/8+ SH/6 ) 
						{	
								return 2;
						}
 

                       // cout << x <<" "<<y<<endl;
					}
					
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
	
	
}

int main(int argc, char const *argv[])
{
	int playing = 1;
	while( menu() && game())
	{
		int a;
		a=menu();
		if(a==1) 
			a= game();
		else if(a== 0)
			break;
		if( a== 1)
			a= game();


	}
	//close();

	closeMenu();
	
	return 0;
}
	