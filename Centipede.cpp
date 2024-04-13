#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <sstream>
#include <cmath>

using namespace std;

// Initializing Dimensions.
// resolutionX and resolutionY determine the rendering resolution.
// Don't edit unless required. Use functions on lines 43, 44, 45 for resizing the game window.
const int resolutionX = 960;
const int resolutionY = 960;
const int boxPixelsX = 32;
const int boxPixelsY = 32;
const int gameRows = resolutionX / boxPixelsX; // Total rows on grid
const int gameColumns = resolutionY / boxPixelsY; // Total columns on grid

// Initializing GameGrid.
int gameGrid[gameRows][gameColumns] = {};

// The following exist purely for readability.
const int x = 0;
const int y = 1;
const int exists = 2;

// function prototypes

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite);
void movePlayer(sf::Event e, float player[],float mushroom[][3],int mushroomN);
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite);
void moveBullet(float bullet[], sf::Clock& bulletClock,sf::Music &bulletSound);
void drawCentipede(sf::RenderWindow& window, float centipede[][3], sf::Sprite centipedeSprite[],int &frame,sf::Texture centipedeHeadTexture,sf::Texture centipedeBodyTexture,sf::Clock &flashClock,bool &regen);  
void moveCentipede(float centipede[][3],float mushroom[][3],int mushroomN,sf::Clock &centipedeClock,int &frame,int gameLevel);
void moveExtraHeads(float centipede[][3],float mushroom[][3],int mushroomN,sf::Clock &extraHeadClock);
void drawMushrooms(sf::RenderWindow& window, float mushroom[][3],int mushroomN, sf::Sprite mushroomSprite[]); 
void fireBullet(float bullet[],float mushroom[][3],int &mushroomN,float centipede[][3],float spider[],sf::Sprite &spiderSprite,bool &spiderScore,float player[],float scorpion[],sf::Music &centipedeFireSound,int &score,int gameLevel,int &segment,int &segmentEnd);   
void playerDeath(float player[],float centipede[][3],float mushroom[][3],int mushroomN,float spider[],int &life,bool &extraHeads,int &score,bool &regen,sf::Music &playerDeathSound);
void gameRegeneration(float centipede[][3],float mushroom[][3],int &mushroomN,int &initMushroom,float spider[],sf::Clock &spiderClock,int &move,bool &regen,bool &levelUp,int gameLevel,int &score,sf::RenderWindow &window,float player[]);
void regenerateMushroom(float mushroom[][3],int mushroomN,int &score);
void regenerateCentipede(float centipede[][3],int gameLevel);
void displayScore(sf::RenderWindow &window,int &score,sf::Text &scoreText,int &life,sf::Text &lifeText,bool lifeatxK[]); //manages score and lives
void checkLevel(float centipede[][3],int &gameLevel,bool &levelUp,sf::Music &levelSound);
void playExplosion(sf::Sprite centipedeSprite[],sf::RenderWindow &window,sf::Texture &explodeTexture,sf::Clock &explodeClock,int &segment,int &segmentEnd,int &explodeFrame);
void deathAnimation(sf::RenderWindow &window,sf::Sprite &deathSprite,sf::Clock &deathClock,int &deathFrameX,int &deathFrameY);
void maxScore(sf::RenderWindow &window,sf::Text &winText,sf::Clock &menuPause,sf::Music &levelSound);
void outOfLives(sf::RenderWindow &window,sf::Text &gameOverText,sf::Clock &menuPause);
void drawSpider(sf::RenderWindow &window,sf::Sprite &spiderSprite,float spider[],int spiderFrame,bool &spiderScore,sf::Clock &spiderClock);
void moveSpider(float spider[],float mushroom[][3],int mushroomN,int &move,int &spiderFrame,sf::Clock &spiderClock);
void regenerateSpider(float spider[],sf::Clock &spiderClock);
void playSpiderSong(sf::Music &spiderMusic,sf::Clock &spiderSongClock);
void drawScorpion(sf::RenderWindow &window,sf::Sprite &scorpionSprite,float scorpion[],int scorpionFrame);
void moveScorpion(float scorpion[],float mushroom[][3],int mushroomN,sf::Clock &scorpionClock,int &scorpionFrame);
void regenerateScorpion(float scorpion[],sf::Clock &scorpionClock);
void playScorpionSong(sf::Music &scorpionMusic,sf::Clock &scorpionSongClock);

