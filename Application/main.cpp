#include <Gui/GuiWindow.hpp>

int main(int argc, char * argv[])
{
    GuiWindow golWindow({1600, 1280}, "golPlayground", 32.0f);
    golWindow.setFps(60);

    golWindow.run();

    return EXIT_SUCCESS;
}