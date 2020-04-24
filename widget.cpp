#include "widget.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QSlider>
#include <QLabel>
#include <QSpinBox>
#include <QDebug>
#include <QShortcut>
#include <QDateTime>
#include "mediator.h"
#include "speed_convertor.h"

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

    m_label_speed = new QLabel(SPEED_LABELS[SPEED_NORMAL]);
    //m_speed_spin_box = new QSpinBox();
    //m_speed_spin_box->setMinimum(-4);
    //m_speed_spin_box->setMinimum(-4);

    m_slider            = new QSlider();
    m_slider->setMinimum(0);
    m_slider->setMaximum(Mediator::get_max_value());

    m_label_value = new QLabel(QString::number(m_slider->minimum()));

    QObject::connect(m_slider, &QSlider::valueChanged, [this](int value)
    {
        qDebug() << "!!!!!" << value;
        m_label_value->setText(QString::number(value));
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
    layout->addWidget(m_label_speed);
    layout->addWidget(m_slider);
    layout->addWidget(m_label_value);

    setLayout(layout);

    setMinimumWidth(200);
    setWindowTitle(objectName());

    QShortcut* key_speed_down = new QShortcut(this);
    key_speed_down->setKey(Qt::Key_Z);
    connect(key_speed_down, &QShortcut::activated, [this]() {emit speed_down_requested();});

    QShortcut* key_speed_up = new QShortcut(this);
    key_speed_up->setKey(Qt::Key_X);
    connect(key_speed_up, &QShortcut::activated, [this]() {emit speed_up_requested();});
}

void Widget::change_slider_value(int value)
{
    qDebug() << "Widget::change_slider_value" << value << m_slider->value()
             << objectName() << QDateTime::currentDateTime().toMSecsSinceEpoch();

    if (m_slider->value() == value)
    {
        return;
    }

    m_slider->disconnect();

    m_slider->setValue(value);
    m_label_value->setText(QString::number(value));

    QObject::connect(m_slider, &QSlider::valueChanged, [this](int value)
    {
        qDebug() << "!!!!!" << value;
        m_label_value->setText(QString::number(value));
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
    m_label_value->setText(QString::number(m_slider->minimum()));
}

void Widget::change_speed(int speed)
{
    m_label_speed->setText(SPEED_LABELS[speed]);
}

void Widget::closeEvent(QCloseEvent *event)
{
    emit closed();
}

Widget::~Widget()
{

}