int main()
{
	srand(time(0));

	// Declaring RenderWindow.
	sf::RenderWindow window(sf::VideoMode(resolutionX, resolutionY), "Centipede", sf::Style::Close | sf::Style::Titlebar);

	// Used to resize your window if it's too big or too small. Use according to your needs.
	window.setSize(sf::Vector2u(640, 640)); // Recommended for 1366x768 (768p) displays.
	 //window.setSize(sf::Vector2u(1280, 1280)); // Recommended for 2560x1440 (1440p) displays.
	// window.setSize(sf::Vector2u(1920, 1920)); // Recommended for 3840x2160 (4k) displays.
	
	// Used to position your window on every launch. Use according to your needs.
	window.setPosition(sf::Vector2i(100, 0));

	// Initializing Background Music.
	sf::Music bgMusic;
	bgMusic.openFromFile("Music/field_of_hopes.ogg");
	bgMusic.play();
	bgMusic.setVolume(60);

	//Initialising sound effects
	sf::Music bulletSound;
	bulletSound.openFromFile("Sound Effects/fire1.wav");
	bulletSound.setVolume(50);
	sf::Music centipedeFireSound;    // for all creatures 
	centipedeFireSound.openFromFile("Sound Effects/kill.wav");
	centipedeFireSound.setVolume(70);
	sf::Music playerDeathSound;
	playerDeathSound.openFromFile("Sound Effects/death.wav");
	playerDeathSound.setVolume(50);
	sf::Music levelSound;
	levelSound.openFromFile("Sound Effects/1up.wav");
	levelSound.setVolume(50);

	// Initializing Background.
	sf::Texture backgroundTexture;
	sf::Sprite backgroundSprite;
	backgroundTexture.loadFromFile("Textures/jungle.jpg");
	backgroundSprite.setTexture(backgroundTexture);
	backgroundSprite.setColor(sf::Color(255, 255, 255, 255 * 0.30)); // Reduces Opacity to 25%   //it was *0.20 i made it *0.30

	// Initializing Player and Player Sprites.
	float player[2] = {};
	player[x] = (gameColumns / 2) * boxPixelsX;
	//player[y] = (gameColumns * 3 / 4) * boxPixelsY;
	player[y]= resolutionY-boxPixelsY;
	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerTexture.loadFromFile("Textures/player.png");
	playerSprite.setTexture(playerTexture);
	playerSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	// Initializing Bullet and Bullet Sprites.
	float bullet[3] = {};
	bullet[x] = player[x];
	bullet[y] = player[y] - boxPixelsY;
	bullet[exists] = false;
	sf::Clock bulletClock;
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletTexture.loadFromFile("Textures/bullet.png");
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, boxPixelsX, boxPixelsY));

	//Initialising mushroom sprites
	float mushroom[900][3]={};
	sf::Texture mushroomTexture;
	sf::Sprite mushroomSprite[900]; 
	mushroomTexture.loadFromFile("Textures/mushroom.png");
	for (int i=0; i<900; i++)
	{
		mushroomSprite[i].setTexture(mushroomTexture);
		mushroomSprite[i].setTextureRect(sf::IntRect(0,0,boxPixelsX,boxPixelsY));
	}
	int mushroomN= 20+rand()%11; // initially 20 to 30 mushrooms
	int initMushroom= mushroomN; //initial num of mushrooms 
	int i=0;
	while(i<mushroomN)
	{
		bool spaceTaken=false;   // so mushrooms dont occupy same cell
		mushroom[i][exists]=2;                       
		mushroom[i][x]= (rand()%30)*boxPixelsX;   
		mushroom[i][y]= (1+(rand()%24))*boxPixelsY;  // row 1 to 24
		for (int j=0; j<i; j++)
		{
			if ((mushroom[i][x]==mushroom[j][x] && mushroom[i][y]==mushroom[j][y])||mushroom[i][x]==1*boxPixelsX||mushroom[i][x]==28*boxPixelsX) //already mushroom there||invalid cell
			{
				spaceTaken=true;
				break;
			}
		}
		if (!spaceTaken)
			i++;
	}

	//Initialising centipede segment sprites
	float centipede[25][3]={};    //each row is a segment and cols are x and y and exists determines movement
	sf::Clock centipedeClock;
	sf::Texture centipedeBodyTexture;
	centipedeBodyTexture.loadFromFile("Textures/c_body_left_walk.png");
	sf::Texture centipedeHeadTexture;
	centipedeHeadTexture.loadFromFile("Textures/c_head_left_walk.png");
	sf::Sprite centipedeSprite[25];   
	centipedeSprite[0].setTexture(centipedeHeadTexture);
	centipedeSprite[0].setTextureRect(sf::IntRect(0,0,(boxPixelsX*7)/8,boxPixelsY));  // a segment is 28x32px
	for (int i=1; i<12; i++)
	{
		centipedeSprite[i].setTexture(centipedeBodyTexture);
		centipedeSprite[i].setTextureRect(sf::IntRect((boxPixelsX*7)/8*((i-1)%8),0,(boxPixelsX*7)/8,boxPixelsY));  
	}
	int pos= rand()%30;  //random location of head
	for (int i=11; i>=0; i--) 
	{
		centipede[i][x]= (pos*boxPixelsX)+boxPixelsX*i;
		centipede[i][y]=0;
		centipede[i][exists]= -1; //-1 for downleft, 1 for downright, same but 2 for up, 0 means destroyed/doesnt exist
	}
	int frame=0;

	//explosion upon being shot or death
	sf::Texture explodeTexture;
	explodeTexture.loadFromFile("Textures/explosion.png");
	sf::Clock explodeClock;
	int explodeFrame=0;
	int segment=-1;
	int segmentEnd=-1;

	//accompanying fast-moving heads
	bool extraHeads;
	sf::Clock extraHeadClock;
	//for when centipede reaches player area
	sf::Clock flashClock;

	//Initilialising score and lives text
	sf::Font font;
	int score=0;
	sf::Text scoreText;
	font.loadFromFile("slkscr.ttf");
	scoreText.setFont(font);
	scoreText.setCharacterSize(30);
	scoreText.setPosition(boxPixelsX/2,0);
	scoreText.setFillColor(sf::Color::White);
	int life=6;
	sf::Text lifeText;
	lifeText.setFont(font);
	lifeText.setCharacterSize(30);
	lifeText.setFillColor(sf::Color::White);
	lifeText.setPosition(resolutionX-boxPixelsX*7.5,0);
	bool lifeatxK[4]={0,0,0,0};  //for extra life at 10k,12k,15k,20k , false all
	sf::Clock menuPause;
	sf:: Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(50);
	gameOverText.setFillColor(sf::Color::White);
	gameOverText.setPosition(resolutionX/3,resolutionY/2.5);
	gameOverText.setString("Game Over!");
	sf::Text winText;
	winText.setFont(font);
	winText.setCharacterSize(30);
	winText.setFillColor(sf::Color::White);
	winText.setPosition(resolutionX/3.5,resolutionY/2);
	winText.setString("You beat the game!"); 

	//regeneration of centipede upon player death
	bool regen=false;

	//Initialising game level
	int gameLevel=1;    // 1 and 2 are 'lower levels', 3 onwards are 'higher levels'
	bool levelUp=false;

	// other creatures
	//spider
	sf::Sprite spiderSprite;
	float spider[3]={};
	sf::Texture spiderTexture;
	spiderTexture.loadFromFile("Textures/spider_and_score.png");
	spiderSprite.setTexture(spiderTexture);
	spiderSprite.setTextureRect(sf::IntRect(0,0,60,32));
	int spiderFrame=0;
	spider[x]=0;
	spider[y]=(20+rand()%9)*boxPixelsY;
	spider[exists]=1;    // -1 moving left, 1 moving right
	int move=rand()%4;
	sf::Music spiderMusic;
	spiderMusic.openFromFile("Sound Effects/spider.wav");
	spiderMusic.setVolume(5);
	sf::Clock spiderClock;
	sf::Clock spiderSongClock;
	bool spiderScore=false;

	//scorpion
	sf::Sprite scorpionSprite;
	float scorpion[3]={};
	sf::Texture scorpionTexture;
	scorpionTexture.loadFromFile("Textures/scorpion.png");
	scorpionSprite.setTexture(scorpionTexture);
	scorpionSprite.setTextureRect(sf::IntRect(0,0,64,32));
	int scorpionFrame=0;
	sf::Music scorpionMusic;
	scorpionMusic.openFromFile("Sound Effects/scorpion.wav");
	scorpionMusic.setVolume(5);
	sf::Clock scorpionClock;
	sf::Clock scorpionSongClock;

	// game loop

	while(window.isOpen()) {

		window.draw(backgroundSprite);
		drawPlayer(window, player, playerSprite);
		drawMushrooms(window,mushroom,mushroomN,mushroomSprite);
		moveCentipede(centipede,mushroom,mushroomN,centipedeClock,frame,gameLevel);
		moveExtraHeads(centipede,mushroom,mushroomN,extraHeadClock);
		drawCentipede(window,centipede,centipedeSprite,frame,centipedeHeadTexture,centipedeBodyTexture,flashClock,regen);

		if (spider[exists]||spiderScore)
		{
			moveSpider(spider,mushroom,mushroomN,move,spiderFrame,spiderClock);
			drawSpider(window,spiderSprite,spider,spiderFrame,spiderScore,spiderClock);
			if (spider[exists])
				playSpiderSong(spiderMusic,spiderSongClock);
		}
		else if (spiderClock.getElapsedTime().asMilliseconds()>5000)
			regenerateSpider(spider,spiderClock);

		if (scorpion[exists])
		{
			drawScorpion(window,scorpionSprite,scorpion,scorpionFrame);
			moveScorpion(scorpion,mushroom,mushroomN,scorpionClock,scorpionFrame);
			playScorpionSong(scorpionMusic,scorpionSongClock);
		}
		else if (scorpionClock.getElapsedTime().asMilliseconds()>10000)
			regenerateScorpion(scorpion,scorpionClock);

		if (bullet[exists]) 
		{
			moveBullet(bullet,bulletClock,bulletSound);
			drawBullet(window, bullet, bulletSprite);
			fireBullet(bullet,mushroom,mushroomN,centipede,spider,spiderSprite,spiderScore,player,scorpion,centipedeFireSound,score,gameLevel,segment,segmentEnd);	
		}
		else
			bulletSound.stop();

		if (segment>-1)
			playExplosion(centipedeSprite,window,explodeTexture,explodeClock,segment,segmentEnd,explodeFrame);

		playerDeath(player,centipede,mushroom,mushroomN,spider,life,extraHeads,score,regen,playerDeathSound);   //sets regen to true if regen		
		displayScore(window,score,scoreText,life,lifeText,lifeatxK);

		if (segment==-1)
			checkLevel(centipede,gameLevel,levelUp,levelSound);   //set levelUp to true if all destroyed

		if ((regen||levelUp) && (segment==-1))
		{
			gameRegeneration(centipede,mushroom,mushroomN,initMushroom,spider,spiderClock,move,regen,levelUp,gameLevel,score,window,player);
			playerDeathSound.stop(); 
			levelSound.stop();
			extraHeads=false;
		}

		//max score?
		if (score>=999999)
		{
			maxScore(window,winText,menuPause,levelSound);
			return 0;
		}
		//ran out of lives
		if (life<=0)
		{
			outOfLives(window,gameOverText,menuPause);
			return 0;
		}


		sf::Event e;
		while (window.pollEvent(e)) {
			if (e.type == sf::Event::Closed) {
				return 0;
			}
			movePlayer(e,player,mushroom,mushroomN);
			if (e.type==sf::Event::KeyPressed)
			{
				if (e.key.code==sf::Keyboard::Space && !bullet[exists])
				{
					bulletSound.play();
					bullet[exists]=true;
					bullet[x] = player[x];
					bullet[y] = player[y];
					centipedeFireSound.stop();
				}
			}		
		}		
		window.display();
		window.clear();
	}
}


