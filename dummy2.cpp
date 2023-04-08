#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <bits/stdc++.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const int TILE_WIDTH = 40;
const int TILE_HEIGHT = 40;
const int TOTAL_TILES = 192;
const int TOTAL_TILE_SPRITES = 12;

int GOV=0;
TTF_Font *gFont = NULL;
int SHESH = 0;
int  nise = 0;

struct scorecard
{
	int s;
	char n[100];
};
scorecard card[8];

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
//Screen dimension constants

int SH = SCREEN_HEIGHT;
int SW = SCREEN_WIDTH;

//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture* loadTexture( std::string path );

//Current displayed texture
SDL_Texture* gStart = NULL;
SDL_Texture* gQuit = NULL;
SDL_Texture* gHS = NULL;
SDL_Texture* gRestart = NULL;
SDL_Texture* gInstruct = NULL;
SDL_Texture* gTexture;
SDL_Texture* rect=NULL;
SDL_Texture* GOT = NULL;
SDL_Texture* GST = NULL;
SDL_Texture* GA = NULL;


bool Caught = false;
int Score = 0;

string name;

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

class Tile2
{
    public:
		Tile2( int x, int y, int tileType );

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

		Player(double, double);
		void HandleEvent(SDL_Event& e);
		void Move(Tile *tiles[]);
		void Move2(Tile2 *tiles[]);
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
		void Move2(Tile2 *tiles[], char arr[][1000]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
		SDL_Rect eCollider;
};

class Enemy2
{
	public:
		static const int ENEMY_HEIGHT = 20;
		static const int ENEMY_WIDTH = 20;

		Enemy2(int , int);
		void Move(Tile *tiles[], char arr[][1000]);
		void Move2(Tile2 *tiles[], char arr[][1000]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
		SDL_Rect eCollider2;
};

class Enemy3
{
	public:
		static const int ENEMY_HEIGHT = 20;
		static const int ENEMY_WIDTH = 20;

		Enemy3(int , int);
		void Move(Tile *tiles[], char arr[][1000]);
		void Move2(Tile2 *tiles[], char arr[][1000]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
		SDL_Rect eCollider;
};

class Enemy4
{
	public:
		static const int ENEMY_HEIGHT = 20;
		static const int ENEMY_WIDTH = 20;

		Enemy4(int , int);
		void Move(Tile *tiles[], char arr[][1000]);
		void Move2(Tile2 *tiles[], char arr[][1000]);
		void render();
		void Detected(SDL_Rect& a);

	private:
		double ePosX, ePosY;
		double eVelX, eVelY;
		double eVel;
		double dir;
		double detector;
		SDL_Rect eCollider2;
};

class Items
{
	public:
		Items(int x, int y);
		void render();
		bool Destroy();
		bool Collect();

	public:
		int itemPosX;
		int itemPosY;
		SDL_Rect a;
		bool collected;
};

class Items1
{
	public:
		Items1(int x, int y);
		void render();
		bool Destroy();
		bool Collect();

	public:
		int itemPosX;
		int itemPosY;
		SDL_Rect a;
		bool collected;
};

class Items2
{
	public:
		Items2(int x, int y);
		void render();
		bool Destroy();
		bool Collect();

