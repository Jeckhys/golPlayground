#ifndef HEADER_GRID_HPP
#define HEADER_GRID_HPP

#include <vector>

class Grid
{
    public:
        Grid(int width, int height);
        ~Grid() = default;

        /**
         * Compute the next generation internally.
         */
        void nextGeneration();

        void setState(int line, int column, bool value);

        bool getState(int line, int column) const;
        /**
         * Gets the current generation number.
         * @return The current generation
         */
        long getGeneration() const;
    private:
        bool isValid(int line, int column) const;
        bool getNextState(int line, int column) const;

        int getNeighborsAlive(int line, int column) const;

        std::vector<bool> getNeighbors(int line, int column) const;
    private:
        std::vector<bool> m_cells; ///< States of the current generation
        std::vector<bool> m_buffer; ///< States of the next generation

        int m_width; ///< Cells per line
        int m_height; ///< Cells per column

        long m_generation; ///< Generation count
};

#endif // HEADER_GRID_HPP