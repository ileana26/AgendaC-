#include "contacto.h"
#include "ui_contacto.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    conexion = QSqlDatabase::addDatabase("QODBC");
    //conexion.setPort(3306);
    conexion.setHostName("root");
    conexion.setPassword("");
    conexion.setDatabaseName("agenda");

    if(conexion.open())
        qDebug()<< "Conexión EXITOSA";
    else
        qDebug()<< "Conexión FALLIDA";

    QStringList titulos;
        ui->tablacontacto->setColumnCount(7);
        titulos << "ID" << "Nombre" << "Numero" <<"Correo electronico" << "Cumpleaños" << "Sexo" << "Domicilio";
        ui->tablacontacto->setHorizontalHeaderLabels(titulos);

        QSqlQuery con;
        con.prepare("select * from contacto");
        con.exec();
        while(con.next()){

                QString idcon= con.value(0).toString();
                QString nombreCompleto= con.value(1).toString() + " "+ con.value(2).toString()+" "+con.value(3).toString();
                QString numero= con.value(4).toString();
                QString correo = con.value(5).toString();
                QString cumple= con.value(6).toString();
                QString sexo = con.value(7).toString();
                QString dom = con.value(8).toString();


                ui->tablacontacto->insertRow(ui->tablacontacto->rowCount());

                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,0,new QTableWidgetItem(idcon));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,1,new QTableWidgetItem(nombreCompleto));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,2,new QTableWidgetItem(numero));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,3,new QTableWidgetItem(correo));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,4,new QTableWidgetItem(cumple));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,5,new QTableWidgetItem(sexo));
                ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,6,new QTableWidgetItem(dom));
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_buscar_textChanged(const QString &nombree)
{

    QSqlQuery buscarcontacto;
    buscarcontacto.prepare("select  * from contacto where nombre LIKE '%"+nombree+"%';");
    if(buscarcontacto.exec()){
       ui->tablacontacto->setRowCount(0);
       while (buscarcontacto.next()) {
           QString idcon= buscarcontacto.value(0).toString();
           QString nombreCompleto= buscarcontacto.value(1).toString() + " "+ buscarcontacto.value(2).toString()+" "+buscarcontacto.value(3).toString();
           QString numero= buscarcontacto.value(4).toString();
           QString correo = buscarcontacto.value(5).toString();
           QString cumple= buscarcontacto.value(6).toString();
           QString sexo = buscarcontacto.value(7).toString();
           QString dom = buscarcontacto.value(8).toString();

            ui->tablacontacto->insertRow(ui->tablacontacto->rowCount());

            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,0,new QTableWidgetItem(idcon));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,1,new QTableWidgetItem(nombreCompleto));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,2,new QTableWidgetItem(numero));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,3,new QTableWidgetItem(correo));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,4,new QTableWidgetItem(cumple));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,5,new QTableWidgetItem(sexo));
            ui->tablacontacto->setItem(ui->tablacontacto->rowCount()-1,6,new QTableWidgetItem(dom));

}
}
}

void MainWindow::on_agregar_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);
}

void MainWindow::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_agregarnuevo_clicked()
{
    QString nom = ui->nombre->text();
    QString ap = ui->apellidop->text();
    QString am = ui->apellidoma->text();
    QString num= ui->numero->text();
    QString correo= ui->correo->text();
    QString cumplea= ui->cumpleanios->text();
    QString sex = ui->sexo->text();
    QString domicilio = ui->domi->text();

    QMessageBox msg;

    //agregar datos
    if(nom.length()>0 && ap.length()>0 && am.length()>0 &&  num.length()>0 && correo.length()>0  && cumplea.length()>0 && sex.length()>0 && domicilio.length()>0)
       {
                 msg.setWindowTitle("Datos Correctos");
                 msg.setIcon(QMessageBox::Question);
                 msg.setText("¿Estas seguro de agregar un nuevo contacto?");
                 QAbstractButton * pButtonYes = msg.addButton(tr("SI"),QMessageBox::YesRole);
                 msg.addButton(tr("NO"),QMessageBox::NoRole);
                 msg.exec();
                 if(msg.clickedButton()==pButtonYes){

                 QSqlQuery nuevo;
                 nuevo.prepare("call InsertarContacto('"+nom+"','"+ap+"','"+am+"','"+num+"','"+correo+"','"+cumplea+"','"+sex+"','"+domicilio+"')");
                 nuevo.exec();


            QMessageBox msg1;
            msg.setWindowTitle("Registro exitoso");
               msg1.setIcon(QMessageBox::Information);
               msg1.setText("El contacto ha sido registrado");
               msg.addButton(tr("Aceptar"),QMessageBox::YesRole);
               msg1.exec();

               foreach(QLineEdit* le, findChildren<QLineEdit*>()) {
                  le->clear();
               }
                 }
                 foreach(QLineEdit* le, findChildren<QLineEdit*>()) {
                    le->clear();
                 }
    }
    else
    {
        msg.setWindowTitle("Campos vacios");
               msg.setIcon(QMessageBox::Warning);
               msg.setText("Por favor rellene todos los campos");
               msg.addButton(tr("Aceptar"),QMessageBox::YesRole);
               msg.exec();
    }

}

