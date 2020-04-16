#include "mediator.h"
#include "player.h"
#include "widget.h"
#include <QDebug>

#define MAX_VALUE 100

Mediator::Mediator(QObject *parent) :
    QObject(parent)
{
    m_is_playing = false;
    m_slider_value = 0;
}

int Mediator::get_max_value()
{
    return MAX_VALUE;
}

void Mediator::set_widgets(Widget* w1, Widget* w2) noexcept
{
    m_count_active_widgets = 2;

    m_ptr_widget_1 = w1;
    m_ptr_widget_2 = w2;
}

void Mediator::set_player(Player* player) noexcept
{
    m_ptr_player = player;
}

void Mediator::change_slider_value(int value)
{
    qDebug() << "Mediator::change_slider_value" << value << m_slider_value << sender()->objectName();

    if (m_slider_value == value || !m_is_playing)
    {
        return;
    }

    m_slider_value = value;

    qDebug() << m_ptr_widget_1->objectName();

    m_ptr_widget_1->change_slider_value(m_slider_value);
    m_ptr_widget_2->change_slider_value(m_slider_value);
    m_ptr_player->value_change(m_slider_value);

    if(sender()->objectName().contains("w"))
    {
        //emit slider_changed(value);
    }
    else
    {
        //emit value_changed(value);
    }
}

void Mediator::play()
{
    m_is_playing = true;
    //emit played();

    m_ptr_widget_1->play();
    m_ptr_widget_2->play();
    m_ptr_player->play();
}

void Mediator::pause()
{
    m_is_playing = false;
    //emit paused();

    m_ptr_widget_1->pause();
    m_ptr_widget_2->pause();
    m_ptr_player->pause();
}

void Mediator::stop()
{
    m_is_playing = false;
    m_slider_value = 0;

    m_ptr_widget_1->stop();
    m_ptr_widget_2->stop();
    m_ptr_player->stop();


    //emit stopped();
    //emit slider_changed(m_slider_value);
}

void Mediator::widget_close()
{
    --m_count_active_widgets;

    if (m_count_active_widgets <= 0)
    {
        qDebug() << "REL";
        m_ptr_player->stop();
        m_ptr_player->release();
    }
}

