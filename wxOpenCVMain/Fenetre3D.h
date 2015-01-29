#ifndef _FENETRE3D_H_
#define _FENETRE3D_H_

#include <osgViewer/Viewer>
#include <wx/wx.h>
#include "NanoSurface.h"
#include "PlanReference.h"
#include "FenetreGraphiqueWX.h"
#include "CaptureInfo3D.h"

class Fenetre3D : public wxFrame
{
public:
Fenetre3D(wxFrame *frame, const wxString& title, const wxPoint& pos, 
    const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);

void MAJEchelleCouleur(float =0,float =16384.);
void MAJAxe();
void MAJNoeud(int);

void SetViewer(osgViewer::Viewer *viewer);
void OnIdle(wxIdleEvent& event);
void OnClose(wxCloseEvent& event);
void OnOuvrir(wxCommandEvent& event);
void OnOuvrir(ImageInfoCV *imReseau,int ,int);
void AjouteNanoSurface(int ,ImageInfoCV **);
void Quitter(wxCommandEvent& event);
void InstallGraphique(char *nomFichier);
void InstallGraphiquePhase1();
void InstallGraphiquePhase2(osg::Group *);
void DefOSGApp(void *w){osgApp=w;};

void AutoriseCapture(){cInfo3D->AutoriseCapture();};
void InterditCapture(){cInfo3D->InterditCapture();};
void AutoriseVideo(){modeVideo=1;};
void InterditVideo(){modeVideo=0;};
void AvanceVideo();

void ActiveGeode(int i);
void DeplaceCurseur(float,float,float);
void DeplaceCurseur(int,int);
void MAJGraphiqueNanoSurface(int d);

osg::Drawable* CreateSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image=NULL);
osg::Drawable* CreerLigne(const osg::Vec3& corner,const osg::Vec3& xdir,float epaisseurTrait=4);
osg::Drawable* CreateAxis(const osg::Vec3& corner,const osg::Vec3& xdir,const osg::Vec3& ydir,const osg::Vec3& zdir);
osg::Group* CreerTriede();
osg::Group* CreateScalarBar();
osg::Group * CreateScalarBar_HUD();




osg::ref_ptr<osgViewer::Viewer>	LireViewer(){return viewer;};

private:
osg::ref_ptr<osgViewer::Viewer>	viewer;
osg::ref_ptr<osg::HeightField>	*gridAffichee;
osg::ref_ptr<osg::Switch>		listeObjets;
osg::ref_ptr<osg::Group>		systemAxe;
osg::ref_ptr<osg::Group>		legendePalette;
osg::ref_ptr<osg::Group>		imageSurface;


int					indSurface;
NanoSurface			*surface;
NanoSurface			**tabSurface;
PlanReference		*surfaceReference;
FenetreGraphiqueWX	*fgOSGWX;

CaptureInfo3D		*cInfo3D;
char				modeVideo;

void *osgApp;

};

#endif