void MainWindow::on_pushButton_2_clicked()
{
      this->close();
}

void MainWindow::on_editarboton_clicked()
{
    if(ui->buscar->text() == ""){
        QMessageBox::critical(this,"Error","Escribe un nombre.","Aceptar");
        return;
    }
    else
    {
      ui->stackedWidget->setCurrentIndex(2);
      bool busqueda;
      QMessageBox msg;
      QSqlQuery mostrar;
      QString nombree = ui->buscar->text();

      if(nombree.length()>0)
      {
      mostrar.prepare("select * from contacto WHERE nombre = '"+nombree+"';");
      mostrar.exec();
      while(mostrar.next())
      {

      QString id = mostrar.value(0).toString();
      QString nombre1 = mostrar.value(1).toString();
      QString ape= mostrar.value(2).toString();
      QString apem = mostrar.value(3).toString();
      QString nume = mostrar.value(4).toString();
      QString correo = mostrar.value(5).toString();
      QString fecha = mostrar.value(6).toString();
      QString sexo = mostrar.value(7).toString();
      QString dom = mostrar.value(8).toString();

      ui->nombrenuevo->setText(nombre1);
      ui->apnuevo->setText(ape);
      ui->amnuevo->setText(apem);
      ui->numeronuevo->setText(nume);
      ui->correonuevo->setText(correo);
      ui->cumplenuevo->setText(fecha);
      ui->sexonuevo->setText(sexo);
      ui->domicilionuevo->setText(dom);


      busqueda =1;
      }
      if(busqueda == 0)
      {
          msg.setWindowTitle("Error busqueda");
                 msg.setIcon(QMessageBox::Critical);
                 msg.setText("El usuario ingresado no se encuentra en el sistema");
                 msg.addButton(tr("Aceptar"),QMessageBox::YesRole);
                 msg.exec();
      }


      }
      }


  }



void MainWindow::on_pushButton_5_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}



void MainWindow::on_actualizar_clicked()
{
    QString nombree=ui->nombrenuevo->text();
    QString apenuevo=ui->apnuevo->text();
    QString amanuevo=ui->amnuevo->text();
    QString numnuevo=ui->numeronuevo->text();
    QString correnuevo=ui->correonuevo->text();
    QString fechan = ui->cumplenuevo->text();
    QString se = ui->sexonuevo->text();
    QString dominuevo = ui->domicilionuevo->text();

    QSqlQuery actualizar;

     if(nombree.length()>0 && apenuevo.length()>0 && amanuevo.length()>0 &&  numnuevo.length()>0 && correnuevo.length()>0  && fechan.length()>0 && se.length()>0 && dominuevo.length()>0)
     {
         QMessageBox msg;
         msg.setWindowTitle("Confirmación");
          msg.setIcon(QMessageBox::Question);
          msg.setText("¿Seguro de guardar cambios?");
          QAbstractButton * pButtonYes = msg.addButton(tr("SI"),QMessageBox::YesRole);
          msg.addButton(tr("NO"),QMessageBox::NoRole);
          msg.exec();

          if(msg.clickedButton()==pButtonYes){

            qDebug() << "si entra";

            ///botones de grupo
            ///
            if(ui->familia->isChecked()){
                  QSqlQuery familiaa;
                  familiaa.prepare("Select idContacto from contacto where nombre = '"+nombree+"';");
                  familiaa.exec();

                  if(familiaa.next()){
                      QString id = familiaa.value(0).toString();
                      qDebug() << id;

                      QString fam = "Familia";
                      QSqlQuery agregar;
                      agregar.prepare("CALL InsertarGrupoFamilia ('"+fam+"','"+id+"')");
                      agregar.exec();
                  }
            }

            if(ui->amigos->isChecked()){
                  QSqlQuery amigoss;
                  amigoss.prepare("Select idContacto from contacto where nombre = '"+nombree+"';");
                  amigoss.exec();

                  if(amigoss.next()){
                      QString id = amigoss.value(0).toString();
                      qDebug() << id;

                      QString am = "Amigos";
                      QSqlQuery agregar;
                      agregar.prepare("CALL InsertarGrupoAmigos ('"+am+"','"+id+"')");
                      agregar.exec();
                  }
            }

            if(ui->trabajo->isChecked()){
                  QSqlQuery trabajoo;
                  trabajoo.prepare("Select idContacto from contacto where nombre = '"+nombree+"';");
                  trabajoo.exec();

                  if(trabajoo.next()){
                      QString id = trabajoo.value(0).toString();
                      qDebug() << id;

                      QString tra = "Trabajo";
                      QSqlQuery agregar;
                      agregar.prepare("CALL InsertarGrupoTrabajo ('"+tra+"','"+id+"')");
                      agregar.exec();
                  }
            }


              actualizar.prepare("update contacto SET nombre='"+nombree+"', apellidor='"+apenuevo+"' , apellidom='"+amanuevo+"' , numero='"+numnuevo+"' , correo='"+correnuevo+"' , fechaNa = '"+fechan+"', sexo = '"+se+"', domicilio = '"+dominuevo+"' where nombre= '"+nombree+"'");
              actualizar.exec();
              QMessageBox::information(this, "Aviso", "Datos actualizados", "Aceptar");
              }

             }
     else {
            QMessageBox msg;
            msg.setWindowTitle("Datos incorrectos");
             msg.setIcon(QMessageBox::Critical);
             msg.setText("Datos incorrectos");
             msg.addButton(tr("VOLVER"),QMessageBox::YesRole);
             msg.exec();
     }
}

