//                                                    geany_encoding=ISO-8859-2
//! \file    draw.cc
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-25
//! \project GUX 2011 Draw (project #1)
//! \brief   Main stuff.
//

#include "draw.hh"

///////////////////////////////////////////////////////////////////////////////
// Experimenting with C++ templates
///////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////
// Common constants for widgets:
//
enum {
    CANVAS_INSTANCE      =  1,
    CANVAS_CLEAR         =  2,
    CANVAS_USE_POINT     =  3,
    CANVAS_USE_LINE      =  4,
    CANVAS_USE_RECT      =  5,
    CANVAS_USE_FRECT     =  6,
    CANVAS_USE_ELLIPSE   =  7,
    CANVAS_USE_FELLIPSE  =  8,
    CANVAS_SET_LNTHICK   =  9,
    CANVAS_SET_LNSTYLE   = 10,
    CANVAS_SET_DBGCOLOR  = 11,
    CANVAS_SET_DFGCOLOR  = 12,
    CANVAS_SET_FBGCOLOR  = 13,
    CANVAS_SET_FFGCOLOR  = 14,
    APP_SIG_BASE         = 15,
    CANVAS_COLOR_BLACK   =  0, // (0,0,0)
    CANVAS_COLOR_RED     =  4, // (1,0,0)
    CANVAS_COLOR_GREEN   =  2, // (0,1,0)
    CANVAS_COLOR_BLUE    =  1, // (0,0,1)
    CANVAS_COLOR_CYAN    =  3, // (0,1,1)
    CANVAS_COLOR_MAGENTA =  5, // (1,0,1)
    CANVAS_COLOR_YELLOW  =  6, // (1,1,0)
    CANVAS_COLOR_WHITE   =  7  // (1,1,1)
};

///////////////////////////////////////////////////////////////////////////////
// Definition of main window geometry:
//   - x, y, width, height, minimal width, minimal height
//
typedef Geometry<150,150,480,400,300,300> ApplicationGeometry;

///////////////////////////////////////////////////////////////////////////////
// 1) APPLICATION MENU WIDGET:
// ---------------------------
//   A - application
//   P - parrent widget
//
template<class A, class P>
class ApplicationMenu {
    public:
    // Buttons ID's and counts
        enum {
            FILE_ITEM     = 0,
            EDIT_ITEM     = 1,
            MENUBAR_NARGS = 2,
            EXIT_BUTTON   = 0,
            FILEPD_NARGS  = 1,
            CLEAR_BUTTON  = 0,
            EDITPD_NARGS  = 1
        };
    // Base type and callback handler type
        typedef ApplicationMenu<A,P> base_type;
        typedef void (base_type::*handler_type)(Widget, int, XtPointer);
    // Underlying widgets definitions
        typedef BaseSimpleMenuBar<P> MenuBar;
        typedef BaseSimplePulldownMenu<MenuBar,FILE_ITEM> FilePDM;
        typedef BaseSimplePulldownMenu<MenuBar,EDIT_ITEM> EditPDM;
    // Callback handlers ID's
        typedef NewType<base_type,FILE_ITEM> FILEPD_HANDLER;
        typedef NewType<base_type,EDIT_ITEM> EDITPD_HANDLER;
    private:
    // Menu callbacks has no way how to pass pointer of menu wrapper class
    // instance to callback function, so we make this static
        static base_type *m_this;
    // Menu resources
        static typename MenuBar::ItemType m_menubar_args[MENUBAR_NARGS];
        static typename FilePDM::ItemType m_filepd_args[FILEPD_NARGS];
        static typename EditPDM::ItemType m_editpd_args[EDITPD_NARGS];
    // Constructor assertions
        Assert m_assert;
    // Menu parts
        MenuBar m_menubar;
        FilePDM m_file_pulldown;
        EditPDM m_edit_pulldown;
    // Root of widget tree - application
        A *m_app;
    public:
    // In constructor we create
        ApplicationMenu(A *app, P *parrent)
        // (first, check if we are one & only)
        : m_assert(m_this == 0,
              "There is still `ApplicationMenu` instance."
          )
        // a) menu bar
        , m_menubar(parrent,m_menubar_args,MENUBAR_NARGS)
        // b) pulldown menu for 'File' button
        , m_file_pulldown(
              &m_menubar, "FileMenu",
              trampolineM<FILEPD_HANDLER>,
              m_filepd_args, FILEPD_NARGS
          )
        // c) pulldown menu for 'Edit' button
        , m_edit_pulldown(
              &m_menubar, "EditMenu",
              trampolineM<EDITPD_HANDLER>,
              m_editpd_args, EDITPD_NARGS
          )
        , m_app(app)
        {
            m_this = this; // for callbacks
        }
        ~ApplicationMenu()
        {
            m_this = 0;
        }
    // Handle of this widget == handle of menubar
        Widget Handle()
        {
            return m_menubar.Handle();
        }
    // Manage widgets recursively
        void Manage()
        {
            m_menubar.Manage();
        }
    // Little hack: this allow callback function to determine pointer to
    // instance of this class by using info about type (see "trampolines.hh")
    // (be sure there is only zero or one instance associated with type)
        static base_type *GetInstance()
        {
            return m_this;
        }
    // Slot receives the signal from widget parent and return:
    //   - pointer to widget, if signal belongs to this widget, after doing
    //     some stuff associated with received signal
    //   - NULL in other cases
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    // Menu buttons handlers
        void FileSubmenuHandler(Widget w, int button, XtPointer p)
        {
            H_TRACE("FileSubmenuHandler");
            if (button == this->m_file_pulldown.GetButton(EXIT_BUTTON)) {
                this->m_app->Exit(w,p);
            }
        }
        void EditSubmenuHandler(Widget w, int button, XtPointer p)
        {
            H_TRACE("EditSubmenuHandler");
            if (button == this->m_edit_pulldown.GetButton(CLEAR_BUTTON)) {
                this->m_app->Signal(CANVAS_CLEAR,w,0,p);
            }
        }
    private:
    // Forbid implicit methods
        ApplicationMenu();
        ApplicationMenu(const ApplicationMenu &);
        ApplicationMenu & operator =(const ApplicationMenu &);
    public:
    // Handlers table (how this is used, see "trampolines.hh")
        static handler_type GetHandler(FILEPD_HANDLER)
        {
            return &base_type::FileSubmenuHandler;
        }
        static handler_type GetHandler(EDITPD_HANDLER)
        {
            return &base_type::EditSubmenuHandler;
        }
};

