#include <QGuiApplication>
#include <QIcon>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);
  app.setWindowIcon(QIcon(":/com/Roboticus/ControlCenter/assets/Icons/"
                          "com.roboticus.DataVisualiser.png"));
  app.setDesktopFileName("com.roboticus.DataVisualiser");
  app.setOrganizationName("Roboticus");
  app.setApplicationDisplayName("Roboticus Data Visualiser");

  QQmlApplicationEngine engine;

  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreationFailed, &app,
      []() { QCoreApplication::exit(-1); }, Qt::QueuedConnection);
  engine.loadFromModule("com.roboticus.DataVisualiser", "Main");

  return app.exec();
}
