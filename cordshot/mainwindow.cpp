#include "mainwindow.h"
#include "screenshotoverlay.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFrame>
#include <QGraphicsDropShadowEffect>
#include <QMenu>
#include <QAction>
#include <QApplication>
#include <QCloseEvent>
#include <QMessageBox>
#include <QPainter>
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_overlay(nullptr)
    , m_trayIcon(nullptr)
{
    setupUI();
    setupTrayIcon();
}

MainWindow::~MainWindow()
{
    if (m_overlay) {
        m_overlay->close();
        delete m_overlay;
    }
}

void MainWindow::setupUI()
{
    // Window settings - compact widget style
    setWindowTitle("Cordshot");
    setFixedSize(280, 320);
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
    
    // Central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(20, 20, 20, 20);
    mainLayout->setSpacing(16);
    
    // App title
    QLabel *titleLabel = new QLabel("📸 Cordshot", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet(R"(
        QLabel {
            font-size: 22px;
            font-weight: bold;
            color: #E8E8E8;
            padding: 8px;
        }
    )");
    mainLayout->addWidget(titleLabel);
    
    // Capture button with icon
    m_captureButton = new QPushButton("✂️  Capture Region", this);
    m_captureButton->setFixedHeight(50);
    m_captureButton->setCursor(Qt::PointingHandCursor);
    m_captureButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            color: white;
            border: none;
            border-radius: 12px;
            font-size: 15px;
            font-weight: bold;
            padding: 12px 24px;
        }
        QPushButton:hover {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #7b8ef8, stop:1 #8b5fbf);
        }
        QPushButton:pressed {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #5a6fd6, stop:1 #6a4190);
        }
    )");
    
    // Add shadow effect to button
    QGraphicsDropShadowEffect *shadow = new QGraphicsDropShadowEffect(this);
    shadow->setBlurRadius(20);
    shadow->setColor(QColor(102, 126, 234, 100));
    shadow->setOffset(0, 4);
    m_captureButton->setGraphicsEffect(shadow);
    
    connect(m_captureButton, &QPushButton::clicked, this, &MainWindow::startScreenshot);
    mainLayout->addWidget(m_captureButton);
    
    // Preview area
    QFrame *previewFrame = new QFrame(this);
    previewFrame->setFixedHeight(140);
    previewFrame->setStyleSheet(R"(
        QFrame {
            background-color: #2A2A3C;
            border: 2px dashed #4A4A5C;
            border-radius: 10px;
        }
    )");
    
    QVBoxLayout *previewLayout = new QVBoxLayout(previewFrame);
    previewLayout->setContentsMargins(8, 8, 8, 8);
    
    m_previewLabel = new QLabel(this);
    m_previewLabel->setAlignment(Qt::AlignCenter);
    m_previewLabel->setText("Preview will appear here");
    m_previewLabel->setStyleSheet(R"(
        QLabel {
            color: #6A6A7A;
            font-size: 12px;
            border: none;
            background: transparent;
        }
    )");
    previewLayout->addWidget(m_previewLabel);
    
    mainLayout->addWidget(previewFrame);
    
    // Status label
    m_statusLabel = new QLabel("Click capture to select a region", this);
    m_statusLabel->setAlignment(Qt::AlignCenter);
    m_statusLabel->setStyleSheet(R"(
        QLabel {
            color: #8A8A9A;
            font-size: 11px;
            padding: 4px;
        }
    )");
    mainLayout->addWidget(m_statusLabel);
    
    mainLayout->addStretch();
    
    // Window styling
    setStyleSheet(R"(
        QMainWindow {
            background-color: #1E1E2E;
        }
    )");
}

