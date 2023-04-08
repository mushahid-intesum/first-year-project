#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_mouse.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

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

class Bullet
{
	public:
		static const int BULLET_HEIGHT = 10;
		static const int BULLET_WIDTH = 10;

		Bullet();
		void render(SDL_Event& e);
		void Move(int x, int y);
		void Destroy(SDL_Rect);

	private:
		SDL_Rect killerbox;
		int bVel;
		int bPosX;
		int bPosY;
};

class Enemy
{
	public:
		static const int ENEMY_HEIGHT = 20;
		static const int ENEMY_WIDTH = 20;

		Enemy(int , int);
		void Move(Tile *tiles[]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
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

bool Alerted = false;
double pPosX, pPosY;

int lastTime = 0;

SDL_Texture* loadTexture( std::string path );
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Texture PlayerTexture;
Texture EnemyTexture;
Texture BulletTexture;
Texture gTileTexture;

SDL_Rect eCollider;
SDL_Rect pCollider;
SDL_Rect pGun;
SDL_Rect Detector;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];

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
/*#if defined (_SDL_TTF_H) || defined(SDL_TTF_H)
bool Texture::LoadFromRenderedText(string TextureText, SDL_Color TextColor)
{
	free();
	SDL_Surface* TextSurface == TTF_RenderText_Solid()
}*/

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

	pGun.w = PLAYER_WIDTH-50;
	pGun.h = PLAYER_HEIGHT+100;

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

Bullet::Bullet()
{
	killerbox.h = BULLET_HEIGHT;
	killerbox.w = BULLET_WIDTH;
	int bVel = 1;
	int bPosX = pPosX;
	int bPosY = pPosX;
}

void Bullet::Move(int x, int y)
{
	// cout << x << " " << y << endl;

	int signX = -1;
	int signY = -1;

	int destX = x, destY = y;

	if(destX >= bPosX)
		signX = 1;

	if(destY >= bPosY)
		signY = 1;

	if(bPosX != destX)
		bPosX += signX*bVel;

	if(bPosY != destY)
		bPosY += signY*bVel;

	killerbox.x = bPosX;
	killerbox.y = bPosY;
}	

void Bullet::render(SDL_Event &e)
{
	if(e.type == SDL_KEYDOWN)// && e.key.repeat == 0)
	{
		switch(e.key.keysym.sym)
		{
			case SDLK_f://_MOUSEBUTTONDOWN:
			{
				cout << "pressed" << endl;
				BulletTexture.render(bPosX, bPosY);
				break;
			}
		}
	}
}

void Bullet::Destroy(SDL_Rect a)
{
	if((bPosX < 0) || (bPosX + BULLET_WIDTH > SCREEN_WIDTH) || CheckCollision(killerbox, a))
		BulletTexture.free();

	if((bPosY < 0) || (bPosY + BULLET_HEIGHT > SCREEN_HEIGHT) || CheckCollision(killerbox, a))
		BulletTexture.free();
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

void Enemy::Move(Tile *tiles[])
{
	//cout << Alerted << endl;
	if(Alerted)
	{
		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;
		eVel = 0.03;
	}

	ePosX += eVel*dir;

	if(!Alerted)
	{
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

	else
	{
		int initX = ePosX;
		int initY = ePosY;

		int signX = -1.0;
		int signY = -1.0;

		if((ePosX < 0) || (ePosX + ENEMY_WIDTH > SCREEN_WIDTH))//|| CheckCollision(eCollider, wall))
			dir *= -1;

		//if(CheckCollision(eCollider, wall))

		int destX = pCollider.x;//+6*dir;
		int destY = pCollider.y;//+6*dir;

		cout << destX << " " << destY << endl;

		if(destX >= initX)
			signX = 1.0;
		// else
		// 	signX *= -1.0;

		if(destY >= initY)
			signY = 1.0;
		// else
		// 	signY *= -1.0;

		if(ePosX > destX)
			ePosX -= eVel;//signX*eVel;

		if(ePosY > destY)// || ePosY < destY-10)
			ePosY -= eVel;//signY*eVel;

		if(ePosX < destX)
			ePosX += eVel;//signX*eVel;
		eCollider.x = ePosX;

		if(ePosY < destY)
			ePosY += eVel;//signX*eVel;

		// if(ePosX < destX+10)// || ePosX > destX - 10)
		// 	cout << SDL_GetTicks() << endl;

		if(CheckCollision(eCollider, Detector))
			cout << SDL_GetTicks() << endl;

		eCollider.y = ePosY;

		cout << ePosX << " " << ePosY << endl;

		// cout << signX << " " << signY << endl;

	}

	// ePosY += eVel*dir;
	// eCollider.y = ePosY;

	// if((ePosY < 0) || (ePosY + ENEMY_HEIGHT > SCREEN_HEIGHT) || CheckCollision(eCollider, wall))
	// {
	// 	dir *= -1;
	// 	eCollider.y = ePosY;
	// }
}

void Enemy::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider, a))
		Alerted = true;

