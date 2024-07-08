#pragma once

// Uncomment and modify defines below to customize ImGuiFileDialog

/////////////////////////////////
//// STL FILE SYSTEM ////////////
/////////////////////////////////

// Uncomment if you need to use your FileSystem Interface.
// If disabled, you have two defualt interfaces, std::filesystem or dirent
// #define USE_CUSTOM_FILESYSTEM
// This option need c++17
#define USE_STD_FILESYSTEM

/////////////////////////////////
//// MISC ///////////////////////
/////////////////////////////////

// The spacing between path buttons can be customized.
// If disabled, the spacing is defined by the imgui theme.
// Define the space between path buttons
// #define CUSTOM_PATH_SPACING 2

// #define MAX_FILE_DIALOG_NAME_BUFFER 1024
// #define MAX_PATH_BUFFER_SIZE 1024

/////////////////////////////////
//// QUICK PATH /////////////////
/////////////////////////////////

// The slash's buttons in path can be used for quick select parallel directories
// #define USE_QUICK_PATH_SELECT

/////////////////////////////////
//// THUMBNAILS /////////////////
/////////////////////////////////

// #define USE_THUMBNAILS
// The thumbnail generation use the stb_image and stb_resize lib who need to define the implementation.
// BTW if you already use them in your app, you can have compiler error due to "implemntation found in double"
// so uncomment these line for prevent the creation of implementation of these libs again
// #define DONT_DEFINE_AGAIN__STB_IMAGE_IMPLEMENTATION
// #define DONT_DEFINE_AGAIN__STB_IMAGE_RESIZE_IMPLEMENTATION
// #define IMGUI_RADIO_BUTTON RadioButton
// #define DisplayMode_ThumbailsList_ImageHeight 32.0f
// #define tableHeaderFileThumbnailsString "Thumbnails"
// #define DisplayMode_FilesList_ButtonString "FL"
// #define DisplayMode_FilesList_ButtonHelp "File List"
// #define DisplayMode_ThumbailsList_ButtonString "TL"
// #define DisplayMode_ThumbailsList_ButtonHelp "Thumbnails List"
// todo
// #define DisplayMode_ThumbailsGrid_ButtonString "TG"
// #define DisplayMode_ThumbailsGrid_ButtonHelp "Thumbnails Grid"

/////////////////////////////////
//// EXPLORATION BY KEYS ////////
/////////////////////////////////

#define USE_EXPLORATION_BY_KEYS
// This mapping by default is for GLFW but you can use another
#include <GLFW/glfw3.h>
// Up key for explore to the top
#define IGFD_KEY_UP ImGuiKey_UpArrow
// Down key for explore to the bottom
#define IGFD_KEY_DOWN ImGuiKey_DownArrow
// Enter key for open directory
#define IGFD_KEY_ENTER ImGuiKey_Enter
// BackSpace for coming back to the last directory
#define IGFD_KEY_BACKSPACE ImGuiKey_Backspace

/////////////////////////////////
//// SHORTCUTS => ctrl + KEY ////
/////////////////////////////////

#define SelectAllFilesKey ImGuiKey_A

/////////////////////////////////
//// DIALOG EXIT ////////////////
/////////////////////////////////

// For example you can quit the dialog by pressing the key excape
#define USE_DIALOG_EXIT_WITH_KEY
#define IGFD_EXIT_KEY ImGuiKey_Escape

/////////////////////////////////
//// WIDGETS ////////////////////
/////////////////////////////////

// Widget
// Begin combo widget
// #define IMGUI_BEGIN_COMBO ImGui::BeginCombo
// When auto resized, FILTER_COMBO_MIN_WIDTH will be considered has minimum width
// FILTER_COMBO_AUTO_SIZE is enabled by default now to 1.
// Uncomment if you want disable
// #define FILTER_COMBO_AUTO_SIZE 0
// Filter combo box width
// #define FILTER_COMBO_MIN_WIDTH 120.0f
// Button widget use for compose path
// #define IMGUI_PATH_BUTTON ImGui::Button
// Standard button
// #define IMGUI_BUTTON ImGui::Button

/////////////////////////////////
//// STRING'S ///////////////////
/////////////////////////////////

