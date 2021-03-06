#ifndef __WXOSGAPP__
#define __WXOSGAPP__

#include "ImageInfo.h"
#include "ParametreOperation.h"
#include <wx/app.h>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <wx/dynlib.h>
#include <wx/fileconf.h>
#include <wx/treebase.h>
#include <string>
#include <map>

//#include <wx/dde.h>
#include "ConnexionDDE.h"

class FenetreZoom;
class FenetrePrincipale;
class ImageStatistiques;
class OutilsImage;
class ControleCamera;
class Operation;

class EnvFenetre // Environnement fen�tre = FenetrePrincipale et zoom
{
public :
FenetrePrincipale	*fPrin;
FenetreZoom			*fZoom;
ImageStatistiques	*fStat;
void				*f3D;
EnvFenetre(FenetrePrincipale *ff,FenetreZoom *ffz,ImageStatistiques *fs){fPrin=ff;fZoom=ffz;fStat=fs;};
};


/*! \class EvtCalculFini
   * \brief la classe d�finit  les caract�ristiques d'un �v�nement
   *
   *  La classe EvtCalculFini d�finit un nouveau type d'�v�nement utilis� pour informer la fen�tre qu'un 
   * calcul est termin�. L'�v�nement est envoy� � la classe wxOSgApp .
   */
class EvtCalculFini : public wxCommandEvent
{
public:
    EvtCalculFini(wxEventType commandType, int id = 0);
    EvtCalculFini(const EvtCalculFini &evt):wxCommandEvent(evt)
	{
		m_Expediteur = evt.m_Expediteur;
		m_sortie = evt.m_sortie;
		m_travailEchoue = evt.m_travailEchoue;
	}

    virtual wxEvent* Clone() const
    {
        return new EvtCalculFini(*this);
    }

	std::vector<ImageInfoCV*> r;
	void*					m_Expediteur;
    bool					m_sortie;
    bool					m_travailEchoue;
	int						indEvt;		/*!< Nombre d'�v�nement envoy� */
};

/* Define a new application type */
class wxOpencvApp : public wxApp
{
protected:
    wxLanguage	langue;							/*!< language choisi */
    wxLocale	locale;							/*!< locale we'll be using */
    wxFileConfig *configApp;					/*!< Fichier de configuration de l'application */
    bool confirmFermeturefenetre;
    bool oclStatus;
    std::vector<ParametreOperation> listeOperation;		/*<! Liste des op�rations individuelles */
    std::map <int, std::vector <ParametreOperation > > tabOperation;	/*!< Tableau des op�rations effectu�es dans une s�quence */
    int 	numOpFaite;								/*!< Nombre d'op�ration faites ou sauvgard�es dans le fichier INI */
    int     numSeqOpe;								/*!< Nombre de s�quence d'op�rations faites ou sauvgard�es dans le fichier INI */
    void    *arbre;
    wxTreeItemId    idArbre;

    wxPoint posFenetre;
    char	utilisateurAbsent;
    bool	quitter;
    ServeurScilab		*serveur;
public:
    wxCriticalSection m_critsectWork;

private:
#ifdef _DLL_DETECTION__
    wxDynamicLibrary *dllAndor;
    wxDynamicLibrary *dllplplot;
    wxDynamicLibrary *dllWXplplotdrv;
    wxDynamicLibrary *dllSVGplplotdrv;
#endif

    char	camAndor;	/*!< �gal � 1 si une cam�ra Andor est branch�e */
    char	camOpenCV;	/*!< �gal � 1 si une cam�ra est d�tect�e par OpenCv */


    std::map <int, EnvFenetre *>	listeFenetre;
    std::map <std::string, wxBitmap >	listeBitmap;

    int					nbFenetre;
    int					indFenetre;

    int					indPointeurSouris;		// Pr�cise le type de cursuer
    int					modeSouris;				// utilis� pour la s�lection d'image

