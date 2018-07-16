#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>

class QLabel;
class QLineEdit;
class QPushButton;
class QZXing;
class MainWindow : public QWidget
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void keyPressEvent(QKeyEvent *event);

private slots:
    void copyToClip();
    void error(QString msg);
    void tagFound(const QString &tag, const QString &format, const QString &charSet);

private:
    QClipboard*     m_clipBoard;
    QLabel*         m_label;
    QLabel*         m_label2;
    QLineEdit*      m_edit;
    QPushButton*    m_button;
    QZXing*         m_zxing;
};

#endif // MAINWINDOW_H
