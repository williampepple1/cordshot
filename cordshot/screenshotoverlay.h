#ifndef SCREENSHOTOVERLAY_H
#define SCREENSHOTOVERLAY_H

#include <QWidget>
#include <QPoint>
#include <QPixmap>

class ScreenshotOverlay : public QWidget
{
    Q_OBJECT

public:
    explicit ScreenshotOverlay(QWidget *parent = nullptr);
    ~ScreenshotOverlay();

signals:
    void screenshotTaken(const QPixmap &screenshot);
    void cancelled();

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private:
    void captureScreen();
    void takeScreenshot();

    QPixmap m_backgroundPixmap;
    QPoint m_firstPoint;
    QPoint m_secondPoint;
    bool m_isSelecting;
    bool m_hasFirstPoint;
    bool m_isDragging;
};

#endif // SCREENSHOTOVERLAY_H
