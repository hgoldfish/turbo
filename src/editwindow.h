#ifndef TVEDIT_EDITWINDOW_H
#define TVEDIT_EDITWINDOW_H

#define Uses_TWindow
#include <tvision/tv.h>

#include "tscintilla.h"
#include "drawviews.h"
#include "util.h"

#include <string_view>
#include <string>
#include <filesystem>

class DocumentView;

struct EditorWindow : public TWindow, Scintilla::TScintillaWindow {

    EditorWindow(const TRect &bounds, std::string_view aFile);
    ~EditorWindow();

    // Subviews

    DocumentView *docView;
    TDrawSubView *leftMargin;
    TScrollBar *hScrollBar, *vScrollBar;
    bool drawing;
    TPoint lastSize;

    // Scintilla

    Scintilla::TScintillaEditor editor;
    TDrawableView editorView;

    TRect editorBounds() const;
    void setUpEditor();
    void redrawEditor();

    void handleEvent(TEvent &ev) override;
    void changeBounds(const TRect &bounds) override;
    void setState(ushort aState, Boolean enable) override;
    Boolean valid(ushort command) override;
    const char* getTitle(short) override;

    // Minimum window size

    void sizeLimits(TPoint &min, TPoint &max);
    static constexpr TPoint minEditWinSize {24, 6};

    void lockSubViews();
    void unlockSubViews();
    void scrollBarEvent(TEvent ev);
    bool scrollBarChanged(TScrollBar *bar);
    void scrollTo(TPoint delta);
    void notify(SCNotification scn) override;
    void setHorizontalScrollPos(int delta, int limit) override;
    void setVerticalScrollPos(int delta, int limit) override;

    // TVEditApp integration

    list_head<EditorWindow> MRUhead;

    // File management

    // The absolute path of the currently open file, or an empty path if
    // no file is open.

    std::filesystem::path file;

    // If there was an error while loading the file, the view is invalid.
    // It shall return False when invoking valid(cmValid).

    bool fatalError;

    // The title of the window. It depends on TVEditApp, which keeps track of
    // things such as the number of editors open on the same file.
    // The title is also modified when a save point is reached or left.

    std::string title;
    bool inSavePoint;

    // These two functions modify the 'title' and 'inSavePoint' variables.
    // They may be invoked due to a Scintilla notification or because the file
    // was just saved and the title must be updated.

    void setSavePointLeft();
    void setSavePointReached();

    // The following is used to notify Scintilla about a save point. But it
    // also sets inSavePoint as if the title had been properly updated.

    void setSavePoint();

    // tryLoadFile is invoked when creating the Window. It decides whether
    // a file should be opened, according to 'file'.

    void tryLoadFile();
    bool loadFile();

    // trySaveFile gets invoked on cmSave. It decides whether to save the
    // file or to show a dialog. It also takes care of updating the window
    // title if necessary.

    void trySaveFile();
    bool saveFile();

    // saveAsDialog keeps popping out a dialog until the user decides
    // not to save or we succeed in writing to file. It also updates
    // the window title.

    bool saveAsDialog();

    // Pops out a msgBox with an error message.

    static void showError(const std::string &s);

};

#endif
