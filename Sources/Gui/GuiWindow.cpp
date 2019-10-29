#include <Gui/GuiWindow.hpp>

#include <iostream>
#include <fstream>

GuiWindow::GuiWindow(sf::VideoMode videoMode, const std::string& title, float cellSize) :
    m_guiGrid(videoMode.width, videoMode.height, cellSize),
    m_renderWindow({videoMode.width, videoMode.height + 123}, title, sf::Style::Titlebar | sf::Style::Close),
    m_gui(m_renderWindow),
    m_showGrid(true),
    m_leftButtonPressed(false),
    m_rightButtonPressed(false),
    m_topLine(sf::Lines, 2),
    m_bottomLine(sf::Lines, 2)
{
    initializeGui();
}

void GuiWindow::run()
{
    while(m_renderWindow.isOpen()) {
        this->handleEvent();
        this->handleUpdate();
        this->handleDisplay();
    }
}

void GuiWindow::setFps(unsigned int fps)
{
    m_renderWindow.setFramerateLimit(fps);
}

void GuiWindow::setGenerationPerSecond(int generationPerSecond)
{
    m_window.setGenerationPerSecond(generationPerSecond);
}

bool GuiWindow::isDrawingAvailable() const
{
    bool available = true;

    available &= !m_window.isRunning();
    available &= !m_gui.get<tgui::ChildWindow>("windowAbout")->isVisible();

    return available;
}

void GuiWindow::handleEvent()
{
    while(m_renderWindow.pollEvent(m_event)) {
        if(m_event.type == sf::Event::Closed) {
            m_renderWindow.close();
        } else if(m_event.type == sf::Event::MouseButtonPressed) {
            if(this->isDrawingAvailable()) {
                m_leftButtonPressed = (m_event.mouseButton.button == sf::Mouse::Button::Left);
                m_rightButtonPressed = (m_event.mouseButton.button == sf::Mouse::Button::Right);

                if(m_leftButtonPressed) {
                    setCellState(m_event.mouseButton.x, m_event.mouseButton.y, true);
                }
                if(m_rightButtonPressed) {
                    setCellState(m_event.mouseButton.x, m_event.mouseButton.y, false);
                }
            }
        } else if(m_event.type == sf::Event::MouseButtonReleased) {
            m_leftButtonPressed = false;
            m_rightButtonPressed = false;
        } else if(m_event.type == sf::Event::MouseMoved) {
            if(this->isDrawingAvailable()) {
                if(m_leftButtonPressed) {
                    setCellState(m_event.mouseMove.x, m_event.mouseMove.y, true);
                }
                if(m_rightButtonPressed) {
                    setCellState(m_event.mouseMove.x, m_event.mouseMove.y, false);
                }
            }
        }

        m_gui.handleEvent(m_event);
    }
}

void GuiWindow::handleUpdate()
{
    m_window.update();

    while(m_window.isUpdatable()) {
        m_guiGrid.nextGeneration();
    }

    m_gui.get<tgui::Label>("labelGenerations")->setText(
        "- " + std::to_string(m_guiGrid.getGeneration()) + " generations"
    );
}

void GuiWindow::handleDisplay()
{
    m_renderWindow.clear(sf::Color(0x0A, 0x19, 0x2F));

    m_renderWindow.draw(m_guiGrid);
    m_renderWindow.draw(m_topLine);
    m_renderWindow.draw(m_bottomLine);

    if(m_showGrid) {
        for(auto& line : m_grid) {
            m_renderWindow.draw(line);
        }
    }

    m_gui.draw();

    m_renderWindow.display();
}

