#include "Yuki.hpp"
#include "UserInput.hpp"
#include "graphics/GraphicsEngine.hpp"
#include "graphics/Scene.hpp"
#include "AssetManager.hpp"
/**
\file Yuki.cpp
\brief Implementation of the top level game engine class Yuki.

\author Christopher Arausa
\version 0.1 Alpha
\date 4/20/2019

*/

/**
\brief Constructor

Sets the program title, debug values, and initializes things to null.
\param debug -- boolean used to set whether the program will have verbose output.
*/
Yuki::Yuki(bool debug){
	DEBUG = debug;
	program_title = "Yuki ";
	program_title+=VERSION;
	std::cout << "Version: " << program_title << std::endl;
	ge = nullptr;
	am = nullptr;
	ui = nullptr;
	init();
}

/**
\brief Destructor
Deletes all scenes and frees the memory allocated to them.
Also deletes the UI, Graphics Engine, and Asset Manager.
*/
Yuki::~Yuki(){
	//Cleanup our scenes.
	while(scenes.size()>0){
		std::map<std::string, Scene*>::iterator it = scenes.begin();
		if(it->second!=nullptr){
			delete it->second;
			it->second = nullptr;
		}
		scenes.erase(it);
	}

	if(ui!=nullptr)
		delete ui;
	if(ge!=nullptr)
		delete ge;
	if(am!=nullptr)
		delete am;
}

/**
\brief Closes the window and terminates the program.
*/
void Yuki::close(){
	ge->close();
}
/**
\brief Checks the OpenGL version on the computer to verify we can run on it.
*/
void Yuki::checkOpenGL(){
	//Straight up yoinked most of this from my graphics professor's code. It seems useful.
	GLint MinMajor = 3;
	GLint MinMinor = 3;
	GLint WindowWidth = 800;
	GLint WindowHeight = 600;

	//Make a test window requesting OpenGL version 10.10 and seeing what it falls back to.
	sf::RenderWindow window(sf::VideoMode(WindowWidth, WindowHeight), "OpenGL Setup", sf::Style::Default, sf::ContextSettings(24, 8, 4, 10, 10, sf::ContextSettings::Core));
	window.setVisible(false);

	if(glewInit()){
		std::cerr << "Unable to initialize GLEW ... exiting." << std::endl;
		exit(-1);
	}

	glGetIntegerv(GL_MAJOR_VERSION, &opengl_major);
	glGetIntegerv(GL_MINOR_VERSION, &opengl_minor);

	bool versionOkay = true;
	if(opengl_major < MinMajor)
		versionOkay = false;
	else if (opengl_minor < MinMinor)
		versionOkay = false;

	if(!versionOkay){
        std::cerr << "Graphics card OpenGL version is " << opengl_major << "." << opengl_minor << std::endl;
        std::cerr << "Program required OpenGL version is " << MinMajor << "." << MinMinor << std::endl;
        std::cerr << "Exiting" << std::endl;;
        exit(-1);
    }

    if (DEBUG){
        std::cout << std::endl;
        std::cout << "Version  = " << glGetString(GL_VERSION) << std::endl;
        std::cout << "Major    = " << opengl_major << std::endl;
        std::cout << "Minor    = " << opengl_minor << std::endl;
        std::cout << "Vendor   = " << glGetString(GL_VENDOR) << std::endl;
        std::cout << "Renderer = " << glGetString(GL_RENDERER) << std::endl;
        std::cout << std::endl;
    }
    window.close();
}

/**
\brief Allocates resources and initializes other components of the game engine.

Initializes the Graphics Engine, Asset Manager, and UI.
*/
void Yuki::init(){
	//First check to see if our computer supports our OpenGL version
	checkOpenGL();
	//If everything didn't explode by now, we can load our assets
	am = new AssetManager(this);
	//If things load decently, we can then create our window
	ge = new GraphicsEngine(this, program_title, opengl_major, opengl_minor);

	//Once that's all nice and finished, let's start our primary event loop.
	ui = new UI(this);

}

/**
\brief Main game engine event loop.

Calls the active scene's tick() function to update game logic, and processes input before calling the graphics engine to render everything.
*/
void Yuki::run(){
	while(ge->isOpen()){
		Scene *s = getActiveScene();
		s->tick();
		if(ui!=nullptr)
			ui->processInput(s);
		ge->display(s);
	}
}
/**
\brief Adds a scene to our map of scenes.
\param id --- The ID of the scene to use as a key in the scene map.
\param scene --- A pointer to the scene to add.
*/
void Yuki::addScene(std::string id, Scene* scene){
	if(scenes.count(id)>0){
		std::cerr << "Scene by this ID already exists. Overwriting old scene." << std::endl;
		removeScene(id);
	}
	scenes.insert(std::pair<std::string, Scene*>(id,scene));
}
/**
\brief Sets the active scene by ID.
\param id --- The ID to look up in our scene map.
*/
void Yuki::setActiveScene(std::string id){
	if(scenes.size()==0){
		std::cerr << "Can't set active scene, no registered scenes." << std::endl;
		return;
	}
	if(scenes.count(id)==0){
		std::cerr << "Can't set active scene. Scene ID not found." << std::endl;
		return;
	}
	active_scene = id;
}

/**
\brief Removes the scene id from our scene map.
\param id --- ID of the scene to remove.
*/
void Yuki::removeScene(std::string id){
	if(scenes.size()==0)
		return;
	if(scenes.count(id)==0)
		return;
	//The actual deletion
	if(scenes[id]!=nullptr){
		delete scenes[id];
		scenes[id] = nullptr;
	}
	scenes.erase(id);
	//If our deleted scene was our primary scene, then we should pick the scene at the bottom of the map. If one doesn't exist then we just set to "".
	if(active_scene.compare(id)==0){
		if(scenes.size()>0)
			active_scene = scenes.begin()->first;
		else
			active_scene.clear();
	}
}
/**
\brief Returns the current active scene.

Returns nullptr if the scene isn't found.
*/
using namespace std;
Scene* Yuki::getActiveScene(){
	if(scenes.size()==0){
		std::cerr << "Can't retrieve active scene, no registered scenes." << std::endl;
		return nullptr;
	}
	if(active_scene.size()==0){
		std::cerr << "Can't retrieve active scene. Active scene ID not set." << std::endl;
		return nullptr;
	}
	if(scenes.count(active_scene)==0){
		std::cerr << "Can't retrieve active scene. Scene ID not found." << std::endl;
		return nullptr;
	}
	return scenes[active_scene];
}
