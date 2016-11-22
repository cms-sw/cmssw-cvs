#if !defined(__CINT__) && !defined(__MAKECINT__)
#include "TCanvas.h"
#include "TPad.h"
#include "TH1D.h"
#include "TLegend.h"
#include "TPaveStats.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TROOT.h"
#include "TStyle.h"
#include "TF1.h"
#include "TProfile.h"

// for testing (comment when using for real)
//const float MINEFF = 0.5;
//const float MAXEFF = 1.025;
//const float MINFAKE = 0.0;
//const float MAXFAKE = 0.7;
#endif

// -----------------------------------------------------------------------------------------------------------//
// helper functions forward declare 

void NormalizeHistograms(TH1* h1, TH1* h2);

void setStats(TH1* s,TH1* r, double startingY, double startingX = .1, bool fit = false);

void fixRangeY(TH1* r,TH1* s);

void plot4histos
(
    TCanvas *canvas, 
    TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
	TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4
);

void plotBuilding
(
    TCanvas *canvas, 
    TH1 **s, 
    TH1 **r, 
    int n,
    unsigned int logx = 0
);

void plotPulls
(
    TCanvas *canvas, 
	TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
	TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4,
	TH1 *s5,TH1 *r5,TH1 *s6,TH1 *r6
);

void plotResolutions
(
    TCanvas *canvas, 
    TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
    TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4,
    TH1 *s5,TH1 *r5,TH1 *s6,TH1 *r6,
    bool logx = false 
);


// -----------------------------------------------------------------------------------------------------------//
// main function to collect and overlay all the histograms

