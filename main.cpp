#include <iostream>
#include "src/Yuki.hpp"
#include "src/graphics/Scene.hpp"
#include "src/Graphics.hpp"
#include "src/UserInput.hpp"
#include <random>
#include "src/scenes/Project.hpp"
using namespace std;


void moveEvent(sf::Event::MouseMoveEvent event, Yuki *yu){
	UI *ui = yu->ui;
	sf::Vector2i last_pos = ui->getLastMousePos();
	sf::Vector2f mouse_sensitivity = ui->getMouseSensitivity();
	if(ui->getMouseDown()){
		float delta_x = event.x - last_pos.x;
		float delta_y = event.y - last_pos.y;
		float x = delta_x * mouse_sensitivity.x;
		float y = delta_y * mouse_sensitivity.y;
	}
	ui->setMousePos(sf::Vector2i(event.x, event.y));
}

void keyPressedEvent(sf::Event::KeyEvent event, Yuki *yu){
	switch(event.code){
		case sf::Keyboard::Escape:
			yu->close();
			break;
		case sf::Keyboard::M:
			yu->ge->toggleWireframe();
			break;
		case sf::Keyboard::F10:
			yu->ge->screenshot();
			break;
	  default:
			break;
	};
}

void stateProcessing(Yuki *yu){
	float m = 2; //Move sensitivity.

	Scene* scene = yu->getActiveScene();
	Camera* camera = scene->getCamera();


}



int main(){
	Yuki yuki;
	yuki.ui->addKeyPressedEvent(keyPressedEvent);
	yuki.ui->addMouseMovedEvent(moveEvent);
	yuki.ui->addKeyStateEvent(stateProcessing);

	yuki.addScene("2d", new ProjectScene(&yuki));
	yuki.setActiveScene("2d");
	yuki.run();
	return 0;
}