// function definitions

void drawPlayer(sf::RenderWindow& window, float player[], sf::Sprite& playerSprite) 
{
	playerSprite.setPosition(player[x], player[y]);
	window.draw(playerSprite);
}
void movePlayer(sf::Event e, float player[],float mushroom[][3],int mushroomN)
{
	if (e.type==sf::Event::KeyPressed)
	{
		bool collision=false;
		if (e.key.code==sf::Keyboard::Up)
		{
			for (int i=0; i<mushroomN; i++)
			{
				if (mushroom[i][x]==player[x] && mushroom[i][y]==player[y]-boxPixelsY && mushroom[i][exists]) 
				{
					collision=true;
					break;
				}
			}
			if (player[y]>=resolutionY-(5*boxPixelsY) && !collision) 
				player[y]-= boxPixelsY;
		}
		if (e.key.code==sf::Keyboard::Down)
		{
			for (int i=0; i<mushroomN; i++)
			{
				if (mushroom[i][x]==player[x] && mushroom[i][y]==player[y]+boxPixelsY && mushroom[i][exists])
				{
					collision=true;
					break;
				}
			}
			if (player[y]<resolutionY-boxPixelsY && !collision)
				player[y]+= boxPixelsY;
		}
		if (e.key.code==sf::Keyboard::Left)
		{
			for (int i=0; i<mushroomN; i++)
			{
				if (mushroom[i][y]==player[y] && mushroom[i][x]==player[x]-boxPixelsX && mushroom[i][exists])
				{
					collision=true;
					break;
				}
			}
			if (player[x]>=boxPixelsX && !collision)
				player[x]-= boxPixelsX;
		}
		if (e.key.code==sf::Keyboard::Right)
		{
			for (int i=0; i<mushroomN; i++)
			{
				if (mushroom[i][y]==player[y] && mushroom[i][x]==player[x]+boxPixelsX && mushroom[i][exists])
				{
					collision=true;
					break;
				}
			}
			if (player[x]<resolutionX-boxPixelsX && !collision)
				player[x]+= boxPixelsX;
		}
	}
}

void moveBullet(float bullet[], sf::Clock& bulletClock,sf::Music &bulletSound) 
{
	if (bulletClock.getElapsedTime().asMilliseconds() < 10)    // originally 20
		return;

	bulletClock.restart();
	bullet[y] -= 10;	
	if (bullet[y] < -32)
		bullet[exists] = false;
}
void drawBullet(sf::RenderWindow& window, float bullet[], sf::Sprite& bulletSprite) 
{
	bulletSprite.setPosition(bullet[x], bullet[y]);
	window.draw(bulletSprite);
}

