#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(const QString &fileName, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionClose, &QAction::triggered, this, &QMainWindow::close);
    connect(ui->actionExit, &QAction::triggered, qApp, &QApplication::closeAllWindows);

    this->setAttribute(Qt::WA_AlwaysShowToolTips, true);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &MainWindow::documentModified);
    this->setWindowTitle(tr("Text Editor[*]"));

    QSettings settings;
    QVariant variant = settings.value("viewFont");
    ui->textEdit->setFont(variant.isNull() ? QApplication::font() : variant.value<QFont>());

    connect(ui->actionAboutQt, &QAction::triggered, qApp, &QApplication::aboutQt);
    connect(ui->actionCut, &QAction::triggered, ui->textEdit, &QTextEdit::cut);
    connect(ui->actionCopy, &QAction::triggered, ui->textEdit, &QTextEdit::copy);
    connect(ui->actionPaste, &QAction::triggered, ui->textEdit, &QTextEdit::paste);
    connect(ui->actionUndo, &QAction::triggered, ui->textEdit, &QTextEdit::undo);
    connect(ui->actionRedo, &QAction::triggered, ui->textEdit, &QTextEdit::redo);
    connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCopy, &QAction::setEnabled);
    connect(ui->textEdit, &QTextEdit::copyAvailable, ui->actionCut, &QAction::setEnabled);
    connect(ui->textEdit, &QTextEdit::undoAvailable, ui->actionUndo, &QAction::setEnabled);
    connect(ui->textEdit, &QTextEdit::redoAvailable, ui->actionRedo, &QAction::setEnabled);

    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionUndo->setEnabled(false);
    ui->actionRedo->setEnabled(false);

    loadFile(fileName);

    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSaveAs, &QAction::triggered, this, &MainWindow::saveAsFile);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *e){
    if(this->isWindowModified())
        switch(QMessageBox::warning(this, "Document Modified",
                   "The document has been modified. "
                   "Do you want to save your changes?\n"
                   "You will lose and unsaved changes.",
                   QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel,
                   QMessageBox::Cancel))
        {
        case QMessageBox::Yes:
            saveFile() ? e->accept() : e->ignore();
            break;
        case QMessageBox::No:
            e->accept();
            break;
        case QMessageBox::Cancel:
            e->ignore();
            break;
        default:
            e->accept();
        }
    else
        e->accept();
}

void MainWindow::on_actionNew_triggered()
{
    MainWindow *newWindow = new MainWindow();
    newWindow->show();
}

void MainWindow::documentModified(){
     this->setWindowModified(true);
}

void MainWindow::on_actionSelectFont_triggered()
{
    bool ok;
    QFont newFont = QFontDialog::getFont(&ok, ui->textEdit->font(), this, tr("Settings"));
    if(ok){
        QSettings settings;
        settings.setValue("viewFont", newFont);
        ui->textEdit->setFont(newFont);
    }
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About"), tr("This application was made by Pavlenty with Qt helper and google)"));
}


void MainWindow::loadFile(const QString &fileName){
    if(fileName.isEmpty()){
        setFileName(QString());
        return;
    }
    QFile file(fileName);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Warning"), tr("Sorry, file doesn't exist"));
        setFileName(QString());
        return;
    }
    QTextStream textStream(&file);
    ui->textEdit->setText(textStream.readAll());
    file.close();
    setFileName(fileName);
    this->setWindowModified(false);
}

void MainWindow::setFileName(const QString &fileName){
    m_fileName = fileName;
    this->setWindowTitle(QString("%1[*] - %2")
                         .arg(m_fileName.isNull() ? "untitled" : QFileInfo(m_fileName).fileName())
                         .arg(QApplication::applicationName()));

}
void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
        "Open document", QDir::currentPath(), "Text documents (*.txt);;All files (*.*);;All all files (*)");
    if(fileName.isNull())
        return;
    if(m_fileName.isNull() || !this->isWindowModified())
        loadFile(fileName);
    else{
        MainWindow *newWindow = new MainWindow(fileName);
        newWindow->show();
    }
}

bool MainWindow::saveFile(){
    if(m_fileName.isNull())
        return saveAsFile();
    QFile file(m_fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        QMessageBox::warning(this, tr("Warning"), tr("Sorry, file doesn't exist"));
        setFileName(QString());
        return false;
    }
    QTextStream textStream(&file);
    textStream << ui->textEdit->toPlainText();
    file.close();
    this->setWindowModified(false);
    return true;
}

bool MainWindow::saveAsFile(){
    QString fileName = QFileDialog::getSaveFileName(this, "Save document",
       m_fileName.isNull() ? QDir::currentPath() : m_fileName, "Text documents (*.txt)");
    if(fileName.isNull())
        return false;
    setFileName(fileName);
    return saveFile();
}
