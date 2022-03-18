#include <TGUI/Backend/SFML-Graphics.hpp>
#include <TGUI/TGUI.hpp>

#pragma once

namespace ted {

class Gui {
  private:
    sf::RenderWindow Window;
    tgui::Gui Interface;
    tgui::Theme WidgetsTheme;
    tgui::MenuBar::Ptr Menu;
    tgui::TextArea::Ptr TextArea;
    tgui::Label::Ptr FileName;

    void setupWindow();

  public:
    Gui();
    ~Gui() = default;
    void loop();
};
} // namespace ted