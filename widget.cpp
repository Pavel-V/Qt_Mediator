#include "widget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QDebug>
#include "mediator.h"

Widget::Widget(const QString& name, QWidget *parent)
    : QWidget(parent)
{
    setObjectName(name);
    create_widgets();
    m_is_playing = false;
}

void Widget::create_widgets() noexcept
{
    m_play_pause_button = new QPushButton("PLAY");
    m_stop_button       = new QPushButton("STOP");

    QObject::connect(m_play_pause_button, &QPushButton::clicked, [this]()
    {
        if (m_is_playing)
        {
            emit pause_button_clicked();
        }
        else
        {
            emit play_button_clicked();
        }
    });

    QObject::connect(m_stop_button, &QPushButton::clicked, this, &Widget::stop_button_clicked);

    m_slider            = new QSlider();
    m_slider->setMinimum(0);
    m_slider->setMaximum(Mediator::get_max_value());

    m_label = new QLabel(QString::number(m_slider->minimum()));

    QObject::connect(m_slider, &QSlider::valueChanged, [this](int value)
    {
        qDebug() << "!!!!!" << value;
        m_label->setText(QString::number(value));
        emit slider_value_changed(value);
    });

    //    QObject::connect(m_slider, &QSlider::sliderMoved, [this](int value)
    //    {
    //        m_label->setText(QString::number(value));
    //        emit slider_value_changed(value);
    //    });


    QVBoxLayout* layout = new QVBoxLayout();

    layout->addWidget(m_play_pause_button);
    layout->addWidget(m_stop_button);
    layout->addWidget(m_slider);
    layout->addWidget(m_label);

    setLayout(layout);

    setMinimumWidth(200);
    setWindowTitle(objectName());
}

void Widget::change_slider_value(int value)
{
    qDebug() << "Widget::change_slider_value" << value << m_slider->value()
             << objectName();

    if (m_slider->value() == value)
    {
        return;
    }

    m_slider->disconnect();

    m_slider->setValue(value);
    m_label->setText(QString::number(value));

    QObject::connect(m_slider, &QSlider::valueChanged, [this](int value)
    {
        qDebug() << "!!!!!" << value;
        m_label->setText(QString::number(value));
        emit slider_value_changed(value);
    });

    //    QObject::connect(m_slider, &QSlider::sliderMoved, [this](int value)
    //    {
    //        m_label->setText(QString::number(value));
    //        emit slider_value_changed(value);
    //    });
}

void Widget::play()
{
    if (!m_is_playing)
    {
        m_slider->setEnabled(true);
        m_play_pause_button->setText("PAUSE");
        m_is_playing = true;
    }
}

void Widget::pause()
{
    if (m_is_playing)
    {
        m_play_pause_button->setText("PLAY");
        m_is_playing = false;
    }
}

void Widget::stop()
{
    pause();
    m_slider->setEnabled(false);
    m_slider->setValue(m_slider->minimum());
    m_label->setText(QString::number(m_slider->minimum()));
}

void Widget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

Widget::~Widget()
{

}
