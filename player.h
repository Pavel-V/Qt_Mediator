#ifndef PLAYER_H
#define PLAYER_H

#include <QObject>
#include <cstdint>

class Player : public QObject
{
    Q_OBJECT
public:
    explicit Player(QObject *parent = 0);
    void run();

signals:
    void next_step_got(int value);
    void ended();

public slots:
    void value_change(int value);
    void play();
    void pause();
    void stop();
    void release();
    void change_speed(int speed);

protected:

private:
    uint64_t m_timestamp_last_change;
    int m_interval_init;
    int m_interval;
    int m_value;
    bool m_is_running;
    bool m_is_proc_loop_started;
};

#endif // PLAYER_H
