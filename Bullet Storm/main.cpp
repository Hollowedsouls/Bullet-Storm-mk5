#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN

#define GLX_GLXEXT_LEGACY //Must be declared so that our local glxext.h is picked up, rather than the system one


#include <windows.h>
#include "windowOGL.h"
#include "GameConstants.h"
#include "cWNDManager.h"
#include "cInputMgr.h"
#include "cSoundMgr.h"
#include "cFontMgr.h"
#include "cSprite.h"
#include "asteroidsGame.h"
#include "cButton.h"
#include <mmsystem.h>

int WINAPI WinMain(HINSTANCE hInstance,
                   HINSTANCE hPrevInstance,
                   LPSTR cmdLine,
                   int cmdShow)
{

    //Set our window settings
    const int windowWidth = 768;
    const int windowHeight = 1024;
    const int windowBPP = 16;
	int  score = 0;
	string str, Ttime,Mtime,Stime;
	string outputMsg = "Score: 0", outputTime="";
	LPCSTR strScore,strTime;
	int milliseconds=0, seconds=0, minutes=0, hours=0;
	bool cheatsy = true;

	


    //This is our window
	static cWNDManager* pgmWNDMgr = cWNDManager::getInstance();

	// This is the input manager
	static cInputMgr* theInputMgr = cInputMgr::getInstance();

	// This is the sound manager
	static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();

	// This is the Font manager
	static cFontMgr* theFontMgr = cFontMgr::getInstance();

	//The example OpenGL code
    windowOGL theOGLWnd;
	
    //Attach our the OpenGL window
	pgmWNDMgr->attachOGLWnd(&theOGLWnd);

	// Attach the keyboard manager
	pgmWNDMgr->attachInputMgr(theInputMgr);

    //Attempt to create the window
	if (!pgmWNDMgr->createWND(windowWidth, windowHeight, windowBPP))
    {
        //If it fails

        MessageBox(NULL, "Unable to create the OpenGL Window", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	if (!theOGLWnd.initOGL(windowWidth, windowHeight)) //Initialize our example
    {
        MessageBox(NULL, "Could not initialize the application", "An error occurred", MB_ICONERROR | MB_OK);
		pgmWNDMgr->destroyWND(); //Reset the display and exit
        return 1;
    }

	//Clear key buffers
	theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);

	/* initialize random seed: */
	srand((unsigned int)time(NULL));



	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\shot.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[4] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav","Z_CAR_M_-_05_-_Down_The_Sewer.mp3" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);
	theSoundMgr->add("Music", gameSounds[3]);
	PlaySound(TEXT("Music"), NULL, SND_FILENAME | SND_ASYNC);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/AndromedaTV.ttf","Fonts/Welbut.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	theFontMgr->addFont("AndromedaTV", gameFonts[2], 24);
	theFontMgr->addFont("Welbut", gameFonts[3], 24);
	theFontMgr->addFont("Welbutbig", gameFonts[3], 72);


	// Create vector array of textures

	for (int astro = 0; astro < 25; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2((rand() % 708 + 5), (rand() % 1024)-1024));
		theAsteroids[astro]->setSpriteTranslation(glm::vec2((rand() % 4 + 1), (rand() % 4 + 1)));
		int randAsteroid = 0;
		theAsteroids[astro]->setTexture(theGameTextures[randAsteroid]->getTexture());
		theAsteroids[astro]->setTextureDimensions(theGameTextures[randAsteroid]->getTWidth(), theGameTextures[randAsteroid]->getTHeight());
		theAsteroids[astro]->setSpriteCentre();
		theAsteroids[astro]->setAsteroidVelocity(glm::vec2(glm::vec2(0.0f, 0.0f)));
		theAsteroids[astro]->setActive(true);
		theAsteroids[astro]->setMdlRadius();
	}


	cTexture textureBkgd;
	textureBkgd.createTexture("Images\\Space background.png");
	cTexture textureTitle;
	textureTitle.createTexture("Images\\Title.png");
	cTexture textureBanner;
	textureBanner.createTexture("Images\\Banner.png");
	cTexture textureConM;
	textureConM.createTexture("Images\\Controls1.png");
	cTexture textureConB;
	textureConB.createTexture("Images\\Controls2.png");
	
	cBkGround spriteTitle;
	spriteTitle.setSpritePos(glm::vec2(140.0f,300.0f));
	spriteTitle.setTexture(textureTitle.getTexture());
	spriteTitle.setTextureDimensions(textureTitle.getTWidth(), textureTitle.getTHeight());

	cBkGround spriteBanner;
	spriteBanner.setSpritePos(glm::vec2(0.0f,0.0f));
	spriteBanner.setTexture(textureBanner.getTexture());
	spriteBanner.setTextureDimensions(textureBanner.getTWidth(), textureBanner.getTHeight());

	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	cBkGround spriteConM;
	spriteConM.setSpritePos(glm::vec2(0.0f, 500.0f));
	spriteConM.setTexture(textureConM.getTexture());
	spriteConM.setTextureDimensions(textureConM.getTWidth(), textureConM.getTHeight());

	cBkGround spriteConB;
	spriteConB.setSpritePos(glm::vec2(580.0f, 500.0f));
	spriteConB.setTexture(textureConB.getTexture());
	spriteConB.setTextureDimensions(textureConB.getTWidth(), textureConB.getTHeight());

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\SpaceShip.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(windowWidth / 2, windowHeight - (rocketTxt.getTHeight()/1.5f)));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Create vector array of button textures
	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/Exitbtn.png", "Images/Buttons/Quitbtn.png", "Images/Buttons/resumebtn.png", "Images/Buttons/Playbtn.png", "Images/Buttons/mainmenubtn.png"};
	for (int tCount = 0; tCount < 5; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}
	 //Creates the buttons for the menu's
	cButton playButton;
	playButton.attachInputMgr(theInputMgr);
	playButton.setTexture(btnTextureList[3]->getTexture());
	playButton.setTextureDimensions(btnTextureList[3]->getTWidth(), btnTextureList[3]->getTHeight());
	
	cButton conButton;
	conButton.attachInputMgr(theInputMgr);
	conButton.setTexture(btnTextureList[2]->getTexture());
	conButton.setTextureDimensions(btnTextureList[2]->getTWidth(), btnTextureList[2]->getTHeight());
	
	cButton exitButton;
	exitButton.attachInputMgr(theInputMgr);
	exitButton.setTexture(btnTextureList[0]->getTexture());
	exitButton.setTextureDimensions(btnTextureList[0]->getTWidth(), btnTextureList[0]->getTHeight());
	
	cButton quitButton;
	quitButton.attachInputMgr(theInputMgr);
	quitButton.setTexture(btnTextureList[1]->getTexture());
	quitButton.setTextureDimensions(btnTextureList[1]->getTWidth(), btnTextureList[1]->getTHeight());
	
	cButton menuButton;
	menuButton.attachInputMgr(theInputMgr);
	menuButton.setTexture(btnTextureList[4]->getTexture());
	menuButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);


	gameState theGameState = MENU;
	btnTypes theBtnType = EXIT;

    //This is the mainloop, we render frames until isRunning returns false
	while (pgmWNDMgr->isWNDRunning())
    {
		pgmWNDMgr->processWNDEvents(); //Process any window events

        //We get the time that passed since the last frame
		float elapsedTime = pgmWNDMgr->getElapsedSeconds();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		switch (theGameState)
		{
		case MENU:
			spriteBkgd.render();
			spriteTitle.render();
			spriteConM.render();
			spriteConB.render();

			playButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth()/2), windowHeight / 2));
			quitButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2 + windowHeight / 10));
			playButton.render();
			quitButton.render();

			theGameState = playButton.update(theGameState, PLAYING);
			theGameState = quitButton.update(theGameState, END);

			
			break;

		case END:
			spriteBkgd.render();

			conButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2));
			menuButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2 + windowHeight / 10));
			exitButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2 + windowHeight / 5));
			conButton.render();
			menuButton.render();
			exitButton.render();


			theGameState = conButton.update(theGameState, PLAYING);
			theGameState = menuButton.update(theGameState, MENU);
			theGameState = exitButton.update(theGameState, END);

			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			if (conButton.getClicked())
			{
				PlaySound(TEXT("Shot"), NULL, SND_FILENAME | SND_ASYNC);
				milliseconds = 0;
				seconds = 0;
				score = 0;
				minutes = 0;
				hours = 0;
			}

			break;
		case GAMEOVER:
			spriteBkgd.render();
			theFontMgr->getFont("Welbutbig")->printText("Game Over", FTPoint((windowWidth / 2) - (windowWidth / 4), -200.0f, 30.0f));
			menuButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2 ));
			exitButton.setSpritePos(glm::vec2(windowWidth / 2 - (btnTextureList[3]->getTWidth() / 2), windowHeight / 2 + windowHeight / 10));
			menuButton.render();
			exitButton.render();

			theGameState = menuButton.update(theGameState, MENU);
			theGameState = exitButton.update(theGameState, END);

			str = to_string(score).c_str(); //score;
			outputMsg = "Score: " + str;
			strScore = outputMsg.c_str();
			theFontMgr->getFont("Welbut")->printText(strScore, FTPoint(30.0f, -75.0f, 30.0f));

			if (exitButton.getClicked())
			{
				SendMessage(pgmWNDMgr->getWNDHandle(), WM_CLOSE, NULL, NULL);
			}
			break;

		case PLAYING:
			spriteBkgd.render();

			rocketSprite.update(elapsedTime);

			vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
			while (asteroidIterator != theAsteroids.end())
			{
				if ((*asteroidIterator)->isActive() == false)
				{
					theGameState = GAMEOVER;
				}
				else
				{
					(*asteroidIterator)->update(elapsedTime);
					(*asteroidIterator)->render();
					++asteroidIterator;
					
				}
			}

			spriteBanner.render();
			//make shift timer
			if (milliseconds == 60)
			{
				seconds++;
				if (cheatsy == true)
				{
					score += 10;
					cheatsy = false;
				}
				else
				{
					cheatsy = true;
				}
				milliseconds = 0;
			}
			if (seconds == 60)
			{
				minutes++;
				seconds = 0;
			}
			if (minutes == 60)
			{
				hours++;
				minutes = 0;
			}
			milliseconds++;


			Ttime = to_string(hours).c_str();
			Mtime = to_string(minutes).c_str();
			Stime = to_string(seconds).c_str();
			outputTime = "Time" + Ttime + ":" + Mtime + ":" + Stime;
			strTime = outputTime.c_str();

			str = to_string(score).c_str(); //score;
			outputMsg = "Score: " + str;
			strScore = outputMsg.c_str();

			rocketSprite.render();
			//theFontMgr->getFont("Welbut")->printText("Asteriods", FTPoint(0.0f, -1.0f, 0.0f));
			theFontMgr->getFont("Welbut")->printText(strTime, FTPoint((windowWidth / 2) - (windowWidth / 10), -30.0f, 30.0f));
			theFontMgr->getFont("Welbut")->printText(strScore, FTPoint(30.0f, -75.0f, 30.0f));

			break;
		
		}
		
		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