template<class A, class P>
typename ApplicationMenu<A,P>::base_type *
ApplicationMenu<A,P>::m_this(0);

// Menubar buttons
template<class A, class P>
typename ApplicationMenu<A,P>::MenuBar::ItemType
ApplicationMenu<A,P>::m_menubar_args[ApplicationMenu<A,P>::MENUBAR_NARGS] = {
    {ApplicationMenu<A,P>::FILE_ITEM, const_cast<char *>("File"), 'F'},
    {ApplicationMenu<A,P>::EDIT_ITEM, const_cast<char *>("Edit"), 'E'}
};

// "File" pulldown menu
template<class A, class P>
typename ApplicationMenu<A,P>::FilePDM::ItemType
ApplicationMenu<A,P>::m_filepd_args[ApplicationMenu<A,P>::FILEPD_NARGS] = {
    {ApplicationMenu<A,P>::EXIT_BUTTON, const_cast<char *>("Exit"), 'x', 0, 0}
};

// "Edit" pulldown menu:
template<class A, class P>
typename ApplicationMenu<A,P>::EditPDM::ItemType
ApplicationMenu<A,P>::m_editpd_args[ApplicationMenu<A,P>::EDITPD_NARGS] = {
    {
        ApplicationMenu<A,P>::CLEAR_BUTTON,
        const_cast<char *>("Clear"),
        'C',
        0,
        0
    }
};