    ControleCamera		*ctrlCamera;
    OutilsImage			*outils;
    void				*fSeqOpe;		// Fenetre permettant d'�c�cuter plusieurs s�quences
    void				*fenOpe;		//  Fenetre pour d�finir plusieurs op�randes

    ParametreOperation pOCV;	/*!< parametre de l'op�rateur Unaire */
    int typeGradient;           /*! 0 Sobel, 1 Scharr, 2 Deriche, 3 Paillou*/
    int offsetOpe;


public:
    wxOpencvApp() { langue = wxLANGUAGE_UNKNOWN; }
    bool OnInit();
    FenetrePrincipale* CreerFenetre(std::vector<ImageInfoCV*> r, int nbres);
    void Quitter(wxFrame *g);
    void Ouvrir(wxCommandEvent &);	/*!< Ouverture d'un document */
    void InitFenAssociee(FenetrePrincipale *f); /*!< Ouvrir les fenetres associ�es � la fenetre principale */
    bool ConfirmFermeturefenetre() {return confirmFermeturefenetre;};
    void Video(wxCommandEvent &, int);
//    void Enregistrer(wxCommandEvent& event);

    void OuvertureOutils();
    void RetirerListe(FenetrePrincipale *);

    // M�thodes li�es � des op�rations sur les images
    void DefOperateurImage(wxString &);
    void DefBitmapOperateur(wxBitmap & b, wxString &);
    wxBitmap BitmapOperateur(wxString & s);
    void DefEtapeOperation(int x) { pOCV.indEtape = x; };
    void DefOperande1(ImageInfoCV* im, int i = -1);
    void DefOperande2(ImageInfoCV* im, int i = -1);
    void DefOperande3(ImageInfoCV* im, int i = -1);
    void DefOperandeN(ImageInfoCV* im, int i = -1);
    void SupOperandeN(ImageInfoCV* im, int i = -1);
    ParametreOperation *Operation() { return &pOCV; };
    void RAZOp() { pOCV.op.clear(); pOCV.indOpFenetre.clear(); pOCV.doubleParam.clear(); pOCV.intParam.clear(); };
    void DefParametreOCV(ParametreOperation &x) { pOCV = x; };
    void DefTypeGradient(int x) { typeGradient = x; };
    int TypeGradient() { return typeGradient; };
    bool VerifImagesExiste(ParametreOperation *);
    bool Operateur(){return pOCV.operateur;} /*!< Vrai si op�ration unaire s�lectionn�e */
    ImageInfoCV *Op1(){return OpId(0);};
    ImageInfoCV *Op2(){return OpId(1);};
    ImageInfoCV *Op3(){return OpId(2);};
    ImageInfoCV *OpId(int id);
    int IndOpId(int id);
    int IndOp1(){return IndOpId(0);};
    int IndOp2(){return IndOpId(1);};
    int IndOp3(){return IndOpId(2);};
//    int IdFenetreOp1pre();
    int NbOperande(){ return pOCV.nbOperande; };
    int NumSeqOpe(int *x=NULL){if (x!=NULL) numSeqOpe=*x;return numSeqOpe;};
    int GetOffsetOpe() { return offsetOpe; };
    void UpdateOffsetOpe() { offsetOpe += 50; };
    void AnnuleOp();
    std::map <int,std::vector <ParametreOperation > >  *TabSeqOperation(){return &tabOperation;}
    std::vector<ImageInfoCV*> ExecuterOperation(ParametreOperation * = NULL, bool noEvt = false);
    /*!
     *  \brief Fonction ExecuterOperation
     *
     *  Execute une op�ration d�finie par le pointeur opBinaireSelec ou opUnaireSelec
     *
     */
void CreerFenetreOperation();
    /*!
     *  \brief Fonction CreerFenetreOperation
     *
     *  Cr�ation d'une nouvelle fen�tre dont le r�sultat est le contenu de ExecuterOperation
     *
     */



wxString NomOperation(){return pOCV.nomOperation;};

void	SauverFichierConfig(wxString s,int id,int type=-1,int taille=-1);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'un param�tre dans le fichier de configuration du programme
     *  \param s : nom du param�tre � sauvegarder
	 *  \param id : indice  d�pendant du param�tre
	 *  \param type : forme pour la morphologie
	 *  \param taille : taille de l'op�rateur 
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, wxString chaine);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param chaine : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, long v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, double v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */
void SauverOperationFichierConfig(ParametreOperation &v);
   /*!
     *  \brief Fonction SauverOperationFichierConfig
     *
     *  Sauvegarde d'un objet ParametreOp�ration dans le fichier de configuration
     *  \param v : objet
	 */