void MainWindow::setupTrayIcon()
{
    m_trayIcon = new QSystemTrayIcon(this);
    
    // Create a simple icon (you can replace with actual icon file)
    QPixmap iconPixmap(32, 32);
    iconPixmap.fill(Qt::transparent);
    QPainter painter(&iconPixmap);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(QColor(102, 126, 234));
    painter.setPen(Qt::NoPen);
    painter.drawEllipse(2, 2, 28, 28);
    painter.setBrush(Qt::white);
    painter.drawEllipse(10, 10, 12, 12);
    painter.end();
    
    m_trayIcon->setIcon(QIcon(iconPixmap));
    m_trayIcon->setToolTip("Cordshot - Screenshot Tool");
    
    // Tray menu
    QMenu *trayMenu = new QMenu(this);
    
    QAction *captureAction = new QAction("Capture Region", this);
    connect(captureAction, &QAction::triggered, this, &MainWindow::startScreenshot);
    trayMenu->addAction(captureAction);
    
    trayMenu->addSeparator();
    
    QAction *showAction = new QAction("Show Window", this);
    connect(showAction, &QAction::triggered, this, &MainWindow::show);
    trayMenu->addAction(showAction);
    
    QAction *quitAction = new QAction("Quit", this);
    connect(quitAction, &QAction::triggered, qApp, &QApplication::quit);
    trayMenu->addAction(quitAction);
    
    m_trayIcon->setContextMenu(trayMenu);
    connect(m_trayIcon, &QSystemTrayIcon::activated, this, &MainWindow::trayIconActivated);
    
    m_trayIcon->show();
}

void MainWindow::startScreenshot()
{
    // Hide main window while taking screenshot
    hide();
    
    // Small delay to ensure window is hidden
    QTimer::singleShot(200, this, [this]() {
        m_overlay = new ScreenshotOverlay();
        connect(m_overlay, &ScreenshotOverlay::screenshotTaken, 
                this, &MainWindow::onScreenshotTaken);
        connect(m_overlay, &ScreenshotOverlay::cancelled, 
                this, &MainWindow::onScreenshotCancelled);
    });
}

void MainWindow::onScreenshotTaken(const QPixmap &screenshot)
{
    m_lastScreenshot = screenshot;
    
    // Update preview
    if (!screenshot.isNull()) {
        QPixmap scaled = screenshot.scaled(m_previewLabel->size() - QSize(10, 10), 
                                           Qt::KeepAspectRatio, 
                                           Qt::SmoothTransformation);
        m_previewLabel->setPixmap(scaled);
        m_statusLabel->setText(QString("✓ Captured %1×%2 • Copied to clipboard")
                              .arg(screenshot.width())
                              .arg(screenshot.height()));
        m_statusLabel->setStyleSheet(R"(
            QLabel {
                color: #4ADE80;
                font-size: 11px;
                padding: 4px;
            }
        )");
    }
    
    // Clean up overlay
    if (m_overlay) {
        m_overlay->deleteLater();
        m_overlay = nullptr;
    }
    
    // Show window again
    show();
    activateWindow();
}

void MainWindow::onScreenshotCancelled()
{
    m_statusLabel->setText("Screenshot cancelled");
    m_statusLabel->setStyleSheet(R"(
        QLabel {
            color: #F87171;
            font-size: 11px;
            padding: 4px;
        }
    )");
    
    // Clean up overlay
    if (m_overlay) {
        m_overlay->deleteLater();
        m_overlay = nullptr;
    }
    
    // Show window again
    show();
    activateWindow();
}

void MainWindow::trayIconActivated(QSystemTrayIcon::ActivationReason reason)
{
    switch (reason) {
    case QSystemTrayIcon::DoubleClick:
        show();
        activateWindow();
        break;
    case QSystemTrayIcon::Trigger:
        // Single click - start screenshot
        startScreenshot();
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    // Minimize to tray instead of closing
    if (m_trayIcon && m_trayIcon->isVisible()) {
        hide();
        m_trayIcon->showMessage("Cordshot", 
                                "Application minimized to tray. Click to capture.",
                                QSystemTrayIcon::Information, 
                                2000);
        event->ignore();
    } else {
        event->accept();
    }
}