	public:
		int itemPosX;
		int itemPosY;
		SDL_Rect a;
		bool collected;
};

class Items3
{
	public:
		Items3(int x, int y);
		void render();
		bool Destroy();
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
bool touchesWall2( SDL_Rect box, Tile2* tiles[] );
bool setTiles( Tile *tiles[] );
void mapToArr(char[][1000]);
bool enterName();
bool loadTTF();
void closeTTF();

bool stuff1 = false, stuff2 = false, stuff3 = false, stuff4 = false;

bool Alerted = false;
double pPosX, pPosY;
char maparr[1000][1000];

int lastTime = 0;

SDL_Texture* loadTexture( std::string path );
SDL_Window* gWindow = NULL;
SDL_Renderer* gRenderer = NULL;

Texture PlayerTexture;
Texture gTileTexture;

Texture ItemTexture1;
Texture ItemTexture2;
Texture ItemTexture3;
Texture ItemTexture4;
Texture EnemyTexture1;
Texture EnemyTexture2;
Texture EnemyTexture3;
Texture EnemyTexture4;

Texture gPromptTextTexture;
Texture gInputTextTexture;
Texture gScore;

SDL_Rect pCollider;
SDL_Rect Detector;
SDL_Rect Change;
SDL_Rect gTileClips[ TOTAL_TILE_SPRITES ];
SDL_Rect gTileClips2[ TOTAL_TILE_SPRITES];

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

Tile2::Tile2( int x, int y, int tileType )
{
    mBox.x = x;
    mBox.y = y;

    mBox.w = TILE_WIDTH;
    mBox.h = TILE_HEIGHT;

    mType = tileType;
}

void Tile2::render()
{
    gTileTexture.render(mBox.x, mBox.y, &gTileClips2[ mType ]);
}

int Tile2::getType()
{
    return mType;
}

SDL_Rect Tile2::getBox()
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

#if defined(_SDL_TTF_H) || defined(SDL_TTF_H)
bool Texture::LoadFromRenderedText( std::string textureText, SDL_Color textColor )
{
	free();

	SDL_Surface* textSurface = TTF_RenderText_Solid( gFont, textureText.c_str(), textColor );
	if( textSurface != NULL )
	{
        lTexture = SDL_CreateTextureFromSurface( gRenderer, textSurface );
		if( lTexture == NULL )
		{
			printf( "Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError() );
		}
		else
		{
			lWidth = textSurface->w;
			lHeight = textSurface->h;
		}

		SDL_FreeSurface( textSurface );
	}
	else
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}
	
	return lTexture != NULL;
}
#endif

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

Player::Player(double x, double y)
{
	pPosX = x;
	pPosY = y;

	pCollider.w = PLAYER_WIDTH;
	pCollider.h = PLAYER_HEIGHT;

	Detector.w = PLAYER_WIDTH + 10;
	Detector.h = PLAYER_HEIGHT + 10;

	pVelX = 0;
	pVelY = 0;
	Vel = 0.2;
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
			case SDLK_UP: 	pVelY = 0; break;
			case SDLK_DOWN: pVelY = 0; break;
			case SDLK_RIGHT:pVelX = 0; break;
			case SDLK_LEFT: pVelX = 0; break;
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
	// cout << pVelX << pVelY << endl;
}

void Player::Move2(Tile2 *tiles[])
{
	pPosX += pVelX;
	pCollider.x = pPosX;
	Detector.x = pPosX-5;

	if((pPosX < 0) || (pPosX + PLAYER_WIDTH > SCREEN_WIDTH) || touchesWall2( pCollider, tiles ))
	{
		pPosX -= pVelX;
		pCollider.x = pPosX;
		Detector.x = pPosX-5;
	}

	pPosY += pVelY;
	pCollider.y = pPosY;
	Detector.y = pPosY-5;

	if((pPosY < 0) || (pPosY + PLAYER_HEIGHT > SCREEN_HEIGHT) || touchesWall2( pCollider, tiles ))
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
	eVel = 0.2;

	detector = 50;

	dir = -1;
}

void Enemy::Move(Tile *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosX += eVel*dir;

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
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider.x = ePosX;
		eCollider.y = ePosY;
		if(CheckCollision(Detector, eCollider))
		{
			Caught = true;
		}
	}
}

void Enemy::Move2(Tile2 *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosX += eVel*dir;

		if(dir < 1)
			eCollider.x = ePosX + detector*dir;

		else
			eCollider.x = ePosX;

		if((ePosX < 0) || (ePosX + ENEMY_WIDTH > SCREEN_WIDTH) || touchesWall2( eCollider, tiles ))
			dir *= -1;

		eCollider.y = ePosY;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall2(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall2(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall2(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall2(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider.x = ePosX;
		eCollider.y = ePosY;
		if(CheckCollision(Detector, eCollider))
		{
			Caught = true;
			// cout << "Detected" << endl;
		}
	}
}


void Enemy::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider, a))
		Alerted = true;
}

void Enemy::render()
{
	EnemyTexture1.render(ePosX, ePosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &eCollider);
}

Enemy3::Enemy3(int x, int y)
{
	ePosX = x;//SCREEN_WIDTH/2;
	ePosY = y;//SCREEN_HEIGHT/2;

	eCollider.w = ENEMY_WIDTH+50;
	eCollider.h = ENEMY_HEIGHT;

	eCollider.y = ePosY;
	eCollider.x = ePosX;

	eVelX = 0;
	eVelY = 0;
	eVel = 0.2;

	detector = 50;

	dir = -1;
}

void Enemy3::Move2(Tile2 *tiles[], char arr[][1000])
{
	if(!Alerted)
	{
		ePosX += eVel*dir;

		if(dir < 1)
			eCollider.x = ePosX + detector*dir;

		else
			eCollider.x = ePosX;

		if((ePosX < 0) || (ePosX + ENEMY_WIDTH > SCREEN_WIDTH) || touchesWall2( eCollider, tiles ))
			dir *= -1;

		eCollider.y = ePosY;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall2(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall2(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall2(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall2(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider.x = ePosX;
		eCollider.y = ePosY;
		if(CheckCollision(Detector, eCollider))
		{
			Caught = true;
		}
	}
}

void Enemy3::Move(Tile *tiles[], char arr[][1000])
{
	if(!Alerted)
	{
		ePosX += eVel*dir;

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
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider.w = ENEMY_WIDTH;
		eCollider.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall(eCollider, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall(eCollider, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider.x = ePosX;
		eCollider.y = ePosY;
		if(CheckCollision(Detector, eCollider))
		{
			Caught = true;
		}
	}
}

void Enemy3::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider, a))
		Alerted = true;
}

void Enemy3::render()
{
	EnemyTexture3.render(ePosX, ePosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &eCollider);
}

Enemy2::Enemy2(int x, int y)
{
	ePosX = x;
	ePosY = y;

	eCollider2.w = ENEMY_WIDTH;
	eCollider2.h = ENEMY_HEIGHT+50;

	eCollider2.y = ePosY;
	eCollider2.x = ePosX;

	eVelX = 0;
	eVelY = 0;
	eVel = 0.2;

	detector = 50;

	dir = -1;
}

void Enemy2::Move(Tile *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosY += eVel*dir;

		if(dir < 1)
			eCollider2.y = ePosY + detector*dir;

		else
			eCollider2.y = ePosY;

		if((ePosY < 0) || (ePosY + ENEMY_HEIGHT > SCREEN_HEIGHT) || touchesWall( eCollider2, tiles ))
			dir *= -1;

		eCollider2.x = ePosX;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider2.w = ENEMY_WIDTH;
		eCollider2.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider2.x = ePosX;
		eCollider2.y = ePosY;
		if(CheckCollision(Detector, eCollider2))
		{
			Caught = true;
		}
	}
}

void Enemy2::Move2(Tile2 *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosY += eVel*dir;

		if(dir < 1)
			eCollider2.y = ePosY + detector*dir;

		else
			eCollider2.y = ePosY;

		if((ePosY < 0) || (ePosY + ENEMY_HEIGHT > SCREEN_HEIGHT) || touchesWall2( eCollider2, tiles ))
			dir *= -1;

		eCollider2.x = ePosX;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider2.w = ENEMY_WIDTH;
		eCollider2.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall2(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall2(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall2(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall2(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider2.x = ePosX;
		eCollider2.y = ePosY;
		if(CheckCollision(Detector, eCollider2))
		{
			Caught = true;
		}
	}
}

void Enemy2::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider2, a))
		Alerted = true;
}

void Enemy2::render()
{
	EnemyTexture2.render(ePosX, ePosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &eCollider2);
}

Enemy4::Enemy4(int x, int y)
{
	ePosX = x;//SCREEN_WIDTH/2;
	ePosY = y;//SCREEN_HEIGHT/2;

	eCollider2.w = ENEMY_WIDTH;
	eCollider2.h = ENEMY_HEIGHT+50;

	eCollider2.y = ePosY;
	eCollider2.x = ePosX;

	eVelX = 0;
	eVelY = 0;
	eVel = 0.2;

	detector = 50;

	dir = -1;
}

void Enemy4::Move(Tile *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosY += eVel*dir;

		if(dir < 1)
			eCollider2.y = ePosY + detector*dir;

		else
			eCollider2.y = ePosY;

		if((ePosY < 0) || (ePosY + ENEMY_HEIGHT > SCREEN_HEIGHT) || touchesWall( eCollider2, tiles ))
			dir *= -1;

		eCollider2.x = ePosX;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider2.w = ENEMY_WIDTH;
		eCollider2.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider2.x = ePosX;
		eCollider2.y = ePosY;
		if(CheckCollision(Detector, eCollider2))
		{
			Caught = true;
		}
	}
}

void Enemy4::Move2(Tile2 *tiles[], char arr[][1000])
{
	//cout << Alerted << endl;
	if(!Alerted)
	{
		ePosY += eVel*dir;

		if(dir < 1)
			eCollider2.y = ePosY + detector*dir;

		else
			eCollider2.y = ePosY;

		if((ePosY < 0) || (ePosY + ENEMY_HEIGHT > SCREEN_HEIGHT) || touchesWall2( eCollider2, tiles ))
			dir *= -1;

		eCollider2.x = ePosX;
	}

	if(Alerted)
	{
		SDL_Rect temp;
		int destX = pPosX;
		int destY = pPosY;

		int initX = ePosX;
		int initY = ePosY;

		eCollider2.w = ENEMY_WIDTH;
		eCollider2.h = ENEMY_HEIGHT;

		{
			if(destX > (int)ePosX)
			{
				eVelX = eVel;
				eVelY = 0;

				if(touchesWall2(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;
					}
				}
			}

			else if(destX < (int)ePosX)
			{
				eVelX = -eVel;
				eVelY = 0;

				if(touchesWall2(eCollider2, tiles))
				{
					eVelX = 0;
					if(ePosY > destY)
					{
						eVelY = -eVel;
					}

					else if(ePosY < destY)
					{
						eVelY = eVel;

					}
				}				
			}

			if(abs((int)ePosY-destY) > 50)
			{
				eVelX = 0;

				if(destY > (int)ePosY)
				{
					eVelY = eVel;
					if(touchesWall2(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}

				else if(destY < (int)ePosY)
				{
					eVelY = -eVel;
					if(touchesWall2(eCollider2, tiles))
					{
						eVelY = 0;
						if((int)ePosX > destX)
							eVelX = -eVel;
						else
							eVelX = eVel;
					}
				}
			}
		}

		ePosX += eVelX;
		ePosY += eVelY;

		eCollider2.x = ePosX;
		eCollider2.y = ePosY;
		if(CheckCollision(Detector, eCollider2))
		{
			Caught = true;
		}
	}
}

void Enemy4::Detected(SDL_Rect& a)
{
	if(CheckCollision(eCollider2, a))
		Alerted = true;
}

void Enemy4::render()
{
	EnemyTexture4.render(ePosX, ePosY);
	SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderDrawRect(gRenderer, &eCollider2);
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
	ItemTexture1.render(itemPosX, itemPosY);
}

bool Items::Destroy()
{
	if(CheckCollision(a, pCollider))
	{	
		collected = true;
		Score++;
		ItemTexture1.free();
		return true;
	}
}

bool Items::Collect()
{
	// cout << collected << endl;
	return collected;
}

Items2::Items2(int x, int y)
{
	itemPosX = x;
	itemPosY = y;
	a.w = 20;
	a.h = 20;
	a.x = itemPosX;
	a.y = itemPosY;

	collected = false;
}

void Items2::render()
{
	ItemTexture3.render(itemPosX, itemPosY);
}

bool Items2::Destroy()
{
	if(CheckCollision(a, pCollider))
	{	
		collected = true;
		Score++;
		ItemTexture3.free();
		return true;
	}
}

bool Items2::Collect()
{
	// cout << collected << endl;
	return collected;
}

Items1::Items1(int x, int y)
{
	itemPosX = x;
	itemPosY = y;
	a.w = 20;
	a.h = 20;
	a.x = itemPosX;
	a.y = itemPosY;

	collected = false;
}

void Items1::render()
{
	ItemTexture2.render(itemPosX, itemPosY);
}

bool Items1::Destroy()
{
	if(CheckCollision(a, pCollider))
	{
		collected = true;
		
		Score++;
		ItemTexture2.free();
		return true;
	}
}

bool Items1::Collect()
{
	// cout << collected << endl;
	return collected;
}

Items3::Items3(int x, int y)
{
	itemPosX = x;
	itemPosY = y;
	a.w = 20;
	a.h = 20;
	a.x = itemPosX;
	a.y = itemPosY;

	collected = false;
}

void Items3::render()
{
	ItemTexture4.render(itemPosX, itemPosY);
}

bool Items3::Destroy()
{
	if(CheckCollision(a, pCollider))
	{
		
		collected = true;
		Score++;
		ItemTexture4.free();
		return true;
	}
}

bool Items3::Collect()
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

bool setTiles2( Tile2* tiles[] )
{
	bool tilesLoaded = true;

    int x = 0, y = 0;

    std::ifstream map( "mappy.map" );

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
				tiles[ i ] = new Tile2( x, y, tileType );
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
			gTileClips2[ TILE_RED ].x = 0;
			gTileClips2[ TILE_RED ].y = 0;
			gTileClips2[ TILE_RED ].w = TILE_WIDTH;
			gTileClips2[ TILE_RED ].h = TILE_HEIGHT;

			gTileClips2[ TILE_GREEN ].x = 0;
			gTileClips2[ TILE_GREEN ].y = 80;
			gTileClips2[ TILE_GREEN ].w = TILE_WIDTH;
			gTileClips2[ TILE_GREEN ].h = TILE_HEIGHT;

			gTileClips2[ TILE_BLUE ].x = 0;
			gTileClips2[ TILE_BLUE ].y = 160;
			gTileClips2[ TILE_BLUE ].w = TILE_WIDTH;
			gTileClips2[ TILE_BLUE ].h = TILE_HEIGHT;

			gTileClips2[ TILE_TOPLEFT ].x = 80;
			gTileClips2[ TILE_TOPLEFT ].y = 0;
			gTileClips2[ TILE_TOPLEFT ].w = TILE_WIDTH;
			gTileClips2[ TILE_TOPLEFT ].h = TILE_HEIGHT;

			gTileClips2[ TILE_LEFT ].x = 80;
			gTileClips2[ TILE_LEFT ].y = 80;
			gTileClips2[ TILE_LEFT ].w = TILE_WIDTH;
			gTileClips2[ TILE_LEFT ].h = TILE_HEIGHT;

			gTileClips2[ TILE_BOTTOMLEFT ].x = 80;
			gTileClips2[ TILE_BOTTOMLEFT ].y = 160;
			gTileClips2[ TILE_BOTTOMLEFT ].w = TILE_WIDTH;
			gTileClips2[ TILE_BOTTOMLEFT ].h = TILE_HEIGHT;

			gTileClips2[ TILE_TOP ].x = 160;
			gTileClips2[ TILE_TOP ].y = 0;
			gTileClips2[ TILE_TOP ].w = TILE_WIDTH;
			gTileClips2[ TILE_TOP ].h = TILE_HEIGHT;

			gTileClips2[ TILE_CENTER ].x = 160;
			gTileClips2[ TILE_CENTER ].y = 80;
			gTileClips2[ TILE_CENTER ].w = TILE_WIDTH;
			gTileClips2[ TILE_CENTER ].h = TILE_HEIGHT;

			gTileClips2[ TILE_BOTTOM ].x = 160;
			gTileClips2[ TILE_BOTTOM ].y = 160;
			gTileClips2[ TILE_BOTTOM ].w = TILE_WIDTH;
			gTileClips2[ TILE_BOTTOM ].h = TILE_HEIGHT;

			gTileClips2[ TILE_TOPRIGHT ].x = 240;
			gTileClips2[ TILE_TOPRIGHT ].y = 0;
			gTileClips2[ TILE_TOPRIGHT ].w = TILE_WIDTH;
			gTileClips2[ TILE_TOPRIGHT ].h = TILE_HEIGHT;

			gTileClips2[ TILE_RIGHT ].x = 240;
			gTileClips2[ TILE_RIGHT ].y = 80;
			gTileClips2[ TILE_RIGHT ].w = TILE_WIDTH;
			gTileClips2[ TILE_RIGHT ].h = TILE_HEIGHT;

			gTileClips2[ TILE_BOTTOMRIGHT ].x = 240;
			gTileClips2[ TILE_BOTTOMRIGHT ].y = 160;
			gTileClips2[ TILE_BOTTOMRIGHT ].w = TILE_WIDTH;
			gTileClips2[ TILE_BOTTOMRIGHT ].h = TILE_HEIGHT;
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

bool touchesWall2( SDL_Rect box, Tile2* tiles[] )
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
		
		gWindow = SDL_CreateWindow( "Mr. Pac", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
	//	SDL_Delay(2000);
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
			if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
					success = false;
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

	if(!EnemyTexture1.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}
	if(!EnemyTexture2.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}

	if(!EnemyTexture3.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}

	if(!EnemyTexture4.LoadFromFile("dot2.bmp"))
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

	if(!ItemTexture1.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture2.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture3.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture4.LoadFromFile("coin.png"))
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

	// gPromptTextTexture.free();
	// gInputTextTexture.free();
	// //Free global font
	// TTF_CloseFont( gFont );
	// gFont = NULL;

	// SDL_DestroyRenderer( gRenderer );
	// SDL_DestroyWindow( gWindow );
	// gWindow = NULL;
	// // gRenderer = NULL;

	// IMG_Quit();
	// SDL_Quit();
}

void close2(Tile2* tiles[])
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

	gPromptTextTexture.free();
	gInputTextTexture.free();

	//Free global font
	TTF_CloseFont( gFont );
	gFont = NULL;

	// SDL_DestroyRenderer( gRenderer );
	// SDL_DestroyWindow( gWindow );
	// gWindow = NULL;
	// // gRenderer = NULL;

	// IMG_Quit();
	// SDL_Quit();
}

bool loadTile2(Tile2* tiles[])
{
	bool success = true;

	if(!PlayerTexture.LoadFromFile("dot.bmp"))
	{
		printf( "Failed to load dot texture!\n" );
		success = false;		
	}

	if(!EnemyTexture1.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}
	if(!EnemyTexture2.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}

	if(!EnemyTexture3.LoadFromFile("dot2.bmp"))
	{
		printf( "Failed to load enemy texture!\n" );
		success = false;		
	}

	if(!EnemyTexture4.LoadFromFile("dot2.bmp"))
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

	if( !setTiles2( tiles ) )
	{
		printf( "Failed to load tile set!\n" );
		success = false;
	}

	if(!ItemTexture1.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture2.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture3.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}

	if(!ItemTexture4.LoadFromFile("coin.png"))
	{
		printf( "Failed to load item!\n" );
		success = false;		
	}
	return success;
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

int game()
{	
	char states[10000];

	double a,b;
	int c,d,g,f;

	FILE* game1;
	int q1 = 0, q2 = 0, q3 = 0, q4 = 0;

	int state = 0;
	// game1 = fopen("game1.txt", "w");
	// fprintf(game1, "%lf %lf\n",pPosX,pPosY );

	game1 = fopen("game1.txt", "r");

	fscanf(game1, "%d %lf %lf %d %d %d %d",&state, &a, &b, &q1, &q2, &q3, &q4);

	fclose(game1);

	nise = 0;

	nise += q1+q2+q3+q4;

	cout << nise << endl;

	if(state == 1)
		return 10;

	// fscanf(states, "%d %d %d %d %d %d", &pPosX, &pPosY, &)

	mapToArr(maparr);
	Caught = false;
	Alerted = false;
	Score = 0;

	Tile* tileSet[ TOTAL_TILES ];
	Player player(a,b);
	Enemy enemy1(200, 218);
	Enemy3 enemy2(95, 337);
	Enemy2 enemy3(536, 266);
	Enemy4 enemy4(380, 350);

	Items item1(60, 215);
	Items1 item2(135, 300);
	Items2 item3(379, 390);
	Items3 item4(626, 170);

	if(SHESH == 5)
	{
		close(tileSet);
		return 0;
	}

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

				//cout << x << " " << y << endl;
			while( SDL_PollEvent( &e ) != 0 )
			{
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}

				player.HandleEvent(e);

				if(e.type == SDL_KEYDOWN)
				{
					switch(e.key.keysym.sym)
					{
						case SDLK_s:
						{
							FILE* game1;
							// game1 = fopen("game1.txt", "w");
							// fprintf(game1, "%lf %lf\n",pPosX,pPosY );

							// game1 = fopen("game1.txt", "r");

							// fscanf(game1, "%lf %lf %d %d %d %d ", &a, &b, &c, &d, &e, &f);

							game1 = fopen("game1.txt", "w");
							// double a,b;
							fprintf(game1, "%d %lf %lf %d %d %d %d\n",state,pPosX,pPosY,q1,q2,q3,q4 );

							fclose(game1);
							return 2;
						}
						break;
					}
				}
			}
			// cout << nise;

			player.Move(tileSet);

			// cout << pPosX << pPosY << endl;

			enemy1.Move(tileSet, maparr);

			enemy1.Detected(pCollider);

			enemy2.Move(tileSet, maparr);

			enemy2.Detected(pCollider);

			enemy3.Move(tileSet, maparr);

			enemy3.Detected(pCollider);

			enemy4.Move(tileSet, maparr);

			enemy4.Detected(pCollider);

			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			for( int i = 0; i < TOTAL_TILES; ++i )
				tileSet[ i ]->render();

			player.render();

			enemy1.render();
			enemy2.render();
			enemy3.render();
			enemy4.render();

			if(!item1.Collect())
			{
				if(q1 == 0)
					item1.render();
				if(item1.Destroy())
					{
						nise++;
						q1++;
					} 
			}

			if(!item2.Collect())
			{
				if(q2 == 0)
					item2.render();
				if(item2.Destroy()) 
					{
						nise++;
						q2++;
					}
			}

			if(!item3.Collect())
			{
				if(q3 == 0)
					item3.render();
				if(item3.Destroy())
				{ 
					nise++;
					q3++;
				}
			}

			if(!item4.Collect())
			{
				if(q4 == 0)
					item4.render();
				if(item4.Destroy())
					{
						nise++;
						q4++;
					}
			}

			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
			SDL_RenderDrawRect( gRenderer, &Change );

			SDL_RenderPresent( gRenderer );

			if(Caught)// || nise == 8)
			{
				nise = 0;
			//	printf("Caught13 %d\n",Caught );
				// close(tileSet);
				//cout << "11111111" << endl;
				GOV = 1;

				FILE* game1 = fopen("game1.txt", "w");
				a = 0.0,b = 0.0;
				c = 0,d = 0,g = 0,f = 0;

				fprintf(game1,"%d %lf %lf %d %d %d %d", state, a, b ,c ,d ,g, f);

				fclose(game1);
					
				return 1;
			}		

			if(nise == 4)
			{
				state = 1;

				FILE* game1 = fopen("game1.txt", "w");
				a = 0.0,b = 0.0;
				c = 0,d = 0,g = 0,f = 0;

				fprintf(game1,"%d %lf %lf %d %d %d %d", state, a, b ,c ,d ,g, f);

				fclose(game1);
				close(tileSet);
				return 10;
			}
		}
	}
	// cout << Score << endl;
	close(tileSet);

	return 0;
}

int game2()
{	
	mapToArr(maparr);
	Caught = false;
	Alerted = false;

	double a,b;
	int c,d,g,f;

	int q1 = 0, q2 = 0, q3 = 0, q4 = 0;
	int state = 0;

	nise = 4;

	FILE* game1;
	// game1 = fopen("game1.txt", "w");
	// fprintf(game1, "%lf %lf\n",pPosX,pPosY );

	game1 = fopen("game2.txt", "r");

	fscanf(game1, "%lf %lf %d %d %d %d", &a, &b, &q1, &q2, &q3, &q4);
	fclose(game1);

	nise += q1+q2+q3+q4;
	cout << nise << endl;

	Tile2* tileSet[ TOTAL_TILES ];
	Player player(a,b);

	Enemy enemy1(395, 100);
	Enemy3 enemy2(325, 415);
	Enemy2 enemy3(100, 150);
	Enemy4 enemy4(300, 178);

	Items item1(54, 460);
	Items1 item2(340, 96);
	Items2 item3(222, 222);
	Items3 item4(60, 258);
	if(SHESH == 5)
	{
		close2(tileSet);
		return 0;
	}

	if(!loadTile2(tileSet))
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

				if(e.type == SDL_KEYDOWN)
				{
					switch(e.key.keysym.sym)
					{
						case SDLK_s:
						{
							FILE* game1;
							// game1 = fopen("game1.txt", "w");
							// fprintf(game1, "%lf %lf\n",pPosX,pPosY );

							// game1 = fopen("game2.txt", "r");

							// fscanf(game1, "%lf %lf %d %d %d %d", &a, &b, &c, &d, &g, &f);

							game1 = fopen("game2.txt", "w");
							// double a,b;
							fprintf(game1, "%lf %lf %d %d %d %d\n",pPosX,pPosY,q1,q2,q3,q4);
							fclose(game1);
							return 2;
						}
						break;
					}
				}

				player.HandleEvent(e);
			}

			player.Move2(tileSet);

			enemy1.Move2(tileSet, maparr);

			enemy1.Detected(pCollider);

			enemy2.Move2(tileSet, maparr);

			enemy2.Detected(pCollider);

			enemy3.Move2(tileSet, maparr);

			enemy3.Detected(pCollider);

			enemy4.Move2(tileSet, maparr);

			enemy4.Detected(pCollider);

			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			for( int i = 0; i < TOTAL_TILES; ++i )
				tileSet[ i ]->render();

			player.render();

			enemy1.render();
			enemy2.render();
			enemy3.render();
			enemy4.render();

			if(!item1.Collect())
			{
				if(q1 == 0)
					item1.render();
				if(item1.Destroy())
					{
						nise++;
						q1++;
					} 
			}

			if(!item2.Collect())
			{
				if(q2 == 0)
					item2.render();
				if(item2.Destroy()) 
					{
						nise++;
						q2++;
					}
			}

			if(!item3.Collect())
			{
				if(q3 == 0)
					item3.render();
				if(item3.Destroy())
				{ 
					nise++;
					q3++;
				}
			}

			if(!item4.Collect())
			{
				if(q4 == 0)
					item4.render();
				if(item4.Destroy())
					{
						nise++;
						q4++;
					}
			}

			SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0xFF, 0xFF );		
			SDL_RenderDrawRect( gRenderer, &Change );

