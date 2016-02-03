#include "popupcolor.h"
#include "settings.h"
#include <windows.h>
#include <QSettings>
#include <QDir>
#include <QPainter>
#include <QApplication>
#include <QDesktopWidget>
#include <QDebug>
#include <QVBoxLayout>
#include <QScreen>
#include <QClipboard>
#include <QDateTime>
#include <QPolygonF>
#include <QLineEdit>
#include <QGraphicsDropShadowEffect>
#include <cmath>
#include "popupmessage.h"

PopUpColor::PopUpColor(QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::FramelessWindowHint |
                   Qt::Tool |
                   Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_ShowWithoutActivating);

    animation.setTargetObject(this);
    animation.setPropertyName("popupOpacity");

    setLayout(&layout);
    layout.setSpacing(0);

    layout.addWidget(&popUpWidget,0,0);
    popUpWidget.setLayout(&layoutPopUp);
    label.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    layoutPopUp.addWidget(&label,0,0,1,3);
    layoutPopUp.addWidget(&closeButton,0,3);
    layoutPopUp.addWidget(&pickerButton,1,0);
    layoutPopUp.addWidget(&gradationButton,1,1);
    comboBox.addItems(QStringList() << "HEX" << "RGB" << "CMYK" << "HSV" << "HSL");
    comboBox.setCurrentIndex(0);
    layoutPopUp.addWidget(&comboBox,1,2,1,2);

    layout.addWidget(&sliderWidget,1,0);
    sliderWidget.setLayout(&layoutSlider);
    sliderSaturation.setOrientation(Qt::Horizontal);
    sliderSaturation.setRange(-100,100);
    layoutSlider.addWidget(&sliderSaturation,0,0);
    sliderBrightness.setOrientation(Qt::Horizontal);
    sliderBrightness.setRange(-100,100);
    layoutSlider.addWidget(&sliderBrightness,1,0);
    sliderWidget.setStyleSheet("QWidget { background-color: #ffffff; padding: 0px; margin: 0px;}");

    layout.addWidget(&gradationWidget,2,0);
    gradationWidget.setLayout(&layoutGradation);
    layoutGradation.setSpacing(0);
    layoutGradation.setContentsMargins(7,0,7,1);
    label_10.setCurrentLightness(0.1);
    label_20.setCurrentLightness(0.2);
    label_30.setCurrentLightness(0.3);
    label_40.setCurrentLightness(0.4);
    label_50.setCurrentLightness(0.5);
    label_60.setCurrentLightness(0.6);
    label_70.setCurrentLightness(0.7);
    label_80.setCurrentLightness(0.8);
    label_90.setCurrentLightness(0.9);
    label_100.setCurrentLightness(1.0);
    layoutGradation.addWidget(&label_10,9,0);
    layoutGradation.addWidget(&label_20,8,0);
    layoutGradation.addWidget(&label_30,7,0);
    layoutGradation.addWidget(&label_40,6,0);
    layoutGradation.addWidget(&label_50,5,0);
    layoutGradation.addWidget(&label_60,4,0);
    layoutGradation.addWidget(&label_70,3,0);
    layoutGradation.addWidget(&label_80,2,0);
    layoutGradation.addWidget(&label_90,1,0);
    layoutGradation.addWidget(&label_100,0,0);

    connect(&comboBox, static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged), this, &PopUpColor::changeIndexComboBoxColor);
    connect(&label, &CodeLabel::setPos, this, &PopUpColor::showPos);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::slotHide);
    connect(&closeButton, &QToolButton::clicked, &dummyTransparentWindow, &TransparentWindow::hide);
    connect(&closeButton, &QToolButton::clicked, this, &PopUpColor::backColor);
    connect(&gradationButton, &QToolButton::clicked, this, &PopUpColor::slotGradationButtonClicked);
    connect(&pickerButton, &QToolButton::clicked, this, &PopUpColor::pickerButtonClicked);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeLabelText);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeStyleSheets);
    connect(this, &PopUpColor::currentColorChanged, this, &PopUpColor::changeSliders);
    connect(&dummyTransparentWindow, &TransparentWindow::changeColor, this, &PopUpColor::setCurrentColor);
    connect(&dummyTransparentWindow, &TransparentWindow::backColor, this, &PopUpColor::backColor);
    connect(&dummyTransparentWindow, &TransparentWindow::saveColor, this, &PopUpColor::saveColor);

    connect(this, &PopUpColor::currentColorChanged, &label_10, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_20, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_30, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_40, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_50, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_60, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_70, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_80, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_90, &GradationLabel::setCurrentColor);
    connect(this, &PopUpColor::currentColorChanged, &label_100, &GradationLabel::setCurrentColor);

    connect(&label_10, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_20, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_30, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_40, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_50, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_60, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_70, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_80, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_90, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);
    connect(&label_100, &GradationLabel::colorForCopy, this, &PopUpColor::slotCopyBuffer);

    reloadSettings();
    setCurrentColor(QColor(Qt::white));
    tempCurrentColor = QColor(Qt::white);

    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    adjustSize();
}

