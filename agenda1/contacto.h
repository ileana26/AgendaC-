#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_buscar_textChanged(const QString &arg1);

    void on_agregar_clicked();

    void on_pushButton_clicked();

    void on_agregarnuevo_clicked();

    void on_pushButton_2_clicked();

    void on_editarboton_clicked();

    void on_pushButton_5_clicked();

    //void on_pushButton_3_clicked();

    void on_actualizar_clicked();

    void on_eliminarboton_clicked();

    void on_perfil_clicked();

    void on_volver_clicked();

    void on_guardar_clicked();

    void on_grupos_clicked();

    void on_regresar_clicked();

private:
    Ui::MainWindow *ui;
QSqlDatabase  conexion;
};
#endif // MAINWINDOW_H
