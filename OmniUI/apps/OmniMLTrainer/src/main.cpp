#include "../../core/include/OmniApp.h"

int main(int argc, char* argv[]) {
    OmniApp app(argc, argv);
    app.initializeJuce();
    app.loadMainSource("qrc:/bobui/qml/Omni/ML/assets/main.qml");
    return app.exec();
}
