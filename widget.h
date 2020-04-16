#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

class QPushButton;
class QLabel;
class QSlider;

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(const QString& name, QWidget *parent = 0);
    ~Widget();

protected:
    void create_widgets() noexcept;

private:
    void closeEvent(QCloseEvent *event);

    QPushButton* m_play_pause_button;
    QPushButton* m_stop_button;
    QSlider* m_slider;
    QLabel* m_label;
    bool m_is_playing;

public slots:
    void change_slider_value(int value);
    void play();
    void pause();
    void stop();

signals:
    void slider_value_changed(int value);
    void play_button_clicked();
    void pause_button_clicked();
    void stop_button_clicked();
    void closed();
};

#endif // WIDGET_H
