#ifndef HEADER_WINDOW_HPP
#define HEADER_WINDOW_HPP

#include <Gui/GuiGrid.hpp>

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Window
{
    public:
        Window();
        ~Window() = default;

        void update();

        bool isUpdatable();
        bool isRunning() const;
        int  getGenerationPerSecond() const;

        void setRunning(bool running);
        void setGenerationPerSecond(int generationPerSecond);
    private:
        void updateThreshold();
    private:
        sf::Clock m_clock;

        double m_threshold;
        double m_currentMs;
        double m_accumulator;

        int m_generationPerSecond;

        bool m_running;
};

#endif // HEADER_WINDOW_HPP