#include "Gui.hpp"

namespace ted {

Gui::Gui() { setupWindow(); }

void Gui::setupWindow() {
    Window.create(sf::VideoMode(800, 600), "Login",
                  sf::Style::Titlebar | sf::Style::Close);
    Window.setFramerateLimit(60);

    Interface.setWindow(Window);

    WidgetsTheme.load("./themes/Black.txt");

    Menu = tgui::MenuBar::create();
    Menu->setRenderer(WidgetsTheme.getRenderer("MenuBar"));
    Menu->setHeight(25.f);
    Menu->addMenu("File");
    Menu->addMenuItem("Load");
    Menu->addMenuItem("Save");
    Menu->addMenuItem("Exit");
    Menu->addMenu("Edit");
    Menu->addMenuItem("Copy");
    Menu->addMenuItem("Cut");
    Menu->addMenuItem("Paste");
    Interface.add(Menu);

    TextArea = tgui::TextArea::create();
    TextArea->setRenderer(WidgetsTheme.getRenderer("TextArea"));
    TextArea->setSize(800, 575);
    TextArea->setPosition(0, 25);
    TextArea->setTextSize(18);
    Interface.add(TextArea);
}

void Gui::loop() {
    while (Window.isOpen()) {
        sf::Event event;
        while (Window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                Window.close();
            }
            Interface.handleEvent(event);
        }
        Window.clear();
        Interface.draw();
        Window.display();
    }
}

} // namespace ted