///////////////////////////////////////////////////////////////////////////////
// 2) DRAWER TOOLBAR WIDGET:
// -------------------------
//   A - application
//   P - parrent
//
template<class A, class P>
class DrawerToolbar {
    public:
    // Resources sizes and button IDs
        enum {
        // Padding between buttons in toolbar
            PADDING = 2,
        // Resources
            POINT_RES_SZ    = 7,
            LINE_RES_SZ     = 8,
            RECT_RES_SZ     = 8,
            FRECT_RES_SZ    = 8,
            ELLIPSE_RES_SZ  = 10,
            FELLIPSE_RES_SZ = 10,
            LNTHICK_RES_SZ  = 8,
            LNSTYLE_RES_SZ  = 9,
            BG_RES_SZ       = 7,
            FG_RES_SZ       = 7,
            BITMAP_SZ       = 32,
        // Line thickness dialog buttons
            LT_DLG_0px = 0,
            LT_DLG_1px = 1,
            LT_DLG_3px = 3,
            LT_DLG_5px = 5,
            LT_DLG_8px = 8,
            LT_DLG_RSZ = 5,
        // Line style dialog buttons
            LS_DLG_SOLID  = LineSolid,
            LS_DLG_DASHED = LineDoubleDash,
            LS_DLG_RSZ    = 2,
        // Colors
            CLR_BLACK   = CANVAS_COLOR_BLACK,
            CLR_RED     = CANVAS_COLOR_RED,
            CLR_GREEN   = CANVAS_COLOR_GREEN,
            CLR_BLUE    = CANVAS_COLOR_BLUE,
            CLR_CYAN    = CANVAS_COLOR_CYAN,
            CLR_MAGENTA = CANVAS_COLOR_MAGENTA,
            CLR_YELLOW  = CANVAS_COLOR_YELLOW,
            CLR_WHITE   = CANVAS_COLOR_WHITE,
            CLR_CNT     = 8
        };
    // Customized widget classes
        typedef DrawerToolbar<A,P> base_type;
        typedef void (base_type::*handler_type)(Widget, XtPointer);
        typedef BaseForm<P> Form;
        typedef BasePushButton<A,Form> Button;
        typedef BaseModalRadioDialog<A,Form,LT_DLG_RSZ,0> LnThickDlg;
        typedef BaseModalRadioDialog<A,Form,LS_DLG_RSZ,1> LnStyleDlg;
        typedef BaseModalColorDialog<A,Form,CLR_CNT,0> DrawBgColorDlg;
        typedef BaseModalColorDialog<A,Form,CLR_CNT,1> DrawFgColorDlg;
        typedef BaseModalColorDialog<A,Form,CLR_CNT,2> FillBgColorDlg;
        typedef BaseModalColorDialog<A,Form,CLR_CNT,3> FillFgColorDlg;
    // Handlers IDs
        typedef NewType<base_type,0> POINT_CB;
        typedef NewType<base_type,1> LINE_CB;
        typedef NewType<base_type,2> RECT_CB;
        typedef NewType<base_type,3> FRECT_CB;
        typedef NewType<base_type,4> ELLIPSE_CB;
        typedef NewType<base_type,5> FELLIPSE_CB;
        typedef NewType<base_type,6> LNTHICK_CB;
        typedef NewType<base_type,7> LNSTYLE_CB;
        typedef NewType<base_type,8> DBG_CB;
        typedef NewType<base_type,9> DFG_CB;
        typedef NewType<base_type,10> FBG_CB;
        typedef NewType<base_type,11> FFG_CB;
    private:
    // Resources
        static int m_point_res[POINT_RES_SZ];
        static int m_line_res[LINE_RES_SZ];
        static int m_rect_res[RECT_RES_SZ];
        static int m_frect_res[FRECT_RES_SZ];
        static int m_ellipse_res[ELLIPSE_RES_SZ];
        static int m_fellipse_res[FELLIPSE_RES_SZ];
        static int m_lnthick_res[LNTHICK_RES_SZ];
        static int m_lnstyle_res[LNSTYLE_RES_SZ];
        static int m_dbg_res[BG_RES_SZ];
        static int m_dfg_res[FG_RES_SZ];
        static int m_fbg_res[BG_RES_SZ];
        static int m_ffg_res[FG_RES_SZ];
        static char m_point_bitmap[BITMAP_SZ];
        static char m_DBg_bitmap[BITMAP_SZ];
        static char m_DFg_bitmap[BITMAP_SZ];
        static char m_FBg_bitmap[BITMAP_SZ];
        static char m_FFg_bitmap[BITMAP_SZ];
        static typename LnThickDlg::ItemType m_ltdlg_res[LT_DLG_RSZ];
        static typename LnStyleDlg::ItemType m_lsdlg_res[LS_DLG_RSZ];
        static int m_cdlg_res[5*CLR_CNT];
        static char *m_cdlg_data;
    // Toolbar parts
        A *m_app;
        Form m_layout;
        Button m_point;
        Button m_line;
        Button m_rect;
        Button m_frect;
        Button m_ellipse;
        Button m_fellipse;
        Button m_lnthick;
        Button m_lnstyle;
        Button m_dbg;
        Button m_dfg;
        Button m_fbg;
        Button m_ffg;
    // Dialogs
        LnThickDlg m_lnthick_dlg;
        LnStyleDlg m_lnstyle_dlg;
        DrawBgColorDlg m_dbgcolor_dlg;
        DrawFgColorDlg m_dfgcolor_dlg;
        FillBgColorDlg m_fbgcolor_dlg;
        FillFgColorDlg m_ffgcolor_dlg;
    public:
    // Create a toolbar
        DrawerToolbar(A *app, P *parrent)
        : m_app(app)
        , m_layout(parrent,"ToolbarLayout")
        , m_point(&m_layout,"PointTool",
              m_point_res,POINT_RES_SZ,m_point_bitmap
          )
        , m_line(&m_layout,"LineTool",
              m_line_res,LINE_RES_SZ,0
          )
        , m_rect(&m_layout,"RectTool",
              m_rect_res,RECT_RES_SZ,0
          )
        , m_frect(&m_layout,"FilledRectTool",
              m_frect_res,FRECT_RES_SZ,0
          )
        , m_ellipse(&m_layout,"EllipseTool",
              m_ellipse_res,ELLIPSE_RES_SZ,0
          )
        , m_fellipse(&m_layout,"FilledEllipseTool",
              m_fellipse_res,FELLIPSE_RES_SZ,0
          )
        , m_lnthick(&m_layout,"LineThicknessTool",
              m_lnthick_res,LNTHICK_RES_SZ,0
          )
        , m_lnstyle(&m_layout,"LineStyleTool",
              m_lnstyle_res,LNSTYLE_RES_SZ,0
          )
        , m_dbg(&m_layout,"DrawBackgroundColorTool",
              m_dbg_res,BG_RES_SZ,m_DBg_bitmap
          )
        , m_dfg(&m_layout,"DrawForegroundColorTool",
              m_dfg_res,FG_RES_SZ,m_DFg_bitmap
          )
        , m_fbg(&m_layout,"FillBackgroundColorTool",
              m_fbg_res,BG_RES_SZ,m_FBg_bitmap
          )
        , m_ffg(&m_layout,"FillForegroundColorTool",
              m_ffg_res,FG_RES_SZ,m_FFg_bitmap
          )
        , m_lnthick_dlg(app,&m_layout,"Line Thickness",0,m_ltdlg_res)
        , m_lnstyle_dlg(app,&m_layout,"Line Style",0,m_lsdlg_res)
        , m_dbgcolor_dlg(
              app,&m_layout,
              "Background Color for Drawing",
              7,4,m_cdlg_res,m_cdlg_data
          )
        , m_dfgcolor_dlg(
              app,&m_layout,
              "Foreground Color for Drawing",
              0,4,m_cdlg_res,m_cdlg_data
          )
        , m_fbgcolor_dlg(
              app,&m_layout,
              "Background Color for Filling",
              7,4,m_cdlg_res,m_cdlg_data
          )
        , m_ffgcolor_dlg(
              app,&m_layout,
              "Foreground Color for Filling",
              0,4,m_cdlg_res,m_cdlg_data
          )
        {
        // Set layout
            // - point button
            m_layout.AttachToTop(m_point.Handle());
            m_layout.AttachToLeft(m_point.Handle());
            m_layout.TopOffset(m_point.Handle(),PADDING);
            m_layout.LeftOffset(m_point.Handle(),PADDING);
            // - line button
            m_layout.AttachToTop(m_line.Handle());
            m_layout.AttachToLeftWidget(m_line.Handle(),m_point.Handle());
            m_layout.AttachToRight(m_line.Handle());
            m_layout.TopOffset(m_line.Handle(),PADDING);
            m_layout.LeftOffset(m_line.Handle(),PADDING);
            m_layout.RightOffset(m_line.Handle(),PADDING);
            // - rect button
            m_layout.AttachToTopWidget(m_rect.Handle(),m_point.Handle());
            m_layout.AttachToLeft(m_rect.Handle());
            m_layout.TopOffset(m_rect.Handle(),PADDING);
            m_layout.LeftOffset(m_rect.Handle(),PADDING);
            // - frect button
            m_layout.AttachToTopWidget(m_frect.Handle(),m_line.Handle());
            m_layout.AttachToLeftWidget(m_frect.Handle(),m_rect.Handle());
            m_layout.AttachToRight(m_frect.Handle());
            m_layout.TopOffset(m_frect.Handle(),PADDING);
            m_layout.LeftOffset(m_frect.Handle(),PADDING);
            m_layout.RightOffset(m_frect.Handle(),PADDING);
            // - ellipse button
            m_layout.AttachToTopWidget(m_ellipse.Handle(),m_rect.Handle());
            m_layout.AttachToLeft(m_ellipse.Handle());
            m_layout.TopOffset(m_ellipse.Handle(),PADDING);
            m_layout.LeftOffset(m_ellipse.Handle(),PADDING);
            // - fellipse button
            m_layout.AttachToTopWidget(m_fellipse.Handle(),m_frect.Handle());
            m_layout.AttachToLeftWidget(
                m_fellipse.Handle(),m_ellipse.Handle()
            );
            m_layout.AttachToRight(m_fellipse.Handle());
            m_layout.TopOffset(m_fellipse.Handle(),PADDING);
            m_layout.LeftOffset(m_fellipse.Handle(),PADDING);
            m_layout.RightOffset(m_fellipse.Handle(),PADDING);
            // - line thickness button
            m_layout.AttachToTopWidget(m_lnthick.Handle(),m_ellipse.Handle());
            m_layout.AttachToLeft(m_lnthick.Handle());
            m_layout.TopOffset(m_lnthick.Handle(),PADDING);
            m_layout.LeftOffset(m_lnthick.Handle(),PADDING);
            m_layout.RightOffset(m_lnthick.Handle(),PADDING);
            // - line style button
            m_layout.AttachToTopWidget(m_lnstyle.Handle(),m_lnthick.Handle());
            m_layout.AttachToLeft(m_lnstyle.Handle());
            m_layout.TopOffset(m_lnstyle.Handle(),PADDING);
            m_layout.LeftOffset(m_lnstyle.Handle(),PADDING);
            m_layout.RightOffset(m_lnstyle.Handle(),PADDING);
            // - drawing bg color set button
            m_layout.AttachToTopWidget(m_dbg.Handle(),m_lnstyle.Handle());
            m_layout.AttachToLeft(m_dbg.Handle());
            m_layout.TopOffset(m_dbg.Handle(),PADDING);
            m_layout.LeftOffset(m_dbg.Handle(),PADDING);
            // - drawing fg color set button
            m_layout.AttachToTopWidget(m_dfg.Handle(),m_lnstyle.Handle());
            m_layout.AttachToLeftWidget(m_dfg.Handle(),m_dbg.Handle());
            m_layout.AttachToRight(m_dfg.Handle());
            m_layout.TopOffset(m_dfg.Handle(),PADDING);
            m_layout.LeftOffset(m_dfg.Handle(),PADDING);
            m_layout.RightOffset(m_dfg.Handle(),PADDING);
            // - filling bg color set button
            m_layout.AttachToTopWidget(m_fbg.Handle(),m_dbg.Handle());
            m_layout.AttachToLeft(m_fbg.Handle());
            m_layout.TopOffset(m_fbg.Handle(),PADDING);
            m_layout.LeftOffset(m_fbg.Handle(),PADDING);
            // - filling fg color set button
            m_layout.AttachToTopWidget(m_ffg.Handle(),m_dfg.Handle());
            m_layout.AttachToLeftWidget(m_ffg.Handle(),m_fbg.Handle());
            m_layout.AttachToRight(m_ffg.Handle());
            m_layout.TopOffset(m_ffg.Handle(),PADDING);
            m_layout.LeftOffset(m_ffg.Handle(),PADDING);
            m_layout.RightOffset(m_ffg.Handle(),PADDING);
        // Set callbacks
            m_point.SetCallback(
                trampolineW<POINT_CB>, static_cast<XtPointer>(this)
            );
            m_line.SetCallback(
                trampolineW<LINE_CB>, static_cast<XtPointer>(this)
            );
            m_rect.SetCallback(
                trampolineW<RECT_CB>, static_cast<XtPointer>(this)
            );
            m_frect.SetCallback(
                trampolineW<FRECT_CB>, static_cast<XtPointer>(this)
            );
            m_ellipse.SetCallback(
                trampolineW<ELLIPSE_CB>, static_cast<XtPointer>(this)
            );
            m_fellipse.SetCallback(
                trampolineW<FELLIPSE_CB>, static_cast<XtPointer>(this)
            );
            m_lnthick.SetCallback(
                trampolineW<LNTHICK_CB>, static_cast<XtPointer>(this)
            );
            m_lnstyle.SetCallback(
                trampolineW<LNSTYLE_CB>, static_cast<XtPointer>(this)
            );
            m_dbg.SetCallback(
                trampolineW<DBG_CB>, static_cast<XtPointer>(this)
            );
            m_dfg.SetCallback(
                trampolineW<DFG_CB>, static_cast<XtPointer>(this)
            );
            m_fbg.SetCallback(
                trampolineW<FBG_CB>, static_cast<XtPointer>(this)
            );
            m_ffg.SetCallback(
                trampolineW<FFG_CB>, static_cast<XtPointer>(this)
            );
        }
        ~DrawerToolbar() {}
    // Manage subwidgets
        void Manage()
        {
            m_ffg.Manage(); m_fbg.Manage();
            m_dfg.Manage(); m_dbg.Manage();
            m_lnstyle.Manage(); m_lnthick.Manage();
            m_fellipse.Manage(); m_ellipse.Manage();
            m_frect.Manage(); m_rect.Manage();
            m_line.Manage(); m_point.Manage();
            m_layout.Manage();
        }
    // Return handle to toolbar container
        Widget Handle()
        {
            return m_layout.Handle();
        }
    // Slot for signal receiving
        void *Slot(int signal, Widget w, XtPointer p, XtPointer c)
        {
            if (signal == A::EXIT_CLEANUP) {
                m_ffg.Slot(signal, w, p, c);
                m_fbg.Slot(signal, w, p, c);
                m_dfg.Slot(signal, w, p, c);
                m_dbg.Slot(signal, w, p, c);
                m_lnstyle.Slot(signal, w, p, c);
                m_lnthick.Slot(signal, w, p, c);
                m_fellipse.Slot(signal, w, p, c);
                m_ellipse.Slot(signal, w, p, c);
                m_frect.Slot(signal, w, p, c);
                m_rect.Slot(signal, w, p, c);
                m_line.Slot(signal, w, p, c);
                m_point.Slot(signal, w, p, c);
                return static_cast<void *>(this);
            }
            return 0;
        }
    // Button handlers
        void PointHandler(Widget w, XtPointer p)
        {
            H_TRACE("PointHandler");
            this->m_app->Signal(CANVAS_USE_POINT, w, 0, p);
        }
        void LineHandler(Widget w, XtPointer p)
        {
            H_TRACE("LineHandler");
            this->m_app->Signal(CANVAS_USE_LINE, w, 0, p);
        }
        void RectHandler(Widget w, XtPointer p)
        {
            H_TRACE("RectHandler");
            this->m_app->Signal(CANVAS_USE_RECT, w, 0, p);
        }
        void FRectHandler(Widget w, XtPointer p)
        {
            H_TRACE("FRectHandler");
            this->m_app->Signal(CANVAS_USE_FRECT, w, 0, p);
        }
        void EllipseHandler(Widget w, XtPointer p)
        {
            H_TRACE("EllipseHandler");
            this->m_app->Signal(CANVAS_USE_ELLIPSE, w, 0, p);
        }
        void FEllipseHandler(Widget w, XtPointer p)
        {
            H_TRACE("FEllipseHandler");
            this->m_app->Signal(CANVAS_USE_FELLIPSE, w, 0, p);
        }
        void LnThickHandler(Widget w, XtPointer p)
        {
            if (this->m_lnthick_dlg.Show() == LnThickDlg::OK) {
                int line_width = this->m_lnthick_dlg.Toggled();
                this->m_app->Signal(CANVAS_SET_LNTHICK,
                    w, static_cast<XtPointer>(&line_width), p
                );
            }
        }
        void LnStyleHandler(Widget w, XtPointer p)
        {
            if (this->m_lnstyle_dlg.Show() == LnStyleDlg::OK) {
                int line_style = this->m_lnstyle_dlg.Toggled();
                this->m_app->Signal(CANVAS_SET_LNSTYLE,
                    w, static_cast<XtPointer>(&line_style), p
                );
            }
        }
        void DrawBgHandler(Widget w, XtPointer p)
        {
            if (this->m_dbgcolor_dlg.Show() == DrawBgColorDlg::OK) {
                int color = this->m_dbgcolor_dlg.Selected();
                this->m_app->Signal(CANVAS_SET_DBGCOLOR,
                    w, static_cast<XtPointer>(&color), p
                );
            }
        }
        void DrawFgHandler(Widget w, XtPointer p)
        {
            if (this->m_dfgcolor_dlg.Show() == DrawFgColorDlg::OK) {
                int color = this->m_dfgcolor_dlg.Selected();
                this->m_app->Signal(CANVAS_SET_DFGCOLOR,
                    w, static_cast<XtPointer>(&color), p
                );
            }
        }
        void FillBgHandler(Widget w, XtPointer p)
        {
            if (this->m_fbgcolor_dlg.Show() == FillBgColorDlg::OK) {
                int color = this->m_fbgcolor_dlg.Selected();
                this->m_app->Signal(CANVAS_SET_FBGCOLOR,
                    w, static_cast<XtPointer>(&color), p
                );
            }
        }
        void FillFgHandler(Widget w, XtPointer p)
        {
            if (this->m_ffgcolor_dlg.Show() == FillFgColorDlg::OK) {
                int color = this->m_ffgcolor_dlg.Selected();
                this->m_app->Signal(CANVAS_SET_FFGCOLOR,
                    w, static_cast<XtPointer>(&color), p
                );
            }
        }
    private:
    // Forbid implicit methods
        DrawerToolbar();
        DrawerToolbar(const DrawerToolbar &);
        DrawerToolbar & operator =(const DrawerToolbar &);
    public:
    // Handler map
        static handler_type GetHandler(POINT_CB)
        {
            return &base_type::PointHandler;
        }
        static handler_type GetHandler(LINE_CB)
        {
            return &base_type::LineHandler;
        }
        static handler_type GetHandler(RECT_CB)
        {
            return &base_type::RectHandler;
        }
        static handler_type GetHandler(FRECT_CB)
        {
            return &base_type::FRectHandler;
        }
        static handler_type GetHandler(ELLIPSE_CB)
        {
            return &base_type::EllipseHandler;
        }
        static handler_type GetHandler(FELLIPSE_CB)
        {
            return &base_type::FEllipseHandler;
        }
        static handler_type GetHandler(LNTHICK_CB)
        {
            return &base_type::LnThickHandler;
        }
        static handler_type GetHandler(LNSTYLE_CB)
        {
            return &base_type::LnStyleHandler;
        }
        static handler_type GetHandler(DBG_CB)
        {
            return &base_type::DrawBgHandler;
        }
        static handler_type GetHandler(DFG_CB)
        {
            return &base_type::DrawFgHandler;
        }
        static handler_type GetHandler(FBG_CB)
        {
            return &base_type::FillBgHandler;
        }
        static handler_type GetHandler(FFG_CB)
        {
            return &base_type::FillFgHandler;
        }
};

