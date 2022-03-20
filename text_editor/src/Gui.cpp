#include "Gui.hpp"

namespace ted {

Gui::Gui() { setupWindow(); }

void Gui::setupWindow() {
    Window.create(sf::VideoMode(800, 600), "Text Editor",
                  sf::Style::Titlebar | sf::Style::Close);
    Window.setFramerateLimit(60);

    Interface.setWindow(Window);

    WidgetsTheme.load("./themes/Black.txt");

    Menu = tgui::MenuBar::create();
    Menu->setRenderer(WidgetsTheme.getRenderer("MenuBar"));
    Menu->setHeight(25.f);
    Menu->addMenu("File");
    Menu->addMenuItem("File", "Create");
    Menu->addMenuItem("File", "Open");
    Menu->addMenuItem("File", "Save");
    Menu->addMenuItem("File", "Close");
    Menu->addMenuItem("File", "Exit");
    Menu->addMenu("Edit");
    Menu->addMenuItem("Edit", "Copy");
    Menu->addMenuItem("Edit", "Cut");
    Menu->addMenuItem("Edit", "Paste");
    Interface.add(Menu);

    Name = tgui::Label::create();
    Name->setRenderer(WidgetsTheme.getRenderer("Label"));
    Name->setTextSize(12);
    Name->setText("Nameless");
    Name->setPosition(370, 5);
    Interface.add(Name);

    TextArea = tgui::TextArea::create();
    TextArea->setRenderer(WidgetsTheme.getRenderer("TextArea"));
    TextArea->setSize(800, 575);
    TextArea->setPosition(0, 25);
    TextArea->setTextSize(18);
    Interface.add(TextArea);

    setupMenuHandlers();
}

void Gui::setupMenuHandlers() {
    Menu->connectMenuItem("File", "Create", [&]() { handler(ted::CREATE); });
    Menu->connectMenuItem("File", "Open", [&]() { handler(ted::OPEN); });
    Menu->connectMenuItem("File", "Save", [&]() { handler(ted::SAVE); });
    Menu->connectMenuItem("File", "Close", [&]() { handler(ted::CLOSE); });
    Menu->connectMenuItem("File", "Exit", [&]() { handler(ted::EXIT); });

    // Menu->connectMenuItem("Edit", "Copy",
    //                       []() { std::cout << "Good" << '\n'; });
    // Menu->connectMenuItem("Edit", "Cut", []() { std::cout << "Good" << '\n';
    // }); Menu->connectMenuItem("Edit", "Paste",
    //                       []() { std::cout << "Good" << '\n'; });
}

void Gui::handler(const int operation) {
    int status;
    if (operation == ted::CREATE) {
        Menu->setEnabled(false);
        initChildWindow();
        ChildButton->onClick([this, &status]() {
            std::string filename =
                static_cast<std::string>(ChildEditBox->getText());
            if (filename.length() > 0 && filename != "Nameless") {
                status = ted::File::createFile(filename);
                if (status == ted::CREATE_SUCCESS) {
                    ChildWindow->close();
                    Name->setText(filename);
                    TextArea->setText("");
                } else if (status == ted::FILE_EXISTS) {
                    ChildLabel->setText("File exists");
                } else {
                    ChildLabel->setText("Unknown dir");
                }
            }
        });
    } else if (operation == ted::OPEN) {
        Menu->setEnabled(false);
        initChildWindow();
        ChildButton->onClick([this, &status]() {
            std::string filename =
                static_cast<std::string>(ChildEditBox->getText());
            if (filename.length() > 0) {
                status = ted::File::openFile(filename, "r");
                if (status == ted::OPEN_SUCCESS) {
                    std::string text;
                    status = ted::File::readFile(text);
                    if (status == ted::READ_SUCCESS) {
                        Name->setText(filename);
                        ChildWindow->close();
                        TextArea->setText(static_cast<tgui::String>(text));
                    } else {
                        ChildLabel->setText("Error read");
                    }
                } else if (status == ted::FILE_DOES_NOT_EXIST) {
                    ChildLabel->setText("File does not exist");
                } else {
                    ChildLabel->setText("Another error");
                }
            }
        });
    } else if (operation == ted::SAVE) {
        if (Name->getText() != "Nameless") {
            status = ted::File::saveFile(
                static_cast<std::string>(Name->getText()),
                static_cast<std::string>(TextArea->getText()));
            if (status != ted::SAVE_SUCCESS) {
                std::cout << "ERROR_SAVE" << '\n';
            }
        }
    } else if (operation == ted::CLOSE) {
        Name->setText("Nameless");
        TextArea->setText("");
        ted::File::closeFile();
    } else if (operation == ted::EXIT) {
        ted::File::closeFile();
        Window.close();
    }
}

void Gui::initChildWindow() {
    ChildWindow = tgui::ChildWindow::create();
    ChildWindow->setRenderer(WidgetsTheme.getRenderer("ChildWindow"));
    ChildWindow->setClientSize({250, 120});
    ChildWindow->setPosition(ChildWindow->getPosition().x + 25,
                             ChildWindow->getPosition().y + 50);
    Interface.add(ChildWindow);

    ChildLabel = tgui::Label::create();
    ChildLabel->setRenderer(WidgetsTheme.getRenderer("Label"));
    ChildLabel->setTextSize(12);
    ChildLabel->setText("Enter filename:");
    ChildLabel->setPosition(25, 30);
    ChildWindow->add(ChildLabel);

    ChildEditBox = tgui::EditBox::create();
    ChildEditBox->setRenderer(WidgetsTheme.getRenderer("EditBox"));
    ChildEditBox->setDefaultText("...");
    ChildEditBox->setSize(150, 30);
    ChildEditBox->setPosition(25, 60);
    ChildEditBox->setTextSize(12);
    ChildWindow->add(ChildEditBox);

    ChildButton = tgui::Button::create();
    ChildButton->setRenderer(WidgetsTheme.getRenderer("Button"));
    ChildButton->setSize(25, 30);
    ChildButton->setPosition(200, 60);
    ChildButton->setText("Ok");
    ChildButton->setTextSize(12);
    ChildWindow->add(ChildButton);

    ChildWindow->onClose.connect([this]() { Menu->setEnabled(true); });
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