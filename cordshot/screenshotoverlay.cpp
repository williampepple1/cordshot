#include "screenshotoverlay.h"
#include <QPainter>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QScreen>
#include <QGuiApplication>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDateTime>
#include <QClipboard>
#include <QMessageBox>

ScreenshotOverlay::ScreenshotOverlay(QWidget *parent)
    : QWidget(parent)
    , m_isSelecting(false)
    , m_hasFirstPoint(false)
    , m_isDragging(false)
{
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint | Qt::Tool);
    setAttribute(Qt::WA_TranslucentBackground, false);
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
    
    captureScreen();
}

ScreenshotOverlay::~ScreenshotOverlay()
{
}

void ScreenshotOverlay::captureScreen()
{
    // Get the virtual desktop geometry (all screens combined)
    QScreen *screen = QGuiApplication::primaryScreen();
    if (screen) {
        // Capture the entire screen
        m_backgroundPixmap = screen->grabWindow(0);
        
        // Set geometry to cover the primary screen
        setGeometry(screen->geometry());
    }
    
    showFullScreen();
    activateWindow();
    raise();
}

void ScreenshotOverlay::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    
    QPainter painter(this);
    
    // Draw the captured screen
    painter.drawPixmap(0, 0, m_backgroundPixmap);
    
    // Draw semi-transparent dark overlay
    painter.fillRect(rect(), QColor(0, 0, 0, 100));
    
    // If we have a selection, draw it
    if (m_hasFirstPoint && m_isSelecting) {
        QRect selectionRect = QRect(m_firstPoint, m_secondPoint).normalized();
        
        // Clear the selection area (show original screen)
        painter.setCompositionMode(QPainter::CompositionMode_Source);
        painter.drawPixmap(selectionRect, m_backgroundPixmap, selectionRect);
        
        // Draw selection border
        painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
        QPen pen(QColor(0, 174, 255), 2);
        painter.setPen(pen);
        painter.drawRect(selectionRect);
        
        // Draw corner handles
        int handleSize = 8;
        QColor handleColor(0, 174, 255);
        painter.setBrush(handleColor);
        painter.setPen(Qt::NoPen);
        
        // Corner handles
        painter.drawRect(selectionRect.left() - handleSize/2, selectionRect.top() - handleSize/2, handleSize, handleSize);
        painter.drawRect(selectionRect.right() - handleSize/2, selectionRect.top() - handleSize/2, handleSize, handleSize);
        painter.drawRect(selectionRect.left() - handleSize/2, selectionRect.bottom() - handleSize/2, handleSize, handleSize);
        painter.drawRect(selectionRect.right() - handleSize/2, selectionRect.bottom() - handleSize/2, handleSize, handleSize);
        
        // Draw dimensions
        QString dimensions = QString("%1 × %2").arg(selectionRect.width()).arg(selectionRect.height());
        QFont font = painter.font();
        font.setPointSize(10);
        font.setBold(true);
        painter.setFont(font);
        
        QFontMetrics fm(font);
        QRect textRect = fm.boundingRect(dimensions);
        textRect.adjust(-8, -4, 8, 4);
        
        int textX = selectionRect.center().x() - textRect.width() / 2;
        int textY = selectionRect.bottom() + 20;
        
        // Keep text on screen
        if (textY + textRect.height() > height()) {
            textY = selectionRect.top() - textRect.height() - 10;
        }
        
        painter.setPen(Qt::NoPen);
        painter.setBrush(QColor(0, 0, 0, 180));
        painter.drawRoundedRect(textX, textY, textRect.width(), textRect.height(), 4, 4);
        
        painter.setPen(Qt::white);
        painter.drawText(textX + 8, textY + fm.ascent() + 4, dimensions);
    }
    
    // Draw instructions
    QString instructions = m_hasFirstPoint ? 
        "Click second point or drag to select • ESC to cancel" : 
        "Click first point or drag to select • ESC to cancel";
    
    QFont font = painter.font();
    font.setPointSize(11);
    painter.setFont(font);
    
    QFontMetrics fm(font);
    QRect textRect = fm.boundingRect(instructions);
    textRect.adjust(-16, -8, 16, 8);
    
    int x = (width() - textRect.width()) / 2;
    int y = 30;
    
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(0, 0, 0, 200));
    painter.drawRoundedRect(x, y, textRect.width(), textRect.height(), 6, 6);
    
    painter.setPen(Qt::white);
    painter.drawText(x + 16, y + fm.ascent() + 8, instructions);
}

void ScreenshotOverlay::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        if (!m_hasFirstPoint) {
            // First click - set first point
            m_firstPoint = event->pos();
            m_secondPoint = event->pos();
            m_hasFirstPoint = true;
            m_isSelecting = true;
            m_isDragging = true;
            update();
        } else if (!m_isDragging) {
            // Second click - set second point and capture
            m_secondPoint = event->pos();
            takeScreenshot();
        }
    } else if (event->button() == Qt::RightButton) {
        // Cancel
        emit cancelled();
        close();
    }
}

void ScreenshotOverlay::mouseMoveEvent(QMouseEvent *event)
{
    if (m_isDragging && m_hasFirstPoint) {
        m_secondPoint = event->pos();
        update();
    }
}

void ScreenshotOverlay::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && m_isDragging) {
        m_secondPoint = event->pos();
        m_isDragging = false;
        
        // If dragged a meaningful distance, take screenshot immediately
        QRect selection = QRect(m_firstPoint, m_secondPoint).normalized();
        if (selection.width() > 5 && selection.height() > 5) {
            takeScreenshot();
        } else {
            // Small click, wait for second click
            update();
        }
    }
}

void ScreenshotOverlay::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape) {
        emit cancelled();
        close();
    } else if (event->key() == Qt::Key_Return || event->key() == Qt::Key_Enter) {
        if (m_hasFirstPoint && m_isSelecting) {
            takeScreenshot();
        }
    }
}

void ScreenshotOverlay::takeScreenshot()
{
    QRect selection = QRect(m_firstPoint, m_secondPoint).normalized();
    
    if (selection.width() < 1 || selection.height() < 1) {
        emit cancelled();
        close();
        return;
    }
    
    // Extract the selected region from the captured screen
    QPixmap screenshot = m_backgroundPixmap.copy(selection);
    
    // Copy to clipboard
    QGuiApplication::clipboard()->setPixmap(screenshot);
    
    // Generate default filename
    QString defaultPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd_hh-mm-ss");
    QString defaultFilename = defaultPath + "/screenshot_" + timestamp + ".png";
    
    // Hide overlay before showing dialog
    hide();
    
    // Ask user where to save
    QString filename = QFileDialog::getSaveFileName(
        nullptr,
        "Save Screenshot",
        defaultFilename,
        "PNG Image (*.png);;JPEG Image (*.jpg *.jpeg);;All Files (*.*)"
    );
    
    if (!filename.isEmpty()) {
        if (screenshot.save(filename)) {
            emit screenshotTaken(screenshot);
        } else {
            QMessageBox::warning(nullptr, "Error", "Failed to save screenshot.");
        }
    } else {
        // User cancelled save, but screenshot is still in clipboard
        emit screenshotTaken(screenshot);
    }
    
    close();
}
