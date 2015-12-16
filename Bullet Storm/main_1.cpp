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


	gameState theGameState = MENU;

	// Create vector array of textures
	LPCSTR texturesToUse[] = { "Images\\shot.png", "Images\\bullet.png" };
	for (int tCount = 0; tCount < 2; tCount++)
	{
		theGameTextures.push_back(new cTexture());
		theGameTextures[tCount]->createTexture(texturesToUse[tCount]);
	}

	// load game sounds
	// Load Sound
	LPCSTR gameSounds[3] = { "Audio/who10Edit.wav", "Audio/shot007.wav", "Audio/explosion2.wav" };

	//theSoundMgr->add("Theme", gameSounds[0]);
	theSoundMgr->add("Shot", gameSounds[1]);
	theSoundMgr->add("Explosion", gameSounds[2]);

	// load game fontss
	// Load Fonts
	LPCSTR gameFonts[4] = { "Fonts/digital-7.ttf", "Fonts/space age.ttf", "Fonts/AndromedaTV.ttf","Fonts/Welbut.ttf" };

	theFontMgr->addFont("SevenSeg", gameFonts[0], 24);
	theFontMgr->addFont("Space", gameFonts[1], 24);
	theFontMgr->addFont("AndromedaTV", gameFonts[2], 24);
	theFontMgr->addFont("Welbut", gameFonts[3], 24);

	// Create vector array of textures

	for (int astro = 0; astro < 5; astro++)
	{
		theAsteroids.push_back(new cAsteroid);
		theAsteroids[astro]->setSpritePos(glm::vec2(windowWidth / 6.0f+ ((windowWidth / 6.0f)*astro), 0.0f));
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
	textureBkgd.createTexture("Images\\generic.png");
	cBkGround spriteBkgd;
	spriteBkgd.setSpritePos(glm::vec2(0.0f, 0.0f));
	spriteBkgd.setTexture(textureBkgd.getTexture());
	spriteBkgd.setTextureDimensions(textureBkgd.getTWidth(), textureBkgd.getTHeight());

	cTexture rocketTxt;
	rocketTxt.createTexture("Images\\SpaceShip.png");
	cRocket rocketSprite;
	rocketSprite.attachInputMgr(theInputMgr); // Attach the input manager to the sprite
	rocketSprite.setSpritePos(glm::vec2(windowWidth / 2, windowHeight - (rocketTxt.getTHeight()/1.5f)));
	rocketSprite.setTexture(rocketTxt.getTexture());
	rocketSprite.setTextureDimensions(rocketTxt.getTWidth(), rocketTxt.getTHeight());
	rocketSprite.setSpriteCentre();
	rocketSprite.setRocketVelocity(glm::vec2(0.0f, 0.0f));

	// Attach sound manager to rocket sprite
	rocketSprite.attachSoundMgr(theSoundMgr);


	vector<cTexture*> btnTextureList;
	LPCSTR btnTexturesToUse[] = { "Images/Buttons/exitBtn.png", "Images/Buttons/instructionsBtn.png", "Images/Buttons/loadBtn.png", "Images/Buttons/playBtn.png", "Images/Buttons/saveBtn.png", "Images/Buttons/settingsBtn.png" };
	for (int tCount = 0; tCount < 6; tCount++)
	{
		btnTextureList.push_back(new cTexture());
		btnTextureList[tCount]->createTexture(btnTexturesToUse[tCount]);
	}

	cButton saveButton;
	saveButton.attachInputMgr(theInputMgr);
	saveButton.setTexture(btnTextureList[4]->getTexture());
	saveButton.setTextureDimensions(btnTextureList[4]->getTWidth(), btnTextureList[4]->getTHeight());

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
			break;
		case END:
			break;
		case PLAYING:
			spriteBkgd.render();

			rocketSprite.update(elapsedTime);

			vector<cAsteroid*>::iterator asteroidIterator = theAsteroids.begin();
			while (asteroidIterator != theAsteroids.end())
			{
				if ((*asteroidIterator)->isActive() == false)
				{
					asteroidIterator = theAsteroids.erase(asteroidIterator);
				}
				else
				{
					(*asteroidIterator)->update(elapsedTime);
					(*asteroidIterator)->render();
					++asteroidIterator;
				}
			}


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
			theFontMgr->getFont("Welbut")->printText(strTime, FTPoint((windowWidth / 2) - (windowWidth / 10), -1.0f, 0.0f));
			theFontMgr->getFont("Welbut")->printText(strScore, FTPoint(0.0f, -1.0f, 0.0f));

			break;
		
		}
		
		pgmWNDMgr->swapBuffers();
		theInputMgr->clearBuffers(theInputMgr->KEYS_DOWN_BUFFER | theInputMgr->KEYS_PRESSED_BUFFER | theInputMgr->MOUSE_BUFFER);
    }

	theOGLWnd.shutdown(); //Free any resources
	pgmWNDMgr->destroyWND(); //Destroy the program window

    return 0; //Return success
}