void TrackValHistoPublisherNew(const char* newFile="NEW_FILE",const char* refFile="REF_FILE")
{

    //gROOT->ProcessLine(".x HistoCompare_Tracks.C");
    gROOT->Reset();
    gROOT->SetBatch();

    //=========  settings ====================
    gROOT->SetStyle("Plain");
    gStyle->SetPadGridX(kTRUE);
    gStyle->SetPadGridY(kTRUE);
    gStyle->SetPadRightMargin(0.07);
    gStyle->SetPadLeftMargin(0.13);
    //gStyle->SetTitleXSize(0.07); 
    //gStyle->SetTitleXOffset(0.6); 
    //tyle->SetTitleYSize(0.3);
    //gStyle->SetLabelSize(0.6) 
    //gStyle->SetTextSize(0.5);
    const char* refLabel = "REF_LABEL, REF_RELEASE REFSELECTION";
    const char* newLabel = "NEW_LABEL, NEW_RELEASE NEWSELECTION";


    //=============================================


    delete gROOT->GetListOfFiles()->FindObject(refFile);
    delete gROOT->GetListOfFiles()->FindObject(newFile); 

    TFile* sfile        = new TFile(newFile);
    TFile* rfile        = new TFile(refFile);
    TDirectory* sdir    = gDirectory;
    TDirectory* rdir    = gDirectory;

    if(sfile->GetDirectory("DQMData/Run 1/RecoTrackV")) sfile->cd("DQMData/Run 1/RecoTrackV/Run summary/Track");
    else if(sfile->GetDirectory("DQMData/RecoTrackV/Track"))sfile->cd("DQMData/RecoTrackV/Track");
    else if(sfile->GetDirectory("DQMData/Run 1/Tracking")) sfile->cd("DQMData/Run 1/Tracking/Run summary/Track");
    else if(sfile->GetDirectory("DQMData/Tracking/Track"))sfile->cd("DQMData/Tracking/Track");
    sdir=gDirectory;
    TList *sl= sdir->GetListOfKeys();
    TString collname2 =sl->At(0)->GetName(); 

    if(rfile->GetDirectory("DQMData/Run 1/RecoTrackV")) rfile->cd("DQMData/Run 1/RecoTrackV/Run summary/Track");
    else if(rfile->GetDirectory("DQMData/RecoTrackV/Track"))rfile->cd("DQMData/RecoTrackV/Track");
    else if(rfile->GetDirectory("DQMData/Run 1/Tracking")) rfile->cd("DQMData/Run 1/Tracking/Run summary/Track");
    else if(rfile->GetDirectory("DQMData/Tracking/Track"))rfile->cd("DQMData/Tracking/Track");
    rdir=gDirectory;
    TList *rl= rdir->GetListOfKeys();
    TString collname1=rl->At(0)->GetName(); 

    TCanvas *canvas;

    TH1 *sh1,*rh1;
    TH1 *sh2,*rh2;
    TH1 *sh3,*rh3;
    TH1 *sh4,*rh4;
    TH1 *sh5,*rh5;
    TH1 *sh6,*rh6;

    const float ytitle_size   = 0.05;
    const float ytitle_offset = 1.2;
    const float xtitle_size   = 0.05;
    const float xtitle_offset = 0.9;
    const float ylabel_size   = 0.04;
    const float xlabel_size   = 0.04;

    const float maxeff  = MAXEFF;
    const float mineff  = MINEFF;
    const float minfake = MINFAKE;
    const float maxfake = MAXFAKE;

    //TH1 *sc1,*rc1;
    //TH1 *sc2,*rc2;
    //TH1 *sc3,*rc3;

    //bool hit  = true;
    //bool chi2 = true;
    //bool rs   = false;
    bool ctf  = true;


    //////////////////////////////////////
    /////////// CTF //////////////////////
    //////////////////////////////////////
    if (ctf)
    {
        // =====================================================================================//
        // building
        // eff vs eta
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic") );
        rh1->GetYaxis()->SetRangeUser(mineff,maxeff);
        sh1->GetYaxis()->SetRangeUser(mineff,maxeff);
        rh1->GetXaxis()->SetTitle("#eta");
        rh1->GetYaxis()->SetTitle("efficiency vs #eta");
        rh1->GetYaxis()->SetTitleSize(ytitle_size);
        rh1->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh1->GetYaxis()->SetLabelSize(ylabel_size);
        rh1->GetXaxis()->SetLabelSize(xlabel_size);

        // fake rate vs eta
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakerate") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakerate") );
        rh2->GetYaxis()->SetRangeUser(minfake,maxfake);
        sh2->GetYaxis()->SetRangeUser(minfake,maxfake);
        rh2->GetXaxis()->SetTitle("#eta");
        rh2->GetYaxis()->SetTitle("fakerate vs #eta");
        rh2->GetYaxis()->SetTitleSize(ytitle_size);
        rh2->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh2->GetYaxis()->SetLabelSize(ylabel_size);
        rh2->GetXaxis()->SetLabelSize(xlabel_size);

        // eff vs pt
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/efficPt") );
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/efficPt") );
        rh3->GetXaxis()->SetRangeUser(0,300);
        sh3->GetXaxis()->SetRangeUser(0,300);
        rh3->GetYaxis()->SetRangeUser(0.0,maxeff);
        sh3->GetYaxis()->SetRangeUser(0.0,maxeff);
        rh3->GetXaxis()->SetTitle("p_{T}");
        rh3->GetYaxis()->SetTitle("efficiency vs p_{T}");
        rh3->GetYaxis()->SetTitleSize(ytitle_size);
        rh3->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh3->SetTitle("");
        rh3->GetYaxis()->SetLabelSize(ylabel_size);
        rh3->GetXaxis()->SetLabelSize(xlabel_size);

        // fake rate vs pt
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakeratePt") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakeratePt") );
        rh4->SetTitle("");
        rh4->GetXaxis()->SetTitle("p_{T}");
        rh4->GetYaxis()->SetTitle("fakrate vs p_{T}");
        rh4->GetYaxis()->SetTitleSize(ytitle_size);
        rh4->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh4->GetYaxis()->SetRangeUser(0.,.80);
        rh4->GetXaxis()->SetRangeUser(0.2,300);
        sh4->GetYaxis()->SetRangeUser(0.,.80);
        sh4->GetXaxis()->SetRangeUser(0.2,300);
        rh4->GetYaxis()->SetLabelSize(ylabel_size);
        rh4->GetXaxis()->SetLabelSize(xlabel_size);

        // eff vs # hit
        rh5 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_hit") );
        sh5 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_hit") );
        rh5->GetXaxis()->SetTitle("hits");
        rh5->GetYaxis()->SetTitle("efficiency vs hits");
        rh5->GetYaxis()->SetRangeUser(0.0,maxeff);
        sh5->GetYaxis()->SetRangeUser(0.0,maxeff);
        rh5->GetYaxis()->SetTitleSize(ytitle_size);
        rh5->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh5->GetYaxis()->SetLabelSize(ylabel_size);
        rh5->GetXaxis()->SetLabelSize(xlabel_size);

        // fake rate vs # hits
        rh6 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakerate_vs_hit") );
        sh6 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakerate_vs_hit") );
        rh6->GetYaxis()->SetRangeUser(0.,1.0);
        rh6->GetYaxis()->SetRangeUser(0.,1.0);
        rh6->GetYaxis()->SetRangeUser(0.0,maxeff);
        sh6->GetYaxis()->SetRangeUser(0.0,maxeff);
        rh6->GetXaxis()->SetTitle("hits");
        rh6->GetYaxis()->SetTitle("fakerate vs hits");
        rh6->GetYaxis()->SetTitleSize(ytitle_size);
        rh6->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh6->GetYaxis()->SetLabelSize(ylabel_size);
        rh6->GetXaxis()->SetLabelSize(xlabel_size);

        TH1* r[6]={rh1,rh2,rh3,rh4,rh5,rh6};
        TH1* s[6]={sh1,sh2,sh3,sh4,sh5,sh6};

        canvas = new TCanvas("Tracks","Tracks: efficiency & fakerate",1000,1400);
        plotBuilding(canvas, s, r, 6, 0xC);

        canvas->cd();
        TLegend* l = new TLegend(0.10,0.64,0.90,0.69);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("building.pdf");   
        delete l;

        // =====================================================================================//
        // hits and pt

        // # hits vs eta
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/nhits_vs_eta_pfx") ); 
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/nhits_vs_eta_pfx") ); 
        rh1->GetYaxis()->SetRangeUser(8,24);
        sh1->GetYaxis()->SetRangeUser(8,24);
        rh1->GetXaxis()->SetTitle("#eta");
        rh1->GetYaxis()->SetTitle("<hits> vs #eta");
        rh1->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh1->GetYaxis()->SetTitleSize(ytitle_size);
        rh1->GetXaxis()->SetTitleSize(xtitle_size);
        rh1->GetYaxis()->SetLabelSize(ylabel_size);
        rh1->GetXaxis()->SetLabelSize(xlabel_size);

        // # hits
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/hits") ); 
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/hits") ); 
        rh2->GetXaxis()->SetRangeUser(0,30);
        sh2->GetXaxis()->SetRangeUser(0,30);
        rh2->GetXaxis()->SetTitle("hits");
        rh2->GetYaxis()->SetLabelSize(ylabel_size);
        rh2->GetXaxis()->SetLabelSize(xlabel_size);

        // sim track pT distribution
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/num_simul_pT") ); 
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/num_simul_pT") ); 
        rh3->GetXaxis()->SetRangeUser(0,10);
        sh3->GetXaxis()->SetRangeUser(0,10);
        rh3->GetXaxis()->SetTitle("p_{T}");
        rh3->GetYaxis()->SetLabelSize(ylabel_size);
        rh3->GetXaxis()->SetLabelSize(xlabel_size);

        // reco track pT distribution
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/num_reco_pT") ); 
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/num_reco_pT") ); 
        rh4->GetXaxis()->SetRangeUser(0,10);
        sh4->GetXaxis()->SetRangeUser(0,10);
        rh4->GetXaxis()->SetTitle("p_{T}");
        rh4->GetYaxis()->SetLabelSize(ylabel_size);
        rh4->GetXaxis()->SetLabelSize(xlabel_size);
        NormalizeHistograms(rh3,sh3);
        NormalizeHistograms(rh4,sh4);

        canvas = new TCanvas("Tracks1","Tracks: hits and Pt",1000,1050);

        plot4histos(canvas,sh1,rh1,sh2,rh2,sh3,rh3,sh4,rh4);

        canvas->cd();
        l = new TLegend(0.10,0.49,0.94,0.54);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("hitsAndPt.pdf");
        delete l;


        // =====================================================================================//
        // tuning

        // chi^2
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/chi2") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/chi2") );
        NormalizeHistograms(rh1,sh1);
        fixRangeY(rh1,sh1);
        rh1->GetXaxis()->SetTitleSize(xtitle_size-0.01);
        rh1->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh1->GetYaxis()->SetLabelSize(ylabel_size);
        rh1->GetXaxis()->SetLabelSize(xlabel_size);
        rh1->GetXaxis()->SetTitle("#chi^{2}");

        // chi^2 probability
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/chi2_prob") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/chi2_prob") );
        NormalizeHistograms(rh2,sh2);
        fixRangeY(rh2,sh2);
        rh2->GetYaxis()->SetLabelSize(ylabel_size);
        rh2->GetXaxis()->SetLabelSize(xlabel_size);
        rh2->GetXaxis()->SetTitleSize(xtitle_size-0.01);
        rh2->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh2->GetXaxis()->SetTitle("Prob(#chi^{2})");

        // chi^2 vs eta
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/chi2_vs_eta_pfx") ); 
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/chi2_vs_eta_pfx") ); 
        rh3->GetYaxis()->SetRangeUser(0,2.5);
        sh3->GetYaxis()->SetRangeUser(0,2.5);
        rh3->SetTitle("");
        rh3->GetYaxis()->SetTitleSize(ytitle_size);
        rh3->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh3->GetYaxis()->SetLabelSize(ylabel_size);
        rh3->GetXaxis()->SetLabelSize(xlabel_size);
        rh3->GetYaxis()->SetTitle("<  #chi^{2} / ndf >");
        rh3->GetXaxis()->SetTitleSize(xtitle_size);
        rh3->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh3->GetXaxis()->SetTitle("#eta");

        // pt resolution vs eta
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/ptres_vs_eta_Mean") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/ptres_vs_eta_Mean") );
        rh4->Scale(100.);
        sh4->Scale(100.);
        rh4->GetYaxis()->SetRangeUser(-1.5,1.5);
        sh4->GetYaxis()->SetRangeUser(-1.5,1.5);
        rh4->SetTitle("");
        rh4->GetYaxis()->SetTitle("< #delta p_{T} / p_{T} > [%]");
        rh4->GetYaxis()->SetLabelSize(ylabel_size);
        rh4->GetXaxis()->SetLabelSize(xlabel_size);
        rh4->GetYaxis()->SetTitleSize(ytitle_size);
        rh4->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh4->GetXaxis()->SetTitleSize(xtitle_size);
        rh4->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh4->GetXaxis()->SetTitle("#eta");

        canvas = new TCanvas("Tracks2","Tracks: chi2 & chi2 probability",1000,1050);

        plot4histos(canvas, sh1, rh1, sh2, rh2, sh3, rh3, sh4, rh4);

        canvas->cd();   
        l = new TLegend(0.10,0.48,0.90,0.53);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("tuning.pdf");
        delete l;


        // =====================================================================================//
        // pulls

        // pull of pt
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullPt") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullPt") );
        NormalizeHistograms(rh1,sh1);
        rh1->GetXaxis()->SetRangeUser(-10,10);
        sh1->GetXaxis()->SetRangeUser(-10,10);

        // pull of q over p 
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullQoverp") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullQoverp") );
        NormalizeHistograms(rh2,sh2);
        rh2->GetXaxis()->SetRangeUser(-10,10);
        sh2->GetXaxis()->SetRangeUser(-10,10);

        // pull of phi
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullPhi") );
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullPhi") );
        NormalizeHistograms(rh3,sh3);
        rh3->GetXaxis()->SetRangeUser(-10,10);
        sh3->GetXaxis()->SetRangeUser(-10,10);

        // pull of theta
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullTheta") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullTheta") );
        NormalizeHistograms(rh4,sh4);
        rh4->GetXaxis()->SetRangeUser(-10,10);
        sh4->GetXaxis()->SetRangeUser(-10,10);

        // pull of d_xy
        rh5 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullDxy") );
        sh5 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullDxy") );
        NormalizeHistograms(rh5,sh5);
        rh5->GetXaxis()->SetRangeUser(-10,10);
        sh5->GetXaxis()->SetRangeUser(-10,10);

        // pull of d_z
        rh6 = dynamic_cast<TH1*>( rdir->Get(collname1+"/pullDz") );
        sh6 = dynamic_cast<TH1*>( sdir->Get(collname2+"/pullDz") );
        NormalizeHistograms(rh6,sh6);
        rh6->GetXaxis()->SetRangeUser(-10,10);
        sh6->GetXaxis()->SetRangeUser(-10,10);

        canvas = new TCanvas("Tracks4","Tracks: pull of Pt, Qoverp and Phi",1000,1400);

        plotPulls(canvas,sh1,rh1,sh2,rh2,sh3,rh3,sh4,rh4,sh5,rh5,sh6,rh6);

        canvas->cd();
        l = new TLegend(0.10,0.655,0.90,0.69);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("pulls.pdf");
        delete l;

        // =====================================================================================//
        // resolutions vs eta

        // resolution of phi vs eta
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/phires_vs_eta_Sigma") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/phires_vs_eta_Sigma") );
        rh1->GetYaxis()->SetRangeUser(0.000009,0.01); // new general range
        sh1->GetYaxis()->SetRangeUser(0.000009,0.01);
        rh1->SetTitle(""); 
        rh1->GetYaxis()->SetTitleSize(ytitle_size);
        rh1->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh1->GetYaxis()->SetTitle("#sigma(#delta #phi) [rad]");
        rh1->GetXaxis()->SetTitleSize(xtitle_size);
        rh1->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh1->GetYaxis()->SetLabelSize(ylabel_size);
        rh1->GetXaxis()->SetLabelSize(xlabel_size);
        rh1->GetXaxis()->SetTitle("#eta");

        // resolution of cos(theta) vs eta
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/cotThetares_vs_eta_Sigma") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/cotThetares_vs_eta_Sigma") );
        rh2->GetYaxis()->SetRangeUser(0.00009,0.03); // new general range
        sh2->GetYaxis()->SetRangeUser(0.00009,0.03); // 0.0009, 0.01 multi-track, 0.00009, 0.01 single pt1, 0.0009, 0.01 single pt1,
        rh2->SetTitle("");
        rh2->GetYaxis()->SetTitleSize(ytitle_size);
        rh2->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh2->GetYaxis()->SetTitle("#sigma(#delta cot(#theta)) ");
        rh2->GetXaxis()->SetTitleSize(xtitle_size);
        rh2->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh2->GetYaxis()->SetLabelSize(ylabel_size);
        rh2->GetXaxis()->SetLabelSize(xlabel_size);
        rh2->GetXaxis()->SetTitle("#eta");

        // resolution of d_xy vs eta
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/dxyres_vs_eta_Sigma") );
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/dxyres_vs_eta_Sigma") );
        rh3->GetYaxis()->SetRangeUser(0.00009,0.05); // 0.0009, 0.02 mulit; 0.00009, 0.002 single pt100 
        sh3->GetYaxis()->SetRangeUser(0.00009,0.05);
        rh3->SetTitle("");
        rh3->GetYaxis()->SetTitleSize(ytitle_size);
        rh3->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh3->GetYaxis()->SetTitle("#sigma(#delta d_{0}) [cm]");
        rh3->GetXaxis()->SetTitleSize(xtitle_size);
        rh3->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh3->GetYaxis()->SetLabelSize(ylabel_size);
        rh3->GetXaxis()->SetLabelSize(xlabel_size);
        rh3->GetXaxis()->SetTitle("#eta"); 

        // resolution of phi vs eta
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/dzres_vs_eta_Sigma") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/dzres_vs_eta_Sigma") );
        rh4->GetYaxis()->SetRangeUser(0.0009,0.1); //0.0009,0.08 multi 
        sh4->GetYaxis()->SetRangeUser(0.0009,0.1);
        rh4->SetTitle("");
        rh4->GetYaxis()->SetTitleSize(ytitle_size);
        rh4->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh4->GetYaxis()->SetTitle("#sigma(#delta z_{0}) [cm]");
        rh4->GetXaxis()->SetTitleSize(xtitle_size);
        rh4->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh4->GetYaxis()->SetLabelSize(ylabel_size);
        rh4->GetXaxis()->SetLabelSize(xlabel_size);
        rh4->GetXaxis()->SetTitle("#eta");

        // resolution of pt vs eta
        rh5 = dynamic_cast<TH1*>( rdir->Get(collname1+"/ptres_vs_eta_Sigma") );
        sh5 = dynamic_cast<TH1*>( sdir->Get(collname2+"/ptres_vs_eta_Sigma") );
        rh5->SetTitle("");
        rh5->GetYaxis()->SetTitleSize(ytitle_size);
        rh5->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh5->GetYaxis()->SetTitle("#sigma(#delta p_{T}/p_{T}) ");
        rh5->GetXaxis()->SetTitleSize(xtitle_size);
        rh5->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh5->GetYaxis()->SetLabelSize(ylabel_size);
        rh5->GetXaxis()->SetLabelSize(xlabel_size);
        rh5->GetXaxis()->SetTitle("#eta");

        canvas = new TCanvas("Tracks7","Tracks: Dxy, Dz, Theta resolution",1000,1400);

        plotResolutions(canvas,sh1,rh1,sh2,rh2,sh3,rh3,sh4,rh4,sh5,rh5,sh6,rh6);

        canvas->cd();
        l = new TLegend(0.10,0.63,0.90,0.67);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("resolutionsEta.pdf");
        delete l;

        // =====================================================================================//
        // resolutions vs pt

        // resolution phi vs pt
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/phires_vs_pt_Sigma") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/phires_vs_pt_Sigma") );
        rh1->SetTitle("");
        rh1->GetYaxis()->SetTitleSize(ytitle_size);
        rh1->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh1->GetYaxis()->SetTitle("#sigma(#delta #phi) [rad]");
        rh1->GetXaxis()->SetTitleSize(xtitle_size);
        rh1->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh1->GetXaxis()->SetTitle("p_{T}");
        rh1->GetYaxis()->SetLabelSize(ylabel_size);
        rh1->GetXaxis()->SetLabelSize(xlabel_size);
        rh1->GetXaxis()->SetRangeUser(0,1000.);
        sh1->GetXaxis()->SetRangeUser(0,1000.);

        // resolution cos(theta) vs pt
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/cotThetares_vs_pt_Sigma") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/cotThetares_vs_pt_Sigma") );
        rh2->SetTitle("");
        rh2->GetYaxis()->SetTitleSize(ytitle_size);
        rh2->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh2->GetYaxis()->SetTitle("#sigma(#delta cot(#theta)) ");
        rh2->GetXaxis()->SetTitleSize(xtitle_size);
        rh2->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh2->GetXaxis()->SetTitle("p_{T}");
        rh2->GetYaxis()->SetLabelSize(ylabel_size);
        rh2->GetXaxis()->SetLabelSize(xlabel_size);
        rh2->GetXaxis()->SetRangeUser(0,1000.);
        sh2->GetXaxis()->SetRangeUser(0,1000.);

        // resolution phi vs pt
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/dxyres_vs_pt_Sigma") );
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/dxyres_vs_pt_Sigma") );
        rh3->SetTitle("");
        rh3->GetYaxis()->SetTitleSize(ytitle_size);
        rh3->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh3->GetYaxis()->SetTitle("#sigma(#delta d_{0}) [cm]");
        rh3->GetXaxis()->SetTitleSize(xtitle_size);
        rh3->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh3->GetYaxis()->SetLabelSize(ylabel_size);
        rh3->GetXaxis()->SetLabelSize(xlabel_size);
        rh3->GetXaxis()->SetTitle("p_{T}");
        rh3->GetXaxis()->SetRangeUser(0,1000.);
        sh3->GetXaxis()->SetRangeUser(0,1000.);

        // resolution phi vs pt
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/dzres_vs_pt_Sigma") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/dzres_vs_pt_Sigma") );
        rh4->SetTitle("");
        rh4->GetYaxis()->SetTitleSize(ytitle_size);
        rh4->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh4->GetYaxis()->SetTitle("#sigma(#delta z_{0}) [cm]");
        rh4->GetXaxis()->SetTitleSize(xtitle_size);
        rh4->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh4->GetXaxis()->SetTitle("p_{T}");
        rh4->GetYaxis()->SetLabelSize(ylabel_size);
        rh4->GetXaxis()->SetLabelSize(xlabel_size);
        rh4->GetXaxis()->SetRangeUser(0,1000.);
        sh4->GetXaxis()->SetRangeUser(0,1000.);

        // resolution phi vs pt
        rh5 = dynamic_cast<TH1*>( rdir->Get(collname1+"/ptres_vs_pt_Sigma") );
        sh5 = dynamic_cast<TH1*>( sdir->Get(collname2+"/ptres_vs_pt_Sigma") );
        rh5->SetTitle("");
        rh5->GetYaxis()->SetTitleSize(ytitle_size);
        rh5->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh5->GetYaxis()->SetTitle("#sigma(#delta p_{T}/p_{T}) ");
        rh5->GetXaxis()->SetTitleSize(xtitle_size);
        rh5->GetXaxis()->SetTitleOffset(xtitle_offset);
        rh5->GetXaxis()->SetTitle("p_{T}");
        rh5->GetYaxis()->SetLabelSize(ylabel_size);
        rh5->GetXaxis()->SetLabelSize(xlabel_size);
        rh5->GetXaxis()->SetRangeUser(0,1000.);
        sh5->GetXaxis()->SetRangeUser(0,1000.);

        canvas = new TCanvas("Tracks7b","Tracks: Dxy, Dz, Theta resolution",1000,1400);

        plotResolutions(canvas,sh1,rh1,sh2,rh2,sh3,rh3,sh4,rh4,sh5,rh5,sh6,rh6, true);

        canvas->cd();

        l = new TLegend(0.10,0.63,0.90,0.67);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("resolutionsPt.pdf");
        delete l;


        // =====================================================================================//
        // building 2

        // eff vs phi
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_phi") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_phi") );
        rh1->GetYaxis()->SetRangeUser(mineff,maxeff);
        sh1->GetYaxis()->SetRangeUser(mineff,maxeff);
        rh1->GetXaxis()->SetTitle("#phi");
        rh1->GetYaxis()->SetTitle("efficiency vs #phi");
        rh1->GetYaxis()->SetTitleSize(ytitle_size);
        rh1->GetYaxis()->SetTitleOffset(ytitle_offset);

        // fake rate vs phi
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakerate_vs_phi") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakerate_vs_phi") );
        rh2->GetXaxis()->SetTitle("#phi");
        rh2->GetYaxis()->SetTitle("fakerate vs #phi");
        rh2->GetYaxis()->SetRangeUser(minfake,maxfake);
        sh2->GetYaxis()->SetRangeUser(minfake,maxfake);
        rh2->GetYaxis()->SetTitleSize(ytitle_size);
        rh2->GetYaxis()->SetTitleOffset(ytitle_offset);

        // eff vs d_xy
        rh3 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_dxy") );
        sh3 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_dxy") );
        rh3->GetXaxis()->SetTitle("dxy");
        rh3->GetYaxis()->SetTitle("efficiency vs dxy");
        rh3->GetYaxis()->SetRangeUser(mineff,maxeff);
        sh3->GetYaxis()->SetRangeUser(mineff,maxeff);
        rh3->GetYaxis()->SetTitleSize(ytitle_size);
        rh3->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh3->SetTitle("");

        // fake rate vs d_xy
        rh4 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakerate_vs_dxy") );
        sh4 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakerate_vs_dxy") );
        rh4->SetTitle("");
        rh4->GetXaxis()->SetTitle("dxy");
        rh4->GetYaxis()->SetTitle("fakerate vs dxy");
        rh4->GetYaxis()->SetTitleSize(ytitle_size);
        rh4->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh4->GetYaxis()->SetRangeUser(minfake,maxfake);
        sh4->GetYaxis()->SetRangeUser(minfake,maxfake);

        // eff vs d_z
        rh5 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_dz") );
        sh5 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_dz") );
        rh5->GetXaxis()->SetTitle("dz");
        rh5->GetYaxis()->SetTitle("efficiency vs dz");
        rh5->GetYaxis()->SetTitleSize(ytitle_size);
        rh5->GetYaxis()->SetTitleOffset(ytitle_offset);
        rh5->GetYaxis()->SetRangeUser(mineff,maxeff);
        sh5->GetYaxis()->SetRangeUser(mineff,maxeff);

        // fake rate vs d_z
        rh6 = dynamic_cast<TH1*>( rdir->Get(collname1+"/fakerate_vs_dz") );
        sh6 = dynamic_cast<TH1*>( sdir->Get(collname2+"/fakerate_vs_dz") );
        rh6->GetYaxis()->SetRangeUser(0.,1.0);
        rh6->GetYaxis()->SetRangeUser(0.,1.0);
        rh6->GetXaxis()->SetTitle("dz");
        rh6->GetYaxis()->SetTitle("fakerate vs dz");
        rh6->GetYaxis()->SetTitleSize(ytitle_size);
        rh6->GetYaxis()->SetTitleOffset(ytitle_offset);

        r[0] = rh1; s[0] = sh1;
        r[1] = rh2; s[1] = sh2;
        r[2] = rh3; s[2] = sh3;
        r[3] = rh4; s[3] = sh4;
        r[4] = rh5; s[4] = sh5;
        r[5] = rh6; s[5] = sh6;

        canvas = new TCanvas("Tracks8","Tracks: efficiency & fakerate",1000,1400);

        plotBuilding(canvas, s, r, 6);

        canvas->cd();
        l = new TLegend(0.10,0.64,0.90,0.69);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("building2.pdf");   
        delete l;


        // =====================================================================================//
        // building 3

        // eff vs xy position
        rh1 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_vertpos") );
        sh1 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_vertpos") );
        if( rh1 || sh1 )
        {
            rh1->GetYaxis()->SetRangeUser(mineff,maxeff);
            sh1->GetYaxis()->SetRangeUser(mineff,maxeff);
            rh1->GetXaxis()->SetTitle("TP vert xy pos");
            rh1->GetYaxis()->SetTitle("efficiency vs vert xy pos");
            rh1->GetYaxis()->SetTitleSize(ytitle_size);
            rh1->GetYaxis()->SetTitleOffset(ytitle_offset);
        }
        else return;

        // eff versus z pos 
        rh2 = dynamic_cast<TH1*>( rdir->Get(collname1+"/effic_vs_zpos") );
        sh2 = dynamic_cast<TH1*>( sdir->Get(collname2+"/effic_vs_zpos") );
        if( rh2 || sh2 )
        {
            rh2->GetXaxis()->SetTitle("TP vert z pos");
            rh2->GetYaxis()->SetTitle("efficiency vs vert z pos");
            rh2->GetYaxis()->SetRangeUser(mineff,maxeff);
            sh2->GetYaxis()->SetRangeUser(mineff,maxeff);
            rh2->GetYaxis()->SetTitleSize(ytitle_size);
            rh2->GetYaxis()->SetTitleOffset(ytitle_offset);
            rh2->SetTitle("");
        }
        else return;

        r[0] = rh1; s[0] = sh1;
        r[1] = rh2; s[1] = sh2;

        canvas = new TCanvas("Tracks9","Tracks: efficiency & fakerate",1000,1400);

        plotBuilding(canvas, s, r, 2);

        canvas->cd();
        l = new TLegend(0.10,0.14,0.90,0.19);
        l->SetTextSize(0.016);
        l->SetLineColor(1);
        l->SetLineWidth(1);
        l->SetLineStyle(1);
        l->SetFillColor(0);
        l->SetBorderSize(3);
        l->AddEntry(rh1,refLabel,"LPF");
        l->AddEntry(sh1,newLabel,"LPF");
        l->Draw();
        canvas->Print("building3.pdf");   
        delete l;

    }
}


