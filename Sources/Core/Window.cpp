#include <Core/Window.hpp>

constexpr int MS_PER_S = 1000;

Window::Window() :
    m_currentMs(0.0),
    m_accumulator(0.0),
    m_generationPerSecond(30),
    m_running(false)
{
    updateThreshold();
}

void Window::update()
{
    if(m_running) {
        double newMs = m_clock.getElapsedTime().asMilliseconds();
        double frameMs = newMs - m_currentMs;

        m_currentMs = newMs;
        m_accumulator += frameMs;
    }
}

bool Window::isUpdatable()
{
    if(m_accumulator >= m_threshold) {
        m_accumulator -= m_threshold;
        return true;
    }

    return false;
}

bool Window::isRunning() const
{
    return m_running;
}

int  Window::getGenerationPerSecond() const
{
    return m_generationPerSecond;
}

void Window::setRunning(bool running)
{
    m_running = running;

    m_currentMs = 0.0;
    m_accumulator = 0.0;

    m_clock.restart();
}

void Window::setGenerationPerSecond(int generationPerSecond)
{
    m_generationPerSecond = std::max(1, generationPerSecond);
    updateThreshold();
}

void Window::updateThreshold()
{
    m_threshold = 1.0f / m_generationPerSecond * MS_PER_S;
}