			SDL_RenderPresent( gRenderer );

			if(Caught || nise == 8)
			{
				nise = 0;
				printf("Caught13 %d\n",Caught );

				FILE* game1 = fopen("game2.txt", "w");
				c = 0,d = 0,g = 0,f = 0;
				a = 0, b = 0;

				if(!Caught)
					fprintf(game1,"%lf %lf %d %d %d %d",pPosX, pPosY ,c ,d ,g, f);

				else if(Caught || nise == 8)
					fprintf(game1,"%lf %lf %d %d %d %d",a, b ,c ,d ,g, f);

				fclose(game1);


				game1 = fopen("game1.txt", "w");
				c = 0,d = 0,g = 0,f = 0;
				int state = 0;

				fprintf(game1,"%d %lf %lf %d %d %d %d", state, a, b ,c ,d ,g, f);

				fclose(game1);

				// close2(tileSet);
				cout << "11111111" << endl;
				GOV = 1;
					
				return 1;
			}		

			// if(nise == 4)
			// 	return 10;		
		}
	}
	// cout << Score << endl;
	// close2(tileSet);

	return 0;
}

int scorefunc()
{
	FILE *p= fopen("hs.txt","r");
	int hiscore,score = Score;
	int cnt;
	for(cnt=0;cnt<5;cnt++)
		fscanf(p,"%d %s",&card[cnt].s,card[cnt].n);
	strcpy(card[5].n, name.c_str());
	card[5].s = score;
	int i,j;
	for(i=0;i<=5;i++)
	{
		for(j=0;j<=4;j++)
		{
			if (card[j].s<card[j+1].s)
			{
				scorecard tmp = card[j];
				card[j]=card[j+1];
				card[j+1]= tmp;
			}
		}
	}
	fclose(p);

	p=fopen("hs.txt","w");
	for(cnt = 0; cnt<5;cnt++)
	{
		fprintf(p, "%d %s\n",card[cnt].s,card[cnt].n);
	}
	fclose(p);
	//printf("%d---------\n",hiscore );
	SDL_SetRenderDrawColor(gRenderer,120,120,120,255);
	char s[10000];
	sprintf(s,"SCORE : %d",Score);

	SDL_Color textColor = {255,255,255};
	//gScore.render(0,0);

	SDL_RenderClear(gRenderer);
	SDL_RenderCopy( gRenderer,GA , NULL, NULL );

	gScore.setAlpha(100);
	gScore.LoadFromRenderedText(s,textColor);
	gScore.render(SW/6, SH/11);
	SDL_RenderPresent(gRenderer);
	bool quit = false;
	SDL_Event e;
	while(!quit)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if(e.type== SDL_QUIT)
				{
					return 0;
				}
					
			if (e.type == SDL_KEYDOWN)
			{
				switch( e.key.keysym.sym )
                {
                    case SDLK_RETURN:
                   		SDL_RenderClear(gRenderer);

                    	return 5;
                    break;
                 }
			}
		}
	}
}

