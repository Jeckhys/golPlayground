#include <SFML/System.hpp>

#include <Core/Grid.hpp>

Grid::Grid(int width, int height) :
    m_cells(width * height, false),
    m_buffer(width * height, false),
    m_width(width),
    m_height(height),
    m_generation(0)
{

}

void Grid::nextGeneration()
{
    for(int c = 0 ; c < m_width ; ++c) {
        for(int l = 0 ; l < m_height ; ++l) {
            m_buffer[l * m_width + c] = getNextState(l, c);
        }
    }

    std::swap(m_cells, m_buffer);

    ++m_generation;
}

void Grid::setState(int line, int column, bool value)
{
    m_cells[line * m_width + column] = value;
}

bool Grid::getState(int line, int column) const
{
    if(!isValid(line, column)) {
        return false;
    }

    return m_cells[line * m_width + column];
}

long Grid::getGeneration() const
{
    return m_generation;
}

bool Grid::isValid(int line, int column) const
{
    return (0 <= line)   &&  (line < m_height) &&
           (0 <= column) && (column < m_width);
}

bool Grid::getNextState(int line, int column) const
{
    auto neighbors = getNeighborsAlive(line, column);
    auto alive = m_cells[line * m_width + column];

    return (neighbors == 3) || (alive && neighbors == 2);
}

int Grid::getNeighborsAlive(int line, int column) const
{
    std::vector<bool> neighbors = getNeighbors(line, column);
    int count = std::count(neighbors.begin(), neighbors.end(), true);

    return count;
}

std::vector<bool> Grid::getNeighbors(int line, int column) const
{
    std::vector<bool> n(8, false);

    n.push_back(getState(line - 1, column - 1));
    n.push_back(getState(line - 1, column));
    n.push_back(getState(line - 1, column + 1));
    n.push_back(getState(line, column - 1));
    n.push_back(getState(line, column + 1));
    n.push_back(getState(line + 1, column - 1));
    n.push_back(getState(line + 1, column));
    n.push_back(getState(line + 1, column + 1));

    return n;
}