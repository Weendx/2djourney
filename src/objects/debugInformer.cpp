#include "objects/debugInformer.h"

DebugInformer::DebugInformer() : m_fps(0.0), m_updateTimer() {
	m_font = new sf::Font();
	if (!(*m_font).loadFromFile("resources/fonts/default.ttf"))
		throw std::runtime_error("Can't load default font");
	m_fpsCounter = new FpsCounter(*m_font);
}

DebugInformer::~DebugInformer() {
	delete m_fpsCounter;
	delete m_font;
}

void DebugInformer::onUpdate(const sf::Time& deltaTime) {
	if (m_updateTimer.getElapsedTime().asSeconds() < 0.5)
		return;
	m_fpsCounter->setFps(m_fps);
	m_updateTimer.restart();
}

void DebugInformer::handleEvent(sf::Event& event) {
	
}

void DebugInformer::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw((sf::Text) *m_fpsCounter, states);
}

void DebugInformer::setFPS(const float& fps) {
	m_fps = fps;
}