void drawCentipede(sf::RenderWindow& window, float centipede[][3], sf::Sprite centipedeSprite[],int &frame,sf::Texture centipedeHeadTexture,sf::Texture centipedeBodyTexture,sf::Clock &flashClock,bool &regen) //just draws all segments using values in array
{      
	int segmentText;    
	for (int i=0; i<25; i++)
	{
		if (centipede[i][exists]) //exists
		{
			if (i==0||i>11)   //head
			{
				centipedeSprite[i].setTexture(centipedeHeadTexture);   
				if (centipede[i][exists]<0)  //left
					centipedeSprite[i].setTextureRect(sf::IntRect((boxPixelsX*7)/8*(frame%8),0,(boxPixelsX*7)/8,boxPixelsY));
				else if (centipede[i][exists]>0) //right
					centipedeSprite[i].setTextureRect(sf::IntRect((boxPixelsX*7)/8*(frame%8)+(boxPixelsX*7/8),0,-(boxPixelsX*7)/8,boxPixelsY));				
			}
			else if (centipede[i-1][exists]==0) //head
			{
				centipedeSprite[i].setTexture(centipedeHeadTexture);
				segmentText=(i-1+frame)%8;  //0 to 7
				if (centipede[i][exists]<0) //left
					centipedeSprite[i].setTextureRect(sf::IntRect(((boxPixelsX*7)/8)*(segmentText),0,(boxPixelsX*7)/8,boxPixelsY));
				else //right
					centipedeSprite[i].setTextureRect(sf::IntRect(((boxPixelsX*7)/8)*(segmentText)+(boxPixelsX*7/8),0,-(boxPixelsX*7)/8,boxPixelsY));
			}
			else
			{
				if (regen)   // set heads that were made by shooting back to body 
					centipedeSprite[i].setTexture(centipedeBodyTexture);
					segmentText=(i-1+frame)%8;  //0 to 7
				if (centipede[i][exists]<0) //left
					centipedeSprite[i].setTextureRect(sf::IntRect(((boxPixelsX*7)/8)*(segmentText),0,(boxPixelsX*7)/8,boxPixelsY));
				else //right
					centipedeSprite[i].setTextureRect(sf::IntRect(((boxPixelsX*7)/8)*(segmentText)+(boxPixelsX*7/8),0,-(boxPixelsX*7)/8,boxPixelsY));
			}
		}
	}
	regen=false;  
	bool restart=false;
	for (int i=0; i<25; i++)
	{
		if (centipede[i][exists])
		{
			if (centipede[i][y]>=resolutionY-(5*boxPixelsY))  // centipede flashes in player area
			{
				if (!(flashClock.getElapsedTime().asMilliseconds()<1))
				{
					restart=true;
					centipedeSprite[i].setPosition(centipede[i][x],centipede[i][y]);
					window.draw(centipedeSprite[i]);
				}
			}
			else 
			{
				centipedeSprite[i].setPosition(centipede[i][x],centipede[i][y]);
				window.draw(centipedeSprite[i]);
			}
		}
	}
	if (restart)
		flashClock.restart();
}
void moveCentipede(float centipede[][3],float mushroom[][3],int mushroomN,sf::Clock &centipedeClock,int &frame,int gameLevel)  
{
	if (centipedeClock.getElapsedTime().asMilliseconds()<(150/pow(2,gameLevel-1)))  // doubles every level
		return;
	centipedeClock.restart(); 

	int collision3=-1;   //centipede collision with itself? (to prevent merging)
	int turn=-1;
	for (int i=11; i>=0; i--)
	{	
		if (centipede[i][exists])
		{
			if (i==0||centipede[i-1][exists]==0) //head segment 
			{
				bool collision1=false;  //collision where it is beside edge/mushroom
				bool collision2=false;  //collision where it is at edge/mushroom
				
				for (int l=0; l<25; l++)
				{
					if (centipede[i][x]==centipede[l][x] && centipede[i][y]==centipede[l][y] && centipede[l][exists] && l!=i && collision3<0 && !(i+1==l&&i<12))   
					{
						collision3++;  // -1 to 0
						turn=i;
					}
				}
				if(collision3==0 && i==turn)  
					centipede[turn][y]+=32;
				else
				{
					if (centipede[i][exists]<0)   //left
					{
						for (int j=0; j<mushroomN; j++)      //mushroom collision1?
						{
							if (mushroom[j][x]+boxPixelsX==centipede[i][x] && mushroom[j][y]==centipede[i][y]&& mushroom[j][exists]) //centipede hit mushroom
							{
								collision1=true;
								break;
							}				
						}
						for (int k=0; k<mushroomN; k++)    //mushroom collision2?
						{
							if (mushroom[k][x]==centipede[i][x] && (mushroom[k][y]-16==centipede[i][y]||mushroom[k][y]+16==centipede[i][y]) && mushroom[k][exists]) //centipede hit mushroom
							{
								collision2=true;
								break;
							}
						}
						if(centipede[i][x]==32||collision1)   //beside left edge
						{
							centipede[i][x]-=32;
							if (centipede[i][exists]==-1) //if its moving down
								centipede[i][y]+=16;   //shift down
							else if (centipede[i][exists]==-2)  //if its moving up
								centipede[i][y]-=16;     //shift up
						}
						else if(centipede[i][x]==0||collision2)   //at left edge
						{
							centipede[i][x]+=32;
							if (centipede[i][exists]==-1)   //if its moving down
								centipede[i][y]+=16;      //shift down
							else if (centipede[i][exists]==-2)    //else up
								centipede[i][y]-=16;					
							centipede[i][exists]*=-1;    //change direction to right
						}
						else 
							centipede[i][x]-=32;       //not at any edge, simply shift left
					}	
					else if (centipede[i][exists]>0)   //right
					{
						for (int j=0; j<mushroomN; j++)
						{
							if (mushroom[j][x]-boxPixelsX==centipede[i][x] && mushroom[j][y]==centipede[i][y] && mushroom[j][exists]) //centipede hit mushroom
							{
								collision1=true;
								break;
							}
						}
						for (int k=0; k<mushroomN; k++)
						{
							if (mushroom[k][x]==centipede[i][x] && (mushroom[k][y]-16==centipede[i][y]||mushroom[k][y]+16==centipede[i][y]) && mushroom[k][exists]) //centipede hit mushroom
							{
								collision2=true;
								break;
							}
						}
						if (centipede[i][x]==resolutionX-2*32||collision1)
						{
							centipede[i][x]+=32;
							if (centipede[i][exists]==1)
								centipede[i][y]+=16;
							else if (centipede[i][exists]==2)
								centipede[i][y]-=16;
						}
						else if (centipede[i][x]==resolutionX-boxPixelsX||collision2)  //beside right edge
						{
							centipede[i][x]-=32;
							if (centipede[i][exists]==1)    //if its moving down 
								centipede[i][y]+=16;      //shift down
							else if (centipede[i][exists]==2)
								centipede[i][y]-=16;        //else up
							centipede[i][exists]*=-1;      //change direction to left
						}
						else
							centipede[i][x]+=32;     //if not at any edge, shift right
					}
				} 
				if (centipede[i][y]==resolutionY-32)  //if it reaches bottom, make it upward
				{
					if (centipede[i][exists]==-1)    //e.g. left and down becomes left and up
						centipede[i][exists]=-2;
					if (centipede[i][exists]==1)
						centipede[i][exists]=2;
				}
				if (centipede[i][y]==resolutionY-(5*boxPixelsY))  //reaches top of player area, make it downward
				{
					if (centipede[i][exists]==2)
						centipede[i][exists]=1;
					if (centipede[i][exists]==-2)
						centipede[i][exists]=-1;				
				}				
			}
			else     //body segment
			{
				centipede[i][x]=centipede[i-1][x];      
				centipede[i][y]=centipede[i-1][y];
				centipede[i][exists]=centipede[i-1][exists];
			}
		}		
	}
	frame++;
}