void PopUpColor::saveSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    settings.setValue(SETTINGS_POS_X,posWin.x());
    settings.setValue(SETTINGS_POS_Y,posWin.y());
}

PopUpColor::~PopUpColor()
{
}

bool PopUpColor::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    Q_UNUSED(eventType)
    Q_UNUSED(result)
#ifdef Q_OS_WIN32
    MSG* msg = reinterpret_cast<MSG*>(message);

    if(msg->message == WM_HOTKEY){

        switch (msg->wParam) {
        case 100: {
            if(!dummyTransparentWindow.isVisible()){
                QScreen *screen = QApplication::primaryScreen();
                QImage img = screen->grabWindow(0).toImage();
                QColor color;
                color.setRgb(img.pixel(QCursor::pos()));;
                setCurrentColor(color);
                tempCurrentColor = color;
                (followCursor) ? showPos(QCursor::pos()) : show();
                slotCopyBuffer(getCurrentColor());
            }
            return true;
            break;
        }
        case 101: {
            QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
            QPixmap pixmap = QApplication::primaryScreen()->grabWindow(0);
            pixmap.save(settings.value(SETTINGS_PATH_SCREENSHOTS, QVariant()).toString() +
                        QDateTime::currentDateTime().toString("/Screen_yyyy-MM-dd_hh-mm-ss") +
                        ".png", "PNG");
            QApplication::clipboard()->setPixmap(pixmap);
            return true;
            break;
        }
        default:
            break;
        }
    }
#endif
    return false;
}

void PopUpColor::show()
{
    adjustSize();
    int x,y;
    if(posWin == QPoint(0,0)){
        x = QApplication::desktop()->availableGeometry().width() - 36 - width() + QApplication::desktop() -> availableGeometry().x();
        y = QApplication::desktop()->availableGeometry().height() - 36 - height() + QApplication::desktop() -> availableGeometry().y();
    }else{
        x = posWin.x();
        y = posWin.y();
    }
    setGeometry(x,y,width(),height());

    if(!isVisible()){
        setWindowOpacity(0.0);

        animation.setDuration(150);
        animation.setStartValue(0.0);
        animation.setEndValue(1.0);

        QWidget::show();

        animation.start();
    } else {
        animation.setDuration(windowOpacity()*150);
        animation.setStartValue(windowOpacity());
        animation.setEndValue(1.0);
        animation.start();
    }
}

void PopUpColor::showPos(QPoint point)
{
    adjustSize();
    int x,y;
    if(QApplication::mouseButtons() == Qt::LeftButton){
        x = point.x();
        y = point.y();
    } else {
        x = ((QApplication::desktop()->width() - point.x()) < width())? point.x() - width() : point.x();
        y = ((QApplication::desktop()->height() - point.y()) < height())? point.y() - height() : point.y();
    }
    setGeometry(x,y,width(),height());

    if(!isVisible()){
        setWindowOpacity(0.0);

        animation.setDuration(150);
        animation.setStartValue(0.0);
        animation.setEndValue(1.0);

        QWidget::show();

        animation.start();
    } else {
        animation.setDuration(windowOpacity()*150);
        animation.setStartValue(windowOpacity());
        animation.setEndValue(1.0);
        animation.start();
    }
    posWin = point;
}

void PopUpColor::reloadSettings()
{
    QSettings settings(ORGANIZATION_NAME, APPLICATION_NAME);
    copyBuffer = settings.value(SETTINGS_COPY_BUFF, false).toBool();
    typeCopyBuffer = settings.value(SETTINGS_TYPE_BUFF, 0).toInt();
    followCursor = settings.value(SETTINGS_FOLLOW_CURSOR, false).toBool();
    posWin.setX(settings.value(SETTINGS_POS_X,0).toInt());
    posWin.setY(settings.value(SETTINGS_POS_Y,0).toInt());
#ifdef Q_OS_WIN32
    UnregisterHotKey((HWND) PopUpColor::winId(), 101);
    if(settings.value(SETTINGS_ALLOW_SCREENSHOTS, false).toBool()){
        RegisterHotKey((HWND)PopUpColor::winId(), 101, 0, VK_SNAPSHOT);
    }
#endif
    keys = QKeySequence(settings.value(KEY_SEQUENCE_PIXEL, QVariant()).toString());
#ifdef Q_OS_WIN32
    UnregisterHotKey((HWND)PopUpColor::winId(), 100);
    RegisterHotKey((HWND)PopUpColor::winId(), 100, winKeyModificator(keys), winHotKey(keys));
#endif
    comboBox.setCurrentIndex(typeCopyBuffer);
}

