#ifndef HEADER_GUIGRID_HPP
#define HEADER_GUIGRID_HPP

#include <Core/Grid.hpp>

#include <SFML/Graphics/RectangleShape.hpp>

class GuiGrid : public sf::Drawable
{
    public:
        GuiGrid(int width, int height, float cellSize);
        ~GuiGrid() = default;

        int getWidth() const;
        int getHeight() const;
        float getCellSize() const;

        void reset();
        void setCellState(int line, int column, bool value);

        void nextGeneration();
        long getGeneration() const;
    private:
        int getCellsCount() const;
        int getCellsPerLine() const;
        int getCellsPerColumn() const;

        const sf::Transform& getCellTransform(int line, int column) const;

        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    private:
        Grid m_grid;

        sf::RectangleShape m_deadCell;
        sf::RectangleShape m_aliveCell;
        std::vector<sf::Transform> m_transforms;

        int m_width;
        int m_height;
        float m_cellSize;
};

#endif // HEADER_GUIGRID_HPP