#ifndef HEADER_GUIWINDOW_HPP
#define HEADER_GUIWINDOW_HPP

#include <Core/Window.hpp>
#include <Gui/GuiGrid.hpp>

#include <TGUI/TGUI.hpp>

constexpr int HEADER_OFFSET = 9;

class GuiWindow
{
    public:
        explicit GuiWindow(sf::VideoMode videoMode, const std::string& title, float cellSize);
        ~GuiWindow() = default;

        void run();

        void setFps(unsigned int fps);
        void setGenerationPerSecond(int generationPerSecond);
    private:
        bool isDrawingAvailable() const;

        void handleEvent();
        void handleUpdate();
        void handleDisplay();

        void initializeGui();

        void signalReset();
        void signalSlower(int n);
        void signalPlay();
        void signalFaster(int n);
        void signalGrid();
        void signalAbout();

        void setCellState(int mouseX, int mouseY, bool state);
    private:
        Window m_window;
        GuiGrid m_guiGrid;

        sf::Event m_event;
        sf::RenderWindow m_renderWindow;

        tgui::Gui m_gui;

        bool m_showGrid;
        bool m_leftButtonPressed;
        bool m_rightButtonPressed;

        sf::VertexArray m_topLine;
        sf::VertexArray m_bottomLine;
        std::vector<sf::VertexArray> m_grid;
};

#endif // HEADER_GUIWINDOW_HPP