// -----------------------------------------------------------------------------------------------------------//
// helper functions implementation 

void NormalizeHistograms(TH1* h1, TH1* h2)
{
    if (h1==0 || h2==0) return;
    float scale1 = -9999.9;
    float scale2 = -9999.9;

    if ( h1->Integral() != 0 && h2->Integral() != 0 ){
        scale1 = 1.0/static_cast<float>(h1->Integral());
        scale2 = 1.0/static_cast<float>(h2->Integral());

        //h1->Sumw2();
        //h2->Sumw2();
        h1->Scale(scale1);
        h2->Scale(scale2);
    }
}

void plot4histos
(
    TCanvas *canvas, 
    TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
	TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4
)
{
    canvas->Divide(2,2);

    s1->SetMarkerStyle(20);
    r1->SetMarkerStyle(21);
    s1->SetMarkerColor(2);
    r1->SetMarkerColor(4);
    s1->SetMarkerSize(0.7);
    r1->SetMarkerSize(0.7);
    s1->SetLineColor(2);
    r1->SetLineColor(4);
    s1->SetLineWidth(2);
    r1->SetLineWidth(2);

    s2->SetMarkerStyle(20);
    r2->SetMarkerStyle(21);
    s2->SetMarkerColor(2);
    r2->SetMarkerColor(4);
    s2->SetMarkerSize(0.1);
    r2->SetMarkerSize(0.1);
    s2->SetLineColor(2);
    r2->SetLineColor(4);
    s2->SetLineWidth(2);
    r2->SetLineWidth(2);

    s3->SetMarkerStyle(20);
    r3->SetMarkerStyle(21);
    s3->SetMarkerColor(2);
    r3->SetMarkerColor(4);
    s3->SetMarkerSize(0.7);
    r3->SetMarkerSize(0.7);
    s3->SetLineColor(2);
    r3->SetLineColor(4);
    r3->SetLineWidth(2);
    s3->SetLineWidth(2);

    s4->SetMarkerStyle(20);
    r4->SetMarkerStyle(21);
    s4->SetMarkerColor(2);
    r4->SetMarkerColor(4);
    s4->SetMarkerSize(0.7);
    r4->SetMarkerSize(0.7);
    s4->SetLineColor(2);
    r4->SetLineColor(4);
    r4->SetLineWidth(2);
    s4->SetLineWidth(2);


    canvas->cd(1);
    setStats(s1,r1, 0.6, 0.65, false);
    r1->Draw();
    s1->Draw("sames");

    canvas->cd(2);
    setStats(s2,r2, 0.6, 0.65, false);
    r2->Draw();
    s2->Draw("sames");

    canvas->cd(3);
    setStats(s3,r3, 0.6, 0.65, false);
    r3->Draw();
    s3->Draw("sames");

    canvas->cd(4);
    setStats(s4,r4, 0.6, 0.65, false);
    r4->Draw();
    s4->Draw("sames");
}


