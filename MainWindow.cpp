#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QClipboard>
#include "MainWindow.h"
#include "QZXing.h"

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
    , m_clipBoard(QGuiApplication::clipboard())
    , m_label(new QLabel(this))
    , m_edit(new QLineEdit(this))
    , m_button(new QPushButton(this))
    , m_zxing(new QZXing(this))
{
    setMinimumSize(500, 240);
    resize(640, 300);
    grabKeyboard();

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(m_label);
    m_label->setFixedSize(200, 200);
    m_label->setStyleSheet("border:2px solid black");
    layout->addWidget(m_edit);
    m_edit->setReadOnly(true);
    layout->addWidget(m_button);
    m_button->setText(QString::fromUtf8("复制"));
    connect(m_button, SIGNAL(clicked()), this, SLOT(copyToClip()));

    m_zxing->setDecoder(QZXing::DecoderFormat_CODABAR | QZXing::DecoderFormat_EAN_13 | QZXing::DecoderFormat_EAN_8);
}

MainWindow::~MainWindow()
{

}

void MainWindow::copyToClip()
{
    if (!m_edit->text().isEmpty()) {
        m_clipBoard->setText(m_edit->text());
    }
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::CTRL && event->key() == Qt::Key_V) {
        QPixmap p = m_clipBoard->pixmap();
        if (p.isNull()) {
            QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("剪贴板中未发现图片，请截图后重试！"), QMessageBox::Ok);
        }
        else {
            m_label->setPixmap(p);
            m_edit->setText(m_zxing->decodeImage(p.toImage()));
            copyToClip();
        }
    }
}