int show_hs()
{
	FILE *p= fopen("hs.txt","r");
	int hiscore,score = Score;
	char _name[1000];
	int cnt;
	for(cnt=0;cnt<5;cnt++)
		fscanf(p,"%d %s",&card[cnt].s,card[cnt].n);
	SDL_SetRenderDrawColor(gRenderer,0,0,0,255);
	char s[10000];
	SDL_RenderClear(gRenderer);
	for(cnt=0;cnt<5;cnt++)
	{
		sprintf(s," %s : %d  ", card[cnt].n,card[cnt].s);

		SDL_Color textColor = {255,255,255};
		//gScore.render(0,0);

	//	SDL_RenderClear(gRenderer);
		//SDL_RenderCopy( gRenderer,GA , NULL, NULL );

		gScore.setAlpha(100);
		gScore.LoadFromRenderedText(s,textColor);
		gScore.render(300-60, 100+cnt*35);
	}
	SDL_RenderPresent(gRenderer);
	bool quit = false;
	SDL_Event e;
	while(!quit)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if(e.type== SDL_QUIT)
				{
					return 0;
				}
			// if (e.type == SDL_MOUSEBUTTONDOWN) 
			// 	{
			// 		SDL_RenderClear(gRenderer);
			// 		return 5;
			// 	}
			if (e.type == SDL_KEYDOWN)
			{
				switch( e.key.keysym.sym )
                {
                    case SDLK_RETURN:
                   		SDL_RenderClear(gRenderer);

                    	return 5;
                    break;
                }
			}
		}		
	}
}

