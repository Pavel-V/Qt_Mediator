#include "mediator.h"
#include "player.h"
#include "widget.h"
#include "speed_convertor.h"
#include <thread>
#include <QDebug>

#define MAX_VALUE 100

Mediator::Mediator(QObject *parent) :
    QObject(parent)
{
    m_is_playing = false;
    m_slider_value = 0;
    m_speed_value = SPEED_NORMAL;
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

void Mediator::mutex_lock() noexcept
{
    m_mutex.lock();
}

bool Mediator::mutex_try_lock() noexcept
{
    qDebug() << "!?";
    return m_mutex.try_lock();
}

void Mediator::mutex_unlock() noexcept
{
    m_mutex.unlock();
}

void Mediator::change_slider_value(int value)
{
    //qDebug() << "Mediator::change_slider_value" << value << m_slider_value << sender()->objectName();

    if (m_slider_value == value || !m_is_playing)
    {
        return;
    }

    m_slider_value = value;

    m_mutex.lock();

    m_ptr_player->value_change(m_slider_value);

    std::this_thread::sleep_for(std::chrono::milliseconds(1500));

    m_ptr_widget_1->change_slider_value(m_slider_value);
    m_ptr_widget_2->change_slider_value(m_slider_value);

    m_mutex.unlock();
}

void Mediator::play()
{
    m_is_playing = true;

    m_ptr_widget_1->play();
    m_ptr_widget_2->play();
    m_ptr_player->play();
}

void Mediator::pause()
{
    m_is_playing = false;

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
}

void Mediator::speed_down()
{
    m_speed_value = std::max(SPEED_MIN, m_speed_value - 1);
    change_speed();
}

void Mediator::speed_up()
{
    m_speed_value = std::min(SPEED_MAX, m_speed_value + 1);
    change_speed();
}

void Mediator::change_speed()
{
    m_ptr_widget_1->change_speed(m_speed_value);
    m_ptr_widget_2->change_speed(m_speed_value);
    m_ptr_player->change_speed(m_speed_value);
}

void Mediator::widget_close()
{
    --m_count_active_widgets;

    if (m_count_active_widgets <= 0)
    {
        m_ptr_player->stop();
        m_ptr_player->release();
    }
}

