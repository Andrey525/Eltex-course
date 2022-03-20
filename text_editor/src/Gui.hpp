#include "Enums.hpp"
#include "File.hpp"
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
    tgui::Label::Ptr Name;

    tgui::ChildWindow::Ptr ChildWindow;
    tgui::Label::Ptr ChildLabel;
    tgui::EditBox::Ptr ChildEditBox;
    tgui::Button::Ptr ChildButton;

    // std::string FileName;

    void setupWindow();
    void setupMenuHandlers();

    void handler(const int operation);
    void initChildWindow();

  public:
    Gui();
    ~Gui() = default;
    void loop();
};

} // namespace ted