#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  event_initialize();

  marlin_host = new Marlin_Host(QThread::HighPriority, this);
  marlin_host_initial_event();
}

MainWindow::~MainWindow()
{
  delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event) {
  QMessageBox::StandardButton resBtn = QMessageBox::question(
                                        this, this->windowTitle(),
                                        tr("Are you sure?\n"),
                                        QMessageBox::Yes | QMessageBox::No,
                                        QMessageBox::No);
  if (resBtn != QMessageBox::Yes) {
    event->ignore();
    return;
  }

  if (marlin_host->MH_IsConnected()) {
    marlin_host->MH_Disconnect();
  }

  event->accept();
}

void MainWindow::event_initialize() {
  connect(ui->btn_serial_connect, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_serial_connect);
  connect(ui->btn_serial_send, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_serial_send);
  connect(ui->line_edit_serial_command, &QLineEdit::returnPressed,
          this, &MainWindow::event_return_pressed_line_edit_serial_command);

  connect(ui->btn_home, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_home);
  connect(ui->btn_disable_motor, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_disable_motor);

  /// MANUAL JOG X
  connect(ui->btn_move_x_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_1);
  connect(ui->btn_move_x_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_10);
  connect(ui->btn_move_x_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_plus_100);

  connect(ui->btn_move_x_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_1);
  connect(ui->btn_move_x_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_10);
  connect(ui->btn_move_x_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x_minus_100);

  connect(ui->btn_move_x, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_x);

  /// MANUAL JOG Y
  connect(ui->btn_move_y_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_1);
  connect(ui->btn_move_y_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_10);
  connect(ui->btn_move_y_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_plus_100);

  connect(ui->btn_move_y_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_1);
  connect(ui->btn_move_y_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_10);
  connect(ui->btn_move_y_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y_minus_100);

  connect(ui->btn_move_y, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_y);

  /// MANUAL JOG Z
  connect(ui->btn_move_z_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_1);
  connect(ui->btn_move_z_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_10);
  connect(ui->btn_move_z_plus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_plus_100);

  connect(ui->btn_move_z_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_1);
  connect(ui->btn_move_z_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_10);
  connect(ui->btn_move_z_minus_100, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z_minus_100);

  connect(ui->btn_move_z, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_z);

  /// MANUAL JOG R
  connect(ui->btn_move_r_plus_01, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_01);
  connect(ui->btn_move_r_plus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_1);
  connect(ui->btn_move_r_plus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_plus_10);

  connect(ui->btn_move_r_minus_01, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_01);
  connect(ui->btn_move_r_minus_1, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_1);
  connect(ui->btn_move_r_minus_10, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r_minus_10);

  connect(ui->btn_move_r, &QPushButton::clicked,
          this, &MainWindow::event_clicked_btn_move_r);
}

void MainWindow::event_clicked_btn_serial_connect() {
  if(!marlin_host->MH_IsConnected()) {
    SerialSettingDialog *serial_setting = new SerialSettingDialog(this);
    connect(serial_setting, &SerialSettingDialog::UserAcceptSerialSetting,
            this, [this] (SerialSetting setting) {
              if (setting.name.isEmpty()) {
                return;
              }
              QString port_name = "Port Name: " + setting.name;
              QString baud_rate = "Baudrate: " + setting.string_baudrate;
              ui->lb_serial_connected_name->setText(port_name);
              ui->lb_serial_connected_baud->setText(baud_rate);
              marlin_host->MH_Connect(setting);
            });
    serial_setting->ShowDialog();
  } else {
    marlin_host->MH_Disconnect();
  }
}

void MainWindow::event_clicked_btn_serial_send() {
  if (marlin_host->MH_IsConnected()) {
    marlin_host->MH_WriteCommand(ui->line_edit_serial_command->text());
  }
}

//void MainWindow::event_clicked_btn_serial_test() {
//  if (marlin_host->MH_IsConnected()) {
//    for(int i=0;i<50;i++) {
//      marlin_host->MH_WriteCommand("G0 X100 Y100 F20000");
//      marlin_host->MH_WriteCommand("G1 E10 F10000");
//      marlin_host->MH_WriteCommand("G0 Z40 F15000");
//      marlin_host->MH_WriteCommand("G0 Z70 F15000");
//      marlin_host->MH_WriteCommand("G0 X150 Y200 F20000");
//      marlin_host->MH_WriteCommand("G1 E0 F10000");
//      marlin_host->MH_WriteCommand("G0 Z40 F15000");
//      marlin_host->MH_WriteCommand("G0 Z70 F15000");
//    }
//  }
//}

void MainWindow::event_return_pressed_line_edit_serial_command() {
  if (marlin_host->MH_IsConnected()) {
    marlin_host->MH_WriteCommand(ui->line_edit_serial_command->text());
  }
}

void MainWindow::event_clicked_btn_home() {
  marlin_host->MH_Home();
}

