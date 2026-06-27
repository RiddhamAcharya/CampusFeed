loginPage = new LoginPage(this);
signupPage = new SignupPage(this);
dashboardPage = new DashboardPage(this);

ui->stackedWidget->addWidget(loginPage);
ui->stackedWidget->addWidget(signupPage);
ui->stackedWidget->addWidget(dashboardPage);

ui->stackedWidget->setCurrentWidget(loginPage);