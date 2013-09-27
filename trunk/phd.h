/*
 *  phd.h
 *  PHD Guiding
 *
 *  Created by Craig Stark.
 *  Copyright (c) 2006-2010 Craig Stark.
 *  All rights reserved.
 *
 *  This source code is distributed under the following "BSD" license
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *    Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *    Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *    Neither the name of Craig Stark, Stark Labs nor the names of its
 *     contributors may be used to endorse or promote products derived from
 *     this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 *  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 *  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 *  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 *  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 *  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 *  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *  POSSIBILITY OF SUCH DAMAGE.
 *
 */

#ifndef PHD_H_INCLUDED
#define PHD_H_INCLUDED
 //#define ORION

#include <stdarg.h>

#include <wx/wx.h>
#include <wx/image.h>
#include <wx/string.h>
#include <wx/html/helpctrl.h>
#include <wx/utils.h>
#include <wx/textfile.h>
#include <wx/socket.h>
#include <wx/thread.h>

#include <wx/config.h>
#include <wx/fileconf.h>
#include <wx/display.h>
#include <wx/statline.h>
#include <wx/bmpbuttn.h>
#include <wx/tglbtn.h>
#include <wx/spinctrl.h>
#include <wx/stdpaths.h>
#include <wx/splash.h>
#include <wx/intl.h>

#include <wx/minifram.h>
#include <wx/stdpaths.h>
#include <wx/ffile.h>

#include <wx/bitmap.h>
#include <wx/dcbuffer.h>
#include <wx/graphics.h>

#include <wx/msgqueue.h>

#include <wx/aui/aui.h>

#include <map>
#include <math.h>

#define APPNAME _T("PHD Guiding")
#define PHDVERSION _T("2.1.1")
#define PHDSUBVER _T("a")
#define FULLVER PHDVERSION PHDSUBVER

#if defined (__WINDOWS__)
#pragma warning(disable:4189)
#pragma warning(disable:4018)
#pragma warning(disable:4305)
#pragma warning(disable:4100)
#pragma warning(disable:4996)

#include <vld.h>

#endif

WX_DEFINE_ARRAY_INT(int, ArrayOfInts);
WX_DEFINE_ARRAY_DOUBLE(double, ArrayOfDbl);

#if defined (__WINDOWS__)
#define PATHSEPCH '\\'
#define PATHSEPSTR "\\"
#endif

#if defined (__APPLE__)
#define PATHSEPCH '/'
#define PATHSEPSTR "/"
#endif

#if defined (__WXGTK__)
#define PATHSEPCH '/'
#define PATHSEPSTR _T("/")
#endif

//#define TEST_TRANSFORMS
//#define BRET_AO_DEBUG

#ifdef BRET_AO_DEBUG
#define USE_LOOPBACK_SERIAL
#endif

#define CROPXSIZE 100
#define CROPYSIZE 100

#define ROUND(x) (int) floor(x + 0.5)

/* eliminate warnings for unused variables */
#define POSSIBLY_UNUSED(x) (void)(x)

// these macros are used for building messages for thrown exceptions
// It is surprisingly hard to get the line number into a string...
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#define THROW_INFO_BASE(intro, file, line) intro " " file ":" TOSTRING(line)
#define LOG_INFO(s) (Debug.AddLine(wxString(THROW_INFO_BASE("At", __FILE__, __LINE__) "->" s)))
#define THROW_INFO(s) (Debug.AddLine(wxString(THROW_INFO_BASE("Throw from", __FILE__, __LINE__) "->" s)))
#define ERROR_INFO(s) (Debug.AddLine(wxString(THROW_INFO_BASE("Error thrown from", __FILE__, __LINE__) "->" s)))

#if defined (__APPLE__)
#include "../cfitsio/fitsio.h"
#else
#include "fitsio.h"
#include <opencv/cv.h>
#endif


#include "phdconfig.h"
#include "configdialog.h"
#include "optionsbutton.h"
#include "usImage.h"
#include "point.h"
#include "star.h"
#include "circbuf.h"
#include "graph.h"
#include "star_profile.h"
#include "target.h"
#include "graph-stepguider.h"
#include "guide_algorithms.h"
#include "guiders.h"
#include "messagebox_proxy.h"
#include "serialports.h"
#include "parallelports.h"
#include "onboard_st4.h"
#include "cameras.h"
#include "mount.h"
#include "scopes.h"
#include "stepguiders.h"
#include "image_math.h"
#include "testguide.h"
#include "advanced_dialog.h"
#include "gear_dialog.h"
#include "myframe.h"
#include "worker_thread.h"
#include "debuglog.h"
#include "guidinglog.h"
#include "event_server.h"
#include "confirm_dialog.h"

extern PhdConfig *pConfig;

extern MyFrame *pFrame;
extern Mount *pMount;
extern Mount *pSecondaryMount;
extern GuideCamera *pCamera;

#define ALWAYS_FLUSH_DEBUGLOG
extern DebugLog Debug;
extern GuidingLog GuideLog;

#ifdef PHD1_LOGGING // deprecated
// these are the logging related globals
extern wxTextFile *LogFile;
extern bool Log_Data;
#endif

// these seem to be the windowing/display related globals
extern int AdvDlg_fontsize;
extern int XWinSize;
extern int YWinSize;

// This is used by eegg.  It could go into guider, but that seem like more
// trouble than it is worth

extern bool RandomMotionMode;

class PhdApp: public wxApp
{
    long m_instanceNumber;
    bool m_resetConfig;
  public:
    PhdApp(void);
    bool OnInit(void);
    int OnExit(void);
    void OnInitCmdLine(wxCmdLineParser& parser);
    bool OnCmdLineParsed(wxCmdLineParser & parser);
    virtual bool Yield(bool onlyIfNeeded=false);
protected:
    wxLocale m_locale;
};

wxDECLARE_APP(PhdApp);

#endif // PHD_H_INCLUDED