void MainWindow::event_clicked_btn_disable_motor() {
  marlin_host->MH_DisableStepper();
}

/// MANUAL JOG X
void MainWindow::event_clicked_btn_move_x_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 1.0);
}

void MainWindow::event_clicked_btn_move_x_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 10.0);
}

void MainWindow::event_clicked_btn_move_x_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, 100.0);
}

void MainWindow::event_clicked_btn_move_x_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -1.0);
}

void MainWindow::event_clicked_btn_move_x_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -10.0);
}

void MainWindow::event_clicked_btn_move_x_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, -100.0);
}

void MainWindow::event_clicked_btn_move_x() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_x->value()) :
                        (ui->dSpinBox_move_x->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::X, distance);
}

/// MANUAL JOG Y
void MainWindow::event_clicked_btn_move_y_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 1.0);
}

void MainWindow::event_clicked_btn_move_y_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 10.0);
}

void MainWindow::event_clicked_btn_move_y_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, 100.0);
}

void MainWindow::event_clicked_btn_move_y_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -1.0);
}

void MainWindow::event_clicked_btn_move_y_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -10.0);
}

void MainWindow::event_clicked_btn_move_y_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, -100.0);
}

void MainWindow::event_clicked_btn_move_y() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_y->value()) :
                        (ui->dSpinBox_move_y->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Y, distance);
}

/// MANUAL JOG Z
void MainWindow::event_clicked_btn_move_z_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 1.0);
}

void MainWindow::event_clicked_btn_move_z_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 10.0);
}

void MainWindow::event_clicked_btn_move_z_plus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, 100.0);
}

void MainWindow::event_clicked_btn_move_z_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -1.0);
}

void MainWindow::event_clicked_btn_move_z_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -10.0);
}

void MainWindow::event_clicked_btn_move_z_minus_100() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, -100.0);
}

void MainWindow::event_clicked_btn_move_z() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_z->value()) :
                        (ui->dSpinBox_move_z->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::Z, distance);
}

/// MANUAL JOG R
void MainWindow::event_clicked_btn_move_r_plus_01() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 0.1);
}

void MainWindow::event_clicked_btn_move_r_plus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 1.0);
}

void MainWindow::event_clicked_btn_move_r_plus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, 10.0);
}

void MainWindow::event_clicked_btn_move_r_minus_01() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -0.1);
}

void MainWindow::event_clicked_btn_move_r_minus_1() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -1.0);
}

void MainWindow::event_clicked_btn_move_r_minus_10() {
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, -10.0);
}

void MainWindow::event_clicked_btn_move_r() {
  double distance = (ui->rbtn_jog_backward->isChecked()) ?
                        (-1.0 * ui->dSpinBox_move_r->value()) :
                        (ui->dSpinBox_move_r->value());
  marlin_host->MH_ManualJog(Marlin_Host::Axis::R, distance);
}

void MainWindow::marlin_host_initial_event() {
  connect(marlin_host, &Marlin_Host::MH_Signal_Connected,
          this, &MainWindow::marlin_host_event_connected);
  connect(marlin_host, &Marlin_Host::MH_Signal_Disconnected,
          this, &MainWindow::marlin_host_event_disconnected);
  connect(marlin_host, &Marlin_Host::MH_Signal_ConnectFailed,
          this, &MainWindow::marlin_host_event_connect_failed);
  connect(marlin_host, &Marlin_Host::MH_Signal_ErrorOccurred,
          this, &MainWindow::marlin_host_event_error_occurred);
  connect(marlin_host, &Marlin_Host::MH_Signal_ReadBytesAvailable,
          this, &MainWindow::marlin_host_event_ready_read);
}

void MainWindow::marlin_host_event_connected() {
  ui->btn_serial_connect->setText("Disconnect");
  ui->statusbar->showMessage("Marlin connected.", 5000);
}
void MainWindow::marlin_host_event_disconnected() {
  QString port_name = "Port Name: ###";
  QString baud_rate = "Baudrate: ###";
  ui->lb_serial_connected_name->setText(port_name);
  ui->lb_serial_connected_baud->setText(baud_rate);
  ui->btn_serial_connect->setText("Connect");
  ui->statusbar->showMessage("Marlin disconnected.", 5000);
}
void MainWindow::marlin_host_event_connect_failed(QString msg) {
  QMessageBox::critical(this, tr("Error"), msg);
  ui->statusbar->showMessage("Marlin connect failed.", 5000);
}
void MainWindow::marlin_host_event_error_occurred(QString msg) {
  QMessageBox::critical(this, tr("Critical Error"), msg);
  ui->statusbar->showMessage("Critical Error, close port.", 5000);
}

void MainWindow::marlin_host_event_ready_read(const QByteArray &data_bytes) {
  QString msg = QString::fromUtf8(data_bytes);
  ui->text_browser_serial_read->append(msg);
//  ui->text_browser_serial_read->insertPlainText(msg);
}
