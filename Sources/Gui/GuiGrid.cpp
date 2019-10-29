#include <Gui/GuiGrid.hpp>

#include <SFML/Graphics/RenderTarget.hpp>

GuiGrid::GuiGrid(int width, int height, float cellSize) :
    m_grid(width / cellSize, height / cellSize),
    m_deadCell(sf::Vector2f(cellSize, cellSize)),
    m_aliveCell(sf::Vector2f(cellSize, cellSize)),
    m_width(width),
    m_height(height),
    m_cellSize(cellSize)
{
    m_deadCell.setFillColor(sf::Color(0x17, 0x2A, 0x45));
    m_aliveCell.setFillColor(sf::Color(0x64, 0xFF, 0xDA));
    m_transforms = std::vector<sf::Transform>(getCellsCount());

    for(int c = 0 ; c < getCellsPerLine() ; ++c) {
        for(int l = 0 ; l < getCellsPerColumn() ; ++l) {
            auto& transform = m_transforms[l * getCellsPerLine() + c];
            transform.translate(c * m_cellSize, l * m_cellSize + 72);
        }
    }
}

int GuiGrid::getWidth() const
{
    return m_width;
}

int GuiGrid::getHeight() const
{
    return m_height;
}

float GuiGrid::getCellSize() const
{
    return m_cellSize;
}

void GuiGrid::reset()
{
    m_grid = Grid(m_width / m_cellSize, m_height / m_cellSize);
}

void GuiGrid::setCellState(int line, int column, bool value)
{
    m_grid.setState(line, column, value);
}

void GuiGrid::nextGeneration()
{
    m_grid.nextGeneration();
}

long GuiGrid::getGeneration() const
{
    return m_grid.getGeneration();
}

int GuiGrid::getCellsCount() const
{
    return getCellsPerLine() * getCellsPerColumn();
}

int GuiGrid::getCellsPerLine() const
{
    return (m_width / m_cellSize);
}

int GuiGrid::getCellsPerColumn() const
{
    return (m_height / m_cellSize);
}

const sf::Transform& GuiGrid::getCellTransform(int line, int column) const
{
    return m_transforms[line * getCellsPerLine() + column];
}

void GuiGrid::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for(int c = 0 ; c < getCellsPerLine() ; ++c) {
        for(int l = 0 ; l < getCellsPerColumn() ; ++l) {
            if(m_grid.getState(l, c)) {
                target.draw(m_aliveCell, getCellTransform(l, c));
            } else {
                target.draw(m_deadCell, getCellTransform(l, c));
            }
        }
    }
}