void drawMushrooms(sf::RenderWindow& window, float mushroom[][3],int mushroomN,sf::Sprite mushroomSprite[])
{
	for (int i=0; i<mushroomN; i++)
	{
		if (mushroom[i][exists]) 
		{
			if (mushroom[i][exists]==1)    // pertially destroyed regular mushroom
			{
				mushroomSprite[i].setTextureRect(sf::IntRect(boxPixelsX*2,0,boxPixelsX,boxPixelsY));
			}
			else if (mushroom[i][exists]==2)   // full health regular mushroom
			{
				mushroomSprite[i].setTextureRect(sf::IntRect(0,0,boxPixelsX,boxPixelsY));
			}
			else if (mushroom[i][exists]==5)   // full health poison mushroom
			{
				mushroomSprite[i].setTextureRect(sf::IntRect(0,boxPixelsY,boxPixelsX,boxPixelsY));
			}
			else if (mushroom[i][exists]==4)   // partially destroyed poison mushroom
			{
				mushroomSprite[i].setTextureRect(sf::IntRect(boxPixelsX*2,boxPixelsY,boxPixelsX,boxPixelsY));
			}
			mushroomSprite[i].setPosition(mushroom[i][x],mushroom[i][y]);
			window.draw(mushroomSprite[i]);
		}
	}
}

void fireBullet(float bullet[],float mushroom[][3],int &mushroomN,float centipede[][3],float spider[],sf::Sprite &spiderSprite,bool &spiderScore,float player[],float scorpion[],sf::Music &centipedeFireSound,int &score,int gameLevel,int &segment,int &segmentEnd) //i might manage shooting centipede here too
{
	// fire centipede
	for (int i=0; i<25; i++)
	{
		if (bullet[x]==centipede[i][x] && (bullet[y]>=centipede[i][y]&&bullet[y]<=centipede[i][y]+boxPixelsY) && centipede[i][exists]) 
		{  
			centipedeFireSound.play();
			if (gameLevel<=2 &&(i==0||centipede[i-1][exists]==0)&&i<12) //hit a head, not a fast moving head
			{
				int j=i;
				segment=i;
				while (centipede[j][exists]!=0 && j<12)
				{
					centipede[j][exists]=0;  // destroy whole section in lower levels
					j++;
				}
				segmentEnd=j-1; 
			}
			else
			{
				centipede[i][exists]=0;   //only destroy that segment
				segment=i;
			}
			//if mushroom already there dont create new mushroom
			bool create=true;
			for (int k=0; k<mushroomN; k++)
			{
				if (centipede[i][x]==mushroom[k][x] && (centipede[i][y]>=mushroom[k][y]&&centipede[i][y]<=mushroom[k][y]+32) && mushroom[k][exists])
				{
					create=false;
					break;
				}
			}
			if (create)
			{
				mushroom[mushroomN][x]=centipede[i][x];
				mushroom[mushroomN][y]=centipede[i][y];
				if (centipede[i][y]>=resolutionY-(5*boxPixelsY))   //poison mushroom within player area
					mushroom[mushroomN][exists]=5;
				else
					mushroom[mushroomN][exists]=2;    
				mushroomN++;    
			}
			bullet[exists]=false;
			if (i==0||centipede[i-1][exists]==0||i>11)		
				score+=20;	// +20 for head
			else	
				score+=10;  //+10 for body
			break;
		}
	}	
	//fire mushroom
	for (int i=0; i<mushroomN; i++)
	{
		if ((bullet[y]>=mushroom[i][y]&&bullet[y]<=mushroom[i][y]+boxPixelsY) && bullet[x]==mushroom[i][x] && mushroom[i][exists]) 
		{	 
			mushroom[i][exists]--; //loses a life
			if (mushroom[i][exists]==3)
				mushroom[i][exists]=0;
			if (!mushroom[i][exists])
				score++;           // +1 score
			bullet[exists]=false;
		}
	}
	//fire spider
	if (bullet[y]>=spider[y]&&bullet[y]<=spider[y]+32 && bullet[x]>=spider[x]&&bullet[x]<=spider[x]+60 && spider[exists])
	{
		centipedeFireSound.play();
		spider[exists]=0;
		if (player[y]==spider[y]+(3*boxPixelsY))
		{
			score+=300;
			spiderSprite.setTextureRect(sf::IntRect(0,32,60,32));
			spiderScore=true;
		}
		if (player[y]==spider[y]+(2*boxPixelsY))
		{
			score+=600;
			spiderSprite.setTextureRect(sf::IntRect(60,32,60,32));
			spiderScore=true;
		}
		if (player[y]==spider[y]+boxPixelsY)     
		{
			score+=900;
			spiderSprite.setTextureRect(sf::IntRect(120,32,60,32));
			spiderScore=true;
		}
		bullet[exists]=false;
	}
	// fire scorpion
	if (bullet[x]>=scorpion[x]&&bullet[x]<=scorpion[x]+64 && bullet[y]>=scorpion[y]&&bullet[y]<=scorpion[y]+32 && scorpion[exists])
	{
		centipedeFireSound.play();
		scorpion[exists]=0;
		score+=1000;
		bullet[exists]=false;
	}

}

