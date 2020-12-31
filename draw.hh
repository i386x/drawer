//                                                    geany_encoding=ISO-8859-2
//! \file    draw.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-12-01
//! \project GUX 2011 Draw (project #1)
//! \brief   Draw toolkit.
//

#ifndef GUX2011_DRAW_HH
#define GUX2011_DRAW_HH

///////////////////////////////////////////////////////////////////////////////
//! \file    defs.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-28
//! \project GUX 2011 Draw (project #1)
//! \brief   Common includes and definitions.
//

#ifndef GUX2011_DEFS_HH
#define GUX2011_DEFS_HH

#define TRACING_ON 0
#define TRACE_INIT 1
#define TRACE_HAND 1
#define TRACE_SIGS 1
#define TRACE_VARS 1

#if (TRACING_ON != 0)
#    define LABEL(l) std::cout << l << std::endl
#else
#    define LABEL(l)
#endif

#if (TRACING_ON != 0 && TRACE_INIT != 0)
#    define I_TRACE0(k) std::cout << k << " initialized." << std::endl
#    define I_TRACE1(k,n1) std::cout << k << "<" << n1 << ">" << std::endl
#    define I_TRACE2(k,n1,n2) std::cout << k << "<" << n1 << ", " << n2 \
         << ">" << std::endl
#else
#    define I_TRACE0(k)
#    define I_TRACE1(k,n1)
#    define I_TRACE2(k,n1,n2)
#endif

#if (TRACING_ON != 0 && TRACE_HAND != 0)
#    define H_TRACE(m) std::cout << m << " handled." << std::endl
#else
#    define H_TRACE(m)
#endif

#if (TRACING_ON != 0 && TRACE_SIGS != 0)
#    define S_TRACE(s) std::cout << s << " received." << std::endl
#else
#    define S_TRACE(s)
#endif

#if (TRACING_ON != 0 && TRACE_VARS != 0)
#    define V_TRACE(p,v) std::cout << p << #v << " = " << v << std::endl
#else
#    define V_TRACE(p,v)
#endif

#include <map>
#include <vector>
#include <iostream>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <X11/Intrinsic.h>
#include <Xm/Xm.h>
#include <Xm/MwmUtil.h>
#include <X11/Shell.h>
#include <Xm/Text.h>
#include <Xm/AtomMgr.h>
#include <Xm/Protocols.h>
#include <Xm/MessageB.h>
#include <Xm/RowColumn.h>
#include <Xm/Form.h>
#include <Xm/LabelG.h>
#include <Xm/PushB.h>
#include <Xm/PushBG.h>
#include <Xm/DialogS.h>
#include <Xm/PanedW.h>
#include <Xm/ScrolledW.h>
#include <Xm/DrawingA.h>

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    Geometry.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-28
//! \project GUX 2011 Draw (project #1)
//! \brief   Template for widget geometry definition.
//

#ifndef GUX2011_GEOMETRY_HH
#define GUX2011_GEOMETRY_HH

template<int x, int y, int w, int h, int mw, int mh>
struct Geometry {
    enum {
        xpos = x,
        ypos = y,
        width = w,
        height = h,
        minw = mw,
        minh = mh
    };
};

