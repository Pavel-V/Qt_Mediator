#include "player.h"
#include <thread>
#include <QDebug>
#include <QApplication>
#include "mediator.h"

Player::Player(QObject *parent) :
    QObject(parent)
{
    m_value = 0;
    m_is_running = false;
    m_is_proc_loop_started = true;
}

void Player::run()
{
    while(m_is_proc_loop_started)
    {
        if (m_is_running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));

            if (m_is_running)
            {
                ++m_value;

                if (m_value <= Mediator::get_max_value())
                {
                    emit next_step_got(m_value);
                }
                else
                {
                    m_value = 0;
                    m_is_running = false;
                    emit ended();
                }

                qDebug() << "!!!" << m_value;
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

    if (was_running)
    {
        play();
    }
}

void Player::play()
{
    m_is_running = true;
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