void PopUpColor::pickerButtonClicked()
{
    tempCurrentColor = getCurrentColor();
    dummyTransparentWindow.showFullScreen();
}

void PopUpColor::hideAnimation()
{
    animation.setDuration(1000);
    animation.setStartValue(1.0);
    animation.setEndValue(0.0);
    animation.start();
}

void PopUpColor::changeIndexComboBoxColor(int index)
{
    typeCopyBuffer = index;
    changeLabelText(getCurrentColor());
    slotCopyBuffer(getCurrentColor());
}

void PopUpColor::backColor()
{
    setCurrentColor(tempCurrentColor);
}

void PopUpColor::saveColor()
{
    tempCurrentColor = getCurrentColor();
    slotCopyBuffer(getCurrentColor());
}

void PopUpColor::changeStyleSheets(const QColor &color)
{
    QColor c;
    qreal lightness = color.lightnessF();
    if(lightness < 0.5){
        lightness += 0.09;
    } else {
        lightness -= 0.09;
    }

    c.setHslF(color.hslHueF(),
                  color.hslSaturationF(),
                  lightness);
    QString strColor = c.name();
    QString fontColor = (color.lightnessF() > 0.7) ? "#000000" : "#ffffff";
    comboBox.setStyleSheet("QComboBox { color: " + fontColor + "; background-color: " + strColor + "; "
                           "border: none; border-radius: 2px;"
                           "padding: 6px;"
                           "font-size: 14px; }"
                           "QComboBox::drop-down {border: none;} "
                           "QComboBox::down-arrow {image: url(noimg); border: none;}"
                           "QComboBox QAbstractItemView { "
                           "border: none;"
                           "color: " + fontColor + "; "
                           "background-color: " + strColor + "; "
                           "border-radius: 2px;"
                           "padding: 6px;"
                           "selection-color: " + fontColor + ";"
                           "selection-background-color: " + color.name() + ";}");
    if(color.lightnessF() > 0.7){
        label.setStyleSheet("QLabel { color: black; border: none; font-size: 16px; }");
        pickerButton.setStyleSheet("QToolButton { image: url(:/images/eyedropper-black.png);"
                                   "icon-size: 16px;"
                                   "height: 16px;"
                                   "width: 16px;"
                                   "padding: 6px;"
                                   "border: none;"
                                   "border-radius: 2px;"
                                   "background-color: " + strColor + "; }"
                                   "QToolButton:pressed { background-color: transparent; }");
        gradationButton.setStyleSheet("QToolButton { image: url(:/images/invert-colors-black.png);"
                                      "icon-size: 16px;"
                                      "height: 16px;"
                                      "width: 16px;"
                                      "padding: 6px;"
                                      "border: none;"
                                      "border-radius: 2px;"
                                      "background-color: " + strColor + "; }"
                                      "QToolButton:pressed { background-color: transparent; }");
        closeButton.setStyleSheet("QToolButton { image: url(:/images/close-circle-outline-black.png);"
                                  "icon-size: 16px;"
                                  "height: 16px;"
                                  "width: 16px;"
                                  "border: none;}"
                                  "QToolButton:pressed { image: url(:/images/close-circle-black.png);}");
    } else {
        label.setStyleSheet("QLabel { color: white; border: none; font-size: 16px; }");
        pickerButton.setStyleSheet("QToolButton { image: url(:/images/eyedropper.png);"
                                   "icon-size: 16px;"
                                   "height: 16px;"
                                   "width: 16px;"
                                   "padding: 6px;"
                                   "border: none;"
                                   "border-radius: 2px;"
                                   "background-color: " + strColor + "; }"
                                   "QToolButton:pressed { background-color: transparent; }");
        gradationButton.setStyleSheet("QToolButton { image: url(:/images/invert-colors.png);"
                                  "icon-size: 16px;"
                                  "height: 16px;"
                                  "width: 16px;"
                                  "padding: 6px;"
                                  "border: none;"
                                  "border-radius: 2px;"
                                  "background-color: " + strColor + "; }"
                                  "QToolButton:pressed { background-color: transparent; }");
        closeButton.setStyleSheet("QToolButton { image: url(:/images/close-circle-outline.png);"
                                  "icon-size: 16px;"
                                  "height: 16px;"
                                  "width: 16px;"
                                  "border: none;}"
                                  "QToolButton:pressed { image: url(:/images/close-circle.png);}");
    }
    sliderWidget.setStyleSheet("QWidget { background-color: " + color.name() + ";"
                               "border: 1px solid " + c.name() + ";"
                               "margin-left: 6px;"
                               "margin-right: 6px;}");
    gradationWidget.setStyleSheet("QWidget { background-color: transparent;"
                                  "border: 1px solid " + c.name() + ";"
                                  "margin-left: 6px;"
                                  "margin-right: 6px;}");
    popUpWidget.setStyleSheet("QWidget { background-color: " + color.name() + ";"
                              "border: 1px solid " + c.name() + ";"
                              "border-radius: 2px;}");
    sliderBrightness.setStyleSheet("QSlider { border: none; margin: 0px; padding: 0px;}");
    sliderSaturation.setStyleSheet("QSlider {border: none; margin: 0px; padding: 0px;}");
    adjustSize();
}

