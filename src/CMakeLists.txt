set(kontaminuti_SRCS settings.cpp timeedit.cpp toplevel.cpp tomatolistmodel.cpp tomato.cpp main.cpp )

kde4_add_ui_files(kontaminuti_SRCS settings.ui timeedit.ui)
kde4_add_app_icon(kontaminuti_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/../data/icons/hi*-app-kontaminuti.png")
kde4_add_executable(kontaminuti ${kontaminuti_SRCS})
target_link_libraries(kontaminuti ${KDE4_KNOTIFYCONFIG_LIBS} ${KDE4_KDEUI_LIBS})

install( TARGETS kontaminuti ${INSTALL_TARGETS_DEFAULT_ARGS} )
install( PROGRAMS kontaminuti.desktop  DESTINATION ${XDG_APPS_INSTALL_DIR} )
install( FILES kontaminuti.notifyrc  DESTINATION ${DATA_INSTALL_DIR}/kontaminuti )