void plotBuilding
(
    TCanvas *canvas, 
    TH1 **s, 
    TH1 **r, 
    int n,
    unsigned int logx
)
{
    canvas->Divide(2,(n+1)/2); //this should work also for odd n
    for(int i=0; i<n;i++){
        s[i]->SetMarkerStyle(20);
        r[i]->SetMarkerStyle(21);
        s[i]->SetMarkerColor(2);
        r[i]->SetMarkerColor(4);
        s[i]->SetMarkerSize(0.7);
        r[i]->SetMarkerSize(0.7);
        s[i]->SetLineColor(1);
        r[i]->SetLineColor(1);
        s[i]->SetLineWidth(1);
        r[i]->SetLineWidth(1);

        canvas->Update();
        TPad *pad = dynamic_cast<TPad*>(canvas->cd(i+1));
        setStats(s[i],r[i], -1, 0, false);
        if((logx>>i)&1) pad->SetLogx();
        r[i]->Draw();
        s[i]->Draw("sames");
    }
}


void plotPulls
(
    TCanvas *canvas, 
	TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
	TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4,
	TH1 *s5,TH1 *r5,TH1 *s6,TH1 *r6
)
{
    canvas->Divide(2,3);

    s1->SetMarkerStyle(20);
    r1->SetMarkerStyle(21);
    s1->SetMarkerColor(2);
    r1->SetMarkerColor(4);
    s1->SetMarkerSize(0.7);
    r1->SetMarkerSize(0.7);
    s1->SetLineColor(2);
    r1->SetLineColor(4);
    s1->SetLineWidth(2);
    r1->SetLineWidth(2);


    s2->SetMarkerStyle(20);
    r2->SetMarkerStyle(21);
    s2->SetMarkerColor(2);
    r2->SetMarkerColor(4);
    s2->SetMarkerSize(0.7);
    r2->SetMarkerSize(0.7);
    s2->SetLineColor(2);
    r2->SetLineColor(4);
    s2->SetLineWidth(2);
    r2->SetLineWidth(2);

    s3->SetMarkerStyle(20);
    r3->SetMarkerStyle(21);
    s3->SetMarkerColor(2);
    r3->SetMarkerColor(4);
    s3->SetMarkerSize(0.7);
    r3->SetMarkerSize(0.7);
    s3->SetLineColor(2);
    r3->SetLineColor(4);
    s3->SetLineWidth(2);
    r3->SetLineWidth(2);

    s4->SetMarkerStyle(20);
    r4->SetMarkerStyle(21);
    s4->SetMarkerColor(2);
    r4->SetMarkerColor(4);
    s4->SetMarkerSize(0.7);
    r4->SetMarkerSize(0.7);
    s4->SetLineColor(2);
    r4->SetLineColor(4);
    s4->SetLineWidth(2);
    r4->SetLineWidth(2);


    s5->SetMarkerStyle(20);
    r5->SetMarkerStyle(21);
    s5->SetMarkerColor(2);
    r5->SetMarkerColor(4);
    s5->SetMarkerSize(0.7);
    r5->SetMarkerSize(0.7);
    s5->SetLineColor(2);
    r5->SetLineColor(4);
    s5->SetLineWidth(2);
    r5->SetLineWidth(2);


    s6->SetMarkerStyle(20);
    r6->SetMarkerStyle(21);
    s6->SetMarkerColor(2);
    r6->SetMarkerColor(4);
    s6->SetMarkerSize(0.7);
    r6->SetMarkerSize(0.7);
    s6->SetLineColor(2);
    r6->SetLineColor(4);
    s6->SetLineWidth(2);
    r6->SetLineWidth(2);

    canvas->cd(1);
    setStats(s1,r1, 0.6, 0.65, true);
    r1->Draw();
    s1->Draw("sames");

    canvas->cd(2);
    setStats(s2,r2, 0.6, 0.65, true);
    r2->Draw();
    s2->Draw("sames");

    canvas->cd(3);
    setStats(s3,r3, 0.6, 0.65, true);
    r3->Draw();
    s3->Draw("sames");

    canvas->cd(4);
    setStats(s4,r4, 0.6, 0.65, true);
    r4->Draw();
    s4->Draw("sames");

    canvas->cd(5);
    setStats(s5,r5, 0.6, 0.65, true);
    r5->Draw();
    s5->Draw("sames");


    canvas->cd(6);
    setStats(s6,r6, 0.6, 0.65, true);
    r6->Draw();
    s6->Draw("sames");
}