void PopUpColor::changeLabelText(const QColor &color)
{
    switch (typeCopyBuffer) {
    case 0:
        label.setText(color.name());
        break;
    case 1:
        label.setText("RGB:\t" +
                      QString::number(color.red()) + " " +
                      QString::number(color.green()) + " " +
                      QString::number(color.blue()));
        break;
    case 2:
        label.setText("CMYK:\t" +
                      QString::number(color.cyan()) + " " +
                      QString::number(color.magenta()) + " " +
                      QString::number(color.yellow()) + " " +
                      QString::number(color.black()));
        break;
    case 3:
        label.setText("HSV:\t" +
                      QString::number(color.hsvHue()) + " " +
                      QString::number(round(color.hsvSaturationF()*100)) + "% " +
                      QString::number(round(color.valueF()*100)) + "%");
        break;
    case 4:
        label.setText("HSL:\t" +
                      QString::number(color.hslHue()) + " " +
                      QString::number(round(color.hslSaturationF()*100)) + "% " +
                      QString::number(round(color.lightnessF()*100)) + "%");
        break;
    default:
        break;
    }
    adjustSize();
}

void PopUpColor::changeSliders()
{
    sliderBrightness.setValue(0);
    sliderSaturation.setValue(0);
}

void PopUpColor::slotCopyBuffer(const QColor &color)
{
    if(copyBuffer){
        switch (typeCopyBuffer) {
        case 0: // HEX
            QApplication::clipboard()->setText(color.name());
            break;
        case 1: // RGB
            QApplication::clipboard()->setText(QString::number(color.red()) + " " +
                                               QString::number(color.green()) + " " +
                                               QString::number(color.blue()));
            break;
        case 2: // CMYK
            QApplication::clipboard()->setText(QString::number(color.cyan()) + " " +
                                               QString::number(color.magenta()) + " " +
                                               QString::number(color.yellow()) + " " +
                                               QString::number(color.black()));
            break;
        case 3: // HSV
            QApplication::clipboard()->setText(QString::number(color.hue()) + " " +
                                               QString::number(round(color.hsvSaturationF()*100)) + " " +
                                               QString::number(round(color.valueF()*100)));
            break;
        case 4: // HSL
            QApplication::clipboard()->setText(QString::number(color.hslHue()) + " " +
                                               QString::number(round(color.hslSaturationF()*100)) + " " +
                                               QString::number(round(color.lightnessF()*100)));
            break;
        default:
            break;
        }
        PopUpMessage::information(this, "Скопировано в буфер обмена");
    }
}

#ifdef Q_OS_WIN32
unsigned int PopUpColor::winKeyModificator(QKeySequence sequence)
{
    QStringList list = sequence.toString().split("+");
    unsigned int keyModificator = 0;

    foreach (QString str, list) {
        if(str == "Ctrl"){
            keyModificator += MOD_CONTROL;
            continue;
        } else if(str == "Alt"){
            keyModificator += MOD_ALT;
            continue;
        } else if(str == "Shift"){
            keyModificator += MOD_SHIFT;
            continue;
        }
    }
    return keyModificator;
}

char PopUpColor::winHotKey(QKeySequence sequence)
{
    QStringList list = sequence.toString().split("+");
    char hotKey = 'E';

    foreach (QString str, list) {
        if(str != "Ctrl" && str != "Alt" && str != "Shift"){
            hotKey = str.at(0).unicode();
        }
    }
    return hotKey;
}
#endif

void PopUpColor::setPopupOpacity(float opacity)
{
    popupOpacity = opacity;
    setWindowOpacity(opacity);
}

void PopUpColor::setCurrentColor(QColor color)
{
    currentColor = color;
    emit currentColorChanged(color);
}

float PopUpColor::getPopupOpacity() const
{
    return popupOpacity;
}

QColor PopUpColor::getCurrentColor() const
{
    return currentColor;
}

void PopUpColor::slotHide()
{
    posWin = pos();
    gradationWidget.setVisible(false);
    sliderWidget.setVisible(false);
    adjustSize();
    hide();
}

void PopUpColor::slotGradationButtonClicked()
{
    (gradationWidget.isVisible()) ? gradationWidget.setVisible(false) : gradationWidget.setVisible(true);
    (sliderWidget.isVisible()) ? sliderWidget.setVisible(false) : sliderWidget.setVisible(true);
    adjustSize();
}
