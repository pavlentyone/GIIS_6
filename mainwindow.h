#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QCloseEvent>
#include <QIcon>
#include <QSettings>
#include <QFontDialog>
#include <QFont>
#include <QVariant>
#include <QTextEdit>
#include <QString>
#include <QTextStream>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(const QString &fileName=QString(), QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *e);

private slots:
    void on_actionNew_triggered();
    void documentModified();

    void on_actionSelectFont_triggered();

    void on_actionAbout_triggered();
    void loadFile(const QString &fileName);

    void on_actionOpen_triggered();
    void setFileName(const QString &fileName);
    bool saveFile();
    bool saveAsFile();


private:
    Ui::MainWindow *ui;

    QString m_fileName;
};

#endif // MAINWINDOW_H
