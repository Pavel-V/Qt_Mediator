#include "player.h"
#include <thread>
#include <QDebug>
#include <QApplication>
#include <QDateTime>
#include "mediator.h"
#include "speed_convertor.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
    m_value = 0;
    m_is_running = false;
    m_is_proc_loop_started = true;
    m_interval_init = 17; // 1000 ms
    m_interval = m_interval_init;
}

void Player::run()
{
    while(m_is_proc_loop_started)
    {
        if (m_is_running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(3));

            if (m_is_running) // пока спали, is_running могло стать false
            {
                uint64_t timestamp = QDateTime::currentDateTime().toMSecsSinceEpoch();
                int delta_time = timestamp - m_timestamp_last_change;

                if (delta_time >= m_interval)
                {
                    int skip = delta_time / m_interval;

                    m_timestamp_last_change = timestamp - (delta_time % m_interval);

                    m_value += skip * m_interval_init;
                    if (m_value <= Mediator::get_max_value())
                    {
                        if (Mediator::get_instance().mutex_try_lock())
                        {
                            //qDebug() << "update" << m_value;
                            emit next_step_got(m_value);
                            Mediator::get_instance().mutex_unlock();
                        }
                        else
                        {
                            qDebug() << "skip" << m_value;
                        }
                    }
                    else
                    {
                        m_value = 0;
                        m_is_running = false;
                        emit ended();
                    }
                }
                //qDebug() << "!!!" << m_value;
            }
        }
        else
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        //QApplication::processEvents();
    }
    qDebug() << "end";
}

void Player::value_change(int value)
{
    qDebug() << "Player::value_change" << value << m_value;

    if (m_value == value)
    {
        return;
    }

    qDebug() << "Player::value_change";

    bool was_running = m_is_running;

    if (m_is_running)
    {
        pause();
    }

    m_value = value;
    m_timestamp_last_change = QDateTime::currentDateTime().toMSecsSinceEpoch();

    if (was_running)
    {
        play();
    }
}

void Player::play()
{
    m_is_running = true;
    m_timestamp_last_change = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

void Player::pause()
{
    m_is_running = false;
}

void Player::stop()
{
    m_value = 0;
    pause();
}

void Player::release()
{
    m_is_proc_loop_started = false;
}

void Player::change_speed(int speed)
{
    m_interval = m_interval_init * SPEED_INTERVAL_COEFS[speed];
}
