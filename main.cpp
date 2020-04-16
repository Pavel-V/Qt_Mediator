#include <QApplication>
#include <QThread>
#include <QDebug>
#include <QtConcurrent/QtConcurrent>

#include "widget.h"
#include "mediator.h"
#include "player.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w1("w1");
    Widget w2("w2");

    QThread player_thread;
    Player player;
    player.setObjectName("player");
    player.moveToThread(&player_thread);

    Mediator::get_instance().setObjectName("mediator");
    Mediator::get_instance().set_widgets(&w1, &w2);
    Mediator::get_instance().set_player(&player);

    QObject::connect(&w1, &Widget::slider_value_changed,    &Mediator::get_instance(), &Mediator::change_slider_value);
    QObject::connect(&w1, &Widget::play_button_clicked,     &Mediator::get_instance(), &Mediator::play);
    QObject::connect(&w1, &Widget::pause_button_clicked,    &Mediator::get_instance(), &Mediator::pause);
    QObject::connect(&w1, &Widget::stop_button_clicked,     &Mediator::get_instance(), &Mediator::stop);
    QObject::connect(&w1, &Widget::closed,                  &Mediator::get_instance(), &Mediator::widget_close);

    QObject::connect(&w2, &Widget::slider_value_changed,    &Mediator::get_instance(), &Mediator::change_slider_value);
    QObject::connect(&w2, &Widget::play_button_clicked,     &Mediator::get_instance(), &Mediator::play);
    QObject::connect(&w2, &Widget::pause_button_clicked,    &Mediator::get_instance(), &Mediator::pause);
    QObject::connect(&w2, &Widget::stop_button_clicked,     &Mediator::get_instance(), &Mediator::stop);
    QObject::connect(&w2, &Widget::closed,                  &Mediator::get_instance(), &Mediator::widget_close);

    QObject::connect(&player, &Player::next_step_got,       &Mediator::get_instance(), &Mediator::change_slider_value);
    QObject::connect(&player, &Player::ended,               &Mediator::get_instance(), &Mediator::stop);

    QObject::connect(&player_thread, &QThread::started,  &player, &Player::run);
    QObject::connect(&player_thread, &QThread::finished, &player, &Player::release);

    player_thread.start();

    w1.show();
    w2.show();

    int ouput = a.exec();

    qDebug() << "!!!!!!!";

    QThread::msleep(2000);
    player_thread.terminate();

    qDebug() << "!!!!!!!";

    return ouput;
}