void plotResolutions
(
    TCanvas *canvas, 
    TH1 *s1,TH1 *r1, TH1 *s2,TH1 *r2, 
    TH1 *s3,TH1 *r3, TH1 *s4,TH1 *r4,
    TH1 *s5,TH1 *r5,TH1 *s6,TH1 *r6,
    bool logx 
)
{
    canvas->Divide(2,3);

    s1->SetMarkerStyle(20);
    r1->SetMarkerStyle(21);
    s1->SetMarkerColor(2);
    r1->SetMarkerColor(4);
    s1->SetMarkerSize(0.7);
    r1->SetMarkerSize(0.7);
    s1->SetLineColor(1);
    r1->SetLineColor(1);

    s2->SetMarkerStyle(20);
    r2->SetMarkerStyle(21);
    s2->SetMarkerColor(2);
    r2->SetMarkerColor(4);
    s2->SetMarkerSize(0.7);
    r2->SetMarkerSize(0.7);
    s2->SetLineColor(1);
    r2->SetLineColor(1);

    s3->SetMarkerStyle(20);
    r3->SetMarkerStyle(21);
    s3->SetMarkerColor(2);
    r3->SetMarkerColor(4);
    s3->SetMarkerSize(0.7);
    r3->SetMarkerSize(0.7);
    s3->SetLineColor(1);
    r3->SetLineColor(1);

    s4->SetMarkerStyle(20);
    r4->SetMarkerStyle(21);
    s4->SetMarkerColor(2);
    r4->SetMarkerColor(4);
    s4->SetMarkerSize(0.7);
    r4->SetMarkerSize(0.7);
    s4->SetLineColor(1);
    r4->SetLineColor(1);


    s5->SetMarkerStyle(20);
    r5->SetMarkerStyle(21);
    s5->SetMarkerColor(2);
    r5->SetMarkerColor(4);
    s5->SetMarkerSize(0.7);
    r5->SetMarkerSize(0.7);
    s5->SetLineColor(1);
    r5->SetLineColor(1);

    s6->SetMarkerStyle(20);
    r6->SetMarkerStyle(21);
    s6->SetMarkerColor(2);
    r6->SetMarkerColor(4);
    s6->SetMarkerSize(0.7);
    r6->SetMarkerSize(0.7);
    s6->SetLineColor(1);
    r6->SetLineColor(1);
    s6->SetLineWidth(2);
    r6->SetLineWidth(2);

    canvas->cd(1);
    gPad->SetLogy(); 
    if(logx)gPad->SetLogx();
    setStats(r1,s1, -1, 0, false);
    r1->Draw();
    s1->Draw("sames");

    canvas->cd(2);
    gPad->SetLogy(); 
    if(logx)gPad->SetLogx();
    setStats(r2,s2, -1, 0, false);
    r2->Draw();
    s2->Draw("sames");

    canvas->cd(3);
    gPad->SetLogy(); 
    if(logx)gPad->SetLogx();
    setStats(r3,s3, -1, 0, false);
    r3->Draw();
    s3->Draw("sames");

    canvas->cd(4);
    gPad->SetLogy(); 
    if(logx)gPad->SetLogx();
    setStats(r4,s4, -1, 0, false);
    r4->Draw();
    s4->Draw("sames");

    canvas->cd(5);
    gPad->SetLogy(); 
    if(logx)gPad->SetLogx();
    setStats(r5,s5, -1, 0, false);
    r5->Draw();
    s5->Draw("sames");
}