		// if(destX >= initX)
		// 	signX = 1.0;

		// if(destY >= initY)
		// 	signY = 1.0;

		// if(ePosX != destX)
		// 	ePosX += signX*eVel;

		// if(ePosY != destY)
		// 	ePosY += signY*eVel;
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
        if( ( tiles[ i ]->getType() >= TILE_CENTER ) && ( tiles[ i ]->getType() <= TILE_TOPLEFT ) )
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

	if(!BulletTexture.LoadFromFile("dot.bmp"))
	{
		printf( "Failed to load bullet texture!\n" );
		success = false;		
	}

	if( !setTiles( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
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

int main( int argc, char* args[] )
{
	Tile* tileSet[ TOTAL_TILES ];
	Player player;
	Enemy enemy2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-18);

	Bullet bullet;

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

			// SDL_Rect pwall;
			// pwall.x = 300;
			// pwall.y = 40;
			// pwall.w = 40;
			// pwall.h = 400;

			// SDL_Rect ewall1;
			// ewall1.x = SCREEN_WIDTH/2-300;
			// ewall1.y = SCREEN_HEIGHT/2-30;
			// ewall1.w = 1;
			// ewall1.h = 50;

			SDL_Rect ewall2;
			ewall2.x = SCREEN_WIDTH/2+200;
			ewall2.y = SCREEN_HEIGHT/2-30;
			ewall2.w = 1;
			ewall2.h = 50;

			while( !quit )
			{
				int x,y;
				SDL_GetMouseState(&x, &y);
				while( SDL_PollEvent( &e ) != 0 )
				{
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}

					player.HandleEvent(e);
					//MouseEvent(e);
					bullet.render(e);
				}

				player.Move(tileSet);

				bullet.Move(x,y);

				// if(!Alerted)
				// {
				// 	enemy.Move(pCollider);
				// 	enemy.Move(pCollider);
				// }
				// else
				// 	enemy.Move(pCollider);

				// enemy.Detected(pCollider);

				// if(!Alerted)
				// {				
				// 	enemy2.Move(ewall2);
				// }

				enemy2.Move(tileSet);

				enemy2.Detected(pCollider);

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				// SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				// SDL_RenderDrawRect(gRenderer, &pwall);

				// if(!Alerted)
				// {
				// 	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				// 	SDL_RenderDrawRect(gRenderer, &ewall1);

				// 	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
				// 	SDL_RenderDrawRect(gRenderer, &ewall2);
				// }	

				for( int i = 0; i < TOTAL_TILES; ++i )
					tileSet[ i ]->render();

				player.render();

				// enemy.render();
				enemy2.render();

				SDL_RenderPresent( gRenderer );
				// Time();
			}
		}
	}
	close(tileSet);

	return 0;
}