void playExplosion(sf::Sprite centipedeSprite[],sf::RenderWindow &window,sf::Texture &explodeTexture,sf::Clock &explodeClock,int &segment,int &segmentEnd,int &explodeFrame)
{
	if (segmentEnd>-1)
	{
		for (int i=segment; i<=segmentEnd; i++)
			window.draw(centipedeSprite[i]);
	}
	else
		window.draw(centipedeSprite[segment]);
		
	if (explodeClock.getElapsedTime().asMilliseconds()<10)
		return;
	explodeClock.restart();
	if (explodeFrame<6)
	{
		if (segmentEnd>-1)   //whole section
		{
			for (int i=segment; i<=segmentEnd; i++)
			{
				centipedeSprite[i].setTexture(explodeTexture);
				centipedeSprite[i].setTextureRect(sf::IntRect(explodeFrame*boxPixelsX,0,boxPixelsX,boxPixelsY));
			}
		} 
		else    //one segment
		{	
			centipedeSprite[segment].setTexture(explodeTexture);
			centipedeSprite[segment].setTextureRect(sf::IntRect(explodeFrame*boxPixelsX,0,boxPixelsX,boxPixelsY));
		}
		explodeFrame++;
	}
	else
	{
		explodeFrame=0;
		segment=-1;
		segmentEnd=-1;
	}
	
}

void playerDeath(float player[],float centipede[][3],float mushroom[][3],int mushroomN,float spider[],int &life,bool &extraHeads,int &score,bool &regen,sf::Music &playerDeathSound)   //deal with coordinates later
{
	//collision with centipede?
	for (int i=0; i<25; i++)
	{
		if (centipede[i][exists] && centipede[i][y]==player[y] && centipede[i][x]==player[x])   //should i do a range for y when turning
		{
			life--;
			playerDeathSound.play();
			regen=true;
			return;
		}
	}
	//collision wtih poison mushroom  (im confused about how to manage this, is it if youre touching it or if you try to enter the same space as it?)
	for (int i=0; i<mushroomN; i++)
	{
		if ((player[x]==mushroom[i][x]-boxPixelsX||player[x]==mushroom[i][x]+boxPixelsX) && (player[y]==mushroom[i][y]+boxPixelsY||player[y]==mushroom[i][y]-boxPixelsY) && mushroom[i][exists]>3) 
		{
			life--;
			playerDeathSound.play();
			regen=true;
			return;
		}
	}
	//collision with spider
	if ((player[x]>=spider[x]&&player[x]<=spider[x]+60) && (spider[y]>=player[y]&&spider[y]<=player[y]+32) && spider[exists])
	{
		life--;
		playerDeathSound.play();
		regen=true;
		return;
	}

}

void displayScore(sf::RenderWindow &window,int &score,sf::Text &scoreText,int &life,sf::Text &lifeText,bool lifeatxK[])
{
	std::ostringstream ss1;
	ss1 << "score: " << score;
	scoreText.setString(ss1.str());
	std::ostringstream ss2;
	ss2 << "lives left: " << life;
	lifeText.setString(ss2.str());

	window.draw(scoreText);
	window.draw(lifeText);

	//extra life?  10k,12k,15k,20k
	if (life<6)
	{
		if (score>=10000 && !lifeatxK[0])
		{
			life++;
			lifeatxK[0]=true;  // only reward once
		}
		if (score>=12000 && !lifeatxK[1])
		{
			life++;
			lifeatxK[1]=true;
		}
		if (score>=15000 && !lifeatxK[2])
		{
			life++;
			lifeatxK[2]=true;
		}
		if (score>=20000 && !lifeatxK[3])
		{
			life++;
			lifeatxK[3]=true;
		}	
	}

}

void gameRegeneration(float centipede[][3],float mushroom[][3],int &mushroomN,int &initMushroom,float spider[],sf::Clock &spiderClock,int &move,bool &regen,bool &levelUp,int gameLevel,int &score,sf::RenderWindow &window,float player[])
{
	regen=true;
	if (levelUp)  //regen and levelup
	{
		//new mushroom organisation
		for (int i=0; i<mushroomN;i++)
			mushroom[i][exists]=0;
		initMushroom*=1.2; //20% more than prev
		mushroomN=initMushroom;
		int i=0;
		while(i<mushroomN)
		{
		bool spaceTaken=false;
		mushroom[i][exists]=2;
		mushroom[i][x]= (rand()%30)*boxPixelsX; 
		if (gameLevel>1 && i%(gameLevel+1)!=0)  
			mushroom[i][y]= (1+(rand()%(24-gameLevel-2)))*boxPixelsY;  //cluster at top of screen
		else
			mushroom[i][y]= (1+(rand()%24))*boxPixelsY;   
			for (int j=0; j<i; j++)
			{
				if ((mushroom[i][x]==mushroom[j][x] && mushroom[i][y]==mushroom[j][y])||mushroom[i][x]==1*boxPixelsX||mushroom[i][x]==28*boxPixelsX)//already mushroom there||invalid place
				{
					spaceTaken=true;
					break;
				}
			}
			if (!spaceTaken)
				i++;
		}
	}
	else
		regenerateMushroom(mushroom,mushroomN,score); 

	regenerateCentipede(centipede,gameLevel); 

	regenerateSpider(spider,spiderClock);
	
	sf::Clock regenClock; 
	sf::Sprite deathSprite;
	sf::Texture deathTexture;
	deathTexture.loadFromFile("Textures/death.png");
	deathSprite.setTexture(deathTexture);
	deathSprite.setTextureRect(sf::IntRect(0,0,64,32));
	sf::Clock deathClock;
	int deathFrameX=0;
	int deathFrameY=0;       
	while (regenClock.getElapsedTime().asMilliseconds()<2000)   //delay
	{
		while (!levelUp && deathFrameY<2)    
		{
			deathSprite.setPosition(player[x]-boxPixelsX/2,player[y]);
			deathAnimation(window,deathSprite,deathClock,deathFrameX,deathFrameY);
			window.draw(deathSprite);      
			window.display();
		}
	}
	deathFrameY=0;
	levelUp=false;	
}

