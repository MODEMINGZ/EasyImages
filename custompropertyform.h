#ifndef CUSTOMPROPERTYFORM_H
#define CUSTOMPROPERTYFORM_H
#include<QGraphicsItem>
#include <QWidget>
#include "graphicdrawview.h"
namespace Ui {
class CustomPropertyForm;
}

class CustomPropertyForm : public QWidget
{
    Q_OBJECT

public:
    explicit CustomPropertyForm(QWidget *parent = nullptr);
    ~CustomPropertyForm();
    void setInfo(QGraphicsItem*item,GraphicDrawView*view);
    void setDefault();
    void reSetInfo();
    QGraphicsItem*pItem=nullptr;
private slots:
    void on_doubleSpinBox_x_valueChanged(double arg1);

    void on_doubleSpinBox_y_valueChanged(double arg1);

    void on_doubleSpinBox_width_valueChanged(double arg1);

    void on_doubleSpinBox_height_valueChanged(double arg1);

    void on_spinBox_opacity_valueChanged(int arg1);

    void on_spinBox_rotate_valueChanged(int arg1);

    void on_spinBox_scale_valueChanged(int arg1);

    void on_spinBox_z_valueChanged(int arg1);

    void on_plainTextEdit_description_textChanged();

    void on_comboBox_brushType_currentIndexChanged(int index);

    void on_doubleSpinBox_centerX_valueChanged(double arg1);

    void on_doubleSpinBox_centerY_valueChanged(double arg1);

    void on_doubleSpinBox_penSize_valueChanged(double arg1);

    void on_comboBox_penStyle_currentIndexChanged(int index);

    void on_comboBox_penCap_currentIndexChanged(int index);

    void on_comboBox_penJoint_currentIndexChanged(int index);

    void on_spinBox_sAngle_valueChanged(double arg1);

    void on_spinBox_spanAngle_valueChanged(double arg1);

    void on_plainTextEdit_text_textChanged();

    void on_fontComboBox_text_currentFontChanged(const QFont &f);

    void on_checkBox_textBold_clicked(bool checked);

    void on_checkBox_textItalic_clicked(bool checked);

    void on_checkBox_textUnderline_clicked(bool checked);

    void on_checkBox_textOverline_clicked(bool checked);

    void on_checkBox_textStrikeOut_clicked(bool checked);

    void on_spinBox_textSize_valueChanged(int arg1);

    void on_spinBox_textSpan_valueChanged(double arg1);

    void on_spinBox_textWidth_valueChanged(double arg1);

    void on_comboBox_textWeight_currentIndexChanged(int index);

private:
    enum{noBrush=0,Solid,Dense1,Dense2,Dense3,Dense4,Dense5,Dense6,
           Dense7,HorPattern,VerPattern,CrossPattern,BDiagPattern,
           FDiagPattern,DiagCrossPattern,};
    enum{NoPen=0,SolidLine,DashLine,DashDotLine,DashDotDotLine};
    enum{SquareCap,FlatCap,RoundCap};
    enum{BevelJoin,MiterJoin,RoundJoin};
    enum{Thin=0,ExtraLight,Light,Normal,Medium,DemiBold,Bold,ExtraBold,Black};
    GraphicDrawView*pView=nullptr;
    void handle_info();
    Ui::CustomPropertyForm *ui;
    QString painterPathToString(const QPainterPath& path);
private slots:
    void slots_label_brushColor_colored(const QColor& color);

    void slots_label_penColor_colored(const QColor& color);

    void slots_label_textColor_colored(const QColor& color);

    void init();
    void on_pushButton_brush_clicked();
};

#endif // CUSTOMPROPERTYFORM_H