bool loadMediaStart()
{
	//Loading success flag
	bool success = true;
	gFont = TTF_OpenFont( "Rubik-Regular.ttf", 30);
	//Load texture
	gStart = loadTexture( "start.png" );
	gRestart = loadTexture("restart.png");
	gInstruct = loadTexture("instruction.png");
	gQuit = loadTexture ("quit.png");
	gHS = loadTexture("Hiscore.png");
	GOT = loadTexture("gameover.png");
	GST= loadTexture("play.png");
	GA = loadTexture("gameover.png");
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
                            flag %= 4;
                            break;

                            case SDLK_DOWN:
                            flag++;
                            flag %= 4;
                            break;

                            case SDLK_RETURN:
                            cout<< flag;
                            SDL_DestroyTexture (selTexture);
                            selTexture = NULL;
                            return flag;

                        }
					}

				if(flag == -1) flag = 3;
				if(flag== -2) flag = 2;
				if(flag == -3) flag=1;


					if (e.type == SDL_MOUSEBUTTONDOWN)
					{
						int x,y;
						SDL_GetMouseState(&x, &y);
						//closeMenu();
						//click on quit
						if(x > SW/2 - SW/8 && x < SW/2 - SW/8 + SW/4 && y > SH/2 + SH /8 && y < SH /2 + SH /8 + SH/6)
							return 0;
						//click on start/restart
						else if(x > SW/2 - SW/8 && x < SW/2 - SW/8 + SW/4 && y > SH/8 && y < SH/8 +SH/6)
							{	
								return 1;
							}
						//click on hs
						else if(x > 120 && x < 120+ SW/4 && y > SH/4 + SH/8 && y < SH/4 +SH/8+ SH/6 ) 
						{	
								return 2;
						} 
						else if(x > 360 && x < 360+ SW/4 && y > SH/4 + SH/8 && y < SH/4 +SH/8+ SH/6 ) 
						{	
								return 3;
						}    

					}
				}

				//Clear screen
		    	SDL_RenderClear( gRenderer );
				SDL_Rect StartRect = {SW/2 - SW/8,SH /8,SW/4,SH/6};
				SDL_Rect QuitRect  = {SW/2 - SW/8,SH /2 + SH /8,SW/4,SH/6};
			    SDL_Rect HSRect    = {120,SH /4+ SH/8 ,SW/4,SH/6};
			    SDL_Rect InsRect    = {360,SH /4+ SH/8 ,SW/4,SH/6};


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

					SDL_RenderSetViewport( gRenderer, &InsRect);
					SDL_SetTextureBlendMode( gInstruct, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gInstruct, 150 );
					SDL_RenderCopy( gRenderer, gInstruct, NULL, NULL );

				  

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

					SDL_RenderSetViewport( gRenderer, &InsRect);
					SDL_SetTextureBlendMode( gInstruct, SDL_BLENDMODE_BLEND );
				    SDL_SetTextureAlphaMod( gInstruct, 150 );
					SDL_RenderCopy( gRenderer, gInstruct, NULL, NULL );


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
				
				if(flag==1 || flag == -3) optRect = StartRect;
				else if(flag==2 || flag == -2) optRect= HSRect;
				else if(!flag) optRect = QuitRect;
				else optRect = InsRect;HSRect;
			
				
				
				optRect2 = {0, 0, 300, 80};
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
}

