#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QBoxLayout>
#include <QGuiApplication>
#include <QKeyEvent>
#include <QMessageBox>
#include <QClipboard>
#include <QDebug>
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

    QVBoxLayout* vlayout = new QVBoxLayout(this);
    vlayout->addWidget(m_edit);
    m_edit->setReadOnly(true);
    m_label2 = new QLabel(this);
    m_label2->setStyleSheet("border:2px solid black");
    vlayout->addWidget(m_label2);
    m_label2->setFixedHeight(30);

    layout->addItem(vlayout);

    layout->addWidget(m_button);
    m_button->setText(QString::fromUtf8("复制"));
    connect(m_button, SIGNAL(clicked()), this, SLOT(copyToClip()));

    m_zxing->setDecoder(QZXing::DecoderFormat_CODABAR | QZXing::DecoderFormat_EAN_13 | QZXing::DecoderFormat_EAN_8 | QZXing::DecoderFormat_MAXICODE
                        | QZXing::DecoderFormat_ITF | QZXing::DecoderFormat_RSS_14 | QZXing::DecoderFormat_UPC_A | QZXing::DecoderFormat_UPC_E);
    connect(m_zxing, SIGNAL(error(QString)), this, SLOT(error(QString)));
    connect(m_zxing, SIGNAL(tagFoundAdvanced(QString,QString,QString)), this, SLOT(tagFound(QString,QString,QString)));
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

void MainWindow::error(QString msg)
{
    m_label2->setText(QString("error:") + msg);
}

void MainWindow::tagFound(const QString &tag, const QString &format, const QString &charSet)
{
    m_label2->setText(QString().sprintf("tag:%s, format:%s, charset:%s", tag.toStdString().c_str(), format.toStdString().c_str(), charSet.toStdString().c_str()));
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::CTRL && event->key() == Qt::Key_V) {
        m_edit->clear();
        m_label2->clear();
        QPixmap p = m_clipBoard->pixmap();
        if (p.isNull()) {
            QMessageBox::warning(this, QString::fromUtf8("警告"), QString::fromUtf8("剪贴板中未发现图片，请截图后重试！"), QMessageBox::Ok);
        }
        else {
            m_label->setPixmap(p);
            QString result = m_zxing->decodeImage(p.toImage());
            if (!result.isEmpty()) {
                m_edit->setText(result);
                copyToClip();
            }
            else {
                qDebug() << "Failed!!";
            }
        }
    }
}