void	LectureFichierConfig();
    /*!
     *  \brief Fonction LectureFichierConfig
     *
     *  Lecture des param�tres contenus dans le fichier de configuration
     */
void CalculFini(EvtCalculFini &w);

// Gestion  du curseur souris
void	DefPointeurSouris(int modeSouris=0,int typeSouris=0);
void    DefNoeudCalcul(void *a, void *id);
int		PointeurSouris(int x){return indPointeurSouris;};
void	SourisQuitterFen(wxMouseEvent &event);
int		ModeSouris(){return modeSouris;};

//// Operations
//void DefOperateurImage(ImageInfoCV* (ImageInfoCV::*f)(ImageInfoCV & ,ImageInfoCV &));
void GenerationGraphDot(ParametreOperation *op);
    /*!
     *  \brief Fonction GenerationGraphDot
     *
     *  Generation du graphe de l'op�ration dans le langage Dot
     */

    // Tile the bitmap
//    bool TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap);

//// Accessors
    wxBitmap& GetBackgroundBitmap() const { return (wxBitmap&) m_background; }

    bool GetUseScreen() const { return m_useScreen; }
    void SetUseScreen(bool useScreen) { m_useScreen = useScreen; }

    void OnUseScreen(wxCommandEvent& event);

protected:
    wxBitmap    m_background;
    bool        m_useScreen;

public :	


char	UtilisateurAbsent(){if (utilisateurAbsent>=2)return 1;return 0;};
bool	Quitter(){return quitter;};
void	DefUtilisateurAbsent(char x){if (x==0)utilisateurAbsent=0;utilisateurAbsent+=x;};
void	SauverConfiguration();

int					VerifFenetre(){if (nbFenetre==0 || indFenetre==-1) return 0;else return 1;};
int					NbFenetre(){return nbFenetre;};
void				IdFenetreActive(int x);
int					RechercheFenetre(ImageInfoCV*); /*<! recherche dans quelle fen�tre est associ�e � l'image */
FenetrePrincipale	*Fenetre(int id){if (id>=0) return Graphique(id);return NULL;};
FenetrePrincipale	*Graphique(int =-1);
ControleCamera		*CtrlCamera(){return ctrlCamera;};
ImageStatistiques	*ImgStat(){if (nbFenetre>0 && indFenetre>=0) return listeFenetre[indFenetre]->fStat;else return NULL;};
OutilsImage			*Outils(){return outils;};
void				*FenetreSeqOpe(void *f){fSeqOpe=f;return fSeqOpe;};
void				*FenetreSeqOpe(){return fSeqOpe;};
void				*FenetreOperande(){return fenOpe;};
void				FenetreOperande(void *f){fenOpe=f;};
ServeurScilab		*Serveur(){return serveur;};
void *ArboCalcul() { return arbre; };
void *NoeudCalcul() { return idArbre; };

void TracerZoom(wxPoint p);

int FilterEvent(wxEvent& event);
};




typedef void (wxEvtHandler::*EvtCalculFiniFonction)(EvtCalculFini&);
#define GstEvtCalculFini(func) wxEVENT_HANDLER_CAST(EvtCalculFiniFonction, func)   




#endif