void setStats(TH1* s,TH1* r, double startingY, double startingX, bool fit)
{
    if ( startingY < 0 )
    {
        s->SetStats(false);
        r->SetStats(false);
    } 
    else 
    {
        // do s
        s->SetStats(true);
        s->Draw();
        gPad->Update(); 

        if (fit){
            s->Fit("gaus");
            TF1* f1 = (TF1*) s->GetListOfFunctions()->FindObject("gaus");
            f1->SetLineColor(2);
            f1->SetLineWidth(1);
        }
        TPaveStats* stat_box;
        if( (stat_box = dynamic_cast<TPaveStats*>(s->FindObject("stats"))) )
        {
            if (fit) 
            {
                stat_box->SetOptFit(0010);    
                stat_box->SetOptStat(1001);
            }
            stat_box->SetX1NDC(startingX);
            stat_box->SetX2NDC(startingX+0.30);
            stat_box->SetY1NDC(startingY+0.20);
            stat_box->SetY2NDC(startingY+0.35);
            stat_box->SetTextColor(s->GetLineColor());
        }
        else cout << "statbox is null" << endl;

        // do r
        r->SetStats(true);
        r->Draw();
        gPad->Update(); 
        if (fit) 
        {
            r->Fit("gaus");
            TF1* f2 = (TF1*) r->GetListOfFunctions()->FindObject("gaus");
            f2->SetLineColor(4);
            f2->SetLineWidth(1);    
        }
        if( (stat_box = dynamic_cast<TPaveStats*>(r->FindObject("stats"))) )
        {
            if (fit) 
            {
                stat_box->SetOptFit(0010);    
                stat_box->SetOptStat(1001);
            }
            stat_box->SetX1NDC(startingX);
            stat_box->SetX2NDC(startingX+0.30);
            stat_box->SetY1NDC(startingY);
            stat_box->SetY2NDC(startingY+0.15);
            stat_box->SetTextColor(r->GetLineColor());
        }
        else cout << "statbox is null" << endl;
    }
}


void fixRangeY(TH1* r,TH1* s)
{
    double ymin = (r->GetBinContent(r->GetMinimumBin()) < s->GetBinContent(s->GetMinimumBin())) ? 
                  r->GetBinContent(r->GetMinimumBin()) : s->GetBinContent(s->GetMinimumBin());
    double ymax = (r->GetBinContent(r->GetMaximumBin()) > s->GetBinContent(s->GetMaximumBin())) ?
                  r->GetBinContent(r->GetMaximumBin()) : s->GetBinContent(s->GetMaximumBin());
    r->GetYaxis()->SetRangeUser(ymin*0.9,ymax*1.1);
    s->GetYaxis()->SetRangeUser(ymin*0.9,ymax*1.1);
}