// Locale strings
// #define createDirButtonString "+"
// #define resetButtonString "R"
#define devicesButtonString "Устройства"
// #define editPathButtonString "E"
#define searchString "Поиск"
#define dirEntryString "[ПУТЬ] "
#define linkEntryString "[ССЫЛКА] "
#define fileEntryString "[ФАЙЛ] "
#define fileNameString "Имя файла: "
#define dirNameString "Путь:"
#define buttonResetSearchString "Сбросить поиск"
#define buttonDriveString "Устройства"
#define buttonEditPathString "Редактировать путь\nДоступно также по нажатию ПКМ на кнопках пути"
#define buttonResetPathString "Вернуться к текущей директории"
#define buttonCreateDirString "Создать директорию"
#define OverWriteDialogTitleString "Файл уже существует!"
#define OverWriteDialogMessageString "Хотите заменить его?"
#define OverWriteDialogConfirmButtonString "Подтвердить"
#define OverWriteDialogCancelButtonString "Закрыть"

// Validation buttons
#define okButtonString " OK"
// #define okButtonWidth 0.0f
#define cancelButtonString " Закрыть"
// #define cancelButtonWidth 0.0f
// Alignement [0:1], 0.0 is left, 0.5 middle, 1.0 right, and other ratios
// #define okCancelButtonAlignement 0.0f
// #define invertOkAndCancelButtons 0

// DateTimeFormat
// See strftime function in <ctime> for customize.
// "%Y/%m/%d %H:%M" give 2021:01:22 11:47
// "%Y/%m/%d %i:%M%p" give 2021:01:22 11:45PM
#define DateTimeFormat "%Y/%m/%d %H:%M"

/////////////////////////////////
//// SORTING ICONS //////////////
/////////////////////////////////

// These icons will appear in table headers
// #define USE_CUSTOM_SORTING_ICON
// #define tableHeaderAscendingIcon "A|"
// #define tableHeaderDescendingIcon "D|"
#define tableHeaderFileNameString " Имя файла"
#define tableHeaderFileTypeString " Тип"
#define tableHeaderFileSizeString " Размер"
#define tableHeaderFileDateTimeString " Дата"
#define fileSizeBytes "Б"
#define fileSizeKiloBytes "KБ"
#define fileSizeMegaBytes "МБ"
#define fileSizeGigaBytes "ГБ"

// Default table sort field.
// Must be FIELD_FILENAME, FIELD_TYPE, FIELD_SIZE, FIELD_DATE or FIELD_THUMBNAILS
#define defaultSortField FIELD_FILENAME

// Default table sort order for each field.
// true => Descending, false => Ascending
// #define defaultSortOrderFilename true
// #define defaultSortOrderType true
// #define defaultSortOrderSize true
// #define defaultSortOrderDate true
// #define defaultSortOrderThumbnails true

/////////////////////////////////
//// PLACES FEATURES ////////////
/////////////////////////////////

// #define USE_PLACES_FEATURE
// #define PLACES_PANE_DEFAULT_SHOWN false
// #define placesPaneWith 150.0f
// #define IMGUI_TOGGLE_BUTTON ToggleButton
// #define placesButtonString "Place"
// #define placesButtonHelpString "Places"
// #define addPlaceButtonString "+"
// #define removePlaceButtonString "-"
// #define validatePlaceButtonString "ok"
// #define editPlaceButtonString "E"

//////////////////////////////////////
//// PLACES FEATURES : BOOKMARKS /////
//////////////////////////////////////

// A group for bookmarks will be added by default, but you can also create it yourself and many more
// #define USE_PLACES_BOOKMARKS
// #define PLACES_BOOKMARK_DEFAULT_OPEPEND true
// #define placesBookmarksGroupName "Bookmarks"
// #define placesBookmarksDisplayOrder 0  // to the first

//////////////////////////////////////
//// PLACES FEATURES : DEVICES ///////
//////////////////////////////////////

// A group for system devices (returned by IFileSystem), but you can also add yours
// if you for example want to display a specific icon for some devices
// #define USE_PLACES_DEVICES
// #define PLACES_DEVICES_DEFAULT_OPEPEND true
// #define placesDevicesGroupName "Devices"
// #define placesDevicesDisplayOrder 10  // to the end