void regenerateMushroom(float mushroom[][3],int mushroomN,int &score)  
{
	for (int i=0; i<mushroomN; i++)   
	{
		//5,4,1 set to 2
		if (mushroom[i][exists] && mushroom[i][exists]!=2)
		{
			mushroom[i][exists]=2;
			score+=5; //5 points for every regenerated mushroom
		}
	}
}
void regenerateCentipede(float centipede[][3],int gameLevel)
{
	for (int i=0; i<25; i++)   
	{
		centipede[i][exists]=0;   // reset all
	}
	int pos= rand()%30; 
	if (gameLevel<=12)
	{
		for (int i=11-gameLevel+1; i>=0; i--) 
		{
			centipede[i][x]= (pos*boxPixelsX)+boxPixelsX*i;
			centipede[i][y]=0;
			centipede[i][exists]= -1; 
		}
		//accompanying head
		for (int i=12; i<12+gameLevel-1; i++)
		{
			centipede[i][x]= (pos*boxPixelsX)+2*(boxPixelsX*i);
			centipede[i][y]= 0;
			centipede[i][exists]= -1;
		}
	}
	else   // always 12 heads for gameLevel>12
	{
		centipede[0][x]= (pos*boxPixelsX);
		centipede[0][y]=0;
		centipede[0][exists]= -1; 
		for (int i=12; i<23; i++)
		{
			centipede[i][x]= (pos*boxPixelsX)+2*(boxPixelsX*i);
			centipede[i][y]= 0;
			centipede[i][exists]= -1;
		}		
	}	
}

void checkLevel(float centipede[][3],int &gameLevel,bool &levelUp,sf::Music &levelSound)
{
	bool nextLevel=true;
	for (int i=0; i<25; i++)
	{
		if (centipede[i][exists])   //not destroyed yet
		{
			nextLevel=false;
			break;
		}
	}
	if (nextLevel)
	{
		gameLevel++;
		levelUp=true;
		levelSound.play();
	}
}

void deathAnimation(sf::RenderWindow &window,sf::Sprite &deathSprite,sf::Clock &deathClock,int &deathFrameX,int &deathFrameY)
{
	if (deathClock.getElapsedTime().asMilliseconds()<50)
		return;
	deathClock.restart();
	deathSprite.setTextureRect(sf::IntRect(deathFrameX*64,deathFrameY*32,64,32));
	deathFrameX++;
	if (deathFrameX>3)
	{
		deathFrameY++;
		deathFrameX=0;
	}
}

void maxScore(sf::RenderWindow &window,sf::Text &winText,sf::Clock &menuPause,sf::Music &levelSound)
{
	window.draw(winText);
	window.display();
	menuPause.restart();
	levelSound.play();
	while (menuPause.getElapsedTime().asMilliseconds()<5000)
	{
		//delay for message, then quit
	}
	levelSound.stop();
}

void outOfLives(sf::RenderWindow &window,sf::Text &gameOverText,sf::Clock &menuPause)
{
	window.draw(gameOverText);
	window.display();
	menuPause.restart();
	while (menuPause.getElapsedTime().asMilliseconds()<5000)
	{
		//delay for message, then quit
	}
}

void moveExtraHeads(float centipede[][3],float mushroom[][3],int mushroomN,sf::Clock &extraHeadClock)
{
	if (extraHeadClock.getElapsedTime().asMilliseconds()<50)
		return;
	extraHeadClock.restart(); 

	int collision3=-1;
	int turn=-1;
	for (int i=24; i>11; i--)
	{	
		if (centipede[i][exists])
		{
			bool collision1=false;  //collision where it is beside edge/mushroom
			bool collision2=false;  //collision where it is at edge/mushroom
			//centipede collision
			for (int l=0; l<25; l++)
			{
				if (centipede[i][x]==centipede[l][x] && centipede[i][y]==centipede[l][y] && centipede[l][exists] && l!=i && collision3<0)   // i think last cond is fine like even if theyre 2 heads smth was destroyed b/w them to theyre not going yo match anwyay
				{
					collision3++;  // -1 to 0
					turn=i;
				}
			}
			if(collision3==0 && i==turn)  
				centipede[turn][y]+=32;
			else
			{
				if (centipede[i][exists]<0)   //left
				{
					for (int j=0; j<mushroomN; j++)      //mushroom collision1?
					{
						if (mushroom[j][x]+boxPixelsX==centipede[i][x] && mushroom[j][y]==centipede[i][y]&& mushroom[j][exists]) //centipede hit mushroom
						{
							collision1=true;
							break;
						}				
					}
					for (int k=0; k<mushroomN; k++)    //mushroom collision2?
					{
						if (mushroom[k][x]==centipede[i][x] && (mushroom[k][y]-16==centipede[i][y]||mushroom[k][y]+16==centipede[i][y]) && mushroom[k][exists]) //centipede hit mushroom
						{
							collision2=true;
							break;
						}
					}
					if(centipede[i][x]==32||collision1)   //beside left edge
					{
						centipede[i][x]-=32;
						if (centipede[i][exists]==-1) 
							centipede[i][y]+=16;  
						else if (centipede[i][exists]==-2) 
							centipede[i][y]-=16;    
					}
					else if(centipede[i][x]==0||collision2)   //at left edge
					{
						centipede[i][x]+=32;
						if (centipede[i][exists]==-1)   
							centipede[i][y]+=16;      
						else if (centipede[i][exists]==-2)    
							centipede[i][y]-=16;					
						centipede[i][exists]*=-1;    
					}
					else 
						centipede[i][x]-=32;       //not at any edge, simply shift left
				}	
				else if (centipede[i][exists]>0)   //right
				{
					for (int j=0; j<mushroomN; j++)
					{
						if (mushroom[j][x]-boxPixelsX==centipede[i][x] && mushroom[j][y]==centipede[i][y] && mushroom[j][exists]) //centipede hit mushroom
						{
							collision1=true;
							break;
						}
					}
					for (int k=0; k<mushroomN; k++)
					{
						if (mushroom[k][x]==centipede[i][x] && (mushroom[k][y]-16==centipede[i][y]||mushroom[k][y]+16==centipede[i][y]) && mushroom[k][exists]) //centipede hit mushroom
						{
							collision2=true;
							break;
						}
					}
					if (centipede[i][x]==resolutionX-2*32||collision1)
					{
						centipede[i][x]+=32;
						if (centipede[i][exists]==1)
							centipede[i][y]+=16;
						else if (centipede[i][exists]==2)
							centipede[i][y]-=16;
					}
					else if (centipede[i][x]==resolutionX-boxPixelsX||collision2)  //beside right edge
					{
						centipede[i][x]-=32;
						if (centipede[i][exists]==1)   
							centipede[i][y]+=16;      
						else if (centipede[i][exists]==2)
							centipede[i][y]-=16;     
						centipede[i][exists]*=-1;      
					}
					else
						centipede[i][x]+=32;     //if not at any edge, shift right
				}
			} 
			if (centipede[i][y]==resolutionY-32)  //if it reaches bottom, make it upward
			{
				if (centipede[i][exists]==-1)   
					centipede[i][exists]=-2;
				if (centipede[i][exists]==1)
					centipede[i][exists]=2;
			}
			if (centipede[i][y]==resolutionY-(5*boxPixelsY))  //reaches top of player area, make it downward
			{
				if (centipede[i][exists]==2)
					centipede[i][exists]=1;
				if (centipede[i][exists]==-2)
					centipede[i][exists]=-1;				
			}				
		}		
	}
}

