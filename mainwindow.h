#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QSystemTrayIcon>
#include <QSettings>

class ScreenshotOverlay;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
    QString getSavePath() const;

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void startScreenshot();
    void onScreenshotTaken(const QPixmap &screenshot, const QString &savedPath);
    void onScreenshotCancelled();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void selectSaveFolder();

private:
    void setupUI();
    void setupTrayIcon();
    void loadSettings();
    void saveSettings();
    void updateSavePathDisplay();

    QPushButton *m_captureButton;
    QPushButton *m_folderButton;
    QLabel *m_statusLabel;
    QLabel *m_previewLabel;
    QLabel *m_savePathLabel;
    ScreenshotOverlay *m_overlay;
    QSystemTrayIcon *m_trayIcon;
    QPixmap m_lastScreenshot;
    QString m_savePath;
    QSettings *m_settings;
};

#endif // MAINWINDOW_H