void GuiWindow::initializeGui()
{
    m_topLine[0].position = sf::Vector2f(0.0f, 72.0f);
    m_topLine[1].position = sf::Vector2f(m_renderWindow.getSize().x, 72.0f);
    m_bottomLine[0].position = sf::Vector2f(0.0f, m_guiGrid.getHeight() + 72.0f);
    m_bottomLine[1].position = sf::Vector2f(m_renderWindow.getSize().x, m_guiGrid.getHeight() + 72.0f);

    m_topLine[0].color = sf::Color(0x0A, 0x19, 0x2F);
    m_topLine[1].color = sf::Color(0x0A, 0x19, 0x2F);
    m_bottomLine[0].color = sf::Color(0x0A, 0x19, 0x2F);
    m_bottomLine[1].color = sf::Color(0x0A, 0x19, 0x2F);

    int cellsPerLine = static_cast<int>(m_guiGrid.getWidth() / m_guiGrid.getCellSize());
    int cellsPerColumn = static_cast<int>(m_guiGrid.getHeight() / m_guiGrid.getCellSize());

    for(int i = 0 ; i < cellsPerLine - 1 ; ++i) {
        auto line = sf::VertexArray(sf::Lines, 2);
        line[0].position = sf::Vector2f((i + 1) * m_guiGrid.getCellSize(), 73.0f);
        line[1].position = sf::Vector2f((i + 1) * m_guiGrid.getCellSize(), 73.0f + m_guiGrid.getHeight() - 2.0f);
        line[0].color = sf::Color(0x0A, 0x19, 0x2F);
        line[1].color = sf::Color(0x0A, 0x19, 0x2F);

        m_grid.push_back(line);
    }

    for(int i = 0 ; i < cellsPerColumn - 1 ; ++i) {
        auto line = sf::VertexArray(sf::Lines, 2);
        line[0].position = sf::Vector2f(0.0f, 73.0f + (i + 1) * m_guiGrid.getCellSize());
        line[1].position = sf::Vector2f(m_guiGrid.getWidth() - 1.0f, 73.0f + (i + 1) * m_guiGrid.getCellSize());
        line[0].color = sf::Color(0x0A, 0x19, 0x2F);
        line[1].color = sf::Color(0x0A, 0x19, 0x2F);

        m_grid.push_back(line);
    }

    tgui::Theme theme;
    theme.load("Themes/default.txt");
    tgui::Theme::setDefault(&theme);

    m_gui.add(tgui::Button::create("Reset"), "buttonReset");
    m_gui.get<tgui::Button>("buttonReset")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonReset")->setPosition(HEADER_OFFSET, HEADER_OFFSET);
    m_gui.get<tgui::Button>("buttonReset")->connect(
        "pressed", &GuiWindow::signalReset, this
    );

    m_gui.add(tgui::Button::create("Play"), "buttonPlay");
    m_gui.get<tgui::Button>("buttonPlay")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonPlay")->setPosition(
        (m_renderWindow.getSize().x - m_gui.get<tgui::Button>("buttonPlay")->getSize().x) / 2.0f,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonPlay")->connect(
        "pressed", &GuiWindow::signalPlay, this
    );

    m_gui.add(tgui::Button::create(">>"), "buttonFaster");
    m_gui.get<tgui::Button>("buttonFaster")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonFaster")->setPosition(
        (m_gui.get<tgui::Button>("buttonPlay")->getPosition().x + m_gui.get<tgui::Button>("buttonPlay")->getSize().x) + HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonFaster")->connect(
        "pressed", &GuiWindow::signalFaster, this, 10
    );

    m_gui.add(tgui::Button::create(">"), "buttonMore");
    m_gui.get<tgui::Button>("buttonMore")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonMore")->setPosition(
        (m_gui.get<tgui::Button>("buttonFaster")->getPosition().x + m_gui.get<tgui::Button>("buttonFaster")->getSize().x) + HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonMore")->connect(
        "pressed", &GuiWindow::signalFaster, this, 1
    );

    m_gui.add(tgui::Button::create("<<"), "buttonSlower");
    m_gui.get<tgui::Button>("buttonSlower")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonSlower")->setPosition(
        (m_gui.get<tgui::Button>("buttonPlay")->getPosition().x - m_gui.get<tgui::Button>("buttonSlower")->getSize().x) - HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonSlower")->connect(
        "pressed", &GuiWindow::signalSlower, this, 10
    );

    m_gui.add(tgui::Button::create("<"), "buttonLess");
    m_gui.get<tgui::Button>("buttonLess")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonLess")->setPosition(
        (m_gui.get<tgui::Button>("buttonSlower")->getPosition().x - m_gui.get<tgui::Button>("buttonLess")->getSize().x) - HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonLess")->connect(
        "pressed", &GuiWindow::signalSlower, this, 1
    );

    m_gui.add(tgui::Button::create("About"), "buttonAbout");
    m_gui.get<tgui::Button>("buttonAbout")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonAbout")->setPosition(
        (m_renderWindow.getSize().x - m_gui.get<tgui::Button>("buttonAbout")->getSize().x) - HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonAbout")->connect(
        "pressed", &GuiWindow::signalAbout, this
    );

    m_gui.add(tgui::Button::create("Grid"), "buttonGrid");
    m_gui.get<tgui::Button>("buttonGrid")->setTextSize(32);
    m_gui.get<tgui::Button>("buttonGrid")->setPosition(
        (m_gui.get<tgui::Button>("buttonAbout")->getPosition().x - m_gui.get<tgui::Button>("buttonGrid")->getSize().x) - HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonGrid")->connect(
        "pressed", &GuiWindow::signalGrid, this
    );

    auto buttonAboutClose = tgui::Button::create("OK");
    buttonAboutClose->setTextSize(32);

    auto labelAbout = tgui::Label::create("Created by Matthieu LOCUSSOL, based on Conway's game of life.");
    labelAbout->setTextSize(24);
    labelAbout->setPosition(HEADER_OFFSET * 2, HEADER_OFFSET * 2);
    labelAbout->setSize(600, 100);

    auto labelWebsite = tgui::Label::create("www.matthieu-locussol.com");
    labelWebsite->setTextSize(24);
    labelWebsite->getRenderer()->setTextColor(sf::Color(0x64, 0xFF, 0xDA));

    m_gui.add(tgui::ChildWindow::create(), "windowAbout");
    m_gui.get<tgui::ChildWindow>("windowAbout")->setSize(640, 256);
    m_gui.get<tgui::ChildWindow>("windowAbout")->setPosition(
        (m_renderWindow.getSize().x - m_gui.get<tgui::ChildWindow>("windowAbout")->getSize().x) / 2.0f,
        (m_renderWindow.getSize().y - m_gui.get<tgui::ChildWindow>("windowAbout")->getSize().y) / 2.0f
    );
    m_gui.get<tgui::ChildWindow>("windowAbout")->setTitle("About");
    m_gui.get<tgui::ChildWindow>("windowAbout")->setVisible(false);
    m_gui.get<tgui::ChildWindow>("windowAbout")->setTitleButtons(0);
    m_gui.get<tgui::ChildWindow>("windowAbout")->setTitleTextSize(32);
    m_gui.get<tgui::ChildWindow>("windowAbout")->getRenderer()->setTitleBarHeight(56);
    m_gui.get<tgui::ChildWindow>("windowAbout")->add(buttonAboutClose, "buttonAboutClose");
    m_gui.get<tgui::ChildWindow>("windowAbout")->add(labelAbout);
    m_gui.get<tgui::ChildWindow>("windowAbout")->add(labelWebsite);
    buttonAboutClose->connect("pressed", [&]{ m_gui.get<tgui::ChildWindow>("windowAbout")->setVisible(false); });

    buttonAboutClose->setPosition(
        (m_gui.get<tgui::ChildWindow>("windowAbout")->getSize().x - buttonAboutClose->getSize().x) / 2.0f,
        m_gui.get<tgui::ChildWindow>("windowAbout")->getSize().y - buttonAboutClose->getSize().y - HEADER_OFFSET * 2
    );
    labelWebsite->setPosition(
        (m_gui.get<tgui::ChildWindow>("windowAbout")->getSize().x - labelWebsite->getSize().x) / 2.0f,
        HEADER_OFFSET * 2 + labelAbout->getSize().y
    );

    m_gui.add(tgui::Label::create(
        std::to_string(m_window.getGenerationPerSecond()) + " generation(s) per second "),
        "labelGenPerSecond"
    );
    m_gui.get<tgui::Label>("labelGenPerSecond")->setTextSize(24);
    m_gui.get<tgui::Label>("labelGenPerSecond")->setPosition(
        HEADER_OFFSET,
        m_guiGrid.getHeight() + 74.0f + HEADER_OFFSET
    );

    m_gui.add(tgui::Label::create(
        "- " + std::to_string(m_guiGrid.getGeneration()) + " generations"),
        "labelGenerations"
    );
    m_gui.get<tgui::Label>("labelGenerations")->setTextSize(24);
    m_gui.get<tgui::Label>("labelGenerations")->setPosition(
        HEADER_OFFSET + m_gui.get<tgui::Label>("labelGenPerSecond")->getSize().x,
        m_guiGrid.getHeight() + 74.0f + HEADER_OFFSET
    );

    m_gui.add(tgui::Label::create("golPlayground by Matthieu LOCUSSOL"), "labelAuthor");
    m_gui.get<tgui::Label>("labelAuthor")->setTextSize(24);
    m_gui.get<tgui::Label>("labelAuthor")->setPosition(
        m_guiGrid.getWidth() - m_gui.get<tgui::Label>("labelAuthor")->getSize().x - HEADER_OFFSET,
        m_guiGrid.getHeight() + 74.0f + HEADER_OFFSET
    );
}

