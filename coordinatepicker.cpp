#include "coordinatepicker.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QClipboard>
#include <QGuiApplication>
#include <QMessageBox>

// ClickableImageLabel implementation
ClickableImageLabel::ClickableImageLabel(QWidget *parent)
    : QLabel(parent)
{
    setMouseTracking(true);
    setCursor(Qt::CrossCursor);
}

void ClickableImageLabel::setPixmap(const QPixmap &pixmap)
{
    m_originalPixmap = pixmap;
    QLabel::setPixmap(pixmap);
    setFixedSize(pixmap.size());
}

void ClickableImageLabel::clearPoints()
{
    m_points.clear();
    update();
}

void ClickableImageLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        QPoint pos = event->pos();
        // Ensure point is within image bounds
        if (pos.x() >= 0 && pos.x() < m_originalPixmap.width() &&
            pos.y() >= 0 && pos.y() < m_originalPixmap.height()) {
            m_points.append(pos);
            emit pointClicked(pos);
            update();
        }
    }
}

void ClickableImageLabel::mouseMoveEvent(QMouseEvent *event)
{
    m_currentPos = event->pos();
    update();
}

void ClickableImageLabel::paintEvent(QPaintEvent *event)
{
    QLabel::paintEvent(event);
    
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Draw crosshair at current mouse position
    if (rect().contains(m_currentPos)) {
        painter.setPen(QPen(QColor(0, 174, 255, 150), 1, Qt::DashLine));
        painter.drawLine(m_currentPos.x(), 0, m_currentPos.x(), height());
        painter.drawLine(0, m_currentPos.y(), width(), m_currentPos.y());
    }
    
    // Draw all clicked points
    for (int i = 0; i < m_points.size(); ++i) {
        const QPoint &pt = m_points[i];
        
        // Draw point marker
        painter.setPen(QPen(Qt::white, 2));
        painter.setBrush(QColor(255, 100, 100));
        painter.drawEllipse(pt, 6, 6);
        
        // Draw point number
        painter.setPen(Qt::white);
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(9);
        painter.setFont(font);
        
        QString numText = QString::number(i + 1);
        QRect textRect(pt.x() + 10, pt.y() - 10, 30, 20);
        
        // Background for number
        painter.setBrush(QColor(0, 0, 0, 180));
        painter.setPen(Qt::NoPen);
        painter.drawRoundedRect(textRect, 4, 4);
        
        painter.setPen(Qt::white);
        painter.drawText(textRect, Qt::AlignCenter, numText);
    }
}

// CoordinatePicker implementation
CoordinatePicker::CoordinatePicker(const QPixmap &screenshot, QWidget *parent)
    : QDialog(parent)
    , m_screenshot(screenshot)
{
    setupUI();
}

CoordinatePicker::~CoordinatePicker()
{
}

