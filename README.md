# qt mini tools

## hotloadui
hot loading ui file works to widget pages
```cpp
class MyWidget : public QWidget, HotLoadUiTool {
public:
    explicit MyWidget(QWidget* parent = nullptr)
        : QWidget(parent)
    {
#ifdef QT_DEBUG
        loadWidget(this);
        //load temp empty widget when debug mode
        ui.setupUi(new QWidget);
#else
        ui.setupUi(this);
#endif
    }

    //to call reload ui when ui file changed
    void requestReloadUi(const QString& fileName) {
        if (fileName == targetUiFileName()) {
            reloadUi();
        }
    }

protected:
    void reloadUi() override {
        loadWidget(this);
    }

    QString targetUiFileName() override {
        return "mywidget.ui";
    }

private:
    Ui::MyWidget ui;
}
```

## filewatcher
listen file changed
```cpp
#ifdef QT_DEBUG
FileWatcher fileWatcher([&](const QString& fileName) {
    qDebug() << "current file changed:" << fileName;
});
fileWatcher.setDirectory(HotLoadUiTool::getProjectDir(), "ui");
#endif
```
hotload widget ui, e.g. `hotloadui`:
```cpp
int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    MyWidget w;
    w.show();

#ifdef QT_DEBUG
    FileWatcher fileWatcher([&](const QString& fileName) {
        w.requestReloadUi(fileName);
    });
    fileWatcher.setDirectory(HotLoadUiTool::getProjectDir(), "ui");
#endif

    return a.exec();
}
```

to hotload qml, such as:
```cpp
int main(int argc, char* argv[]) {
    QGuiApplication a(argc, argv);

    QQuickView view;
#ifdef QT_DEBUG
    const QDir workDir(PROJECT_UI_PATH);
    const QUrl sourceFile = QUrl::fromLocalFile(workDir.filePath("main.qml"));
    view.setSource(sourceFile);
#else
    view.setSource("qrc:/ui/main.qml");
#endif
    view.show();

#ifdef QT_DEBUG
    FileWatcher fileWatcher([&](const QString& fileName) {
        view.engine()->clearComponentCache();
        view.setSource(sourceFile);
    });
    fileWatcher.setDirectory(PROJECT_UI_PATH, "qml");
#endif

    return a.exec();
}
```