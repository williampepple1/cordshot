#ifndef COORDINATEPICKER_H
#define COORDINATEPICKER_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QScrollArea>
#include <QPixmap>
#include <QPoint>
#include <QVector>

class ClickableImageLabel : public QLabel
{
    Q_OBJECT

public:
    explicit ClickableImageLabel(QWidget *parent = nullptr);
    void setPixmap(const QPixmap &pixmap);
    void clearPoints();

signals:
    void pointClicked(const QPoint &point);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap m_originalPixmap;
    QVector<QPoint> m_points;
    QPoint m_currentPos;
};

class CoordinatePicker : public QDialog
{
    Q_OBJECT

public:
    explicit CoordinatePicker(const QPixmap &screenshot, QWidget *parent = nullptr);
    ~CoordinatePicker();

private slots:
    void onPointClicked(const QPoint &point);
    void copyLastCoordinate();
    void copyAllCoordinates();
    void clearPoints();

private:
    void setupUI();
    void updateCoordinateDisplay();

    QPixmap m_screenshot;
    ClickableImageLabel *m_imageLabel;
    QScrollArea *m_scrollArea;
    QLabel *m_coordLabel;
    QLabel *m_instructionLabel;
    QPushButton *m_copyLastButton;
    QPushButton *m_copyAllButton;
    QPushButton *m_clearButton;
    QPushButton *m_closeButton;
    QVector<QPoint> m_points;
};

#endif // COORDINATEPICKER_H