void MainWindow::on_eliminarboton_clicked()
{
    QSqlQuery borrar;

    if(ui->buscar->text()==""){
        QMessageBox msg1;
        msg1.setWindowTitle("Campos vacios");
        msg1.setIcon(QMessageBox::Warning);
        msg1.setText("Por favor ingrese un nombre");
        msg1.addButton(tr("Aceptar"),QMessageBox::YesRole);
        msg1.exec();

    }
    else
    {
    QString nombree=ui->buscar->text();
        QMessageBox msg;
        msg.setWindowTitle("Eliminación");
         msg.setIcon(QMessageBox::Question);
         msg.setText("¿Seguro de eliminar a '"+nombree+"'");
         QAbstractButton * pButtonYes = msg.addButton(tr("SI"),QMessageBox::YesRole);
         msg.addButton(tr("NO"),QMessageBox::NoRole);
         msg.exec();

         if(msg.clickedButton()==pButtonYes)
         {

             QString contac=ui->buscar->text();

    QSqlQuery eliminar;
    eliminar.prepare("delete from contacto where nombre='"+contac+"';");
    eliminar.exec();
    eliminar.next();
    QMessageBox::information(this, "Aviso", "Contacto eliminado con éxito", "Aceptar");

    }
}
}

void MainWindow::on_perfil_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);
    QSqlQuery mostrar;

    mostrar.prepare("select * from usuario");
    mostrar.exec();
    while(mostrar.next())
    {
    QString nombreusu = mostrar.value(1).toString();
    QString apeusu= mostrar.value(2).toString();
    QString apemusu = mostrar.value(3).toString();
    QString numeusu = mostrar.value(4).toString();
    QString casausu = mostrar.value(5).toString();
    QString correousu = mostrar.value(6).toString();
    QString fechausu = mostrar.value(7).toString();
    QString nota = mostrar.value(8).toString();

    ui->nombreusuario->setText(nombreusu);
    ui->apeusuario->setText(apeusu);
    ui->amusuario->setText(apemusu);
    ui->numusuario->setText(numeusu);
    ui->numcasa->setText(casausu);
    ui->correousuario->setText(correousu);
    ui->cumpleusuario->setText(fechausu);
    ui->notas->setText(nota);



    }
}

void MainWindow::on_volver_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::on_guardar_clicked()
{
    QString nombreeusu=ui->nombreusuario->text();
    QString apenuevousu=ui->apeusuario->text();
    QString amanuevousu=ui->amusuario->text();
    QString numnuevousu=ui->numusuario->text();
    QString numcasausu = ui->numcasa->text();
    QString correnuevousu=ui->correousuario->text();
    QString fechanusu = ui->cumpleusuario->text();
    QString notausu = ui->notas->toPlainText();

    QSqlQuery actualizar;

     if(nombreeusu.length()>0 && apenuevousu.length()>0 && amanuevousu.length()>0 &&  numnuevousu.length()>0 && numcasausu.length()>=0 && correnuevousu.length()>0  && fechanusu.length()>0 && notausu.length()>0)
     {
         QMessageBox msg;
         msg.setWindowTitle("Confirmación");
          msg.setIcon(QMessageBox::Question);
          msg.setText("¿Seguro de guardar cambios?");
          QAbstractButton * pButtonYes = msg.addButton(tr("SI"),QMessageBox::YesRole);
          msg.addButton(tr("NO"),QMessageBox::NoRole);
          msg.exec();

          if(msg.clickedButton()==pButtonYes){

            qDebug() << "si entra";
              actualizar.prepare("update usuario SET nombre='"+nombreeusu+"', apellidor='"+apenuevousu+"' , apellidom='"+amanuevousu+"' , numero='"+numnuevousu+"' , numcasa ='"+numcasausu+"' ,correo='"+correnuevousu+"' , cumpleanios = '"+fechanusu+"', notas = '"+notausu+"' where idUsuario= 0");
              actualizar.exec();
              QMessageBox::information(this, "Aviso", "Datos actualizados", "Aceptar");
              }

             }
     else {
            QMessageBox msg;
            msg.setWindowTitle("Datos incorrectos");
             msg.setIcon(QMessageBox::Critical);
             msg.setText("Datos incorrectos");
             msg.addButton(tr("VOLVER"),QMessageBox::YesRole);
             msg.exec();
     }
}