void drawSpider(sf::RenderWindow &window,sf::Sprite &spiderSprite,float spider[],int spiderFrame,bool &spiderScore,sf::Clock &spiderClock)
{
	if (spider[exists])
	{
		spiderSprite.setTextureRect(sf::IntRect((spiderFrame%8)*60,0,60,32));
		spiderSprite.setPosition(spider[x],spider[y]);
		window.draw(spiderSprite);
	}
	else if (spiderScore)
	{
		if (spiderClock.getElapsedTime().asMilliseconds()<2000)
			window.draw(spiderSprite);   // show score for 2 seconds
		else
			spiderScore=false;
	}
}
void moveSpider(float spider[],float mushroom[][3],int mushroomN,int &move,int &spiderFrame,sf::Clock &spiderClock)
{
	if (spider[exists])
	{
		if (spiderClock.getElapsedTime().asMilliseconds()<200)
			return;
		spiderClock.restart();
		int top= resolutionY-(11*boxPixelsY);
		int bottom= resolutionY-boxPixelsY;
		int mid= resolutionY-(5*boxPixelsY);
		
		if (spider[y]==top && (move==0||move==1)) // if at top stop upward motions, change to downwards
			move= rand()%2 + 2; //  2 or 3
		else if (spider[y]==bottom && (move==2||move==3))  // if at bottom stop downward motions, change to upwards , creates zigzag
			move= rand()%2; // 0 1 
			
		switch(move)
		{
			case 0: //up
				spider[y]-=boxPixelsY;
				break;
			case 1: //diagonal up
				if (spider[exists]>0)  //right
				{
					spider[y]-=boxPixelsY;
					spider[x]+=boxPixelsX;
				}
				else  //left
				{
					spider[y]-=boxPixelsY;
					spider[x]-=boxPixelsX;
				}
				break;
			case 2:  //diagonal down
				if (spider[exists]>0)
				{
					spider[y]+=boxPixelsY;
					spider[x]+=boxPixelsX;
				}
				else
				{
					spider[y]+=boxPixelsY;
					spider[x]-=boxPixelsX;
				}
				break;
			case 3:  //down
				spider[y]+=boxPixelsY;
				break;
		}
		//hit mushroom?
		for (int i=0; i<mushroomN; i++)
		{
			if ((spider[x]>=mushroom[i][x]&&spider[x]<=mushroom[i][x]+32)&&(spider[y]>=mushroom[i][y]&&spider[y]<=mushroom[i][y]+32)&&mushroom[i][exists])
			{
				mushroom[i][exists]=0;
			}
		}
		if (spider[x]>resolutionX||spider[x]<0)   //out of screen
			spider[exists]=0;
		spiderFrame++;
	}
}
void regenerateSpider(float spider[],sf::Clock &spiderClock)
{
	int whichSide= rand()%2;
	if (whichSide)
	{
		spider[x]=0;   //1 from left
		spider[exists]=1;
	}
	else
	{
		spider[x]=resolutionX-boxPixelsX; //0 from right
		spider[exists]=-1;
	}		
	spider[y]=(20+rand()%9)*boxPixelsY;
	spiderClock.restart();
}
void playSpiderSong(sf::Music &spiderMusic,sf::Clock &spiderSongClock)
{
	if (spiderMusic.getStatus()!=sf::Music::Playing)
		spiderMusic.play();
	if (spiderSongClock.getElapsedTime().asMilliseconds()>750)  // once finished playing, replay 
	{
		spiderSongClock.restart();
		spiderMusic.stop();
	}		
}

void drawScorpion(sf::RenderWindow &window,sf::Sprite &scorpionSprite,float scorpion[],int scorpionFrame)
{
	scorpionSprite.setTextureRect(sf::IntRect(64*(scorpionFrame%4),0,64,32));
	scorpionSprite.setPosition(scorpion[x],scorpion[y]);
	window.draw(scorpionSprite);
}
void moveScorpion(float scorpion[],float mushroom[][3],int mushroomN,sf::Clock &scorpionClock,int &scorpionFrame)
{
	if (scorpion[exists])
	{
		if (scorpionClock.getElapsedTime().asMilliseconds()<250)  //4 steps/s
			return;
		scorpionClock.restart();

		if (scorpion[exists]>0)   //right
			scorpion[x]+=boxPixelsX;
		if (scorpion[exists]<0)   //left
			scorpion[x]-=boxPixelsX;

		if (scorpion[x]<0||scorpion[x]>resolutionX)  //out of screen
			scorpion[exists]=0;

		for (int i=0; i<mushroomN; i++)   // poisoning mushroom
		{
			if (scorpion[x]>=mushroom[i][x]&&scorpion[x]<=mushroom[i][x]+32 && scorpion[y]==mushroom[i][y] && mushroom[i][exists] && mushroom[i][exists]<3)
				mushroom[i][exists]+=3;  //2 to 5 and 1 to 4
		}
	}
	scorpionFrame++;
}
void regenerateScorpion(float scorpion[],sf::Clock &scorpionClock)
{
	int whichSide= rand()%2;
	if (whichSide)
	{
		scorpion[x]=0;   //1 from left
		scorpion[exists]=1;
	}
	else
	{
		scorpion[x]=resolutionX-boxPixelsX; //0 from right
		scorpion[exists]=-1;
	}		
	scorpion[y]= (1+rand()%24) *boxPixelsY;
	scorpionClock.restart();
}
void playScorpionSong(sf::Music &scorpionMusic,sf::Clock &scorpionSongClock)
{
	if (scorpionMusic.getStatus()!=sf::Music::Playing)
		scorpionMusic.play();
	if (scorpionSongClock.getElapsedTime().asMilliseconds()>750)
	{
		scorpionSongClock.restart();
		scorpionMusic.stop();
	}
}










