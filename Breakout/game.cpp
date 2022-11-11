#include "classes.h"

void restart();
void setWindow(string win);

class MyFramework : public Framework {
private:
	Sprite* mapSprite;
	player* mainHero;
	ball* arkanoidBall;
	ability* ab;
	std::string map[widthMap];
	int amountOfDestroyedBlocks;

public:

	MyFramework()
	{
		mapSprite = nullptr;
		mainHero = new player;
		arkanoidBall = new ball;
		ab = new ability;
		amountOfDestroyedBlocks = 0;
	}
	void restart()
	{
		delete[] mainHero;
		mainHero = new player;
		delete[] arkanoidBall;
		arkanoidBall = new ball;
		delete[] ab;
		ab = new ability;
		amountOfDestroyedBlocks = 0;

		setMap();
	}
	void newLvl()
	{
		delete[] mainHero;
		mainHero = new player;
		delete[] arkanoidBall;
		arkanoidBall = new ball;
		delete[] ab;
		ab = new ability;
		amountOfDestroyedBlocks = 0;

		amountOfBlocks = 22;
		redBlocks = 2;
		blockPixelsWidth = screenWidth / widthMap;

		copy(newMap, newMap + widthMap, map);

	}
	void setMap()
	{
		copy(mainMap, mainMap + widthMap, map);

	}
	void mapBuilding()
	{
		for (int i = 0; i < heightMap; i++)
			for (int j = 0; j < widthMap; j++)
			{

				if (map[i][j] == '1')
				{
					if (arkanoidBall->checkColision(j * blockPixelsWidth, i * blockPixelsHeight, blockPixelsWidth, blockPixelsHeight))
					{
						arkanoidBall->ballDirection(j * blockPixelsWidth, i * blockPixelsHeight);
						map[i][j] = ' ';
						++amountOfDestroyedBlocks;
						arkanoidBall->inRowCounter();

						continue;
					}
					mapSprite = createSprite("data/01-Breakout-Tiles.png");
					setSpriteSize(mapSprite, blockPixelsWidth, blockPixelsHeight);
					drawSprite(mapSprite, j * blockPixelsWidth, i * blockPixelsHeight);
					destroySprite(mapSprite);
				}
				if (map[i][j] == '2')
				{

					if (arkanoidBall->checkColision(j * blockPixelsWidth, i * blockPixelsHeight, blockPixelsWidth, blockPixelsHeight))
					{
						arkanoidBall->ballDirection(j * blockPixelsWidth, i * blockPixelsHeight);
						if (amountOfDestroyedBlocks == amountOfBlocks - redBlocks) {
							++amountOfDestroyedBlocks;
							map[i][j] = ' ';
							arkanoidBall->inRowCounter();
							continue;
						}
					}
					mapSprite = createSprite("data/07-Breakout-Tiles.png");
					setSpriteSize(mapSprite, blockPixelsWidth, blockPixelsHeight);
					drawSprite(mapSprite, j * blockPixelsWidth, i * blockPixelsHeight);
					destroySprite(mapSprite);
				}
			}

	}

	virtual void PreInit(int& width, int& height, bool& fullscreen)
	{
		width = screenWidth;
		height = screenHeight;
		fullscreen = false;
	}
	void initSprites()
	{

	}
	virtual bool Init() {
		return true;
	}

	virtual void Close() {

	}

	virtual bool Tick() {

		//drawSprite(createSprite("data/arkanoidBackGrond.jpg"), 0, 0); // back ground 
		mapBuilding();

		if (amountOfDestroyedBlocks == amountOfBlocks) newLvl();

		if (arkanoidBall->getInRow() == 3)
		{
			Sprite* three;
			three = createSprite("data/x3.png");
			drawSprite(three, screenWidth / 2, screenHeight / 2);
		}
		else if (arkanoidBall->getInRow() > 3)
		{
			Sprite* inRow;
			inRow = createSprite("data/row.png");
			drawSprite(inRow, screenWidth / 2, screenHeight / 2);
		}


		if (ab->getIsOnMap())
		{
			ab->update();
			ab->changeSize();
			ab->draw();
			ab->checkColisionWithPlayer(mainHero);

		}
		else {
			if (ab->getTime() == 0) ab->setTime(getTickCount());
			if (getTickCount() - ab->getTime() >= 15000)
			{
				ab->setAppearing(true);
			}
		}

		mainHero->update();
		arkanoidBall->update(mainHero);


		if (!mainHero->isAlive() || amountOfBlocks == amountOfDestroyedBlocks) restart();

		return false;
	}

	virtual void onMouseMove(int x, int y, int xrelative, int yrelative) {
		arkanoidBall->setMousePos(x, y);
	}

	virtual void onMouseButtonClick(FRMouseButton button, bool isReleased) {
		if (button == FRMouseButton::LEFT && isReleased == false && arkanoidBall->getShoot())
		{
			arkanoidBall->followCursPosition();
		}
	}

	virtual void onKeyPressed(FRKey k) {
		if (k == FRKey::RIGHT) mainHero->setVelocity(playerVelocity);
		if (k == FRKey::LEFT) mainHero->setVelocity(-playerVelocity);
	}

	virtual void onKeyReleased(FRKey k) {
		if (k == FRKey::RIGHT) mainHero->setVelocity(0);
		if (k == FRKey::LEFT) mainHero->setVelocity(0);
	}

	virtual const char* GetTitle() override
	{
		return "Arcanoid";
	}
};

int main(int argc, char* argv[])
{
	if (argc >= 2)
		setWindow(string(argv[2]));

	blockPixelsHeight = screenHeight / 10.0;
	blockPixelsWidth = screenWidth / widthMap;

	srand(time(NULL));
	MyFramework game;
	game.setMap();
	run(&game);


}


void setWindow(string win)
{
	string numbers;
	int position = win.find('x');
	if (position == -1) return;
	for (size_t i = 0; i < position; i++)
	{
		numbers += win[i];
	}
	screenWidth = stoi(numbers);
	numbers = "";
	for (size_t i = position + 1; i < win.length(); i++)
	{
		numbers += win[i];
	}
	screenHeight = stoi(numbers);
}

void restart()
{
	if (run(new MyFramework));
}