void MainWindow::on_grupos_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);

    //tabla familia
    QStringList titulos;
        ui->tablafamilia->setColumnCount(3);
        titulos << "ID" << "Nombre" << "Numero";
        ui->tablafamilia->setHorizontalHeaderLabels(titulos);

        QSqlQuery con;
        con.prepare("select idContacto, nombre, apellidor, apellidom, numero from contacto inner join grupoFamilia on idContacto = id_contacto ");
        con.exec();
        while(con.next()){

                QString idC= con.value(0).toString();
                QString nombreCompleto= con.value(1).toString() + " "+ con.value(2).toString()+" "+con.value(3).toString();
                QString numero= con.value(4).toString();

                qDebug()<< idC << nombreCompleto << numero;

                ui->tablafamilia->insertRow(ui->tablafamilia->rowCount());

                ui->tablafamilia->setItem(ui->tablafamilia->rowCount()-1,0,new QTableWidgetItem(idC));
                ui->tablafamilia->setItem(ui->tablafamilia->rowCount()-1,1,new QTableWidgetItem(nombreCompleto));
                ui->tablafamilia->setItem(ui->tablafamilia->rowCount()-1,2,new QTableWidgetItem(numero));

        }

        //tabla amigos
        QStringList titulos1;
            ui->tablafamilia->setColumnCount(3);
            titulos1 << "ID" << "Nombre" << "Numero";
            ui->tablafamilia->setHorizontalHeaderLabels(titulos1);

            QSqlQuery con1;
            con1.prepare("select idContacto, nombre, apellidor, apellidom, numero from contacto inner join grupoAmigos on idContacto = id_contacto ");
            con1.exec();
            while(con1.next()){

                    QString idC= con1.value(0).toString();
                    QString nombreCompleto= con1.value(1).toString() + " "+ con1.value(2).toString()+" "+con1.value(3).toString();
                    QString numero= con1.value(4).toString();

                    qDebug()<< idC << nombreCompleto << numero;

                    ui->tablaamigos->insertRow(ui->tablaamigos->rowCount());

                    ui->tablaamigos->setItem(ui->tablaamigos->rowCount()-1,0,new QTableWidgetItem(idC));
                    ui->tablaamigos->setItem(ui->tablaamigos->rowCount()-1,1,new QTableWidgetItem(nombreCompleto));
                    ui->tablaamigos->setItem(ui->tablaamigos->rowCount()-1,2,new QTableWidgetItem(numero));

        }

            //tabla trabajo
            QStringList titulos2;
                ui->tablafamilia->setColumnCount(3);
                titulos2 << "ID" << "Nombre" << "Numero";
                ui->tablafamilia->setHorizontalHeaderLabels(titulos2);

                QSqlQuery con2;
                con2.prepare("select idContacto, nombre, apellidor, apellidom, numero from contacto inner join grupoTrabajo on idContacto = id_contacto ");
                con2.exec();
                while(con2.next()){

                        QString idC= con2.value(0).toString();
                        QString nombreCompleto= con2.value(1).toString() + " "+ con2.value(2).toString()+" "+con2.value(3).toString();
                        QString numero= con2.value(4).toString();

                        qDebug()<< idC << nombreCompleto << numero;

                        ui->tablatrabajo->insertRow(ui->tablatrabajo->rowCount());

                        ui->tablatrabajo->setItem(ui->tablatrabajo->rowCount()-1,0,new QTableWidgetItem(idC));
                        ui->tablatrabajo->setItem(ui->tablatrabajo->rowCount()-1,1,new QTableWidgetItem(nombreCompleto));
                        ui->tablatrabajo->setItem(ui->tablatrabajo->rowCount()-1,2,new QTableWidgetItem(numero));
            }
}



void MainWindow::on_regresar_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}