bool loadTTF()
{
	bool success = true;

	//Open the font
	gFont = TTF_OpenFont( "Rubik-Regular.ttf", 28 );
	if( gFont == NULL )
	{
		//printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}
	else
	{
		SDL_Color textColor = { 0, 0, 0, 0xFF };
		if( !gPromptTextTexture.LoadFromRenderedText( "Enter Name:", textColor ) )
		{
			printf( "Failed to render prompt text!\n" );
			success = false;
		}
	}

	return success;
}

void closeTTF()
{
	gPromptTextTexture.free();
	gInputTextTexture.free();

	TTF_CloseFont( gFont );
	gFont = NULL;

	TTF_Quit();
}

bool enterName()
{
	if( !loadTTF() )
	{
		printf( "Failed to load media!\n" );
	}
	else
	{	
		bool quit = false;

		SDL_Event e;

		SDL_Color textColor = { 0, 0, 0, 0xFF };

		// std::string inputText = "";
		gInputTextTexture.LoadFromRenderedText( name.c_str(), textColor );

		SDL_StartTextInput();

		while( !quit )
		{
			bool renderText = false;

			while( SDL_PollEvent( &e ) != 0 )
			{
				if( e.type == SDL_QUIT )
				{
					quit = true;
				}
				else if( e.type == SDL_KEYDOWN )
				{
					if( e.key.keysym.sym == SDLK_BACKSPACE && name.length() > 0 )
					{
						name.pop_back();
						renderText = true;
					}

					else if(e.key.keysym.sym == SDLK_RETURN)
					{
						return true;
					}
				}
				else if( e.type == SDL_TEXTINPUT )
				{
					if( !( SDL_GetModState() & KMOD_CTRL && ( e.text.text[ 0 ] == 'c' || e.text.text[ 0 ] == 'C' || e.text.text[ 0 ] == 'v' || e.text.text[ 0 ] == 'V' ) ) )
					{
						name += e.text.text;
						renderText = true;
					}
				}
			}

			if( renderText )
			{
				if( name != "" )
				{
					gInputTextTexture.LoadFromRenderedText( name.c_str(), textColor );
				}
				else
				{
					gInputTextTexture.LoadFromRenderedText( " ", textColor );
				}
			}

			SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
			SDL_RenderClear( gRenderer );

			gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
			gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

			SDL_RenderPresent( gRenderer );
		}
			
		SDL_StopTextInput();
	}
	return true;
}
int instructfunc()
{
	SDL_RenderClear(gRenderer);
	SDL_Event e;
	SDL_SetRenderDrawColor(gRenderer,0,0,0,0);
	SDL_Texture* Instructionviewer = NULL;
	SDL_Rect instructview ={0,0, SW , SH};
	SDL_RenderSetViewport( gRenderer, &instructview );
	Instructionviewer = loadTexture("instruct.png");
	SDL_SetTextureBlendMode( Instructionviewer, SDL_BLENDMODE_BLEND );
 	SDL_SetTextureAlphaMod( Instructionviewer, 150 );
	SDL_RenderCopy( gRenderer, Instructionviewer, NULL, NULL );
	SDL_RenderPresent(gRenderer);
	
	bool quit = false;
	while(!quit)
	{
		while(SDL_PollEvent(&e)!=0)
		{
			if(e.type== SDL_QUIT)
				{
					return 0;
				}
					
			if (e.type == SDL_KEYDOWN)
			{
				switch( e.key.keysym.sym )
                {
                    case SDLK_RETURN:
                   		SDL_RenderClear(gRenderer);

                    	return 5;
                    break;
                 }
			}
		}
	}



}

int main(int argc, char const *argv[])
{
	int playing = 1;
	int a, _game_ = 0;
	int savestart;
	if(!init())
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{	
		bool nametaken;
		// savestart = saveLoad();
				name.clear();
				nametaken = enterName();
		while(playing)
		{
			

			
			a = menu();
			cout<< a;

			if(a==3)
			{
				a=instructfunc();
				
				if(a)
					continue;
			}


			if(a == 1)
			{
				
				if(nametaken)// && !saveStart)
					a = game();
			}

			if(a == 10)
				a = game2();

			if(a == 2)
			{
				show_hs();
				continue;
			}
			// if(a==3)
			// {
			// 	instructfunc();
			// 	continue;
			// }

			
			if (a == 0)
		    {
		    	SHESH = 5;
		     	int a = game();
		     	closeMenu();
		     	break ;
		    }

		    a = scorefunc();

		    if (a == 0)
		    {
		     	SHESH = 5;
		     	int a = game();
		     	closeMenu();
		     	break ;
	     	}
			cout << Score << endl;
		}
	}
	closeMenu();
	
	printf("END\n");

	return 0;
}
	