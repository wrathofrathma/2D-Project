#ifndef USER_INPUT_HPP
#define USER_INPUT_HPP
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <thread>

class Yuki;
class Scene;
/**
\file UserInput.hpp
\brief Header file for UserInput.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 4/20/2019

*/


/**
\class UI
\brief The UI class handles user input.
*/

class UI {
	protected:
		sf::Vector2i last_position; ///< Last position of the mouse on screen.
		Yuki* yuki; ///< Our link back up to the rest of our resources.

		std::vector<void (*)(sf::Event::KeyEvent event, Yuki *yu)> keyPressedEvents; ///< Vector of registered global keybinding functions.
		std::vector<void (*)(Yuki *yu)> keyStateEvents; ///< Vector of registered global keystate functions.
		std::vector<void (*)(sf::Event::MouseButtonEvent event, Yuki *yu)> mouseButtonEvents; ///< Vector of registered global mouse button functions.
		std::vector<void (*)(sf::Event::MouseMoveEvent event, Yuki *yu)> mouseMovedEvents; ///< Vector of registered global move event functions.
		sf::Vector2i last_mouse_pos; ///< The last recorded mouse position.
		bool mouseDown; ///< Boolean for whether the mouse is currently down or not.
		sf::Vector2f mouse_sensitivity; ///< Variable for our current mouse sensitivity.
	public:
		UI(Yuki *yu);
		~UI();

		void processInput(Scene *scene);

		//Adding things to our input-event loop check.
		void addKeyPressedEvent(void (*callback)(sf::Event::KeyEvent event, Yuki *yu));
		void addKeyStateEvent(void (*callback)(Yuki *yu));
		void addMouseButtonEvent(void (*callback)(sf::Event::MouseButtonEvent event, Yuki *yu));
		void addMouseMovedEvent(void (*callback)(sf::Event::MouseMoveEvent event, Yuki *yu));
		void setMouseDown(bool);
		bool getMouseDown();
		sf::Vector2i getLastMousePos();
		void setMousePos(sf::Vector2i pos);
		void setMouseSensitivity(sf::Vector2f sensitivity);
		sf::Vector2f getMouseSensitivity();
};

#endif
