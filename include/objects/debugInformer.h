#pragma once

#include <SFML/System/Clock.hpp>
#include "object.h"
#include "objects/fpsCounter.h"

class DebugInformer : public Object {
 public:
	DebugInformer();
	~DebugInformer();
	void onUpdate(const sf::Time& deltaTime) override;
	void handleEvent(sf::Event& event) override;
	void adjustScale(const sf::Vector2f& factors) {}
	operator std::string() const { return "DebugInformer"; }
	void setFPS(const float& fps);
 private:
	 void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	 sf::Font* m_font;
	 sf::Clock m_updateTimer;
	 FpsCounter* m_fpsCounter;
	 float m_fps;
};