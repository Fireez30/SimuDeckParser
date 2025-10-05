#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QCheckBox>
#include <QComboBox>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void LoadSimulator(std::string simulator_path);
    void switchToUi(const QString &uiFilePath);
private:
    Ui::MainWindow *ui;
private slots:
    void LoadButtonClicked();
    void OnExit();
    void OnUnloadSimulator();
    void SwitchToCardList();
    void SwitchToDeckEditor();
};
#endif // MAINWINDOW_H