template<int x, int y, int w, int h>
struct Geometry<x,y,w,h,0,0> {
    enum {
        xpos = x,
        ypos = y,
        width = w,
        height = h,
        minw = 0,
        minh = 0
    };
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    NewType.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-28
//! \project GUX 2011 Draw (project #1)
//! \brief   This template create new, unique type.
//

#ifndef GUX2011_NEWTYPE_HH
#define GUX2011_NEWTYPE_HH

template<class B, int V>
struct NewType {
    enum { value = V };
    typedef B base_type;
    typedef NewType<B,V> Type;
    static Type MakeType() { return Type(); };
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    trampolines.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-28
//! \project GUX 2011 Draw (project #1)
//! \brief   Callback functions template, which redirect call to associated
//!          class.
//

#ifndef GUX2011_TRAMPOLINES_HH
#define GUX2011_TRAMPOLINES_HH

template<class T>
static void trampolineW(
    Widget widget, XtPointer client_data, XtPointer call_data
)
{
    // Get instance of wrapper class...
    typename T::base_type *caller = static_cast<typename T::base_type *>(client_data);
    // ...and do right work:
    (caller->*(T::base_type::GetHandler(T::MakeType())))(widget,call_data);
}

template<class T>
static void trampolineM(
    Widget widget, XtPointer client_data, XtPointer call_data
)
{
    // Because menu callbacks returns pressed button IDs, we must use class
    // variables to obtain pointer to caller (let all menus are singletons):
    typename T::base_type *caller = T::base_type::GetInstance();
    (caller->*(T::base_type::GetHandler(T::MakeType())))(
        widget, reinterpret_cast<int>(client_data), call_data
    );
}

template<class T>
static void trampolineA(
    Widget widget, XEvent *event, String *args, Cardinal *nargs
)
{
    typename T::base_type *caller = T::base_type::GetInstance();
    (caller->*(T::base_type::GetHandler(T::MakeType())))(
        widget, event, args, nargs
    );
}

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    Assert.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-16
//! \project GUX 2011 Draw (project #1)
//! \brief   Assertion control in constructors.
//

#ifndef GUX2011_ASSERT_HH
#define GUX2011_ASSERT_HH

class Assert {
    public:
        Assert(bool cond, const char *msg)
        {
            if (!cond) {
                std::cerr << "Assertion failed: " << msg << std::endl;
                std::exit(1);
            }
        }
        ~Assert() {}
    private:
    // Forbidden methods
        Assert();
        Assert(const Assert &);
        Assert & operator =(const Assert &);
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseForm.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-05
//! \project GUX 2011 Draw (project #1)
//! \brief   Template which wraps Motif Form class.
//

#ifndef GUX2011_BASEFORM_HH
#define GUX2011_BASEFORM_HH

template<class P>
class BaseForm {
    private:
    // Handle of form
        Widget m_form;
    public:
    // Create form
        BaseForm(P *parrent, const char *name)
        {
            m_form = XmCreateForm(
                parrent->Handle(),
                const_cast<String>(name),
                static_cast<ArgList>(0),
                0
            );
            I_TRACE1("BaseForm",name);
        }
        ~BaseForm() {}
    // Attachments
        void AttachToTop(Widget w)
        {
            XtVaSetValues(
                w,
                XmNtopAttachment, XmATTACH_FORM,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToBottom(Widget w)
        {
            XtVaSetValues(
                w,
                XmNbottomAttachment, XmATTACH_FORM,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToLeft(Widget w)
        {
            XtVaSetValues(
                w,
                XmNleftAttachment, XmATTACH_FORM,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToRight(Widget w)
        {
            XtVaSetValues(
                w,
                XmNrightAttachment, XmATTACH_FORM,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToTopWidget(Widget w, Widget top)
        {
            XtVaSetValues(
                w,
                XmNtopAttachment, XmATTACH_WIDGET,
                XmNtopWidget, top,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToBottomWidget(Widget w, Widget bottom)
        {
            XtVaSetValues(
                w,
                XmNbottomAttachment, XmATTACH_WIDGET,
                XmNbottomWidget, bottom,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToLeftWidget(Widget w, Widget left)
        {
            XtVaSetValues(
                w,
                XmNleftAttachment, XmATTACH_WIDGET,
                XmNleftWidget, left,
                static_cast<XtPointer>(0)
            );
        }
        void AttachToRightWidget(Widget w, Widget right)
        {
            XtVaSetValues(
                w,
                XmNrightAttachment, XmATTACH_WIDGET,
                XmNrightWidget, right,
                static_cast<XtPointer>(0)
            );
        }
        void TopOffset(Widget w, int offset)
        {
            XtVaSetValues(
                w,
                XmNtopOffset, offset,
                static_cast<XtPointer>(0)
            );
        }
        void BottomOffset(Widget w, int offset)
        {
            XtVaSetValues(
                w,
                XmNbottomOffset, offset,
                static_cast<XtPointer>(0)
            );
        }
        void LeftOffset(Widget w, int offset)
        {
            XtVaSetValues(
                w,
                XmNleftOffset, offset,
                static_cast<XtPointer>(0)
            );
        }
        void RightOffset(Widget w, int offset)
        {
            XtVaSetValues(
                w,
                XmNrightOffset, offset,
                static_cast<XtPointer>(0)
            );
        }
    // Manage this widget
        void Manage()
        {
            XtManageChild(m_form);
        }
    // Handle to exist form
        Widget Handle()
        {
            return m_form;
        }
    // Slot for signal receiving
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    private:
    // Forbidden methods
        BaseForm();
        BaseForm(const BaseForm &);
        BaseForm & operator =(const BaseForm &);
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseSimpleMenuBar.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-29
//! \project GUX 2011 Draw (project #1)
//! \brief   Template for simple menu bar.
//

#ifndef GUX2011_BASESIMPLEMENUBAR_HH
#define GUX2011_BASESIMPLEMENUBAR_HH

template<class P>
class BaseSimpleMenuBar {
    public:
    // Menu item definition struct
        typedef struct tagItemType {
            int id;
            String name;
            KeySym mnic;
        } ItemType;
    private:
    // Binding between button position number and its ID
        std::map<int,int> m_id_button;
    // Handle to menubar
        Widget m_menubar;
    public:
    // Create simple menu bar in constructor
        BaseSimpleMenuBar(P *parrent, ItemType *items, int nitems)
        {
            int max_count = (nitems > 0) ? nitems : 1;
            XmString *button_strings = new XmString[max_count];
            KeySym *button_mnemonics = new KeySym[max_count];
            unsigned int n;
            Arg arglist[3];

            for (int i = 0; i < max_count; i++) {
                if (i < nitems) {
                    m_id_button[items[i].id] = i;
                    button_strings[i] = XmStringCreateLocalized(items[i].name);
                    button_mnemonics[i] = items[i].mnic;
                }
                else {
                    button_strings[i] = 0;
                    button_mnemonics[i] = 0;
                }
            }
            n = 0;
            XtSetArg(arglist[n],
                XmNbuttonCount, int((nitems < 0) ? 0 : nitems)
            ); n++;
            XtSetArg(arglist[n], XmNbuttons, button_strings); n++;
            XtSetArg(arglist[n], XmNbuttonMnemonics, button_mnemonics); n++;
            assert(n <= XtNumber(arglist));
            m_menubar = XmCreateSimpleMenuBar(
                parrent->Handle(),
                const_cast<String>("SimpleMenuBar"),
                arglist, n
            );
            for (int i = 0; i < max_count; i++) {
                int j = max_count - i - 1;
                if (button_strings[j] != 0) {
                    XmStringFree(button_strings[j]);
                }
            }
            delete[] button_mnemonics;
            delete[] button_strings;
            I_TRACE0("BaseSimpleMenuBar");
        }
        ~BaseSimpleMenuBar() {}
    // Return button order number according its ID
        int GetButton(int id)
        {
            std::map<int,int>::iterator it = m_id_button.find(id);
            if (it == m_id_button.end()) {
                return -1;
            }
            return (*it).second;
        }
    // Manage this widget (called recursively)
        void Manage()
        {
            XtManageChild(m_menubar);
        }
    // Handle of simple menu bar
        Widget Handle()
        {
            return m_menubar;
        }
    // Slot for signal receiving
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    private:
    // Forbidden methods
        BaseSimpleMenuBar();
        BaseSimpleMenuBar(const BaseSimpleMenuBar &);
        BaseSimpleMenuBar & operator =(const BaseSimpleMenuBar &);
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseSimplePulldownMenu.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-03
//! \project GUX 2011 Draw (project #1)
//! \brief   Template for simple pulldown menu.
//

#ifndef GUX2011_BASESIMPLEPULLDOWNMENU_HH
#define GUX2011_BASESIMPLEPULLDOWNMENU_HH

template<class P, int I>
class BaseSimplePulldownMenu {
    public:
    // Menu item definition struct
        typedef struct tagItemType {
            int id;
            String name;
            KeySym mnic;
            String accel;
            String taccel;
        } ItemType;
    private:
    // Map between button ID and button order number
        std::map<int,int> m_id_button;
    // Pulldown menu handle
        Widget m_pulldown;
    public:
    // Create simple pulldown menu
        BaseSimplePulldownMenu(
            P *parrent,
            const char *name,
            XtCallbackProc callback,
            ItemType *items,
            int nitems
        )
        {
            int max_count = (nitems > 0) ? nitems : 1;
            XmButtonType *button_types = new XmButtonType[max_count];
            XmString *button_strings = new XmString[max_count];
            KeySym *button_mnemonics = new KeySym[max_count];
            String *button_accel = new String[max_count];
            XmString *button_acc_text = new XmString[max_count];
            unsigned int n;
            Arg arglist[8];

            for (int i = 0; i < max_count; i++) {
                if (i < nitems) {
                    m_id_button[items[i].id] = i;
                    button_types[i] = XmPUSHBUTTON;
                    button_strings[i] = XmStringCreateLocalized(items[i].name);
                    button_mnemonics[i] = items[i].mnic;
                    button_accel[i] = items[i].accel;
                    button_acc_text[i] = (items[i].taccel)
                    ? (XmStringCreateLocalized(items[i].taccel))
                    : (0);
                }
                else {
                    button_types[i] = 0;
                    button_strings[i] = 0;
                    button_mnemonics[i] = 0;
                    button_accel[i] = 0;
                    button_acc_text[i] = 0;
                }
            }
            n = 0;
            XtSetArg(
                arglist[n], XmNpostFromButton, parrent->GetButton(I)
            ); n++;
            XtSetArg(arglist[n], XmNsimpleCallback, callback); n++;
            XtSetArg(arglist[n],
                XmNbuttonCount, int((nitems < 0) ? 0 : nitems)
            ); n++;
            XtSetArg(arglist[n], XmNbuttonType, button_types); n++;
            XtSetArg(arglist[n], XmNbuttons, button_strings); n++;
            XtSetArg(arglist[n], XmNbuttonMnemonics, button_mnemonics); n++;
            XtSetArg(arglist[n], XmNbuttonAccelerators, button_accel); n++;
            XtSetArg(
                arglist[n], XmNbuttonAcceleratorText, button_acc_text
            ); n++;
            assert(n <= XtNumber(arglist));
            m_pulldown = XmCreateSimplePulldownMenu(
                parrent->Handle(),
                const_cast<String>(name),
                arglist, n
            );
            for (int i = 0; i < max_count; i++) {
                int j = max_count - i - 1;
                if (button_acc_text[j] != 0) {
                    XmStringFree(button_acc_text[j]);
                }
                if (button_strings[j] != 0) {
                    XmStringFree(button_strings[j]);
                }
            }
            delete[] button_acc_text;
            delete[] button_accel;
            delete[] button_mnemonics;
            delete[] button_strings;
            delete[] button_types;
            I_TRACE1("BaseSimpleMenu",name);
        }
        ~BaseSimplePulldownMenu() {}
    // Get button order number according its ID
        int GetButton(int id)
        {
            std::map<int,int>::iterator it = m_id_button.find(id);
            if (it == m_id_button.end()) {
                return -1;
            }
            return (*it).second;
        }
    // Manage widget (don't care in pulldowns)
        void Manage() {}
    // Handle of pulldown menu
        Widget Handle()
        {
            return m_pulldown;
        }
    // Slot for signal receiving
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    private:
    // Forbidden methods
        BaseSimplePulldownMenu();
        BaseSimplePulldownMenu(const BaseSimplePulldownMenu &);
        BaseSimplePulldownMenu & operator =(const BaseSimplePulldownMenu &);
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BasePushButton.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-06
//! \project GUX 2011 Draw (project #1)
//! \brief   Template which wraps Motif PushButton.
//

#ifndef GUX2011_BASEPUSHBUTTON_HH
#define GUX2011_BASEPUSHBUTTON_HH

template<class A, class P>
class BasePushButton {
    public:
    // Rendering VM's instructions
        enum {
            END = 0,            // ()
            IMAGE,              // (width, height)
            TEXT,               // (offset)
            POINT,              // (x,y)
            LINE,               // (x1,y1,x2,y2)
            RECT,               // (x,y,width,height)
            FRECT,              // (x,y,width,height)
            ARC,                // (x,y,width,height,start,stop)
            FARC,               // (x,y,width,height,start,stop)
            LINE_STYLE_SOLID,   // ()
            LINE_STYLE_DASHED,  // ()
            LINE_STYLE_DDASHED, // ()
            BITMAP,             // (width,height,offset)
            BG_COLOR,           // (offset)
            FG_COLOR            // (offset)
        };
    private:
        P *m_parrent;
        GC m_gc;
        Pixmap m_pixmap;
        Widget m_button;
        std::vector<Pixel> m_acolors;
    public:
    // Create push button
        BasePushButton(
            P *parrent,
            const char *name,
            int res[], int nres, char *data
        )
        : m_parrent(parrent),m_gc(0),m_pixmap(0),m_button(0)
        {
            XGCValues gcv;
            Pixel bg,fg;
            bool quitloop;

            // 1) Inherits FG and BG from parrent:
            XtVaGetValues(
                parrent->Handle(),
                XmNforeground, &fg,
                XmNbackground, &bg,
                static_cast<XtPointer>(0)
            );
            gcv.foreground = static_cast<unsigned long>(fg);
            gcv.background = static_cast<unsigned long>(bg);
            gcv.function = GXcopy;
            gcv.line_width = 1;
            gcv.line_style = LineSolid;
            gcv.fill_style = FillSolid;
            // 2) Process resources (bytecode for tiny rendering machine):
            quitloop = false;
            for (int i = 0; i < nres; i++) {
                switch (res[i]) {
                    case END:
                        quitloop = true;
                        break;
                    case IMAGE:
                        if (i + 2 < nres) {
                            // Create pixmap
                            m_pixmap = XCreatePixmap(
                                XtDisplay(parrent->Handle()),
                                RootWindowOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                static_cast<unsigned int>(res[i + 1]),
                                static_cast<unsigned int>(res[i + 2]),
                                DefaultDepthOfScreen(XtScreen(
                                    parrent->Handle()
                                ))
                            );
                            // Create GC
                            m_gc = XCreateGC(
                                XtDisplay(parrent->Handle()),
                                RootWindowOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                GCForeground | GCBackground | GCFunction |
                                GCLineStyle  | GCLineWidth  | GCFillStyle,
                                &gcv
                            );
                            gcv.foreground = static_cast<unsigned long>(bg);
                            GC cgc = XCreateGC(
                                XtDisplay(parrent->Handle()),
                                RootWindowOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                GCForeground | GCBackground | GCFunction |
                                GCLineStyle  | GCLineWidth  | GCFillStyle,
                                &gcv
                            );
                            gcv.foreground = static_cast<unsigned long>(fg);
                            // Clear bitmap
                            XFillRectangle(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, cgc, 0, 0,
                                static_cast<unsigned int>(res[i + 1]),
                                static_cast<unsigned int>(res[i + 2])
                            );
                            XFreeGC(XtDisplay(parrent->Handle()),cgc);
                            i += 2;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case TEXT:
                        // Button is text
                        if (i + 1 < nres && data != 0) {
                            m_button = XmCreatePushButton(
                                parrent->Handle(), &data[res[++i]],
                                static_cast<ArgList>(0), 0
                            );
                        }
                        quitloop = true;
                        break;
                    case POINT:
                        if (i + 2 < nres) {
                            XDrawPoint(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2]
                            );
                            i += 2;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case LINE:
                        if (i + 4 < nres) {
                            XDrawLine(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2], res[i + 3], res[i + 4]
                            );
                            i += 4;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case RECT:
                        if (i + 4 < nres) {
                            XDrawRectangle(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2],
                                static_cast<unsigned int>(res[i + 3]),
                                static_cast<unsigned int>(res[i + 4])
                            );
                            i += 4;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case FRECT:
                        if (i + 4 < nres) {
                            XFillRectangle(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2],
                                static_cast<unsigned int>(res[i + 3]),
                                static_cast<unsigned int>(res[i + 4])
                            );
                            i += 4;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case ARC:
                        if (i + 6 < nres) {
                            XDrawArc(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2],
                                static_cast<unsigned int>(res[i + 3]),
                                static_cast<unsigned int>(res[i + 4]),
                                res[i + 5], res[i + 6]
                            );
                            i += 6;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case FARC:
                        if (i + 6 < nres) {
                            XFillArc(
                                XtDisplay(parrent->Handle()),
                                m_pixmap, m_gc,
                                res[i + 1], res[i + 2],
                                static_cast<unsigned int>(res[i + 3]),
                                static_cast<unsigned int>(res[i + 4]),
                                res[i + 5], res[i + 6]
                            );
                            i += 6;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case LINE_STYLE_SOLID:
                        gcv.line_style = LineSolid;
                        XChangeGC(
                            XtDisplay(parrent->Handle()),
                            m_gc, GCLineStyle, &gcv
                        );
                        break;
                    case LINE_STYLE_DASHED:
                        gcv.line_style = LineOnOffDash;
                        XChangeGC(
                            XtDisplay(parrent->Handle()),
                            m_gc, GCLineStyle, &gcv
                        );
                        break;
                    case LINE_STYLE_DDASHED:
                        gcv.line_style = LineDoubleDash;
                        XChangeGC(
                            XtDisplay(parrent->Handle()),
                            m_gc, GCLineStyle, &gcv
                        );
                        break;
                    case BITMAP:
                        if (i + 3 < nres && data != 0) {
                            Pixmap bitmap = XCreatePixmapFromBitmapData(
                                XtDisplay(parrent->Handle()),
                                RootWindowOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                &data[res[i + 3]],
                                static_cast<unsigned int>(res[i + 1]),
                                static_cast<unsigned int>(res[i + 2]),
                                gcv.foreground, gcv.background,
                                DefaultDepthOfScreen(XtScreen(
                                    parrent->Handle()
                                ))
                            );
                            XCopyArea(
                                XtDisplay(parrent->Handle()),
                                bitmap, m_pixmap, m_gc,
                                0, 0,
                                static_cast<unsigned int>(res[i + 1]),
                                static_cast<unsigned int>(res[i + 2]),
                                0, 0
                            );
                            XFreePixmap(XtDisplay(parrent->Handle()),bitmap);
                            i += 3;
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case BG_COLOR:
                        if (i + 1 < nres && data != 0) {
                            String color = &data[res[++i]];
                            XColor col,unused;
                            if (XAllocNamedColor(
                                XtDisplay(parrent->Handle()),
                                DefaultColormapOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                color, &col, &unused
                            )) {
                                XSetBackground(
                                    XtDisplay(parrent->Handle()),
                                    m_gc, col.pixel
                                );
                                m_acolors.push_back(col.pixel);
                            }
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    case FG_COLOR:
                        if (i + 1 < nres && data != 0) {
                            String color = &data[res[++i]];
                            XColor col,unused;
                            if (XAllocNamedColor(
                                XtDisplay(parrent->Handle()),
                                DefaultColormapOfScreen(XtScreen(
                                    parrent->Handle()
                                )),
                                color, &col, &unused
                            )) {
                                XSetForeground(
                                    XtDisplay(parrent->Handle()),
                                    m_gc, col.pixel
                                );
                                m_acolors.push_back(col.pixel);
                            }
                        }
                        else {
                            quitloop = true;
                        }
                        break;
                    default:
                        quitloop = true;
                        break;
                }
                if (quitloop) {
                    break;
                }
            }
            // 3) Create button:
            if (m_button == 0) {
                unsigned int n;
                Arg args[2];
                n = 0;
                XtSetArg(args[n], XmNlabelType, XmPIXMAP); n++;
                XtSetArg(args[n], XmNlabelPixmap, m_pixmap); n++;
                assert(n <= XtNumber(args));
                m_button = XmCreatePushButton(
                    parrent->Handle(), const_cast<String>(name), args, n
                );
            }
            I_TRACE1("BasePushButton",name);
        }
        ~BasePushButton() {}
    // Set callback routine
        void SetCallback(XtCallbackProc fnc, XtPointer data)
        {
            XtAddCallback(m_button, XmNactivateCallback, fnc, data);
        }
    // Handle of button
        Widget Handle()
        {
            return m_button;
        }
    // Slot for signal receiving
        void *Slot(int signal, Widget, XtPointer, XtPointer)
        {
            if (signal == A::EXIT_CLEANUP) {
                Display *display = XtDisplay(m_parrent->Handle());
                Colormap colormap = DefaultColormapOfScreen(XtScreen(
                    m_parrent->Handle()
                ));
                for (
                    std::vector<Pixel>::iterator i = m_acolors.begin();
                    i < m_acolors.end();
                    ++i
                ) {
                    Pixel c = *i;
                    XFreeColors(display,colormap,&c,1,0);
                }
                if (m_gc) {
                    XFreeGC(display,m_gc);
                }
                if (m_pixmap) {
                    XFreePixmap(display,m_pixmap);
                }
                return static_cast<void *>(this);
            }
            return 0;
        }
    // Managing button (called by parrent->Manage())
        void Manage()
        {
            XtManageChild(m_button);
        }
    private:
    // Forbidden methods
        BasePushButton();
        BasePushButton(const BasePushButton &);
        BasePushButton & operator =(const BasePushButton &);
};

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseModalRadioDialog.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-12
//! \project GUX 2011 Draw (project #1)
//! \brief   Template of modal dialog with radio buttons.
//

#ifndef GUX2011_BASEMODALRADIODIALOG_HH
#define GUX2011_BASEMODALRADIODIALOG_HH

template<class A, class P, int N, int U>
class BaseModalRadioDialog {
    private:
    // Internal constants
        enum {
            MAXCNT = (N > 0) ? N : 1,
            NARGS = 4
        };
    public:
    // Unique ID, which ensure us the template instantiation makes unique type
        enum { UID = U };
    // How the dialog has been canceled
        enum {
            NONE, OK, CANCEL
        };
    // Resource item
        typedef struct tagItemType {
            int id;
            String name;
        } ItemType;
    // Base type and handlers types
        typedef BaseModalRadioDialog<A,P,N,U> base_type;
        typedef void (base_type::*handlerm_type)(Widget, int, XtPointer);
        typedef void (base_type::*handlerw_type)(Widget, XtPointer);
    // Handlers IDs
        typedef NewType<base_type,0> SELECT_HANDLER;
        typedef NewType<base_type,1> DESTROY_HANDLER;
        typedef NewType<base_type,2> OK_HANDLER;
        typedef NewType<base_type,3> CANCEL_HANDLER;
    private:
        static base_type *m_this;
        Assert m_assert;
        A *m_app;
        P *m_parrent;
        char *m_name;
        std::map<int,int> m_button_id;
        XmString m_button_strings[MAXCNT];
        Arg m_arglist[NARGS];
        int m_cancelation;
        int m_toggled;
        int m_returned;
        bool m_running;
        Widget m_topshell;
    public:
    // Constructor
        BaseModalRadioDialog(
            A *application,
            P *parrent,
            const char *name,
            int button_set,
            ItemType *items
        )
        : m_assert(m_this == 0, // Ensure we have only one instance
              "There is still `BaseModalRadioDialog` instance."
          )
        , m_app(application)
        , m_parrent(parrent)
        , m_name(const_cast<char *>(name))
        {
            unsigned int n;
            for (int i = 0; i < MAXCNT; i++) {
                if (i < N) {
                    m_button_id[i] = items[i].id;
                    m_button_strings[i] = XmStringCreateLocalized(
                        items[i].name
                    );
                }
                else {
                    m_button_strings[i] = 0;
                }
            }
            n = 0;
            XtSetArg(m_arglist[n], XmNbuttonSet, button_set); n++;
            XtCallbackProc callback_proc = trampolineM<SELECT_HANDLER>;
            XtSetArg(m_arglist[n], XmNsimpleCallback, callback_proc); n++;
            XtSetArg(m_arglist[n], XmNbuttonCount, int((N > 0) ? N : 0)); n++;
            XtSetArg(m_arglist[n], XmNbuttons, m_button_strings); n++;
            assert(n <= NARGS);
            m_cancelation = NONE;
            m_toggled = button_set;
            m_returned = button_set;
            m_running = false;
            m_topshell = m_parrent->Handle();
            while (m_topshell && !XtIsWMShell(m_topshell)) {
                m_topshell = XtParent(m_topshell);
            }
            m_this = this;
        }
    // Destructor
        ~BaseModalRadioDialog()
        {
            for (int i = 0; i < MAXCNT; i++) {
                int j = MAXCNT - i - 1;
                if (m_button_strings[j] != 0) {
                    XmStringFree(m_button_strings[j]);
                }
            }
            m_this = 0;
        }
    // Popups the dialog and wait for user response
        int Show()
        {
            unsigned int n;
            Arg arglist[2];

            // Create dialog
            n = 0;
            XtSetArg(arglist[n], XmNdeleteResponse, XmDO_NOTHING); n++;
            XtSetArg(arglist[n],
                XmNmwmInputMode, MWM_INPUT_FULL_APPLICATION_MODAL
            ); n++;
            assert(n <= XtNumber(arglist));
            Widget dialog = XmCreateDialogShell(
                m_topshell, m_name, arglist, n
            );
            // Set delete callback
            Atom WM_DELETE_WINDOW = XmInternAtom(
                XtDisplay(dialog),
                const_cast<String>("WM_DELETE_WINDOW"),
                False
            );
            XmAddWMProtocolCallback(
                dialog,
                WM_DELETE_WINDOW,
                trampolineW<DESTROY_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Create paned window with tiny sashes
            n = 0;
            XtSetArg(arglist[n], XmNsashWidth, static_cast<Dimension>(1)); n++;
            XtSetArg(
                arglist[n], XmNsashHeight, static_cast<Dimension>(1)
            ); n++;
            assert(n <= XtNumber(arglist));
            Widget pane = XmCreatePanedWindow(
                dialog, const_cast<String>("pane"), arglist, n
            );
            // Create radiobox as control part
            n = 0;
            XtSetArg(m_arglist[n], XmNbuttonSet, m_returned); n++;
            assert(n <= XtNumber(arglist));
            Widget radiobox = XmCreateSimpleRadioBox(
                pane, const_cast<String>("radiobox"), m_arglist, NARGS
            );
            XtManageChild(radiobox);
            // Create form for action part
            n = 0;
            XtSetArg(arglist[n], XmNfractionBase, 5); n++;
            assert(n <= XtNumber(arglist));
            Widget form = XmCreateForm(
                pane, const_cast<String>("form"), arglist, n
            );
            // Create OK button
            Widget ok = XmCreatePushButtonGadget(
                form, const_cast<String>("OK"), 0, 0
            );
            XtVaSetValues(ok,
                XmNtopAttachment,                XmATTACH_FORM,
                XmNbottomAttachment,             XmATTACH_FORM,
                XmNleftAttachment,               XmATTACH_POSITION,
                XmNleftPosition,                 1,
                XmNrightAttachment,              XmATTACH_POSITION,
                XmNrightPosition,                2,
                XmNshowAsDefault,                True,
                XmNdefaultButtonShadowThickness, static_cast<Dimension>(1),
                static_cast<XtPointer>(0)
            );
            XtManageChild(ok);
            XtAddCallback(
                ok,
                XmNactivateCallback,
                trampolineW<OK_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Create Cancel button
            Widget cancel = XmCreatePushButtonGadget(
                form, const_cast<String>("Cancel"), 0, 0
            );
            XtVaSetValues(cancel,
                XmNtopAttachment,                XmATTACH_FORM,
                XmNbottomAttachment,             XmATTACH_FORM,
                XmNleftAttachment,               XmATTACH_POSITION,
                XmNleftPosition,                 3,
                XmNrightAttachment,              XmATTACH_POSITION,
                XmNrightPosition,                4,
                XmNshowAsDefault,                False,
                XmNdefaultButtonShadowThickness, static_cast<Dimension>(1),
                static_cast<XtPointer>(0)
            );
            XtManageChild(cancel);
            XtAddCallback(
                cancel,
                XmNactivateCallback,
                trampolineW<CANCEL_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Fix the action area & disable resizing
            Dimension h;
            XtManageChild(form);
            XtVaGetValues(cancel, XmNheight, &h, static_cast<XtPointer>(0));
            XtVaSetValues(form,
                XmNpaneMaximum, h,
                XmNpaneMinimum, h,
                static_cast<XtPointer>(0)
            );
            XtManageChild(pane);
            // Wait for user answer
            m_cancelation = NONE;
            m_running = true;
            while (m_running || XtAppPending(m_app->Context())) {
                XtAppProcessEvent(m_app->Context(), XtIMAll);
            }
            // Destroy dialog
            XtDestroyWidget(dialog);
            // Return dialog cancelation mode
            return m_cancelation;
        }
    // Return select choice
        int Toggled()
        {
            std::map<int,int>::iterator it = m_button_id.find(m_returned);
            if (it == m_button_id.end()) {
                return -1;
            }
            return (*it).second;
        }
    // Nothing to manage
        void Manage() {}
    // This class is don't wraps any widget
        Widget Handle()
        {
            return 0;
        }
    // Slot for signal receiving
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    // Return instance of this class
        static base_type *GetInstance()
        {
            return m_this;
        }
    // Handlers
        void SelectHandler(Widget, int button, XtPointer)
        {
            this->m_toggled = button;
        }
        void DestroyHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_cancelation = CANCEL;
        }
        void OkHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_returned = this->m_toggled;
            this->m_cancelation = OK;
        }
        void CancelHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_cancelation = CANCEL;
        }
    private:
    // Forbidden methods
        BaseModalRadioDialog();
        BaseModalRadioDialog(const BaseModalRadioDialog &);
        BaseModalRadioDialog & operator =(const BaseModalRadioDialog &);
    public:
    // Handler map
        static handlerm_type GetHandler(SELECT_HANDLER)
        {
            return &base_type::SelectHandler;
        }
        static handlerw_type GetHandler(DESTROY_HANDLER)
        {
            return &base_type::DestroyHandler;
        }
        static handlerw_type GetHandler(OK_HANDLER)
        {
            return &base_type::OkHandler;
        }
        static handlerw_type GetHandler(CANCEL_HANDLER)
        {
            return &base_type::CancelHandler;
        }
};

template<class A, class P, int N, int U>
typename BaseModalRadioDialog<A,P,N,U>::base_type *
BaseModalRadioDialog<A,P,N,U>::m_this(0);

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseModalColorDialog.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-17
//! \project GUX 2011 Draw (project #1)
//! \brief   Template of modal dialog for color selection.
//

#ifndef GUX2011_BASEMODALCOLORDIALOG_HH
#define GUX2011_BASEMODALCOLORDIALOG_HH

template<class A, class P, int N, int U>
class BaseModalColorDialog {
    private:
        enum {
            MAXCNT = (N > 0) ? N : 1
        };
    public:
    // Unique ID, like in BaseModalRadioDialog
        enum { UID = U };
    // Cancelation modes
        enum { NONE, OK, CANCEL };
    // Base type and handlers types
        typedef BaseModalColorDialog<A,P,N,U> base_type;
        typedef void (base_type::*handlerm_type)(Widget, int, XtPointer);
        typedef void (base_type::*handlerw_type)(Widget, XtPointer);
    // Handlers IDs
        typedef NewType<base_type,0> SELECT_HANDLER;
        typedef NewType<base_type,1> DESTROY_HANDLER;
        typedef NewType<base_type,2> OK_HANDLER;
        typedef NewType<base_type,3> CANCEL_HANDLER;
    private:
    // This class instance
        static base_type *m_this;
    // Constructor assertion
        Assert m_assert;
    // Parameters holders
        A *m_app;
        P *m_parrent;
        char *m_name;
        int m_color_set;
        int m_color_ret;
        int m_cols;
        int *m_items;
        char *m_data;
    // Auxiliary stuff
        Widget m_topshell;
        int m_cancelation;
        bool m_running;
        Pixmap m_lpixmap;
        Widget m_label;
    // Temporary references
        Widget m_pane_ref;
        GC m_gc_ref;
        bool *m_valid_ref;
        Pixel *m_colors_ref;
    public:
    // Init
        BaseModalColorDialog(
            A *application,
            P *parrent,
            const char *name,
            int color_set,
            int cols,
            int *items,
            char *data
        )
        : m_assert(m_this == 0,
              "There is still `BaseModalColorDialog` instance."
          )
        , m_app(application)
        , m_parrent(parrent)
        , m_name(const_cast<char *>(name))
        , m_color_set(color_set)
        , m_color_ret(color_set)
        , m_cols(cols)
        , m_items(items)
        , m_data(data)
        {
            m_this = this;
            m_topshell = m_parrent->Handle();
            while (m_topshell && !XtIsWMShell(m_topshell)) {
                m_topshell = XtParent(m_topshell);
            }
            m_cancelation = NONE;
            m_running = false;
            m_lpixmap = 0;
            m_label = 0;
            m_pane_ref = 0;
            m_gc_ref = 0;
            m_valid_ref = 0;
            m_colors_ref = 0;
        }
    // Done
        ~BaseModalColorDialog()
        {
            m_this = 0;
        }
    // Popups the dialog
        int Show()
        {
            unsigned int n;
            Arg arglist[7];
            XtWidgetGeometry size;
            Dimension h;
            XGCValues gcv;
            GC gc;
            Pixel bg, fg;
            Widget buttons[MAXCNT];
            Pixel colors[MAXCNT];
            Pixmap pixmaps[MAXCNT];
            bool valid[MAXCNT];

            // Create dialog shell
            n = 0;
            XtSetArg(arglist[n], XmNdeleteResponse, XmDO_NOTHING); n++;
            XtSetArg(arglist[n],
                XmNmwmInputMode, MWM_INPUT_FULL_APPLICATION_MODAL
            ); n++;
            assert(n <= XtNumber(arglist));
            Widget dialog = XmCreateDialogShell(
                m_topshell, m_name, arglist, n
            );
            // Set delete callback
            Atom WM_DELETE_WINDOW = XmInternAtom(
                XtDisplay(dialog),
                const_cast<String>("WM_DELETE_WINDOW"),
                False
            );
            XmAddWMProtocolCallback(
                dialog,
                WM_DELETE_WINDOW,
                trampolineW<DESTROY_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Create paned window with tiny sashes
            n = 0;
            XtSetArg(arglist[n], XmNsashWidth, static_cast<Dimension>(1)); n++;
            XtSetArg(
                arglist[n], XmNsashHeight, static_cast<Dimension>(1)
            ); n++;
            assert(n <= XtNumber(arglist));
            Widget pane = XmCreatePanedWindow(
                dialog, const_cast<String>("pane"), arglist, n
            );
            // Create row column container
            n = 0;
            XtSetArg(arglist[n], XmNpacking, XmPACK_COLUMN); n++;
            XtSetArg(arglist[n], XmNnumColumns, m_cols); n++;
            XtSetArg(arglist[n], XmNadjustLast, False); n++;
            assert(n <= XtNumber(arglist));
            Widget container1 = XmCreateRowColumn(
                pane, const_cast<String>("c1"), arglist, n
            );
            // Create GC
            XtVaGetValues(
                pane,
                XmNforeground, &fg,
                XmNbackground, &bg,
                static_cast<XtPointer>(0)
            );
            gcv.foreground = static_cast<unsigned long>(fg);
            gcv.background = static_cast<unsigned long>(bg);
            gcv.function = GXcopy;
            gcv.line_width = 1;
            gcv.line_style = LineSolid;
            gcv.fill_style = FillSolid;
            gc = XCreateGC(
                XtDisplay(pane),
                RootWindowOfScreen(XtScreen(pane)),
                GCForeground | GCBackground | GCFunction |
                GCLineWidth  | GCLineStyle  | GCFillStyle,
                &gcv
            );
            // Create color buttons
            for (int i = 0; i < MAXCNT; i++) {
                // {COLOR_ID, offset, r,g,b}
                if (i < N) {
                    XColor color;
                    int b = i * 5;

                    color.red = static_cast<unsigned short>(m_items[b + 2]);
                    color.green = static_cast<unsigned short>(m_items[b + 3]);
                    color.blue = static_cast<unsigned short>(m_items[b + 4]);
                    if (XAllocColor(
                        XtDisplay(pane),
                        DefaultColormapOfScreen(XtScreen(pane)),
                        &color
                    ) != 0) {
                        colors[i] = color.pixel;
                        XSetForeground(XtDisplay(pane),gc,color.pixel);
                        pixmaps[i] = XCreatePixmap(
                            XtDisplay(pane),
                            RootWindowOfScreen(XtScreen(pane)),
                            16,16,
                            DefaultDepthOfScreen(XtScreen(pane))
                        );
                        XFillRectangle(XtDisplay(pane), pixmaps[i], gc,
                            0,0, 16,16
                        );
                        n = 0;
                        XtSetArg(arglist[n], XmNlabelType, XmPIXMAP); n++;
                        XtSetArg(arglist[n], XmNlabelPixmap, pixmaps[i]); n++;
                        assert(n <= XtNumber(arglist));
                        buttons[i] = XmCreatePushButtonGadget(
                            container1, m_data + m_items[b + 1], arglist, n
                        );
                        XtAddCallback(
                            buttons[i],
                            XmNactivateCallback,
                            trampolineM<SELECT_HANDLER>,
                            reinterpret_cast<XtPointer>(i)
                        );
                        XtManageChild(buttons[i]);
                        valid[i] = true;
                    }
                    else {
                        valid[i] = false;
                    }
                }
                else {
                    valid[i] = false;
                }
            }
            XtManageChild(container1);
            // Fix container1 height
            size.request_mode = CWHeight;
            XtQueryGeometry(
                container1, static_cast<XtWidgetGeometry *>(0), &size
            );
            XtVaSetValues(container1,
                XmNpaneMaximum, size.height,
                XmNpaneMinimum, size.height,
                static_cast<XtPointer>(0)
            );
            // Create current color view label
            Widget container2 = XmCreateForm(
                pane, const_cast<String>("c2"), 0, 0
            );
            m_lpixmap = XCreatePixmap(
                XtDisplay(pane),
                RootWindowOfScreen(XtScreen(pane)),
                m_cols * 16, 16,
                DefaultDepthOfScreen(XtScreen(pane))
            );
            if (valid[m_color_ret]) {
                XSetForeground(XtDisplay(pane), gc, colors[m_color_ret]);
                XFillRectangle(XtDisplay(pane), m_lpixmap, gc,
                    0, 0, m_cols * 16, 16
                );
            }
            n = 0;
            XtSetArg(arglist[n], XmNlabelType, XmPIXMAP); n++;
            XtSetArg(arglist[n], XmNlabelPixmap, m_lpixmap); n++;
            XtSetArg(arglist[n], XmNalignment, XmALIGNMENT_CENTER); n++;
            XtSetArg(arglist[n], XmNtopAttachment, XmATTACH_FORM); n++;
            XtSetArg(arglist[n], XmNbottomAttachment, XmATTACH_FORM); n++;
            XtSetArg(arglist[n], XmNleftAttachment, XmATTACH_FORM); n++;
            XtSetArg(arglist[n], XmNrightAttachment, XmATTACH_FORM); n++;
            assert(n <= XtNumber(arglist));
            m_label = XmCreateLabelGadget(
                container2, const_cast<String>("label"), arglist, n
            );
            XtManageChild(m_label);
            XtManageChild(container2);
            // Fix container2 height
            size.request_mode = CWHeight;
            XtQueryGeometry(
                container2, static_cast<XtWidgetGeometry *>(0), &size
            );
            XtVaSetValues(container2,
                XmNpaneMaximum, size.height,
                XmNpaneMinimum, size.height,
                static_cast<XtPointer>(0)
            );
            // Create form for action part
            n = 0;
            XtSetArg(arglist[n], XmNfractionBase, 5); n++;
            assert(n <= XtNumber(arglist));
            Widget container3 = XmCreateForm(
                pane, const_cast<String>("c3"), arglist, n
            );
            // Create OK button
            Widget ok = XmCreatePushButtonGadget(
                container3, const_cast<String>("OK"), 0, 0
            );
            XtVaSetValues(ok,
                XmNtopAttachment,                XmATTACH_FORM,
                XmNbottomAttachment,             XmATTACH_FORM,
                XmNleftAttachment,               XmATTACH_POSITION,
                XmNleftPosition,                 1,
                XmNrightAttachment,              XmATTACH_POSITION,
                XmNrightPosition,                2,
                XmNshowAsDefault,                True,
                XmNdefaultButtonShadowThickness, static_cast<Dimension>(1),
                static_cast<XtPointer>(0)
            );
            XtManageChild(ok);
            XtAddCallback(
                ok,
                XmNactivateCallback,
                trampolineW<OK_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Create Cancel button
            Widget cancel = XmCreatePushButtonGadget(
                container3, const_cast<String>("Cancel"), 0, 0
            );
            XtVaSetValues(cancel,
                XmNtopAttachment,                XmATTACH_FORM,
                XmNbottomAttachment,             XmATTACH_FORM,
                XmNleftAttachment,               XmATTACH_POSITION,
                XmNleftPosition,                 3,
                XmNrightAttachment,              XmATTACH_POSITION,
                XmNrightPosition,                4,
                XmNshowAsDefault,                False,
                XmNdefaultButtonShadowThickness, static_cast<Dimension>(1),
                static_cast<XtPointer>(0)
            );
            XtManageChild(cancel);
            XtAddCallback(
                cancel,
                XmNactivateCallback,
                trampolineW<CANCEL_HANDLER>,
                static_cast<XtPointer>(this)
            );
            // Fix the action area & disable resizing
            XtManageChild(container3);
            XtVaGetValues(cancel, XmNheight, &h, static_cast<XtPointer>(0));
            XtVaSetValues(container3,
                XmNpaneMaximum, h,
                XmNpaneMinimum, h,
                static_cast<XtPointer>(0)
            );
            XtManageChild(pane);
            // Wait for user action
            m_cancelation = NONE;
            m_running = true;
            m_pane_ref = pane;
            m_gc_ref = gc;
            m_valid_ref = valid;
            m_colors_ref = colors;
            while (m_running || XtAppPending(m_app->Context())) {
                XtAppProcessEvent(m_app->Context(), XtIMAll);
            }
            // Destroy dialog & free all resources
            Display *display = XtDisplay(pane);
            Screen *screen = XtScreen(pane);
            int nn = 0;
            XFreePixmap(display,m_lpixmap);
            for (int i = 0; i < MAXCNT; i++) {
                if (valid[i]) {
                    colors[nn] = colors[i];
                    nn++;
                    XFreePixmap(display,pixmaps[i]);
                }
            }
            if (nn > 0) {
                XFreeColors(display, DefaultColormapOfScreen(screen),
                    colors, nn, 0
                );
            }
            XFreeGC(display,gc);
            m_pane_ref = 0;
            m_gc_ref = 0;
            m_valid_ref = 0;
            m_colors_ref = 0;
            XtDestroyWidget(dialog);
            return m_cancelation;
        }
    // Return selected color ID
        int Selected()
        {
            return m_items[m_color_ret * 5];
        }
    // Nothing to manage
        void Manage() {}
    // Return class instance
        static base_type *GetInstance()
        {
            return m_this;
        }
    // No wrapped widget
        Widget Handle()
        {
            return 0;
        }
    // No slots
        void *Slot(int, Widget, XtPointer, XtPointer)
        {
            return 0;
        }
    // Handlers
        void SelectHandler(Widget, int button, XtPointer)
        {
            if (this->m_valid_ref != 0 && this->m_valid_ref[button]) {
                this->m_color_set = button;
                XSetForeground(XtDisplay(this->m_pane_ref), this->m_gc_ref,
                    this->m_colors_ref[button]
                );
                XFillRectangle(
                    XtDisplay(this->m_pane_ref),
                    this->m_lpixmap,
                    this->m_gc_ref,
                    0,0,
                    this->m_cols*16,16
                );
                XtUnmanageChild(this->m_label);
                XtVaSetValues(this->m_label,
                    XmNlabelPixmap, this->m_lpixmap,
                    static_cast<XtPointer>(0)
                );
                XtManageChild(this->m_label);
            }
        }
        void DestroyHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_cancelation = CANCEL;
        }
        void OkHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_color_ret = this->m_color_set;
            this->m_cancelation = OK;
        }
        void CancelHandler(Widget, XtPointer)
        {
            this->m_running = false;
            this->m_cancelation = CANCEL;
        }
    private:
    // Forbid implicit methods
        BaseModalColorDialog();
        BaseModalColorDialog(const BaseModalColorDialog &);
        BaseModalColorDialog & operator =(const BaseModalColorDialog &);
    public:
    // Handler map
        static handlerm_type GetHandler(SELECT_HANDLER)
        {
            return &base_type::SelectHandler;
        }
        static handlerw_type GetHandler(DESTROY_HANDLER)
        {
            return &base_type::DestroyHandler;
        }
        static handlerw_type GetHandler(OK_HANDLER)
        {
            return &base_type::OkHandler;
        }
        static handlerw_type GetHandler(CANCEL_HANDLER)
        {
            return &base_type::CancelHandler;
        }
};

template<class A, class P, int N, int U>
typename BaseModalColorDialog<A,P,N,U>::base_type *
BaseModalColorDialog<A,P,N,U>::m_this(0);

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseCanvas.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-11-18
//! \project GUX 2011 Draw (project #1)
//! \brief   Template of canvas.
//

#ifndef GUX2011_BASECANVAS_HH
#define GUX2011_BASECANVAS_HH

template<class A, class P>
class BaseCanvas {
    public:
    // Base types
        typedef BaseCanvas<A,P> base_type;
        typedef void (base_type::*handlerw_type)(Widget, XtPointer);
        typedef void (base_type::*handlera_type)(
            Widget, XEvent *, String *, Cardinal *
        );
    // Handlers IDs
        typedef NewType<base_type,0> DRAW_HANDLER;
        typedef NewType<base_type,1> EXPOSE_HANDLER;
        typedef NewType<base_type,2> RESIZE_HANDLER;
    // Drawed shape
        struct Shape {
            enum {
                NONE, POINT, LINE, RECT, FRECT, ELLIPSE, FELLIPSE
            };
            int x,y,w,h;
            Pixel dbg,dfg;
            Pixel fbg,ffg;
            int lw,ls;
            int shape;
            Shape()
            : x(0)   , y(0)
            , w(0)   , h(0)
            , dbg(0) , dfg(0)
            , fbg(0) , ffg(0)
            , lw(0)  , ls(0)
            , shape(NONE)
            {}
            Shape(const Shape &sh)
            : x(sh.x)     , y(sh.y)
            , w(sh.w)     , h(sh.h)
            , dbg(sh.dbg) , dfg(sh.dfg)
            , fbg(sh.fbg) , ffg(sh.ffg)
            , lw(sh.lw)   , ls(sh.ls)
            , shape(sh.shape)
            {}
            Shape(
                int x, int y,
                int w, int h,
                Pixel dbg,   Pixel dfg,
                Pixel fbg,   Pixel ffg,
                int lw,      int ls,
                int shape
            )
            {
                this->x = x; this->y = y;
                this->w = w; this->h = h;
                this->dbg = dbg; this->dfg = dfg;
                this->fbg = fbg; this->ffg = ffg;
                this->lw = lw; this->ls = ls;
                this->shape = shape;
            }
            Shape & operator =(const Shape &sh)
            {
                this->x = sh.x; this->y = sh.y;
                this->w = sh.w; this->h = sh.h;
                this->dbg = sh.dbg; this->dfg = sh.dfg;
                this->fbg = sh.fbg; this->ffg = sh.ffg;
                this->lw = sh.lw; this->ls = sh.ls;
                this->shape = sh.shape;
                return *this;
            }
            ~Shape() {}
        };
    private:
    // Constructor assertion
        static base_type *m_this;
        Assert m_assert;
    // Top widgets
        A *m_app;
        P *m_parrent;
    // Canvas widgets
        Widget m_drawing_area;
    // Clear and draw GCs
        GC m_dgc, m_fgc;
        GC m_hdgc, m_hfgc;
        GC m_cgc;
    // Buffers
        Pixmap m_backscreen;
        Pixmap m_offscreen;
    // Canvas dimensions
        Dimension m_width;
        Dimension m_height;
    // Colors and shapes
        std::map<int,Pixel> m_colortab;
        std::vector<Shape> m_history;
    // Canvas state
        int m_shape;
    // Input state
        bool m_pressed;
        int m_x0, m_y0, m_xx, m_yy, m_x1, m_y1;
    public:
    // In constructor
        BaseCanvas(
            A *application,
            P *parrent,
            const char *name1
        )
        : m_assert(m_this == 0,
              "There is still `BaseScrolledCanvas` instance."
          )
        , m_app(application)
        , m_parrent(parrent)
        {
            unsigned int n;
            Arg arglist[3];
            XGCValues gcv;
            XtActionsRec actions;
            XtTranslations translations;

            actions.string = const_cast<String>("draw");
            actions.proc = trampolineA<DRAW_HANDLER>;
            XtAppAddActions(m_app->Context(), &actions, 1);
            translations = XtParseTranslationTable(
                "<Btn1Down>:   draw(down)\n"
                "<Btn1Up>:     draw(up)\n"
                "<Btn1Motion>: draw(motion)"
            );
            n = 0;
            XtSetArg(arglist[n], XmNwidth, A::geometry_type::width); n++;
            XtSetArg(arglist[n], XmNheight, A::geometry_type::height); n++;
            XtSetArg(arglist[n], XmNtranslations, translations); n++;
            assert(n <= XtNumber(arglist));
            m_drawing_area = XmCreateDrawingArea(
                m_parrent->Handle(), const_cast<String>(name1), arglist, n
            );
            XtAddCallback(
                m_drawing_area,
                XmNexposeCallback, trampolineW<EXPOSE_HANDLER>,
                static_cast<XtPointer>(this)
            );
            XtAddCallback(
                m_drawing_area,
                XmNresizeCallback, trampolineW<RESIZE_HANDLER>,
                static_cast<XtPointer>(this)
            );
            gcv.function = GXcopy;
            gcv.line_width = 1;
            gcv.line_style = LineSolid;
            gcv.fill_style = FillSolid;
            unsigned long mask = GCFunction  | GCLineWidth
                               | GCLineStyle | GCFillStyle;
            m_dgc = XCreateGC(
                XtDisplay(m_drawing_area),
                RootWindowOfScreen(XtScreen(m_drawing_area)),
                mask, &gcv
            );
            m_fgc = XCreateGC(
                XtDisplay(m_drawing_area),
                RootWindowOfScreen(XtScreen(m_drawing_area)),
                mask, &gcv
            );
            m_hdgc = XCreateGC(
                XtDisplay(m_drawing_area),
                RootWindowOfScreen(XtScreen(m_drawing_area)),
                mask, &gcv
            );
            m_hfgc = XCreateGC(
                XtDisplay(m_drawing_area),
                RootWindowOfScreen(XtScreen(m_drawing_area)),
                mask, &gcv
            );
            m_cgc = XCreateGC(
                XtDisplay(m_drawing_area),
                RootWindowOfScreen(XtScreen(m_drawing_area)),
                mask, &gcv
            );
            m_backscreen = 0;
            m_offscreen = 0;
            m_width = 0;
            m_height = 0;
            m_shape = Shape::NONE;
            m_pressed = false;
            m_x0 = m_y0 = m_xx = m_yy = m_x1 = m_y1 = 0;
            m_this = this;
            I_TRACE1("BaseScrolledCanvas",name1);
        }
        ~BaseCanvas()
        {
            m_this = 0;
        }
    // Color management - set this before manage
        bool AllocColor(
            int id, unsigned short r, unsigned short g, unsigned short b
        )
        {
            XColor color;
            color.red = r; color.green = g; color.blue = b;
            if (XAllocColor(
                XtDisplay(m_drawing_area),
                DefaultColormapOfScreen(XtScreen(m_drawing_area)),
                &color
            ) != 0) {
                m_colortab[id] = color.pixel;
                return true;
            }
            return false;
        }
    // GC management
        bool SetDrawBackground(int id)
        {
            std::map<int,Pixel>::iterator it = m_colortab.find(id);
            if (it == m_colortab.end()) {
                return false;
            }
            unsigned long color = static_cast<unsigned long>((*it).second);
            XSetBackground(XtDisplay(m_drawing_area), m_dgc, color);
            return true;
        }
        bool SetDrawForeground(int id)
        {
            std::map<int,Pixel>::iterator it = m_colortab.find(id);
            if (it == m_colortab.end()) {
                return false;
            }
            unsigned long color = static_cast<unsigned long>((*it).second);
            XSetForeground(XtDisplay(m_drawing_area), m_dgc, color);
            return true;
        }
        bool SetFillBackground(int id)
        {
            std::map<int,Pixel>::iterator it = m_colortab.find(id);
            if (it == m_colortab.end()) {
                return false;
            }
            unsigned long color = static_cast<unsigned long>((*it).second);
            XSetBackground(XtDisplay(m_drawing_area), m_fgc, color);
            return true;
        }
        bool SetFillForeground(int id)
        {
            std::map<int,Pixel>::iterator it = m_colortab.find(id);
            if (it == m_colortab.end()) {
                return false;
            }
            unsigned long color = static_cast<unsigned long>((*it).second);
            XSetForeground(XtDisplay(m_drawing_area), m_fgc, color);
            return true;
        }
        bool SetClearColor(int id)
        {
            std::map<int,Pixel>::iterator it = m_colortab.find(id);
            if (it == m_colortab.end()) {
                return false;
            }
            unsigned long color = static_cast<unsigned long>((*it).second);
            XSetForeground(XtDisplay(m_drawing_area), m_cgc, color);
            XSetBackground(XtDisplay(m_drawing_area), m_cgc, color);
            return true;
        }
        void SetLineStyle(int ls)
        {
            XGCValues gcv;
            gcv.line_style = ls;
            XChangeGC(XtDisplay(m_drawing_area), m_dgc, GCLineStyle, &gcv);
        }
        void SetLineWidth(int lw)
        {
            XGCValues gcv;
            gcv.line_width = lw;
            XChangeGC(XtDisplay(m_drawing_area), m_dgc, GCLineWidth, &gcv);
        }
    // Set shape
        void SetShape(int shape)
        {
            m_shape = shape;
        }
    // Copy buffers
        void Copy(Pixmap src, Pixmap dest)
        {
            XCopyArea(
                XtDisplay(m_drawing_area), src, dest, m_dgc,
                0, 0, m_width, m_height, 0, 0
            );
        }
    // Update canvas content
        void UpdateContent(Pixmap buffer)
        {
            XCopyArea(
                XtDisplay(m_drawing_area),
                buffer, XtWindow(m_drawing_area),
                m_dgc,
                0, 0, m_width, m_height, 0, 0
            );
        }
    // Clear canvas
        void Clear()
        {
            XFillRectangle(XtDisplay(m_drawing_area), m_backscreen, m_cgc,
                0,0, m_width, m_height
            );
            XFillRectangle(XtDisplay(m_drawing_area), m_offscreen, m_cgc,
                0,0, m_width, m_height
            );
            m_history.clear();
        }
    // Drawing routines and storing routines
        void DrawPoint(Pixmap buffer, GC dgc, int x, int y)
        {
            XGCValues gcv;
            XGetGCValues(XtDisplay(m_drawing_area), dgc, GCLineWidth, &gcv);
            if (gcv.line_width > 0) {
                XFillArc(
                    XtDisplay(m_drawing_area), buffer, dgc,
                    x - gcv.line_width, y - gcv.line_width,
                    gcv.line_width * 2, gcv.line_width * 2,
                    0, 360 * 64
                );
            }
            else {
                XDrawPoint(
                    XtDisplay(m_drawing_area), buffer, dgc,
                    x, y
                );
            }
        }
        void StorePoint(int x, int y)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth,
                &gcv
            );
            m_history.push_back(Shape(
                x, y, 0, 0,
                gcv.background, gcv.foreground, 0, 0,
                gcv.line_width, 0,
                Shape::POINT
            ));
        }
        void DrawLine(Pixmap buffer, GC dgc, int x0, int y0, int x1, int y1)
        {
            XDrawLine(
                XtDisplay(m_drawing_area), buffer, dgc,
                x0, y0, x1, y1
            );
        }
        void StoreLine(int x0, int y0, int x1, int y1)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                &gcv
            );
            m_history.push_back(Shape(
                x0, y0, x1, y1,
                gcv.background, gcv.foreground, 0, 0,
                gcv.line_width, gcv.line_style,
                Shape::LINE
            ));
        }
        void DrawRect(Pixmap buffer, GC dgc, int x, int y, int w, int h)
        {
            XDrawRectangle(
                XtDisplay(m_drawing_area), buffer, dgc,
                x, y, w, h
            );
        }
        void StoreRect(int x, int y, int w, int h)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                &gcv
            );
            m_history.push_back(Shape(
                x, y, w, h,
                gcv.background, gcv.foreground, 0, 0,
                gcv.line_width, gcv.line_style,
                Shape::RECT
            ));
        }
        void DrawFRect(
            Pixmap buffer, GC dgc, GC fgc, int x, int y, int w, int h
        )
        {
            XFillRectangle(
                XtDisplay(m_drawing_area), buffer, fgc,
                x, y, w, h
            );
            XDrawRectangle(
                XtDisplay(m_drawing_area), buffer, dgc,
                x, y, w, h
            );
        }
        void StoreFRect(int x, int y, int w, int h)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_fgc,
                GCForeground | GCBackground,
                &gcv
            );
            Pixel ffg = gcv.foreground, fbg = gcv.background;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                &gcv
            );
            m_history.push_back(Shape(
                x, y, w, h,
                gcv.background, gcv.foreground, fbg, ffg,
                gcv.line_width, gcv.line_style,
                Shape::FRECT
            ));
        }
        void DrawEllipse(Pixmap buffer, GC dgc, int x, int y, int w, int h)
        {
            XDrawArc(
                XtDisplay(m_drawing_area), buffer, dgc,
                x, y, w, h, 0, 360 * 64
            );
        }
        void StoreEllipse(int x, int y, int w, int h)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                &gcv
            );
            m_history.push_back(Shape(
                x, y, w, h,
                gcv.background, gcv.foreground, 0, 0,
                gcv.line_width, gcv.line_style,
                Shape::ELLIPSE
            ));
        }
        void DrawFEllipse(
            Pixmap buffer, GC dgc, GC fgc, int x, int y, int w, int h
        )
        {
            XFillArc(
                XtDisplay(m_drawing_area), buffer, fgc,
                x, y, w, h, 0, 360 * 64
            );
            XDrawArc(
                XtDisplay(m_drawing_area), buffer, dgc,
                x, y, w, h, 0, 360 * 64
            );
        }
        void StoreFEllipse(int x, int y, int w, int h)
        {
            XGCValues gcv;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_fgc,
                GCForeground | GCBackground,
                &gcv
            );
            Pixel fbg = gcv.background, ffg = gcv.foreground;
            XGetGCValues(
                XtDisplay(m_drawing_area), m_dgc,
                GCForeground | GCBackground | GCLineWidth | GCLineStyle,
                &gcv
            );
            m_history.push_back(Shape(
                x, y, w, h,
                gcv.background, gcv.foreground, fbg, ffg,
                gcv.line_width, gcv.line_style,
                Shape::FELLIPSE
            ));
        }
        void StoreShape(int x, int y, int w, int h)
        {
            switch (m_shape) {
                case Shape::POINT:
                    StorePoint(x,y);
                    break;
                case Shape::LINE:
                    StoreLine(x,y,w,h);
                    break;
                case Shape::RECT:
                    StoreRect(x,y,w,h);
                    break;
                case Shape::FRECT:
                    StoreFRect(x,y,w,h);
                    break;
                case Shape::ELLIPSE:
                    StoreEllipse(x,y,w,h);
                    break;
                case Shape::FELLIPSE:
                    StoreFEllipse(x,y,w,h);
                    break;
                default:
                    break;
            }
        }
    // Manage canvas
        void Manage()
        {
            XtManageChild(m_drawing_area);
        }
    // Return handle of canvas widget
        Widget Handle()
        {
            return m_drawing_area;
        }
    // Process POST_MANAGE and EXIT_CLEANUP signals
        void *Slot(int signal, Widget, XtPointer, XtPointer)
        {
            if (signal == A::POST_MANAGE) {
                // Setup canvas geometry
                Dimension width, height;
                XtVaGetValues(
                    m_drawing_area,
                    XmNwidth, &width, XmNheight, &height,
                    static_cast<XtPointer>(0)
                );
                m_width = width;
                m_height = height;
                LABEL("POST_MANAGE sizes:");
                V_TRACE("  ",m_width);
                V_TRACE("  ",m_height);
                m_backscreen = XCreatePixmap(
                    XtDisplay(m_drawing_area),
                    RootWindowOfScreen(XtScreen(m_drawing_area)),
                    m_width, m_height,
                    DefaultDepthOfScreen(XtScreen(m_drawing_area))
                );
                m_offscreen = XCreatePixmap(
                    XtDisplay(m_drawing_area),
                    RootWindowOfScreen(XtScreen(m_drawing_area)),
                    m_width, m_height,
                    DefaultDepthOfScreen(XtScreen(m_drawing_area))
                );
                Clear();
                return static_cast<void *>(this);
            }
            if (signal == A::EXIT_CLEANUP) {
                Display *display = XtDisplay(m_drawing_area);
                Colormap colormap = DefaultColormapOfScreen(XtScreen(
                    m_drawing_area
                ));
                XFreePixmap(display,m_offscreen);
                XFreePixmap(display,m_backscreen);
                XFreeGC(display,m_cgc);
                XFreeGC(display,m_dgc);
                XFreeGC(display,m_fgc);
                XFreeGC(display,m_hdgc);
                XFreeGC(display,m_hfgc);
                for (
                    std::map<int,Pixel>::iterator i = m_colortab.begin();
                    i != m_colortab.end();
                    ++i
                ) {
                    Pixel c = (*i).second;
                    XFreeColors(display,colormap,&c,1,0);
                }
                return static_cast<void *>(this);
            }
            return 0;
        }
    // Get instance of canvas
        static base_type *GetInstance()
        {
            return m_this;
        }
    // OnDown() input handler
        void OnDown()
        {
            switch (m_shape) {
                case Shape::POINT:
                    StoreShape(m_x0,m_y0,0,0);
                    DrawPoint(m_backscreen,m_dgc,m_x0,m_y0);
                    UpdateContent(m_backscreen);
                    break;
                default:
                    break;
            }
        }
    // OnMotion() input handler
        void OnMotion()
        {
            int x = (m_x0 <= m_xx) ? m_x0 : m_xx;
            int y = (m_y0 <= m_yy) ? m_y0 : m_yy;
            int w = (m_x0 <= m_xx) ? (m_xx - m_x0) : (m_x0 - m_xx);
            int h = (m_y0 <= m_yy) ? (m_yy - m_y0) : (m_y0 - m_yy);
            int ex = (m_x0 <= m_xx) ? (x - w) : x;
            int ey = (m_y0 <= m_yy) ? (y - h) : y;
            switch (m_shape) {
                case Shape::LINE:
                    Copy(m_backscreen,m_offscreen);
                    DrawLine(m_offscreen,m_dgc,m_x0,m_y0,m_xx,m_yy);
                    UpdateContent(m_offscreen);
                    break;
                case Shape::RECT:
                    Copy(m_backscreen,m_offscreen);
                    DrawRect(m_offscreen,m_dgc,x,y,w,h);
                    UpdateContent(m_offscreen);
                    break;
                case Shape::FRECT:
                    Copy(m_backscreen,m_offscreen);
                    DrawFRect(m_offscreen,m_dgc,m_fgc,x,y,w,h);
                    UpdateContent(m_offscreen);
                    break;
                case Shape::ELLIPSE:
                    Copy(m_backscreen,m_offscreen);
                    DrawEllipse(m_offscreen,m_dgc,ex,ey,w*2,h*2);
                    UpdateContent(m_offscreen);
                    break;
                case Shape::FELLIPSE:
                    Copy(m_backscreen,m_offscreen);
                    DrawFEllipse(m_offscreen,m_dgc,m_fgc,ex,ey,w*2,h*2);
                    UpdateContent(m_offscreen);
                    break;
                default:
                    break;
            }
        }
    // OnUp() input handler
        void OnUp()
        {
            int x = (m_x0 <= m_x1) ? m_x0 : m_x1;
            int y = (m_y0 <= m_y1) ? m_y0 : m_y1;
            int w = (m_x0 <= m_x1) ? (m_x1 - m_x0) : (m_x0 - m_x1);
            int h = (m_y0 <= m_y1) ? (m_y1 - m_y0) : (m_y0 - m_y1);
            int ex = (m_x0 <= m_x1) ? (x - w) : x;
            int ey = (m_y0 <= m_y1) ? (y - h) : y;
            switch (m_shape) {
                case Shape::LINE:
                    StoreShape(m_x0,m_y0,m_x1,m_y1);
                    DrawLine(m_backscreen,m_dgc,m_x0,m_y0,m_x1,m_y1);
                    UpdateContent(m_backscreen);
                    break;
                case Shape::RECT:
                    StoreShape(x,y,w,h);
                    DrawRect(m_backscreen,m_dgc,x,y,w,h);
                    UpdateContent(m_backscreen);
                    break;
                case Shape::FRECT:
                    StoreShape(x,y,w,h);
                    DrawFRect(m_backscreen,m_dgc,m_fgc,x,y,w,h);
                    UpdateContent(m_backscreen);
                    break;
                case Shape::ELLIPSE:
                    StoreShape(ex,ey,w*2,h*2);
                    DrawEllipse(m_backscreen,m_dgc,ex,ey,w*2,h*2);
                    UpdateContent(m_backscreen);
                    break;
                case Shape::FELLIPSE:
                    StoreShape(ex,ey,w*2,h*2);
                    DrawFEllipse(m_backscreen,m_dgc,m_fgc,ex,ey,w*2,h*2);
                    UpdateContent(m_backscreen);
                    break;
                default:
                    break;
            }
        }
    // Handlers
        void DrawHandler(Widget, XEvent *event, String *args, Cardinal *nargs)
        {
            if (*nargs != 1) {
                return;
            }
            if (std::strcmp(args[0],"down") == 0) {
                // Save button down coords
                this->m_pressed = true;
                this->m_x0 = reinterpret_cast<XButtonEvent *>(event)->x;
                this->m_y0 = reinterpret_cast<XButtonEvent *>(event)->y;
                this->OnDown();
            }
            else if (std::strcmp(args[0],"motion") == 0) {
                // Save cursor move coords
                if (this->m_pressed) {
                    this->m_xx = reinterpret_cast<XMotionEvent *>(event)->x;
                    this->m_yy = reinterpret_cast<XMotionEvent *>(event)->y;
                    this->OnMotion();
                }
            }
            else if (std::strcmp(args[0],"up") == 0) {
                // Save button up coord
                this->m_pressed = false;
                this->m_x1 = reinterpret_cast<XButtonEvent *>(event)->x;
                this->m_y1 = reinterpret_cast<XButtonEvent *>(event)->y;
                this->OnUp();
            }
        }
        void ExposeHandler(Widget, XtPointer call_data)
        {
            XmDrawingAreaCallbackStruct *dacs =
                static_cast<XmDrawingAreaCallbackStruct *>(call_data);
            XEvent *event = dacs->event;
            if (event->xexpose.count == 0) {
                this->UpdateContent(this->m_backscreen);
            }
        }
        void ResizeHandler(Widget, XtPointer)
        {
            // Get new widget geometry
            Dimension width, height;
            XtVaGetValues(
                this->m_drawing_area,
                XmNwidth, &width, XmNheight, &height,
                static_cast<XtPointer>(0)
            );
            this->m_width = width;
            this->m_height = height;
            // Free old pixmaps
            XFreePixmap(XtDisplay(this->m_drawing_area), this->m_backscreen);
            XFreePixmap(XtDisplay(this->m_drawing_area), this->m_offscreen);
            // Alloc new
            this->m_backscreen = XCreatePixmap(
                XtDisplay(this->m_drawing_area),
                RootWindowOfScreen(XtScreen(this->m_drawing_area)),
                this->m_width, this->m_height,
                DefaultDepthOfScreen(XtScreen(this->m_drawing_area))
            );
            this->m_offscreen = XCreatePixmap(
                XtDisplay(this->m_drawing_area),
                RootWindowOfScreen(XtScreen(this->m_drawing_area)),
                this->m_width, this->m_height,
                DefaultDepthOfScreen(XtScreen(this->m_drawing_area))
            );
            // Clear it
            XFillRectangle(
                XtDisplay(this->m_drawing_area),
                this->m_backscreen, this->m_cgc,
                0,0, this->m_width, this->m_height
            );
            XFillRectangle(
                XtDisplay(this->m_drawing_area),
                this->m_offscreen, this->m_cgc,
                0,0, this->m_width, this->m_height
            );
            // Draw history
            for (
               typename std::vector<Shape>::iterator i =
                   this->m_history.begin();
               i < this->m_history.end();
               ++i
            ) {
                XGCValues gcv;
                switch ((*i).shape) {
                    case Shape::POINT:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground | GCLineWidth,
                            &gcv
                        );
                        DrawPoint(
                            this->m_backscreen, this->m_hdgc,
                            (*i).x, (*i).y
                        );
                        break;
                    case Shape::LINE:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        gcv.line_style = (*i).ls;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground
                            | GCLineWidth | GCLineStyle,
                            &gcv
                        );
                        DrawLine(
                            this->m_backscreen, this->m_hdgc,
                            (*i).x, (*i).y, (*i).w, (*i).h
                        );
                        break;
                    case Shape::RECT:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        gcv.line_style = (*i).ls;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground
                            | GCLineWidth | GCLineStyle,
                            &gcv
                        );
                        DrawRect(
                            this->m_backscreen, this->m_hdgc,
                            (*i).x, (*i).y, (*i).w, (*i).h
                        );
                        break;
                    case Shape::FRECT:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        gcv.line_style = (*i).ls;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground
                            | GCLineWidth | GCLineStyle,
                            &gcv
                        );
                        gcv.foreground = (*i).ffg;
                        gcv.background = (*i).fbg;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hfgc,
                            GCForeground | GCBackground,
                            &gcv
                        );
                        DrawFRect(
                            this->m_backscreen, this->m_hdgc, this->m_hfgc,
                            (*i).x, (*i).y, (*i).w, (*i).h
                        );
                        break;
                    case Shape::ELLIPSE:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        gcv.line_style = (*i).ls;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground
                            | GCLineWidth | GCLineStyle,
                            &gcv
                        );
                        DrawEllipse(
                            this->m_backscreen, this->m_hdgc,
                            (*i).x, (*i).y, (*i).w, (*i).h
                        );
                        break;
                    case Shape::FELLIPSE:
                        gcv.foreground = (*i).dfg;
                        gcv.background = (*i).dbg;
                        gcv.line_width = (*i).lw;
                        gcv.line_style = (*i).ls;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hdgc,
                            GCForeground | GCBackground
                            | GCLineWidth | GCLineStyle,
                            &gcv
                        );
                        gcv.foreground = (*i).ffg;
                        gcv.background = (*i).fbg;
                        XChangeGC(
                            XtDisplay(this->m_drawing_area),
                            this->m_hfgc,
                            GCForeground | GCBackground,
                            &gcv
                        );
                        DrawFEllipse(
                            this->m_backscreen, this->m_hdgc, this->m_hfgc,
                            (*i).x, (*i).y, (*i).w, (*i).h
                        );
                        break;
                    default:
                        break;
                }
            }
        }
    private:
    // Forbid implicit methods
        BaseCanvas();
        BaseCanvas(const BaseCanvas &);
        BaseCanvas & operator =(const BaseCanvas &);
    public:
    // Handler map
        static handlera_type GetHandler(DRAW_HANDLER)
        {
            return &base_type::DrawHandler;
        }
        static handlerw_type GetHandler(EXPOSE_HANDLER)
        {
            return &base_type::ExposeHandler;
        }
        static handlerw_type GetHandler(RESIZE_HANDLER)
        {
            return &base_type::ResizeHandler;
        }
};

template<class A, class P>
typename BaseCanvas<A,P>::base_type *
BaseCanvas<A,P>::m_this(0);

#endif

///////////////////////////////////////////////////////////////////////////////
//! \file    BaseApplication.hh
//! \author  Jiøí Kuèera, <xkucer28@stud.fit.vutbr.cz>
//! \date    2011-10-23
//! \project GUX 2011 Draw (project #1)
//! \brief   Application class template.
//

#ifndef GUX2011_BASEAPPLICATION_HH
#define GUX2011_BASEAPPLICATION_HH

// Geometry, Widgets, Signal base
template<class G, template<class A> class W, int B>
class BaseApplication {
    public:
    // Question dialog cancelation modes
        enum { NONE, OK, CANCEL };
    // Application signals
        enum {
            POST_MANAGE = B, // Sent after managing all widgets
            EXIT_CLEANUP     // Sent before exit
        };
    // Keep geometry info
        typedef G geometry_type;
    // Base type
        typedef BaseApplication<G,W,B> base_type;
    // Widget tree type
        typedef W<base_type> Widgets;
    // Callback handler type
        typedef void (base_type::*handler_type)(Widget, XtPointer);
    // Callback handlers ID's
        typedef NewType<base_type,0> EXIT_HANDLER;
        typedef NewType<base_type,1> OK_HANDLER;
        typedef NewType<base_type,2> CANCEL_HANDLER;
    private:
    // Application arguments
        int *m_argc;
        char **m_argv;
    // Application handle and context
        Widget m_app_shell;
        XtAppContext m_app_ctx;
    // Widget tree
        Widgets *m_pwidgets;
    // Question dialog stuff
        Widget m_question;
        int m_cancelation;
        bool m_running;
    public:
    // Create and initialize application
        BaseApplication(
            int *argc, char **argv, const char *name, const char *title = 0
        )
        {
            // Fallback resources
            String fall[] = {
                const_cast<String>(
                    "*question.dialogTitle: Quit from application"
                ),
                const_cast<String>(
                    "*question.messageString: Do you realy want to quit?"
                ),
                const_cast<String>("*question.okLabelString: Quit"),
                const_cast<String>("*question.cancelLabelString: Cancel"),
                const_cast<String>(
                    "*question.messageAlignment: XmALIGNMENT_CENTER"
                ),
                static_cast<String>(0)
            };

            // - set language handlig
            XtSetLanguageProc(
                static_cast<XtAppContext>(0),
                static_cast<XtLanguageProc>(0),
                static_cast<XtPointer>(0)
            );
            // - create application
            m_app_shell = XtVaOpenApplication(
                &m_app_ctx,                         // application context
                name,                               // application class name
                static_cast<XrmOptionDescRec *>(0), // options ptr
                static_cast<Cardinal>(0),           // number of options
                (m_argc = argc),                    // argc ptr (in/out)
                (m_argv = argv),                    // argv ptr (in/out)
                fall,                               // fallback resources
                sessionShellWidgetClass,            // widget class
                // va_list start:
                // - from WMCore class:
                XmNx, static_cast<Position>(G::xpos),
                XmNy, static_cast<Position>(G::ypos),
                XmNwidth, static_cast<Dimension>(G::width),
                XmNheight, static_cast<Dimension>(G::height),
                // - from WMShell class:
                XmNminWidth, static_cast<int>(G::minw),
                XmNminHeight, static_cast<int>(G::minh),
                XmNwidthInc, 5,
                XmNheightInc, 5,
                XmNtitle, (title ? title : argv[0]),
                // - from VendorShell class:
                XmNdeleteResponse, XmDO_NOTHING,
                // va_list end:
                static_cast<XtPointer>(0)
            );
            // ^^^ ugly exit & atexit stuff inside XToolkit handle any error
            // - register callback for handling close window events from WM
            Atom WM_DELETE_WINDOW = XmInternAtom(
                XtDisplay(m_app_shell),
                const_cast<String>("WM_DELETE_WINDOW"),
                False
            );
            XmAddWMProtocolCallback(
                m_app_shell,
                WM_DELETE_WINDOW,
                trampolineW<EXIT_HANDLER>, // Generate specific function
                static_cast<XtPointer>(this)
            );
            // - we have no widgets now
            m_pwidgets = 0;
            m_question = 0;
            m_cancelation = NONE;
            m_running = false;
        }
        ~BaseApplication()
        {
            m_pwidgets = 0;
        }
    // Application args acces
        int *GetArgcPtr()
        {
            return m_argc;
        }
        char **GetArgv()
        {
            return m_argv;
        }
    // Top level widget handle
        Widget Handle()
        {
            return m_app_shell;
        }
    // Application contex
        XtAppContext Context()
        {
            return m_app_ctx;
        }
    // Sending signals to widgets in widget tree (if we have any)
        void *Signal(
            int signal,
            Widget widget, XtPointer client_data, XtPointer call_data
        )
        {
            if (m_pwidgets != 0) {
                return m_pwidgets->Slot(signal,widget,client_data,call_data);
            }
            return 0;
        }
    // Application entry point
        void Run()
        {
            LABEL("Do Widget tree construction");
            Widgets widgets(this); // create widget tree on stack

            m_pwidgets = &widgets; // and update address to it
            LABEL("Do Managing");
            widgets.Manage();      // manage widgets in widget tree
            LABEL("Do POST_MANAGE");
            Signal(POST_MANAGE, 0, 0, 0);
            LABEL("Do Realize");
            XtRealizeWidget(m_app_shell); // realize widgets in widget tree
            XtAppMainLoop(m_app_ctx); // run main loop (no return)
        }
    // Handlers
    // - exit handler
        void Exit(Widget, XtPointer)
        {
            if (this->m_question == 0) {
                this->m_question = XmCreateQuestionDialog(
                    this->m_app_shell, const_cast<String>("question"), 0, 0
                );
                XtVaSetValues(this->m_question,
                    XmNdialogStyle, XmDIALOG_FULL_APPLICATION_MODAL,
                    static_cast<XtPointer>(0)
                );
                XtUnmanageChild(XmMessageBoxGetChild(
                    this->m_question, XmDIALOG_HELP_BUTTON
                ));
                XtAddCallback(
                    this->m_question,
                    XmNokCallback, trampolineW<OK_HANDLER>,
                    static_cast<XtPointer>(this)
                );
                XtAddCallback(
                    this->m_question,
                    XmNcancelCallback, trampolineW<CANCEL_HANDLER>,
                    static_cast<XtPointer>(this)
                );
            }
            XtManageChild(this->m_question);
            this->m_cancelation = NONE;
            this->m_running = true;
            while (this->m_running || XtAppPending(this->m_app_ctx)) {
                XtAppProcessEvent(this->m_app_ctx, XtIMAll);
            }
            if (this->m_cancelation == OK) {
                Signal(EXIT_CLEANUP, 0, 0, 0);
                XtDestroyWidget(this->m_app_shell);
                XtDestroyApplicationContext(this->m_app_ctx);
                std::exit(0);
            }
        }
        void OkHandler(Widget, XtPointer)
        {
            this->m_cancelation = OK;
            this->m_running = false;
        }
        void CancelHandler(Widget, XtPointer)
        {
            this->m_cancelation = CANCEL;
            this->m_running = false;
        }
    private:
    // Forbidden methods
        BaseApplication();
        BaseApplication(const BaseApplication &);
        BaseApplication & operator =(const BaseApplication &);
    public:
    // Handlers mapping (see "trampolines.hh")
        static handler_type GetHandler(EXIT_HANDLER)
        {
            return &base_type::Exit;
        }
        static handler_type GetHandler(OK_HANDLER)
        {
            return &base_type::OkHandler;
        }
        static handler_type GetHandler(CANCEL_HANDLER)
        {
            return &base_type::CancelHandler;
        }
};

#endif

///////////////////////////////////////////////////////////////////////////////

#endif
