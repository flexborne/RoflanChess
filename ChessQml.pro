QT += quick multimedia

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++2a

SOURCES += \
        BoardManager/boardManager.cpp \
        Engine/pseudoLegalMoveGenerators.cpp \
        Move/move.cpp \
        RandomGenerator/pseudoRandomGenerator.cpp \
        Model/boardModel.cpp \
        Defines/defines.cpp \
        Engine/engine.cpp \
        Defines/engineDefines.cpp \
        DAO/piecesDAO.cpp \
        Factory/factory.cpp \
        FenParser/fenParser.cpp \
        Game/game.cpp \
        main.cpp \
        Pieces/bishop.cpp \
        Pieces/king.cpp \
        Pieces/knight.cpp \
        Pieces/pawn.cpp \
        Pieces/piece.cpp \
        Pieces/queen.cpp \
        Pieces/rook.cpp \

RESOURCES += qml.qrc
# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#INCLUDEPATH += "C:\DevSoft\boost\include\boost-1_79"
INCLUDEPATH += "./Pieces" \
                "./RandomGenerator" \
                "./Engine" \
                "./Model" \
                "./Defines" \
                "./Utils" \
                "./External" \
                "./BoardManager" \
                "./DAO" \
                "./Factory" \
                "./FenParser" \
                "./Move" \
                "./DataStructures" \
                "./Error" \
                "./Game" \

HEADERS += \
    BoardManager/boardManager.h \
    Defines/enum_GameStatus.h \
    Utils/enumArray.h \
    Utils/u_defer.h \
    Utils/u_overloaded.h \
    Error/error.h \
    External/spdlog/async.h \
    External/spdlog/async_logger-inl.h \
    External/spdlog/async_logger.h \
    External/spdlog/cfg/argv.h \
    External/spdlog/cfg/env.h \
    External/spdlog/cfg/helpers-inl.h \
    External/spdlog/cfg/helpers.h \
    External/spdlog/common-inl.h \
    External/spdlog/common.h \
    External/spdlog/details/backtracer-inl.h \
    External/spdlog/details/backtracer.h \
    External/spdlog/details/circular_q.h \
    External/spdlog/details/console_globals.h \
    External/spdlog/details/file_helper-inl.h \
    External/spdlog/details/file_helper.h \
    External/spdlog/details/fmt_helper.h \
    External/spdlog/details/log_msg-inl.h \
    External/spdlog/details/log_msg.h \
    External/spdlog/details/log_msg_buffer-inl.h \
    External/spdlog/details/log_msg_buffer.h \
    External/spdlog/details/mpmc_blocking_q.h \
    External/spdlog/details/null_mutex.h \
    External/spdlog/details/os-inl.h \
    External/spdlog/details/os.h \
    External/spdlog/details/periodic_worker-inl.h \
    External/spdlog/details/periodic_worker.h \
    External/spdlog/details/registry-inl.h \
    External/spdlog/details/registry.h \
    External/spdlog/details/synchronous_factory.h \
    External/spdlog/details/tcp_client-windows.h \
    External/spdlog/details/tcp_client.h \
    External/spdlog/details/thread_pool-inl.h \
    External/spdlog/details/thread_pool.h \
    External/spdlog/details/udp_client-windows.h \
    External/spdlog/details/udp_client.h \
    External/spdlog/details/windows_include.h \
    External/spdlog/fmt/bin_to_hex.h \
    External/spdlog/fmt/bundled/args.h \
    External/spdlog/fmt/bundled/chrono.h \
    External/spdlog/fmt/bundled/color.h \
    External/spdlog/fmt/bundled/compile.h \
    External/spdlog/fmt/bundled/core.h \
    External/spdlog/fmt/bundled/format-inl.h \
    External/spdlog/fmt/bundled/format.h \
    External/spdlog/fmt/bundled/locale.h \
    External/spdlog/fmt/bundled/os.h \
    External/spdlog/fmt/bundled/ostream.h \
    External/spdlog/fmt/bundled/printf.h \
    External/spdlog/fmt/bundled/ranges.h \
    External/spdlog/fmt/bundled/xchar.h \
    External/spdlog/fmt/chrono.h \
    External/spdlog/fmt/compile.h \
    External/spdlog/fmt/fmt.h \
    External/spdlog/fmt/ostr.h \
    External/spdlog/fmt/ranges.h \
    External/spdlog/fmt/xchar.h \
    External/spdlog/formatter.h \
    External/spdlog/fwd.h \
    External/spdlog/logger-inl.h \
    External/spdlog/logger.h \
    External/spdlog/pattern_formatter-inl.h \
    External/spdlog/pattern_formatter.h \
    External/spdlog/sinks/android_sink.h \
    External/spdlog/sinks/ansicolor_sink-inl.h \
    External/spdlog/sinks/ansicolor_sink.h \
    External/spdlog/sinks/base_sink-inl.h \
    External/spdlog/sinks/base_sink.h \
    External/spdlog/sinks/basic_file_sink-inl.h \
    External/spdlog/sinks/basic_file_sink.h \
    External/spdlog/sinks/daily_file_sink.h \
    External/spdlog/sinks/dist_sink.h \
    External/spdlog/sinks/dup_filter_sink.h \
    External/spdlog/sinks/hourly_file_sink.h \
    External/spdlog/sinks/mongo_sink.h \
    External/spdlog/sinks/msvc_sink.h \
    External/spdlog/sinks/null_sink.h \
    External/spdlog/sinks/ostream_sink.h \
    External/spdlog/sinks/qt_sinks.h \
    External/spdlog/sinks/ringbuffer_sink.h \
    External/spdlog/sinks/rotating_file_sink-inl.h \
    External/spdlog/sinks/rotating_file_sink.h \
    External/spdlog/sinks/sink-inl.h \
    External/spdlog/sinks/sink.h \
    External/spdlog/sinks/stdout_color_sinks-inl.h \
    External/spdlog/sinks/stdout_color_sinks.h \
    External/spdlog/sinks/stdout_sinks-inl.h \
    External/spdlog/sinks/stdout_sinks.h \
    External/spdlog/sinks/syslog_sink.h \
    External/spdlog/sinks/systemd_sink.h \
    External/spdlog/sinks/tcp_sink.h \
    External/spdlog/sinks/udp_sink.h \
    External/spdlog/sinks/win_eventlog_sink.h \
    External/spdlog/sinks/wincolor_sink-inl.h \
    External/spdlog/sinks/wincolor_sink.h \
    External/spdlog/spdlog-inl.h \
    External/spdlog/spdlog.h \
    External/spdlog/stopwatch.h \
    External/spdlog/tweakme.h \
    External/spdlog/version.h \
    Move/move.h \
    RandomGenerator/pseudoRandomGenerator.h \
    Utils/utils.h \
    Engine/bitboard.h \
    Model/boardModel.h \
    Defines/defines.h \
    Engine/engine.h \
    Defines/engineDefines.h \
    DAO/piecesDAO.h \
    DAO/DAO.h \
    Factory/factory.h \
    FenParser/fenParser.h \
    Game/game.h \
    Pieces/bishop.h \
    Pieces/king.h \
    Pieces/knight.h \
    Pieces/pawn.h \
    Pieces/piece.h \
    Pieces/queen.h \
    Pieces/rook.h \
    DataStructures/gameBoardData.h \
    DataStructures/smallPieceInfo.h

DISTFILES += \
    External/spdlog/fmt/bundled/fmt.license.rst