// Resources
template<class A, class P>
int DrawerToolbar<A,P>::m_point_res[DrawerToolbar<A,P>::POINT_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::BITMAP,16,16,0
};
template<class A, class P>
int DrawerToolbar<A,P>::m_line_res[DrawerToolbar<A,P>::LINE_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::LINE,2,2,13,13
};
template<class A, class P>
int DrawerToolbar<A,P>::m_rect_res[DrawerToolbar<A,P>::RECT_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::RECT,1,3,13,9
};
template<class A, class P>
int DrawerToolbar<A,P>::m_frect_res[DrawerToolbar<A,P>::FRECT_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::FRECT,1,3,14,10
};
template<class A, class P>
int DrawerToolbar<A,P>::m_ellipse_res[DrawerToolbar<A,P>::ELLIPSE_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::ARC,1,3,14,10,0,360*64
};
template<class A, class P>
int DrawerToolbar<A,P>::m_fellipse_res[DrawerToolbar<A,P>::FELLIPSE_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::FARC,1,3,14,10,0,360*64
};
template<class A, class P>
int DrawerToolbar<A,P>::m_lnthick_res[DrawerToolbar<A,P>::LNTHICK_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,46,11,
    DrawerToolbar<A,P>::Button::FRECT,2,4,42,3
};
template<class A, class P>
int DrawerToolbar<A,P>::m_lnstyle_res[DrawerToolbar<A,P>::LNSTYLE_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,46,11,
    DrawerToolbar<A,P>::Button::LINE_STYLE_DASHED,
    DrawerToolbar<A,P>::Button::LINE,2,5,43,5
};
template<class A, class P>
int DrawerToolbar<A,P>::m_dbg_res[DrawerToolbar<A,P>::BG_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::BITMAP,16,16,0
};
template<class A, class P>
int DrawerToolbar<A,P>::m_dfg_res[DrawerToolbar<A,P>::FG_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::BITMAP,16,16,0
};
template<class A, class P>
int DrawerToolbar<A,P>::m_fbg_res[DrawerToolbar<A,P>::BG_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::BITMAP,16,16,0
};
template<class A, class P>
int DrawerToolbar<A,P>::m_ffg_res[DrawerToolbar<A,P>::FG_RES_SZ] = {
    DrawerToolbar<A,P>::Button::IMAGE,16,16,
    DrawerToolbar<A,P>::Button::BITMAP,16,16,0
};
template<class A, class P>
char DrawerToolbar<A,P>::m_point_bitmap[DrawerToolbar<A,P>::BITMAP_SZ] = {
    '\x00','\x00', '\x00','\x00', '\x00','\x00', '\x00','\x00',
    '\x80','\x01', '\x80','\x01', '\x80','\x01', '\xf0','\x0f',
    '\xf0','\x0f', '\x80','\x01', '\x80','\x01', '\x80','\x01',
    '\x00','\x00', '\x00','\x00', '\x00','\x00', '\x00','\x00'
};
// Bity v bajtu => pixely
// 0123456701234567
//"________________"
//"________________"
//"________________"
//"________________"
//"_00000__________"
//"__0___0_________"
//"__0___0__000_00_"
//"__0000__0___00__"
//"__0___0_0____0__"
//"__0___0_0____0__"
//"__0___0_0___00__"
//"_00000___000_0__"
//"_____________0__"
//"_________0000___"
//"________________"
//"________________"
template<class A, class P>
char DrawerToolbar<A,P>::m_DBg_bitmap[DrawerToolbar<A,P>::BITMAP_SZ] = {
    '\x00','\x00', '\x00','\x00', '\x00','\x00', '\x00','\x00',
    '\x3E','\x00', '\x44','\x00', '\x44','\x6E', '\x3C','\x31',
    '\x44','\x21', '\x44','\x21', '\x44','\x31', '\x3E','\x2E',
    '\x00','\x20', '\x00','\x1E', '\x00','\x00', '\x00','\x00'
};
//"________________"
//"________________"
//"________________"
//"________________"
//"_000000_________"
//"__0___0_________"
//"__0_0____000_00_"
//"__000___0___00__"
//"__0_0___0____0__"
//"__0_____0____0__"
//"__0_____0___00__"
//"_000_____000_0__"
//"_____________0__"
//"_________0000___"
//"________________"
//"________________"
template<class A, class P>
char DrawerToolbar<A,P>::m_DFg_bitmap[DrawerToolbar<A,P>::BITMAP_SZ] = {
    '\x00','\x00', '\x00','\x00', '\x00','\x00', '\x00','\x00',
    '\x7E','\x00', '\x44','\x00', '\x14','\x6E', '\x1C','\x31',
    '\x14','\x21', '\x04','\x21', '\x04','\x31', '\x0E','\x2E',
    '\x00','\x20', '\x00','\x1E', '\x00','\x00', '\x00','\x00'
};
template<class A, class P>
char DrawerToolbar<A,P>::m_FBg_bitmap[DrawerToolbar<A,P>::BITMAP_SZ] = {
    '\xFF','\xFF', '\xFF','\xFF', '\xFF','\xFF', '\xFF','\xFF',
    '\xC1','\xFF', '\xBB','\xFF', '\xBB','\x91', '\xC3','\xCE',
    '\xBB','\xDE', '\xBB','\xDE', '\xBB','\xCE', '\xC1','\xD1',
    '\xFF','\xDF', '\xFF','\xE1', '\xFF','\xFF', '\xFF','\xFF'
};
template<class A, class P>
char DrawerToolbar<A,P>::m_FFg_bitmap[DrawerToolbar<A,P>::BITMAP_SZ] = {
    '\xFF','\xFF', '\xFF','\xFF', '\xFF','\xFF', '\xFF','\xFF',
    '\x81','\xFF', '\xBB','\xFF', '\xEB','\x91', '\xE3','\xCE',
    '\xEB','\xDE', '\xFB','\xDE', '\xFB','\xCE', '\xF1','\xD1',
    '\xFF','\xDF', '\xFF','\xE1', '\xFF','\xFF', '\xFF','\xFF'
};
template<class A, class P>
typename DrawerToolbar<A,P>::LnThickDlg::ItemType
DrawerToolbar<A,P>::m_ltdlg_res[DrawerToolbar<A,P>::LT_DLG_RSZ] = {
    { DrawerToolbar<A,P>::LT_DLG_0px, const_cast<char *>("0px thickness") },
    { DrawerToolbar<A,P>::LT_DLG_1px, const_cast<char *>("1px thickness") },
    { DrawerToolbar<A,P>::LT_DLG_3px, const_cast<char *>("3px thickness") },
    { DrawerToolbar<A,P>::LT_DLG_5px, const_cast<char *>("5px thickness") },
    { DrawerToolbar<A,P>::LT_DLG_8px, const_cast<char *>("8px thickness") }
};
template<class A, class P>
typename DrawerToolbar<A,P>::LnStyleDlg::ItemType
DrawerToolbar<A,P>::m_lsdlg_res[DrawerToolbar<A,P>::LS_DLG_RSZ] = {
    { DrawerToolbar<A,P>::LS_DLG_SOLID,  const_cast<char *>("Solid") },
    { DrawerToolbar<A,P>::LS_DLG_DASHED, const_cast<char *>("Dashed") }
};
template<class A, class P>
int DrawerToolbar<A,P>::m_cdlg_res[5*DrawerToolbar<A,P>::CLR_CNT] = {
    DrawerToolbar<A,P>::CLR_BLACK,    0,     0,     0,     0,
    DrawerToolbar<A,P>::CLR_RED,      8, 65535,     0,     0,
    DrawerToolbar<A,P>::CLR_GREEN,   16,     0, 65535,     0,
    DrawerToolbar<A,P>::CLR_BLUE,    24,     0,     0, 65535,
    DrawerToolbar<A,P>::CLR_CYAN,    32,     0, 65535, 65535,
    DrawerToolbar<A,P>::CLR_MAGENTA, 40, 65535,     0, 65535,
    DrawerToolbar<A,P>::CLR_YELLOW,  48, 65535, 65535,     0,
    DrawerToolbar<A,P>::CLR_WHITE,   56, 65535, 65535, 65535
};
template<class A, class P>
char *DrawerToolbar<A,P>::m_cdlg_data = const_cast<char *>(
    "clrbtn0\0" //0
    "clrbtn1\0" //8
    "clrbtn2\0" //16
    "clrbtn3\0" //24
    "clrbtn4\0" //32
    "clrbtn5\0" //40
    "clrbtn6\0" //48
    "clrbtn7\0" //56
);

