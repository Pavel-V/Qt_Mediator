#ifndef MEDIATOR_H
#define MEDIATOR_H

#include <QObject>
class Player;
class Widget;

class Mediator : public QObject
{
    Q_OBJECT
public:
    static Mediator& get_instance()
    {
        static Mediator instance;
        return instance;
    }

    static int get_max_value();

    void set_widgets(Widget* w1, Widget* w2) noexcept;
    void set_player(Player* player) noexcept;

public slots:
    void change_slider_value(int value);
    void play();
    void pause();
    void stop();
    void widget_close();
    void speed_up();
    void speed_down();

protected:
    void change_speed();

private:
    explicit Mediator(QObject* parent = nullptr);
    ~Mediator()= default;
    Mediator(const Mediator&)= delete;
    Mediator& operator=(const Mediator&)= delete;

    Widget* m_ptr_widget_1;
    Widget* m_ptr_widget_2;
    Player* m_ptr_player;

    int m_count_active_widgets;

    int m_slider_value;
    int m_speed_value;
    bool m_is_playing;
};


#endif // MEDIATOR_H
