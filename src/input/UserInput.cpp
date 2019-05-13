#include "UserInput.hpp"
#include "../Yuki.hpp"
#include "../graphics/GraphicsEngine.hpp"
#include "../graphics/Scene.hpp"

/**
\file UserInput.cpp
\brief Implementation of the UI class.

\author    Christopher Arausa
\version   0.1
\date      05/8/2019

*/


/**
\brief Constructor

Sets the default mouse sensitivity.
\param Yuki class pointer.
*/
UI::UI(Yuki *yu){
	yuki=yu;
	mouseDown=false;
	setMouseSensitivity(sf::Vector2f(0.001f, 0.001f));
}
/**
\brief Destructor

Default empty destructor. Nothing to cleanup
*/
UI::~UI(){

}
/**
\brief Sets the mouse down value.
\param md --- Boolean representing whether the mouse is currently up or down.
*/
void UI::setMouseDown(bool md){
	mouseDown = md;
}
/**
\brief Returns whether the mouse is currently down.
*/
bool UI::getMouseDown(){
	return mouseDown;
}
/**
\brief Sets the mouse sensitivity value.
\param sensitivity --- sf::Vector2f containing the new sensitivity value.
*/
void UI::setMouseSensitivity(sf::Vector2f sensitivity){
	mouse_sensitivity = sensitivity;
}
/**
\brief Returns the current mouse sensitivity
*/
sf::Vector2f UI::getMouseSensitivity(){
	return mouse_sensitivity;
}

/**
\brief Processes our input based on the currently registered global input functions and the scene.

Calls all stored key pressed, mouse moved, mouse button, and key state callback functions registered to the UI and the related functions in the scene.
\param scene --- The current scene to process input for. Typically the active scene.
*/
void UI::processInput(Scene *scene){
	sf::Event event;
	while(yuki->ge->pollEvent(event)){
		if(event.type == sf::Event::Closed){
			yuki->close();
		}
		if(event.type == sf::Event::KeyPressed){
			for(void (*cb)(sf::Event::KeyEvent event, Yuki *yu) : keyPressedEvents){
				cb(event.key, yuki);
			}
			if(scene!=nullptr)
				scene->keyPressedEventHandler(event.key);
		}
		else if(event.type == sf::Event::MouseMoved){
			for(void (*cb)(sf::Event::MouseMoveEvent event, Yuki *yu) : mouseMovedEvents)
				cb(event.mouseMove, yuki);
			if(scene!=nullptr)
				scene->mouseMoveEventHandler(event.mouseMove);
		}
		else if(event.type == sf::Event::MouseButtonPressed){
			if(event.mouseButton.button == sf::Mouse::Left)
				mouseDown = true;
			for(void (*cb)(sf::Event::MouseButtonEvent event, Yuki *yu) : mouseButtonEvents){
				cb(event.mouseButton, yuki);
			}
			if(scene!=nullptr)
				scene->mouseButtonEventHandler(event.mouseButton);
		}
		else if(event.type == sf::Event::Resized){
			yuki->ge->resize();
		}
		else if(event.type == sf::Event::MouseButtonReleased){
			if(event.mouseButton.button == sf::Mouse::Left)
				mouseDown = false;
		}
	}
	for(void (*cb)(Yuki *yu) : keyStateEvents){
		cb(yuki);
	}
	if(scene!=nullptr)
		scene->keyStateEventHandler();
}
/**
\brief Sets the last mouse position.
\param pos --- sf::Vector2i containing the position to record.
*/
void UI::setMousePos(sf::Vector2i pos){
	last_mouse_pos = pos;
}
/**
\brief Returns the last known mouse position set by setMousePos.
*/
sf::Vector2i UI::getLastMousePos(){
	return last_mouse_pos;
}
/**
\brief Adds an event to our list of callbacks for key pressed events.
\param callback --- Callback to store, which is void type and takes the parameters (sf::Event::KeyEvent , Yuki*)
*/
void UI::addKeyPressedEvent(void (*callback)(sf::Event::KeyEvent event, Yuki *yu)){
	keyPressedEvents.push_back(callback);
}
/**
\brief Adds an event to our list of callbacks for key state events.
\param callback --- Callback to store, which is void type and takes the parameter (Yuki*)
*/
void UI::addKeyStateEvent(void (*callback)(Yuki *yu)){
	keyStateEvents.push_back(callback);
}
/**
\brief Adds an event to our list of callbacks for mouse button events.
\param callback --- Callback to store, which is void type and takes the parameters (sf::Event::MouseButtonEvent, Yuki*)
*/
void UI::addMouseButtonEvent(void (*callback)(sf::Event::MouseButtonEvent event, Yuki *yu)){
	mouseButtonEvents.push_back(callback);
}
/**
\brief Adds an event to our list of callbacks for mouse move events.
\param callback --- Callback to store, which is void type and takes the parameters (sf::Event::MouseMoveEvent, Yuki*)
*/
void UI::addMouseMovedEvent(void (*callback)(sf::Event::MouseMoveEvent event, Yuki *yu)){
	mouseMovedEvents.push_back(callback);
}
