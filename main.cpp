#include "graphics.h"
#include "util.h"
#include "global.h"
#include "filmui.h"
#include "film.h"

void update(float ms)
{
    FilmUI* filmUI{ FilmUI::getInstance() };
    filmUI->update();
    
}

void draw()
{
    FilmUI* filmUI{ FilmUI::getInstance() };
    filmUI->draw();
}

int main(int argc, char* argv) {

    graphics::createWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "FilmGUI");

    FilmUI* filmUI{ FilmUI::getInstance() };

    graphics::setDrawFunction(draw);
    graphics::setUpdateFunction(update);

    graphics::setCanvasSize(CANVAS_WIDTH, CANVAS_HEIGHT);
    graphics::setCanvasScaleMode(graphics::CANVAS_SCALE_FIT);

    filmUI->init();
    graphics::startMessageLoop();
    graphics::destroyWindow();
    
    FilmUI::releaseInstance();

    return 0;
}