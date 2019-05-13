#ifndef YUKI_HPP
#define YUKI_HPP

#include <GL/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <map>

//Forward declaration hell
class UI;
class GraphicsEngine;
class AssetManager;
class Scene;

/**
\file Yuki.hpp
\brief Header file for Yuki.cpp

\author Christopher Arausa
\version 0.1 Alpha
\date 4/20/2019

*/


/**
\class Yuki
\brief The top level class of the game engine. It is the direct IO link between the various components of the game engine.

It also manages adding, removing, and setting the the active scene.
*/

class Yuki {
	private:
		//Handles any initialization we need, such as testing OpenGL versions, loading resources, etc.
		void init();
		void checkOpenGL();

		bool DEBUG; ///< Boolean to flag whether we're debugging or not. Typically this enables more verbose printing.
		GLint opengl_major; ///< The OpenGL major version detected.
		GLint opengl_minor; ///< The OpenGL minor version detected.

		inline static const std::string VERSION = "0.1"; ///< Version number

		std::map<std::string, Scene*> scenes; ///< A map of the scenes registered to our game engine.
		std::string active_scene; ///< String containing the ID of our active scene.

	public:
		Yuki(bool debug=false);
		~Yuki();
		void run();
		void close();

		AssetManager *am;	///< Pointer to our Asset Manager
		GraphicsEngine *ge;	///< Pointer to our Graphics Engine
		UI *ui; ///< Pointer to our input class.

		//Scene management functions.
		void addScene(std::string s, Scene* scene);
		void setActiveScene(std::string s);
		void removeScene(std::string s);
		std::string program_title; ///< The title of our game.

		Scene* getActiveScene();
};

#endif