void CoordinatePicker::setupUI()
{
    setWindowTitle("Coordinate Picker - Click to get coordinates");
    setMinimumSize(600, 500);
    
    // Make it resizable
    setWindowFlags(windowFlags() | Qt::WindowMaximizeButtonHint);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    mainLayout->setSpacing(10);
    
    // Instruction label
    m_instructionLabel = new QLabel("Click on the image to get coordinates. Points are numbered in order.", this);
    m_instructionLabel->setStyleSheet(R"(
        QLabel {
            color: #E0E0E0;
            font-size: 12px;
            padding: 8px;
            background-color: #2A2A3C;
            border-radius: 6px;
        }
    )");
    mainLayout->addWidget(m_instructionLabel);
    
    // Scroll area for the image
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidgetResizable(false);
    m_scrollArea->setAlignment(Qt::AlignCenter);
    m_scrollArea->setStyleSheet(R"(
        QScrollArea {
            background-color: #1A1A2A;
            border: 2px solid #3A3A4C;
            border-radius: 8px;
        }
    )");
    
    m_imageLabel = new ClickableImageLabel();
    m_imageLabel->setPixmap(m_screenshot);
    connect(m_imageLabel, &ClickableImageLabel::pointClicked, this, &CoordinatePicker::onPointClicked);
    
    m_scrollArea->setWidget(m_imageLabel);
    mainLayout->addWidget(m_scrollArea, 1);
    
    // Coordinate display
    m_coordLabel = new QLabel("No points selected", this);
    m_coordLabel->setMinimumHeight(60);
    m_coordLabel->setWordWrap(true);
    m_coordLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
    m_coordLabel->setStyleSheet(R"(
        QLabel {
            color: #4ADE80;
            font-size: 13px;
            font-family: Consolas, monospace;
            padding: 10px;
            background-color: #1E1E2E;
            border: 1px solid #3A3A4C;
            border-radius: 6px;
        }
    )");
    mainLayout->addWidget(m_coordLabel);
    
    // Button row
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(10);
    
    m_copyLastButton = new QPushButton("📋 Copy Last", this);
    m_copyLastButton->setEnabled(false);
    m_copyLastButton->setCursor(Qt::PointingHandCursor);
    m_copyLastButton->setStyleSheet(R"(
        QPushButton {
            background-color: #3A3A4C;
            color: #D0D0E0;
            border: none;
            border-radius: 6px;
            font-size: 12px;
            padding: 10px 16px;
        }
        QPushButton:hover {
            background-color: #4A4A5C;
        }
        QPushButton:pressed {
            background-color: #2A2A3C;
        }
        QPushButton:disabled {
            background-color: #2A2A3C;
            color: #5A5A6A;
        }
    )");
    connect(m_copyLastButton, &QPushButton::clicked, this, &CoordinatePicker::copyLastCoordinate);
    buttonLayout->addWidget(m_copyLastButton);
    
    m_copyAllButton = new QPushButton("📋 Copy All", this);
    m_copyAllButton->setEnabled(false);
    m_copyAllButton->setCursor(Qt::PointingHandCursor);
    m_copyAllButton->setStyleSheet(m_copyLastButton->styleSheet());
    connect(m_copyAllButton, &QPushButton::clicked, this, &CoordinatePicker::copyAllCoordinates);
    buttonLayout->addWidget(m_copyAllButton);
    
    m_clearButton = new QPushButton("🗑️ Clear", this);
    m_clearButton->setEnabled(false);
    m_clearButton->setCursor(Qt::PointingHandCursor);
    m_clearButton->setStyleSheet(m_copyLastButton->styleSheet());
    connect(m_clearButton, &QPushButton::clicked, this, &CoordinatePicker::clearPoints);
    buttonLayout->addWidget(m_clearButton);
    
    buttonLayout->addStretch();
    
    m_closeButton = new QPushButton("Close", this);
    m_closeButton->setCursor(Qt::PointingHandCursor);
    m_closeButton->setStyleSheet(R"(
        QPushButton {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
            color: white;
            border: none;
            border-radius: 6px;
            font-size: 12px;
            font-weight: bold;
            padding: 10px 24px;
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
    connect(m_closeButton, &QPushButton::clicked, this, &QDialog::accept);
    buttonLayout->addWidget(m_closeButton);
    
    mainLayout->addLayout(buttonLayout);
    
    // Window styling
    setStyleSheet(R"(
        QDialog {
            background-color: #1E1E2E;
        }
    )");
    
    // Size the dialog based on screenshot size
    int dialogWidth = qMin(m_screenshot.width() + 40, 1200);
    int dialogHeight = qMin(m_screenshot.height() + 180, 800);
    resize(dialogWidth, dialogHeight);
}

void CoordinatePicker::onPointClicked(const QPoint &point)
{
    m_points.append(point);
    updateCoordinateDisplay();
    
    // Auto-copy to clipboard
    QString coord = QString("(%1, %2)").arg(point.x()).arg(point.y());
    QGuiApplication::clipboard()->setText(coord);
    
    m_copyLastButton->setEnabled(true);
    m_copyAllButton->setEnabled(true);
    m_clearButton->setEnabled(true);
}

void CoordinatePicker::updateCoordinateDisplay()
{
    if (m_points.isEmpty()) {
        m_coordLabel->setText("No points selected");
        return;
    }
    
    QStringList lines;
    for (int i = 0; i < m_points.size(); ++i) {
        const QPoint &pt = m_points[i];
        lines << QString("Point %1: (%2, %3)").arg(i + 1).arg(pt.x()).arg(pt.y());
    }
    
    QString lastCoord = QString("(%1, %2)").arg(m_points.last().x()).arg(m_points.last().y());
    m_coordLabel->setText(lines.join("  |  ") + "\n\n✓ Copied: " + lastCoord);
}

void CoordinatePicker::copyLastCoordinate()
{
    if (m_points.isEmpty()) return;
    
    const QPoint &pt = m_points.last();
    QString coord = QString("(%1, %2)").arg(pt.x()).arg(pt.y());
    QGuiApplication::clipboard()->setText(coord);
    
    m_coordLabel->setText(m_coordLabel->text().split("\n\n").first() + "\n\n✓ Copied: " + coord);
}

void CoordinatePicker::copyAllCoordinates()
{
    if (m_points.isEmpty()) return;
    
    QStringList coords;
    for (const QPoint &pt : m_points) {
        coords << QString("(%1, %2)").arg(pt.x()).arg(pt.y());
    }
    
    QString allCoords = coords.join(", ");
    QGuiApplication::clipboard()->setText(allCoords);
    
    m_coordLabel->setText(m_coordLabel->text().split("\n\n").first() + "\n\n✓ Copied all: " + allCoords);
}

void CoordinatePicker::clearPoints()
{
    m_points.clear();
    m_imageLabel->clearPoints();
    updateCoordinateDisplay();
    
    m_copyLastButton->setEnabled(false);
    m_copyAllButton->setEnabled(false);
    m_clearButton->setEnabled(false);
    
    m_coordLabel->setText("No points selected");
}