///////////////////////////////////////////////////////////////////////////////
// 3) CANVAS WIDGET:
// ----------------------------------------------------------------------------
//   A - application
//   P - parrent
//
template<class A, class P>
class DrawerCanvas {
    public:
    // Base type
        typedef DrawerCanvas<A,P> base_type;
    // Canvas definition
        typedef BaseCanvas<A,P> Canvas;
    private:
        A *m_app;
        P *m_parrent;
        Canvas m_canvas;
    public:
    // Canvas initialization
        DrawerCanvas(
            A *app,
            P *parrent
        )
        : m_app(app)
        , m_parrent(parrent)
        , m_canvas(app,parrent,"draw_da")
        {
            // Setup colors
            for (int i = 0; i < 8; ++i) {
                m_canvas.AllocColor(i,
                    ((i & 4) == 4) ? 65535 : 0,
                    ((i & 2) == 2) ? 65535 : 0,
                    ((i & 1) == 1) ? 65535 : 0
                );
            }
            // Clear color is white
            m_canvas.SetClearColor(CANVAS_COLOR_WHITE);
            // Foreground draw is black as default
            m_canvas.SetDrawForeground(CANVAS_COLOR_BLACK);
            // Background draw is white as default
            m_canvas.SetDrawBackground(CANVAS_COLOR_WHITE);
            // Foreground fill is black as default
            m_canvas.SetFillForeground(CANVAS_COLOR_BLACK);
            // Background fill is white as default
            m_canvas.SetFillBackground(CANVAS_COLOR_WHITE);
            // Line style is solid as default
            m_canvas.SetLineStyle(LineSolid);
            // Line width is 0 as default
            m_canvas.SetLineWidth(0);
            // Point as default
            m_canvas.SetShape(Canvas::Shape::POINT);
        }
        ~DrawerCanvas() {}
    // Manage canvas
        void Manage()
        {
            m_canvas.Manage();
        }
    // Obtain canvas handle
        Widget Handle()
        {
            return m_canvas.Handle();
        }
    // Slot for signal handling
        void *Slot(int signal, Widget w, XtPointer client, XtPointer call)
        {
            switch (signal) {
                case CANVAS_INSTANCE:
                    S_TRACE("CANVAS_INSTANCE");
                    return static_cast<void *>(&m_canvas);
                    break;
                case CANVAS_CLEAR:
                    S_TRACE("CANVAS_CLEAR");
                    m_canvas.Clear();
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_POINT:
                    S_TRACE("CANVAS_USE_POINT");
                    m_canvas.SetShape(Canvas::Shape::POINT);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_LINE:
                    S_TRACE("CANVAS_USE_LINE");
                    m_canvas.SetShape(Canvas::Shape::LINE);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_RECT:
                    S_TRACE("CANVAS_USE_RECT");
                    m_canvas.SetShape(Canvas::Shape::RECT);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_FRECT:
                    S_TRACE("CANVAS_USE_FRECT");
                    m_canvas.SetShape(Canvas::Shape::FRECT);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_ELLIPSE:
                    S_TRACE("CANVAS_USE_ELLIPSE");
                    m_canvas.SetShape(Canvas::Shape::ELLIPSE);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_USE_FELLIPSE:
                    S_TRACE("CANVAS_USE_FELLIPSE");
                    m_canvas.SetShape(Canvas::Shape::FELLIPSE);
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_LNTHICK:
                    S_TRACE("CANVAS_SET_LNTHICK");
                    m_canvas.SetLineWidth(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_LNSTYLE:
                    S_TRACE("CANVAS_SET_LNSTYLE");
                    m_canvas.SetLineStyle(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_DBGCOLOR:
                    S_TRACE("CANVAS_SET_DBGCOLOR");
                    m_canvas.SetDrawBackground(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_DFGCOLOR:
                    S_TRACE("CANVAS_SET_DFGCOLOR");
                    m_canvas.SetDrawForeground(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_FBGCOLOR:
                    S_TRACE("CANVAS_SET_FBGCOLOR");
                    m_canvas.SetFillBackground(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case CANVAS_SET_FFGCOLOR:
                    S_TRACE("CANVAS_SET_FFGCOLOR");
                    m_canvas.SetFillForeground(*static_cast<int *>(client));
                    return static_cast<void *>(this);
                    break;
                case A::POST_MANAGE:
                case A::EXIT_CLEANUP:
                    return m_canvas.Slot(signal, w, client, call);
                    break;
                default:
                    break;
            }
            return static_cast<void *>(0);
        }
    private:
    // Forbid implicit methods
        DrawerCanvas();
        DrawerCanvas(const DrawerCanvas &);
        DrawerCanvas & operator =(const DrawerCanvas &);
};

///////////////////////////////////////////////////////////////////////////////
// Definition of application layout:
// ---------------------------------
//   A - application
//
template<class A>
class ApplicationWidgets {
    public:
        typedef BaseForm<A> Form;
        typedef ApplicationMenu<A,Form> Menu;
        typedef DrawerToolbar<A,Form> Toolbar;
        typedef DrawerCanvas<A,Form> Canvas;
    private:
        Form m_layout;
        Menu m_menu;
        Toolbar m_toolbar;
        Canvas m_canvas;
    public:
        ApplicationWidgets(A *app)
        : m_layout(app,"GuxDrawLayout")
        , m_menu(app,&m_layout)
        , m_toolbar(app,&m_layout)
        , m_canvas(app,&m_layout)
        {
            m_layout.AttachToTop(m_menu.Handle());
            m_layout.AttachToLeft(m_menu.Handle());
            m_layout.AttachToTopWidget(
                m_toolbar.Handle(), m_menu.Handle()
            );
            m_layout.AttachToLeft(m_toolbar.Handle());
            m_layout.AttachToBottom(m_toolbar.Handle());
            m_layout.AttachToTopWidget(
                m_canvas.Handle(), m_menu.Handle()
            );
            m_layout.AttachToLeftWidget(
                m_canvas.Handle(), m_toolbar.Handle()
            );
            m_layout.AttachToBottom(m_canvas.Handle());
            m_layout.AttachToRight(m_canvas.Handle());
        }
        ~ApplicationWidgets() {}
        Widget Handle()
        {
            return m_layout.Handle();
        }
        void Manage()
        {
            m_menu.Manage();
            m_toolbar.Manage();            
            m_canvas.Manage();
            m_layout.Manage();
        }
        void *Slot(
            int signal,
            Widget widget, XtPointer client_data, XtPointer call_data
        )
        {
            switch (signal) {
                case A::POST_MANAGE:
                case A::EXIT_CLEANUP:
                    m_canvas.Slot(signal,widget,client_data,call_data);
                    m_toolbar.Slot(signal,widget,client_data,call_data);
                    return static_cast<void *>(this);
                    break;
                default:
                    return m_canvas.Slot(signal,widget,client_data,call_data);
                    break;
            }
            return 0;
        }
};

typedef BaseApplication<
    ApplicationGeometry,ApplicationWidgets,APP_SIG_BASE
> Application;

int main(int argc, char *argv[])
{
    Application app(&argc,argv,"Draw","GUX 2011 Draw");

    app.Run();
    return 0;
}