void GuiWindow::signalReset()
{
    if(m_window.isRunning()) {
        this->signalPlay();
    }

    m_guiGrid.reset();
}

void GuiWindow::signalSlower(int n)
{
    m_window.setGenerationPerSecond(
        m_window.getGenerationPerSecond() - n
    );

    m_gui.get<tgui::Label>("labelGenPerSecond")->setText(std::to_string(m_window.getGenerationPerSecond()) + " generation per second ");
    m_gui.get<tgui::Label>("labelGenerations")->setPosition(
        HEADER_OFFSET + m_gui.get<tgui::Label>("labelGenPerSecond")->getSize().x,
        m_guiGrid.getHeight() + 74.0f + HEADER_OFFSET
    );
}

void GuiWindow::signalPlay()
{
    if(m_window.isRunning()) {
        m_window.setRunning(false);
        m_gui.get<tgui::Button>("buttonPlay")->setText("Play");
    } else {
        m_window.setRunning(true);
        m_gui.get<tgui::Button>("buttonPlay")->setText("Pause");
    }

    m_gui.get<tgui::Button>("buttonFaster")->setPosition(
        (m_gui.get<tgui::Button>("buttonPlay")->getPosition().x + m_gui.get<tgui::Button>("buttonPlay")->getSize().x) + HEADER_OFFSET,
        HEADER_OFFSET
    );
    m_gui.get<tgui::Button>("buttonMore")->setPosition(
        (m_gui.get<tgui::Button>("buttonFaster")->getPosition().x + m_gui.get<tgui::Button>("buttonFaster")->getSize().x) + HEADER_OFFSET,
        HEADER_OFFSET
    );
}

void GuiWindow::signalFaster(int n)
{
    m_window.setGenerationPerSecond(
        m_window.getGenerationPerSecond() + n
    );

    m_gui.get<tgui::Label>("labelGenPerSecond")->setText(std::to_string(m_window.getGenerationPerSecond()) + " generation(s) per second ");
    m_gui.get<tgui::Label>("labelGenerations")->setPosition(
        HEADER_OFFSET + m_gui.get<tgui::Label>("labelGenPerSecond")->getSize().x,
        m_guiGrid.getHeight() + 74.0f + HEADER_OFFSET
    );
}

void GuiWindow::signalGrid()
{
    m_showGrid = !m_showGrid;
}

void GuiWindow::signalAbout()
{
    m_gui.get<tgui::ChildWindow>("windowAbout")->setVisible(true);
}

void GuiWindow::setCellState(int mouseX, int mouseY, bool state)
{
    if(mouseY <= 73 || mouseY > (m_guiGrid.getHeight() + 73)) {
        return;
    }

    mouseY = mouseY - 73;

    float cellSize = m_guiGrid.getCellSize();
    int x = static_cast<int>(mouseX / cellSize);
    int y = static_cast<int>(mouseY / cellSize);

    m_guiGrid.setCellState(y, x, state);
}