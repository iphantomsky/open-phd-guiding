/*
 *  eegg.cpp
 *  PHD Guiding
 *
 *  Created by Craig Stark.
 *  Copyright (c) 2007-2010 Craig Stark.
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

#include "phd.h"

void TestGuide() {

#ifdef BRET_TODO
    wxMessageBox(_("W RA+")); wxTheApp->Yield(); pMount->Guide(WEST,2000); wxTheApp->Yield();
    wxMessageBox(_("N Dec+"));  wxTheApp->Yield(); pMount->Guide(NORTH,2000);wxTheApp->Yield();
    wxMessageBox(_("E RA-"));  wxTheApp->Yield(); pMount->Guide(EAST,2000);wxTheApp->Yield();
    wxMessageBox(_("S Dec-"));  wxTheApp->Yield(); pMount->Guide(SOUTH,2000);wxTheApp->Yield();
    wxMessageBox(_("Done"));
#endif
}

void MyFrame::OnEEGG(wxCommandEvent &evt) {

    if ((evt.GetId() == EEGG_TESTGUIDEDIR) && (pMount->IsConnected()))
        TestGuide();
    else if (evt.GetId() == EEGG_RANDOMMOTION) {
        RandomMotionMode = !RandomMotionMode;
        wxMessageBox(wxString::Format(_T("Random motion mode set to %d"),(int) RandomMotionMode));
    }
    else if (evt.GetId() == EEGG_MANUALCAL) {
        wxString tmpstr;
        double xRate   = pMount->xRate();
        double yRate  = pMount->yRate();
        double xAngle  = pMount->xAngle();
        double yAngle = pMount->yAngle();

        tmpstr = wxGetTextFromUser(_("Enter parameter (e.g. 0.005)"), _("RA rate"), wxString::Format(_T("%.4f"),xRate));
        if (tmpstr.IsEmpty()) return;
        tmpstr.ToDouble(&xRate); // = 0.0035;

        tmpstr = wxGetTextFromUser(_("Enter parameter (e.g. 0.005)"), _("Dec rate"), wxString::Format(_T("%.4f"),yRate));
        if (tmpstr.IsEmpty()) return;
        tmpstr.ToDouble(&yRate); // = 0.0035;

        tmpstr = wxGetTextFromUser(_("Enter parameter (e.g. 0.5)"), _("RA angle"), wxString::Format(_T("%.3f"),xAngle));
        if (tmpstr.IsEmpty()) return;
        tmpstr.ToDouble(&xAngle); // = 0.0035;

        tmpstr = wxGetTextFromUser(_("Enter parameter (e.g. 2.1)"), _("Dec angle"), wxString::Format(_T("%.3f"),yAngle));
        if (tmpstr.IsEmpty()) return;
        tmpstr.ToDouble(&yAngle); // = 0.0035;

        pMount->SetCalibration(xAngle, yAngle, xRate, yRate);
        SetStatusText(_T("Cal"),5);
    }
    else if (evt.GetId() == EEGG_CLEARCAL) {
        pMount->ClearCalibration(); // clear calibration
        SetStatusText(_T("No cal"),5);
    }
    else if (evt.GetId() == EEGG_FLIPRACAL) {
        if (!pMount->IsCalibrated())
            return;

        double orig=pMount->xAngle();
        pMount->FlipCalibration();
        double xAngle  = pMount->xAngle();
        wxMessageBox(wxString::Format(_("RA calibration angle flipped: %.2f to %.2f"),orig,pMount->xAngle()));
    }
    else if (evt.GetId() == EEGG_MANUALLOCK) {
        if (!pMount->IsConnected() || !pCamera && !pCamera->Connected || !pMount->IsCalibrated())
            return;
        if (pGuider->GetState() > STATE_SELECTED) return;  // must not be calibrating or guiding already
        if (evt.IsChecked()) {
            double LockX, LockY;
            wxString tmpstr;
            tmpstr = wxGetTextFromUser(_("Enter x-lock position (or 0 for center)"), _("X-lock position"));
            if (tmpstr.IsEmpty()) return;
            //ManualLock = true;
            tmpstr.ToDouble(&LockX);
            LockX = fabs(LockX);
            if (LockX < 0.0001) {
                LockX = pCamera->FullSize.GetWidth() / 2;
                LockY = pCamera->FullSize.GetHeight() / 2;
            }
            else {
                tmpstr = wxGetTextFromUser(_("Enter y-lock position"), _("Y-lock position"));
                if (tmpstr.IsEmpty()) return;
                tmpstr.ToDouble(&LockY);
                LockY = fabs(LockY);
            }
            pFrame->pGuider->SetLockPosition(PHD_Point(LockX, LockY));
        }
        else {
            //ManualLock = false;
        }
    }
    else evt.Skip();

}



void MyFrame::OnDriftTool(wxCommandEvent& WXUNUSED(ect)) {

}
