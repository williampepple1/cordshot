#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QSystemTrayIcon>

class ScreenshotOverlay;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void startScreenshot();
    void onScreenshotTaken(const QPixmap &screenshot);
    void onScreenshotCancelled();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);

private:
    void setupUI();
    void setupTrayIcon();

    QPushButton *m_captureButton;
    QLabel *m_statusLabel;
    QLabel *m_previewLabel;
    ScreenshotOverlay *m_overlay;
    QSystemTrayIcon *m_trayIcon;
    QPixmap m_lastScreenshot;
};

#endif // MAINWINDOW_H
