/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This file is part of the ERIS tool (https://github.com/telina/eris).
 * Copyright (c) 2023 Rhea Rinaldo (Rhea@Rinaldo.biz).
 *
 * Authors:
 *      - 2020-2023 Rhea Rinaldo (Rhea@Rinaldo.biz)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "main_window.h"
#include "checks.h"
#include "graphic_scene.h"
#include "main_window_manager.h"
#include "prism.h"
#include "error_handler.h"

#include <QApplication>
#include <QDesktopWidget>
#include <QScreen>
#include <QFile>

#include <QCommandLineParser>
#include <QFileInfo>

#if IS_LINUX

#include <execinfo.h>
#include <unistd.h>

#include <signal.h>

static void
handler(int sig)
{
    FILE* debug_file = fopen("debug.log", "w+");
    int fd = fileno(debug_file);
    void* array[1000];
    int size;

    // get void*'s for all entries on the stack
    size = backtrace(array, 1000);

    // print out all the frames to stderr
    fprintf(debug_file, " : Signal %d\n", sig);
    backtrace_symbols_fd(array, size, fd);
    backtrace_symbols_fd(array, size, 2);

    fprintf(stderr, " : \033[31mFatal error\033[m : Got signal %d ... exiting.\n ", sig);

    fclose(debug_file);
    exit(1);
}
#endif

using widgets::MainWindow;

int
main(int argc, char* argv[])
{
#if IS_LINUX
    signal(SIGSEGV, handler);
#endif
    QApplication app(argc, argv);
    
    // This is needed to ensure that qt signals are connected
    // on the main thread.
    widgets::ErrorHandler::getInstance();

    QCommandLineParser parser;
    parser.setApplicationDescription("A tool for Safety and Security Assessment and Evaluation of "
                                     "(autonomous) automotive Structures.");
    parser.addHelpOption();
    parser.addVersionOption();

    // An option with a value
    QCommandLineOption model_path_opt(
            QStringList() << "m"
                          << "eris-model",
            QCoreApplication::translate("main", "Path to an eris model i.e. </home/model.xml> ."),
            QCoreApplication::translate("main", "path"));
    parser.addOption(model_path_opt);
    parser.process(app);

    const QStringList args = parser.positionalArguments();
    QString model_path = parser.value(model_path_opt);
    if (!model_path.isEmpty())
    {
        if (!QFileInfo::exists(model_path))
        {
            PRINT_ERROR("The given model path = %s does not exist",
                        model_path.toStdString().c_str());
            return 1;
        }
        QFileInfo fileInfo(model_path);
        if (!fileInfo.isAbsolute() && !fileInfo.makeAbsolute())
        {
            PRINT_ERROR("Failed to convert the given model path = %s to an absoulte path",
                        model_path.toStdString().c_str());
            return 1;
        }
        model_path = fileInfo.absoluteFilePath();
    }

    // QFile file(":/qdarkstyle/light/light_style");
    // file.open(QFile::ReadOnly);
    // QString styleSheet = QLatin1String(file.readAll());

    // app.setStyleSheet(styleSheet);

    // Ensure the main window is initialized

    ERIS_CHECK(MainWindow::getInstance());
    ERIS_CHECK(MainWindow::getInstance()->init());

    if (!model_path.isEmpty())
    {
        PRINT_INFO("Model Path : %s ", model_path.toStdString().c_str());
        ERIS_CHECK(widgets::MainWindowManager::getInstance()->createNewSession(model_path));
    }

    MainWindow::getInstance()->showMaximized();

    return QApplication::exec();
}
